#include "Audio.h"
void Audio::ExitIfError()
{
	if (error != paNoError)
	{
		printf("PortAudio Error : %s", Pa_GetErrorText(error));
		exit(EXIT_FAILURE);
	}
}

Audio::Audio()
{
	error = Pa_Initialize();
	ExitIfError();
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
	std::cout << "Audio Intitialized" << std::endl;
}

void Audio::StartStream(Modes mode)
{
	std::cout << "Starting audio stream..." << std::endl;
	error = Pa_OpenDefaultStream(&stream, 1, 0, paFloat32, PA_SAMPLE_RATE, PA_FRAMES_PER_BUFFER, CallbackFactory::Generate(mode), nullptr);
	ExitIfError();

	error = Pa_StartStream(stream);
	ExitIfError();
}

Audio::~Audio()
{
	stopRequested = true;
	Pa_Sleep(5000);

	// Forcefully stop the stream if it hasn't stopped yet
	if (Pa_IsStreamActive(stream))
		Pa_AbortStream(stream);

	error = Pa_CloseStream(stream);
	ExitIfError();
	Pa_Terminate();

	std::cout << "Audio Terminated" << std::endl;
}
