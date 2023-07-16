#pragma once
#include <queue>
#include <condition_variable>
#include <mutex>


const double PA_SAMPLE_RATE = 16000;
const unsigned int PA_FRAMES_PER_BUFFER = 512;

// Using extern because Globals is included separately by Audio.h and Transcriber.h and both Audio.h and Transcriber.h are included in Gui.cpp leading to "X is already defined error"
extern bool stopRequested;
extern int count;
extern std::queue<std::vector<float>> pcm32_bufferQueue;
extern std::vector<float> pcm32_buffer;
extern std::condition_variable bufferCV;
extern std::mutex bufferMutex;
