#pragma once
#include <whisper.h>
#include <stdio.h>
#include <iostream>
#include <thread>
#include <chrono>
#include <iomanip>

#include "Globals.h"
#include "utils.h"

class Transcriber
{
private:
	const char* MODEL_PATH = "./Models/ggml-model-whisper-base.en.bin";

	struct whisper_context* ctx;
	//whisper_params wh_params;
	whisper_full_params wh_full_params;

	void RealTimeTransciber();

public:
	Transcriber();

	void BeginRealTimeTranscription();
	int TranscribeFromWav(std::vector<float> pcmf32Wav, int processors);

	~Transcriber();
};

