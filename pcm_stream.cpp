#include <iostream>
#include <vector>
#include "whisper.h"
#include "utils.h"
#include <portaudio.h>

#define PA_SAMPLE_RATE WHISPER_SAMPLE_RATE
#define PA_FRAMES_PER_BUFFER 512

std::vector<float> pcm32;
whisper_params wh_params;
struct whisper_context* ctx = whisper_init_from_file(wh_params.model.c_str());
whisper_full_params wh_full_params = whisper_full_default_params(WHISPER_SAMPLING_GREEDY);


static int Pa_Callback(const void* inputBuffer, void* outputBuffer,
	unsigned long framesPerBuffer,
	const PaStreamCallbackTimeInfo* timeInfo,
	PaStreamCallbackFlags statusFlags,
	void* userData)
{
	const float* input = static_cast<const float*>(inputBuffer);

	for (int i = 0; i < framesPerBuffer; i++)
	{
		pcm32.push_back(input[i]);
	}
	int size = pcm32.size();
	if (size > 16000 * 3)
	{

		if (whisper_full(ctx, wh_full_params, pcm32.data(), size) != 0)
		{
			fprintf(stderr, "%s: failed to process audio\n", "Audio");
			return -1;
		}
		const int n_segments = whisper_full_n_segments(ctx);
		for (int i = 0; i < n_segments; ++i)
		{
			const char* text = whisper_full_get_segment_text(ctx, i);

			printf("%s\n", text);
			fflush(stdout);
		}
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

	std::cout << "Listening... Press Enter to stop." << std::endl;
	std::cin.ignore();

	err = Pa_StopStream(stream);
	if (err != paNoError)
	{
		Pa_AbortStream(stream);
		Pa_CloseStream(stream);
		Pa_Terminate();
		fprintf(stderr, "PortAudio Error: %s\n", Pa_GetErrorText(err));
		return -1;
	};

	err = Pa_CloseStream(stream);
	Pa_CheckError(err);

	Pa_Terminate();

	/*std::cout << "Recorded " << pcm32.size() << " samples." << std::endl;

	if (whisper_full(ctx, wh_full_params, pcm32.data(), pcm32.size()) != 0)
	{
		fprintf(stderr, "%s: failed to process audio\n", "Audio");
		return -1;
	}

	const int n_segments = whisper_full_n_segments(ctx);
	for (int i = 0; i < n_segments; ++i)
	{
		const char* text = whisper_full_get_segment_text(ctx, i);

		printf("%s", text);
		fflush(stdout);
	}
	std::cout << std::endl;*/

	pcm32.clear();
	whisper_free(ctx);
	return 0;
}

