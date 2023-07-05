#include <iostream>
#include <vector>
#include "whisper.h"
#include "utils.h"
#include <portaudio.h>
#include <thread>

#define PA_SAMPLE_RATE WHISPER_SAMPLE_RATE
#define PA_FRAMES_PER_BUFFER 512

std::vector<float> pcm32;

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

	for (int i = 0; i < framesPerBuffer; i++)
	{
		pcm32.push_back(input[i]);
	}
	/*int size = pcm32.size();
	if (size / (PA_SAMPLE_RATE * 3) > count)
	{
		count++;
		if (whisper_full(ctx, wh_full_params, pcm32.data(), size) != 0)
		{
			fprintf(stderr, "%s: failed to process audio\n", "Audio");
			return 6;
		}
		const int n_segments = whisper_full_n_segments(ctx);
		for (int i = 0; i < n_segments; ++i)
		{
			const char* text = whisper_full_get_segment_text(ctx, i);

			printf("%s\n", text);
			fflush(stdout);
		}
	}*/


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
	whisper_params wh_params;
	whisper_full_params wh_full_params = whisper_full_default_params(WHISPER_SAMPLING_GREEDY);
	wh_full_params.print_progress = false;

	struct whisper_context* ctx = whisper_init_from_file(wh_params.model.c_str());

	while (!stopRequested)
	{
		long long int size = 0;
		size = pcm32.size();
		//printf("size : %d\n", size);
		if (size / (PA_SAMPLE_RATE * 3) > count)
		{
			count++;
			if (whisper_full(ctx, wh_full_params, pcm32.data(), size) != 0)
			{
				fprintf(stderr, "%s: failed to process audio\n", "Audio");
				return;
			}
			const int n_segments = whisper_full_n_segments(ctx);
			for (int i = 0; i < n_segments; ++i)
			{
				const char* text = whisper_full_get_segment_text(ctx, i);

				printf("%s\n", text);
				fflush(stdout);
			}
		}
	}

	whisper_free(ctx);
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

	std::cout << "Recorded " << pcm32.size() << " samples." << std::endl;

	whisper_params wh_params;
	whisper_full_params wh_full_params = whisper_full_default_params(WHISPER_SAMPLING_GREEDY);
	wh_full_params.print_progress = false;

	struct whisper_context* ctx = whisper_init_from_file(wh_params.model.c_str());

	if (whisper_full(ctx, wh_full_params, pcm32.data(), pcm32.size()) != 0)
	{
		fprintf(stderr, "%s: failed to process audio\n", "Audio");
		return 6;
	}

	const int n_segments = whisper_full_n_segments(ctx);
	for (int i = 0; i < n_segments; ++i)
	{
		const char* text = whisper_full_get_segment_text(ctx, i);

		printf("%s", text);
		fflush(stdout);
	}
	std::cout << std::endl;

	pcm32.clear();
	whisper_free(ctx);
	return 0;
}
