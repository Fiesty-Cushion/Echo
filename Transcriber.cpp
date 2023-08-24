#include "Transcriber.h"
#include "whisper.h"
#include "Globals.h"

#include <atomic>
#include <cmath>
#include <mutex>
#include <string>
#include <thread>
#include <vector>
#include <math.h>
#include "Audio.h"
#include <chrono>
#include <iostream>

#define M_PI 3.1415


void high_pass_filter(std::vector<float>& data, float cutoff, float sample_rate)
{
   
    const float rc = 1.0f / (2.0f * M_PI * cutoff);
    const float dt = 1.0f / sample_rate;
    const float alpha = dt / (rc + dt);

    float y = data[0];

    for (size_t i = 1; i < data.size(); i++) {
        y = alpha * (y + data[i] - data[i - 1]);
        data[i] = y;
    }
}

/** Check if speech is ending. */
bool vad_simple(std::vector<float>& pcmf32, int sample_rate, int last_ms, float vad_thold, float freq_thold, bool verbose)
{
    const int n_samples = pcmf32.size();
    const int n_samples_last = (sample_rate * last_ms) / 1000;

    if (n_samples_last >= n_samples) {
        // not enough samples - assume no speech
        return false;
    }

    if (freq_thold > 0.0f) {
        high_pass_filter(pcmf32, freq_thold, sample_rate);
    }

    float energy_all = 0.0f;
    float energy_last = 0.0f;

    for (int i = 0; i < n_samples; i++) {
        energy_all += fabsf(pcmf32[i]);

        if (i >= n_samples - n_samples_last) {
            energy_last += fabsf(pcmf32[i]);
        }
    }

    energy_all /= n_samples;
    energy_last /= n_samples_last;

    if (verbose) {
        fprintf(stderr, "%s: energy_all: %f, energy_last: %f, vad_thold: %f, freq_thold: %f\n", __func__, energy_all, energy_last, vad_thold, freq_thold);
    }

    if ((energy_all < 0.0001f && energy_last < 0.0001f) || energy_last > vad_thold * energy_all) {
        return false;
    }

    return true;
}

Transcriber::Transcriber(Audio& aud) : audio(aud)
{
    ctx = whisper_init_from_file(MODEL_PATH.c_str());
    is_running = true;
    worker = std::thread(&Transcriber::Run, this);
    t_last_iter = std::chrono::high_resolution_clock::now();
}

Transcriber::~Transcriber()
{
    is_running = false;
    if (worker.joinable())
        worker.join();
    whisper_free(ctx);
}

/** Add audio data in PCM f32 format. */
void Transcriber::AddAudioData(const std::vector<float>& data)
{
    std::lock_guard<std::mutex> lock(s_mutex);
    std::cout << "AddAudioData: remaining: " << (int)s_queued_pcmf32.size() << ", new: " << (int)data.size() << std::endl;
    s_queued_pcmf32.insert(s_queued_pcmf32.end(), data.begin(), data.end());
}

/** Get newly transcribed text. */
std::vector<transcribed_msg> Transcriber::GetTranscribed()
{
    std::vector<transcribed_msg> transcribed;
    std::lock_guard<std::mutex> lock(s_mutex);
    transcribed = std::move(s_transcribed_msgs);
    s_transcribed_msgs.clear();
    return transcribed;
}

