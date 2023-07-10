#include <iostream>
#include <vector>
#include "whisper.h"
#include "utils.h"
#include <portaudio.h>
#include <thread>
#include <chrono>
#include <iomanip>
#include <queue>
#include <condition_variable>
#include <mutex>

#define PA_SAMPLE_RATE WHISPER_SAMPLE_RATE
#define PA_FRAMES_PER_BUFFER 512

std::queue<std::vector<float>> pcm32_bufferQueue;
std::vector<float> pcm32_buffer;
std::condition_variable bufferCV;
std::mutex bufferMutex;


struct whisper_context* ctx = whisper_init_from_file("D:/Projects/C++/Echo/Models/ggml-model-whisper-base.en.bin");

bool stopRequested = false;
int count = 0;

static int Pa_Callback(const void* inputBuffer, void* outputBuffer,
	unsigned long framesPerBuffer,
	const PaStreamCallbackTimeInfo* timeInfo,
	PaStreamCallbackFlags statusFlags,
	void* userData)
{

	if (stopRequested)
		return paComplete;
	const float* input = static_cast<const float*>(inputBuffer);

	//pcm32_new.insert(pcm32_new.begin()+count*framesPerBuffer, inputBuffer, inputBuffer+512);
	for (int i = 0; i < framesPerBuffer; i++)
	{
		pcm32_buffer.insert(pcm32_buffer.begin() + (count * PA_FRAMES_PER_BUFFER), 1, input[i]);
	}
	count++;
	if (pcm32_buffer.size() >= PA_SAMPLE_RATE * 2)
	{
		std::unique_lock<std::mutex> lock(bufferMutex);
		pcm32_bufferQueue.push(pcm32_buffer);
		count = 0;
		pcm32_buffer.clear();
		bufferCV.notify_one();
	}

	return paContinue;
}

static void Pa_CheckError(PaError err)
{
	if (err != paNoError)
	{
		printf("PortAudio Error : %s", Pa_GetErrorText(err));
		exit(EXIT_FAILURE);
	}
}


void Wh_Transcribe()
{
	auto start = std::chrono::high_resolution_clock::now();
	whisper_params wh_params;
	whisper_full_params wh_full_params = whisper_full_default_params(WHISPER_SAMPLING_BEAM_SEARCH);
	wh_full_params.print_progress = false;
	wh_full_params.print_realtime = true;


	while (!stopRequested)
	{
		//printf("transcription called\n");
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
		//printf("size : %u\n", pcm32_combined.size());;
		if (whisper_full(ctx, wh_full_params, pcm32_combined.data(), pcm32_combined.size()) != 0)
		{
			fprintf(stderr, "%s: failed to process audio\n", "Audio");
			return;
		}

		//printf("transcription completed\n");
		const int n_segments = whisper_full_n_segments(ctx);
		for (int i = 0; i < n_segments; ++i)
		{
			const char* text = whisper_full_get_segment_text(ctx, i);
			auto end = std::chrono::high_resolution_clock::now();
			double duration = std::chrono::duration_cast<std::chrono::nanoseconds>(end - start).count();
			duration *= 1e-9;
			printf("%s\n", text);
			std::cout << "Time Elapsed: " << std::fixed << duration << std::setprecision(9) << "sec" << std::endl;
			start = end;
			fflush(stdout);
		}

	}
}

int main()
{
	PaError err;
	err = Pa_Initialize();
	Pa_CheckError(err);

	int numDevices = Pa_GetDeviceCount();
	if (numDevices < 0)
	{
		printf("Error getting device count.\n");
		exit(EXIT_FAILURE);
	}
	else if (numDevices == 0)
	{
		printf("There are no available audio devices on this device.\n");
		exit(EXIT_SUCCESS);
	}

	PaStream* stream;
	err = Pa_OpenDefaultStream(&stream, 1, 0, paFloat32, PA_SAMPLE_RATE, PA_FRAMES_PER_BUFFER, Pa_Callback, nullptr);
	Pa_CheckError(err);

	err = Pa_StartStream(stream);
	Pa_CheckError(err);

	// Running the whisper transcription parallelly
	std::thread transcribeThread(Wh_Transcribe);
	transcribeThread.detach();

	std::cout << "Listening... Press Enter to stop." << std::endl;
	std::cin.ignore();

	stopRequested = true;
	Pa_Sleep(5000);

	// Forcefully stop the stream if it hasn't stopped yet
	if (Pa_IsStreamActive(stream))
		Pa_AbortStream(stream);

	err = Pa_CloseStream(stream);
	Pa_CheckError(err);
	Pa_Terminate();

	whisper_free(ctx);
	return 0;
}