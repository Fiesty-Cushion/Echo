#include <iostream>
#include <vector>
#include "whisper.h"
#include "utils.h"
#include <portaudio.h>
#include <thread>
#include <chrono>
#include <iomanip>

#define PA_SAMPLE_RATE WHISPER_SAMPLE_RATE
#define PA_FRAMES_PER_BUFFER 512

std::vector<float> pcm32_old;
std::vector<float> pcm32;
std::vector<float> pcm32_new;


struct whisper_context* ctx = whisper_init_from_file("D:/Projects/C++/Echo/Models/ggml-model-whisper-base.en.bin");

bool stopRequested = false;
int count = 0;
bool isReadyForTranscription = false;

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
		pcm32_new.insert(pcm32_new.begin() + (count * 512), 1, input[i]);
	}
	count++;
	auto pcmNewSize = pcm32_new.size();
	if (pcmNewSize >= 48000)
	{
		int samplesToTake = 48000;
		//memcpy(pcm32.data(), pcm32_old.data() + 24000, sizeof(float) * 24000);
		if (pcm32_old.size() > 0) // only runs on second iteration
		{
			for (int i = 0; i < samplesToTake; i++) {
				pcm32[i] = pcm32_old[pcm32_old.size() - samplesToTake + i];
			}
		}
		else {
			for (int i = 0; i < samplesToTake; i++) {
				pcm32.push_back(0);
			}
		}
		auto pcmSize = pcm32.size();
		for (int i = 0; i < pcmNewSize + samplesToTake -pcmSize; i++) {
			pcm32.push_back(0);
		}
		//auto y = pcm32_new.size();
		//memcpy(pcm32.data() + 24000, pcm32_new.data(), sizeof(float) * pcm32_new.size());
		std::copy(pcm32_new.begin(), pcm32_new.begin()+pcmNewSize, pcm32.begin() + samplesToTake);
		pcm32_old = pcm32; 
		count = 0;
		pcm32_new.clear();
		isReadyForTranscription = true;
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

	while (!stopRequested)
	{
		//printf("size : %d\n", size);
		if (isReadyForTranscription)
		{
			//std::vector<float> pcmPortion(bufferSize);
			//pcmPortion.resize(bufferSize);
			//std::copy(pcm32.begin() + bufferSize * count, pcm32.begin() + bufferSize * (count + 1), pcmPortion.begin());
			//memcpy(pcmPortion.data(), pcm32.data() + bufferSize * count, sizeof(float) * bufferSize);
			//count++;

			isReadyForTranscription = false;
			if (whisper_full(ctx, wh_full_params, pcm32_old.data(), pcm32_old.size()) != 0)
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
				std::cout << "Time Elapsed: " << std::fixed << duration << std::setprecision(9) << "sec" << std::endl;
				start = end;
				fflush(stdout);
			}

		}
	}
}

int mainn()
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

	std::cout << "Recorded " << pcm32.size() << " samples." << std::endl;

	pcm32.clear();
	whisper_free(ctx);
	return 0;
}
