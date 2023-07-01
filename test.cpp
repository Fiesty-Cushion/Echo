#include <cmath>
#include <fstream>
#include <cstdio>
#include <string>
#include <thread>
#include <vector>
#include <cstring>
#include <whisper.h>

#include "utils.h"
#include "common.h"

void whisper_print_segment_callback(struct whisper_context* ctx, struct whisper_state* /*state*/, int n_new, void* user_data) {
    const auto& params = *((whisper_print_user_data*)user_data)->params;
    const auto& pcmf32s = *((whisper_print_user_data*)user_data)->pcmf32s;

    const int n_segments = whisper_full_n_segments(ctx);

    std::string speaker = "";

    int64_t t0 = 0;
    int64_t t1 = 0;

    // print the last n_new segments
    const int s0 = n_segments - n_new;

    if (s0 == 0) {
        printf("\n");
    }

    for (int i = s0; i < n_segments; i++) {
        if (!params.no_timestamps || params.diarize) {
            t0 = whisper_full_get_segment_t0(ctx, i);
            t1 = whisper_full_get_segment_t1(ctx, i);
        }

        {
            const char* text = whisper_full_get_segment_text(ctx, i);

            printf("%s%s", speaker.c_str(), text);
        }

        // with timestamps or speakers: each segment on new line
        if (!params.no_timestamps || params.diarize) {
            printf("\n");
        }

        fflush(stdout);
    }
}

int main(int argc, char** argv) {
    whisper_params params;

    struct whisper_context* ctx = whisper_init_from_file(params.model.c_str());

    if (ctx == nullptr) {
        fprintf(stderr, "error: failed to initialize whisper context\n");
        return 3;
    }

    const std::string fname_inp = params.fname_inp[0];

    std::vector<float> pcmf32;               // mono-channel F32 PCM
    std::vector<std::vector<float>> pcmf32s; // stereo-channel F32 PCM

    if (!::read_wav(fname_inp, pcmf32, pcmf32s, params.diarize)) {
        fprintf(stderr, "error: failed to read WAV file '%s'\n", fname_inp.c_str());
        return -1;
    }

    // run the inference
    whisper_full_params wparams = whisper_full_default_params(WHISPER_SAMPLING_GREEDY);
    whisper_print_user_data user_data = { &params, &pcmf32s };

    // this callback is called on each new segment
    if (!wparams.print_realtime) {
        wparams.new_segment_callback = whisper_print_segment_callback;
        wparams.new_segment_callback_user_data = &user_data;

        static bool is_aborted = false; // NOTE: this should be atomic to avoid data 
        wparams.encoder_begin_callback = [](struct whisper_context* /*ctx*/, struct whisper_state* /*state*/, void* user_data) {
            bool is_aborted = *(bool*)user_data;
            return !is_aborted;
        };
        wparams.encoder_begin_callback_user_data = &is_aborted;

        if (whisper_full_parallel(ctx, wparams, pcmf32.data(), pcmf32.size(), params.n_processors) != 0) {
            fprintf(stderr, "%s: failed to process audio\n", argv[0]);
            return 10;
        }

        whisper_free(ctx);
        return 0;
    }
}