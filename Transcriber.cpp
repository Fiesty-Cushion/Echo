#include "Transcriber.h"

Transcriber::Transcriber()
{
	ctx = whisper_init_from_file(MODEL_PATH);
	if (!ctx)
	{
		std::cerr << "CTX: MODEL FAILED TO INITIALIZE" << std::endl;
		exit(2);
	}
	wh_full_params = whisper_full_default_params(WHISPER_SAMPLING_BEAM_SEARCH);
	wh_full_params.n_threads = 4;

	std::cout << "Transcriber Intitialized" << std::endl;
}

void Transcriber::BeginRealTimeTranscription()
{
	// Running the transcription parallelly
	std::thread transcribeThread(&Transcriber::RealTimeTransciber, this);
	transcribeThread.detach();
}

void Transcriber::RealTimeTransciber()
{
	auto start = std::chrono::high_resolution_clock::now();

	whisper_full_params wh_realtime_params = wh_full_params;
	wh_realtime_params.print_progress = false;


	while (!abortTranscription)
	{

		std::unique_lock<std::mutex> lock(bufferMutex);
		bufferCV.wait(lock, [] {return pcm32_bufferQueue.size() >= 3; });

		// taking data in this sequence
		// Itr 1: 1,2,3
		// Itr 2: 3,4,5
		// Itr 3: 5,6,7
		std::vector<float> pcm32_combined = pcm32_bufferQueue.front();
		for (int i = 0; i < 2; i++)
		{
			pcm32_bufferQueue.pop();
			std::vector<float> pcm32 = pcm32_bufferQueue.front();
			pcm32_combined.insert(pcm32_combined.end(), pcm32.begin(), pcm32.end());
		}

		lock.unlock();

		if (whisper_full(ctx, wh_realtime_params, pcm32_combined.data(), pcm32_combined.size()) != 0)
		{
			fprintf(stderr, "%s: failed to process audio\n", "Audio");
			return;
		}


		const int n_segments = whisper_full_n_segments(ctx);
		for (int i = 0; i < n_segments; ++i)
		{
			const char* text = whisper_full_get_segment_text(ctx, i);
			auto end = std::chrono::high_resolution_clock::now();
			double duration = std::chrono::duration_cast<std::chrono::nanoseconds>(end - start).count();
			duration *= 1e-9;
			printf("%s\n", text);
			transcribedText += text;
			std::cout << "Time Elapsed: " << std::fixed << duration << std::setprecision(9) << "sec" << std::endl;
			start = end;
			fflush(stdout);
		}

	}
}

int Transcriber::TranscribeFromWav(std::vector<float> pcmf32Wav, int processors)
{
	wh_full_params.print_realtime = true;
	if (whisper_full_parallel(ctx, wh_full_params, pcmf32Wav.data(), pcmf32Wav.size(), processors) != 0)
	{
		fprintf(stderr, "failed to process audio\n");
		return -1;
	}

	const int n_segments = whisper_full_n_segments(ctx);
	for (int i = 0; i < n_segments; ++i)
	{
		const char* text = whisper_full_get_segment_text(ctx, i);

		std::cout << text << std::endl;
	}

}

Transcriber::~Transcriber()
{
	whisper_free(ctx);
	std::cout << "Transcriber Terminated" << std::endl;
}
