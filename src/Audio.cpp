#include <iostream>
#include <mutex>
#include <whisper.h>
#include <portaudio.h>
#include <dr_wav.h>

#include "Globals.h"
#include "Audio.h"
#include "Modes.h"

int MyRealTimeCallback(const void* inputBuffer, void* outputBuffer,
	unsigned long framesPerBuffer,
	const PaStreamCallbackTimeInfo* timeInfo,
	PaStreamCallbackFlags statusFlags,
	void* userData)
{
	Audio* audio = reinterpret_cast<Audio*>(userData);
	return audio->RealTimeCallback(inputBuffer, outputBuffer, framesPerBuffer, timeInfo, statusFlags, nullptr); 
}

void Audio::ExitIfError()
{
	if (error != paNoError)
	{
		std::cerr << "PortAudio Error: " << Pa_GetErrorText(error) << std::endl;
		std::exit(EXIT_FAILURE);
	}
}

int Audio::RealTimeCallback(const void* inputBuffer, void* outputBuffer, unsigned long framesPerBuffer, const PaStreamCallbackTimeInfo* timeInfo, PaStreamCallbackFlags statusFlags, void* userData)
{
	std::lock_guard<std::mutex> lock(queueMutex);

	if (abortTranscription)
		return paComplete;
	if (!takeMicrophoneInput)
		return paContinue;

	const float* input = static_cast<const float*>(inputBuffer);

	for (int i = 0; i < framesPerBuffer; i++)
	{
		pcm32_input.push_back(input[i]);
		count++;
	}

	if (count >= 48000)
	{
		audioQueue.push(pcm32_input);
		queueCondition.notify_one();
		pcm32_input.clear();
		count = 0;
	}

	return paContinue;
}

Audio::Audio()
{
	error = Pa_Initialize();
	ExitIfError();

	int numDevices = Pa_GetDeviceCount();
	if (numDevices < 0)
	{
		std::cerr << "PortAudio: Error Getting Device Count " << std::endl;
		std::exit(EXIT_FAILURE);
	}
	else if (numDevices == 0)
	{
		std::cerr << "PortAudio: There are no available audio devices on this device." << std::endl;
		std::exit(EXIT_FAILURE);
	}

	std::cout << "PortAudio: Audio Intitialized" << std::endl;
}

void Audio::StartStream(Modes mode)
{
	std::cout << "Starting audio stream..." << std::endl;
	
	error = Pa_OpenDefaultStream(&stream, 1, 0, paFloat32, PA_SAMPLE_RATE, PA_FRAMES_PER_BUFFER, MyRealTimeCallback, this);
	ExitIfError();

	error = Pa_StartStream(stream);
	ExitIfError();
}

bool Audio::readPCMFromWav(const std::string& fname, std::vector<float>& pcmf32, std::vector<std::vector<float>>& pcmf32s, bool stereo)
{
	drwav wav;
	std::vector<uint8_t> wav_data;

	if (drwav_init_file(&wav, fname.c_str(), nullptr) == false)
	{
		fprintf(stderr, "error: failed to open '%s' as WAV file\n", fname.c_str());
		return false;
	}

	if (wav.channels != 1 && wav.channels != 2)
	{
		fprintf(stderr, "%s: WAV file '%s' must be mono or stereo\n", __func__, fname.c_str());
		return false;
	}

	if (stereo && wav.channels != 2)
	{
		fprintf(stderr, "%s: WAV file '%s' must be stereo for diarization\n", __func__, fname.c_str());
		return false;
	}

	if (wav.sampleRate != WHISPER_SAMPLE_RATE)
	{
		fprintf(stderr, "%s: WAV file '%s' must be %i kHz\n", __func__, fname.c_str(), WHISPER_SAMPLE_RATE / 1000);
		return false;
	}

	if (wav.bitsPerSample != 16)
	{
		fprintf(stderr, "%s: WAV file '%s' must be 16-bit\n", __func__, fname.c_str());
		return false;
	}

	const uint64_t n = wav_data.empty() ? wav.totalPCMFrameCount : wav_data.size() / (wav.channels * wav.bitsPerSample / 8);

	std::vector<int16_t> pcm16;
	pcm16.resize(n * wav.channels);
	drwav_read_pcm_frames_s16(&wav, n, pcm16.data());
	drwav_uninit(&wav);

	// convert to mono, float
	pcmf32.resize(n);
	if (wav.channels == 1)
	{
		for (uint64_t i = 0; i < n; i++)
		{
			pcmf32[i] = float(pcm16[i]) / 32768.0f;
		}
	}
	else
	{
		for (uint64_t i = 0; i < n; i++)
		{
			pcmf32[i] = float(pcm16[2 * i] + pcm16[2 * i + 1]) / 65536.0f;
		}
	}

	if (stereo)
	{
		// convert to stereo, float
		pcmf32s.resize(2);

		pcmf32s[0].resize(n);
		pcmf32s[1].resize(n);
		for (uint64_t i = 0; i < n; i++)
		{
			pcmf32s[0][i] = float(pcm16[2 * i]) / 32768.0f;
			pcmf32s[1][i] = float(pcm16[2 * i + 1]) / 32768.0f;
		}
	}

	return true;
}

Audio::~Audio()
{
	abortTranscription = true;
	Pa_Sleep(5000);

	// Forcefully stop the stream if it hasn't stopped yet
	if (Pa_IsStreamActive(stream))
		Pa_AbortStream(stream);

	error = Pa_CloseStream(stream);
	ExitIfError();
	Pa_Terminate();

	std::cout << "Audio Terminated" << std::endl;
}
