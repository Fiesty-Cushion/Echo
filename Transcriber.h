#pragma once

#include <string>
#include <atomic>
#include <chrono>
#include <mutex>
#include <thread>
#include <vector>

#include "Audio.h"


struct transcribed_msg {
	std::string text;
	bool is_partial;
};

class Transcriber
{
public:
	Transcriber(Audio& aud);

	void AddAudioData(const std::vector<float>& new_data);
	std::vector<transcribed_msg> GetTranscribed();

	~Transcriber();

private:
	Audio& audio;

	struct whisper_context* ctx;

	std::atomic<bool> is_running;
	std::vector<float> s_queued_pcmf32;
	std::vector<transcribed_msg> s_transcribed_msgs;
	std::mutex s_mutex;  // for accessing shared variables from both main thread and worker thread
	std::thread worker;
	std::chrono::time_point<std::chrono::high_resolution_clock> t_last_iter;

	void Run();
};