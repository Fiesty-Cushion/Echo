#include "Audio.h"
void Audio::ExitIfError()
{
	if (error != paNoError)
	{
		printf("PortAudio Error : %s", Pa_GetErrorText(error));
		exit(EXIT_FAILURE);
	}
}

auto Audio::GenerateCallback(Modes mode)
{
	switch (mode)
	{
	case Modes::RealTime:
		return RealTimeCallback;
		break;
	case Modes::Subtitles:
		return SubtitlesCallback;
		break;
	case Modes::Lyrics:
		return LyricsCallback;
		break;
	default:
		break;
	}
}

int Audio::RealTimeCallback(const void* inputBuffer, void* outputBuffer, unsigned long framesPerBuffer, const PaStreamCallbackTimeInfo* timeInfo, PaStreamCallbackFlags statusFlags, void* userData)
{
	std::cout << "RT Callback called" << std::endl;
	if (stopRequested)
		return paComplete;
	const float* input = static_cast<const float*>(inputBuffer);

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

int Audio::SubtitlesCallback(const void* inputBuffer, void* outputBuffer, unsigned long framesPerBuffer, const PaStreamCallbackTimeInfo* timeInfo, PaStreamCallbackFlags statusFlags, void* userData)
{
	// TODO: Implement this
	return 0;
}

int Audio::LyricsCallback(const void* inputBuffer, void* outputBuffer, unsigned long framesPerBuffer, const PaStreamCallbackTimeInfo* timeInfo, PaStreamCallbackFlags statusFlags, void* userData)
{
	// TODO: Implement this
	return 0;
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
	//error = Pa_OpenDefaultStream(&stream, 1, 0, paFloat32, PA_SAMPLE_RATE, PA_FRAMES_PER_BUFFER, GenerateCallback(mode), nullptr);
	error = Pa_OpenDefaultStream(&stream, 1, 0, paFloat32, PA_SAMPLE_RATE, PA_FRAMES_PER_BUFFER, RealTimeCallback, nullptr);
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
