#pragma once
#include <queue>
#include <mutex>
#include <portaudio.h>

#include "Modes.h"


class Audio
{
private:
	PaError error;
	PaStream* stream = nullptr;

	void ExitIfError();

public:
	Audio();

	std::queue<std::vector<float>> audioQueue;
	std::mutex queueMutex;
	std::condition_variable queueCondition;

	void StartStream(Modes mode);
	bool readPCMFromWav(const std::string& fname, std::vector<float>& pcmf32, std::vector<std::vector<float>>& pcmf32s, bool stereo);

	int RealTimeCallback(const void* inputBuffer, void* outputBuffer,
		unsigned long framesPerBuffer,
		const PaStreamCallbackTimeInfo* timeInfo,
		PaStreamCallbackFlags statusFlags,
		void* userData);

	~Audio();
};