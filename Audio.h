#pragma once
#include <portaudio.h>
#include <stdio.h>
#include <iostream>
#include "Modes.h"
#include "Globals.h"


class Audio
{
private:

	PaError error;
	PaStream* stream = nullptr;

	void ExitIfError();
	auto GenerateCallback(Modes mode);

	static int RealTimeCallback(const void* inputBuffer, void* outputBuffer,
		unsigned long framesPerBuffer,
		const PaStreamCallbackTimeInfo* timeInfo,
		PaStreamCallbackFlags statusFlags,
		void* userData);

	static int SubtitlesCallback(const void* inputBuffer, void* outputBuffer,
		unsigned long framesPerBuffer,
		const PaStreamCallbackTimeInfo* timeInfo,
		PaStreamCallbackFlags statusFlags,
		void* userData);

	static int LyricsCallback(const void* inputBuffer, void* outputBuffer,
		unsigned long framesPerBuffer,
		const PaStreamCallbackTimeInfo* timeInfo,
		PaStreamCallbackFlags statusFlags,
		void* userData);
public:
	Audio();

	void StartStream(Modes mode);
	bool readPCMFromWav(const std::string& fname, std::vector<float>& pcmf32, std::vector<std::vector<float>>& pcmf32s, bool stereo);

	~Audio();
};

