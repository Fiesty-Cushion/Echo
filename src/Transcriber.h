#pragma once

#include <string>
#include <atomic>
#include <chrono>
#include <mutex>
#include <thread>
#include <vector>
#include <fstream>

#include "Audio.h"

#include "StringUtils.h"
#include "ScriptUtils.h"
#include "VideoUtils.h"

struct transcribed_msg {
	std::string text;
	bool is_partial;
};

class Transcriber
{
public:
	void AddAudioData(const std::vector<float>& new_data);
	std::vector<transcribed_msg> GetTranscribed();
	bool GenerateKaraoke(const char* inputPath, const char* outputDir);
	bool BurnInSubtitles(const char* inputPath, const char* outputDir);
	void BeginStreaming();

	~Transcriber();

	static Transcriber* GetInstance();

private:
	static Transcriber* instance;
	static std::string currentPath; // path to currently loaded model
	static Transcriber* createInstance();

	Audio audio;
	Transcriber(struct whisper_context* stt_ctx, struct whisper_context* karaoke_ctx, struct whisper_context* subtitles_ctx);

	struct whisper_context* ctx;
	struct whisper_context* karaoke_ctx;
	struct whisper_context* subtitles_ctx;

	std::atomic<bool> is_running;
	std::vector<float> s_queued_pcmf32;
	std::vector<transcribed_msg> s_transcribed_msgs;
	std::mutex s_mutex;  // for accessing shared variables from both main thread and worker thread
	std::thread worker;
	std::chrono::time_point<std::chrono::high_resolution_clock> t_last_iter;

	void Run();

	int timestamp_to_sample(int64_t t, int n_samples);
	std::string estimate_diarization_speaker(std::vector<std::vector<float>> pcmf32s, int64_t t0, int64_t t1, bool id_only = false);

    const char* font_mono = "../assets/Fonts/VeraMono-Bold.ttf";
};