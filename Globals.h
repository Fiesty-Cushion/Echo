#pragma once
#include <queue>
#include <condition_variable>
#include <mutex>

#define MGRAY CLITERAL(Color) {225, 225, 225, 255}
#define MBG CLITERAL(Color) {26, 26, 36, 255}
#define LGRAY CLITERAL(Color) {73, 76, 93, 255}
#define LIGHTBLUE CLITERAL(Color) {229, 241, 251, 255}
// #define LIGHTGRAY CLITERAL(Color) {173, 173, 173, 255}

const double PA_SAMPLE_RATE = 16000;
const unsigned int PA_FRAMES_PER_BUFFER = 512;

// Using extern because Globals is included separately by Audio.h and Transcriber.h and both Audio.h and Transcriber.h are included in Gui.cpp leading to "X is already defined error"
extern bool stopRequested;
extern int count;
extern std::queue<std::vector<float>> pcm32_bufferQueue;
extern std::vector<float> pcm32_buffer;
extern std::condition_variable bufferCV;
extern std::mutex bufferMutex;