/** Run Whisper in its own thread to not block the main thread. */
void Transcriber::Run()
{
    struct whisper_full_params wparams = whisper_full_default_params(whisper_sampling_strategy::WHISPER_SAMPLING_GREEDY);

    wparams.n_threads = 4;
    wparams.no_context = true;
    wparams.single_segment = true;
    wparams.print_progress = false;
    wparams.print_realtime = false;
    wparams.print_special = false;
    wparams.print_timestamps = false;
    wparams.max_tokens = 64;
    wparams.language = "en";
    wparams.translate = false;

    /**
     * Experimental optimization: Reduce audio_ctx to 15s (half of the chunk
     * size whisper is designed for) to speed up 2x.
     * https://github.com/ggerganov/whisper.cpp/issues/137#issuecomment-1318412267
     */
    wparams.audio_ctx = 768;

    /* When more than this amount of audio received, run an iteration. */
    const int trigger_ms = 400;
    const int n_samples_trigger = (trigger_ms / 1000.0) * WHISPER_SAMPLE_RATE;
    /**
     * When more than this amount of audio accumulates in the audio buffer,
     * force finalize current audio context and clear the buffer. Note that
     * VAD may finalize an iteration earlier.
     */
     // This is recommended to be smaller than the time wparams.audio_ctx
     // represents so an iteration can fit in one chunk.
    const int iter_threshold_ms = trigger_ms * 35;
    const int n_samples_iter_threshold = (iter_threshold_ms / 1000.0) * WHISPER_SAMPLE_RATE;

     /* VAD parameters */
     // The most recent 3s.
    const int vad_window_s = 3;
    const int n_samples_vad_window = WHISPER_SAMPLE_RATE * vad_window_s;
    // In VAD, compare the energy of the last 500ms to that of the total 3s.
    const int vad_last_ms = 500;
    // Keep the last 0.5s of an iteration to the next one for better
    // transcription at begin/end.
    const int n_samples_keep_iter = WHISPER_SAMPLE_RATE * 0.5;
    const float vad_thold = 0.3f;
    const float freq_thold = 200.0f;

    /* Audio buffer */
    std::vector<float> pcmf32;

    /* Processing loop */
    while (is_running) {
        {
            std::unique_lock<std::mutex> lock(audio.queueMutex);
            audio.queueCondition.wait(lock, [&]() { return !audio.audioQueue.empty(); });

            AddAudioData(audio.audioQueue.front());
            audio.audioQueue.pop();

            lock.unlock();
        }

        {
            std::lock_guard<std::mutex> lock(s_mutex);

            if (s_queued_pcmf32.size() > 2 * n_samples_iter_threshold) {
                //fprintf(stderr, "\n\n%s: WARNING: too much audio is going to be processed, result may not come out in real time\n\n", __func__);
                std::cerr << __func__ << ": WARNING: too much audio is going to be processed, result may not come out in real time" << std::endl;
            }
        }

        {
            std::lock_guard<std::mutex> lock(s_mutex);

            pcmf32.insert(pcmf32.end(), s_queued_pcmf32.begin(), s_queued_pcmf32.end());

            // printf("existing: %d, new: %d, will process: %d, threshold: %d\n",
            //        n_samples_old, n_samples_new, (int)pcmf32.size(), n_samples_iter_threshold);

            s_queued_pcmf32.clear();
        }

        {
            int ret = whisper_full(ctx, wparams, pcmf32.data(), pcmf32.size());
            if (ret != 0) {
                std::cerr << "Failed to process audio, returned " << ret << std::endl;
                continue;
            }
        }

        {
            transcribed_msg msg;

            const int n_segments = whisper_full_n_segments(ctx);
            for (int i = 0; i < n_segments; ++i) {
                const char* text = whisper_full_get_segment_text(ctx, i);
                msg.text += text;
            }

            /**
             * Simple VAD from the "stream" example in whisper.cpp
             * https://github.com/ggerganov/whisper.cpp/blob/231bebca7deaf32d268a8b207d15aa859e52dbbe/examples/stream/stream.cpp#L378
             */
            bool speech_has_end = false;

            /* Need enough accumulated audio to do VAD. */
            if ((int)pcmf32.size() >= n_samples_vad_window) {
                std::vector<float> pcmf32_window(pcmf32.end() - n_samples_vad_window, pcmf32.end());
                speech_has_end = vad_simple(pcmf32_window, WHISPER_SAMPLE_RATE, vad_last_ms,
                    vad_thold, freq_thold, false);
                if (speech_has_end)
                    printf("speech end detected\n");
            }

            /**
             * Clear audio buffer when the size exceeds iteration threshold or
             * speech end is detected.
             */
            if (pcmf32.size() > n_samples_iter_threshold || speech_has_end) {
                const auto t_now = std::chrono::high_resolution_clock::now();
                const auto t_diff = std::chrono::duration_cast<std::chrono::milliseconds>(t_now - t_last_iter).count();

                std::cout << "Iteration Took: " << t_diff << std::endl;
                t_last_iter = t_now;

                msg.is_partial = false;
                /**
                 * Keep the last few samples in the audio buffer, so the next
                 * iteration has a smoother start.
                 */
                std::vector<float> last(pcmf32.end() - n_samples_keep_iter, pcmf32.end());
                pcmf32 = std::move(last);
            }
            else {
                msg.is_partial = true;
            }

            std::lock_guard<std::mutex> lock(s_mutex);
            s_transcribed_msgs.insert(s_transcribed_msgs.end(), std::move(msg));
            
        }
    }
}