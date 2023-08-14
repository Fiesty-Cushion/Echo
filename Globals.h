#pragma once
#include <queue>
#include <condition_variable>
#include <mutex>

#include <raylib-cpp.hpp>

#include "Audio.h"
#include "Transcriber.h"

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

extern std::string transcribedText;
// extern Transcriber* transcriber;
// extern Audio* audio;

//Load the Deafult font
extern raylib::Font m_font;
const std::string m_font_path = "/Users/macbook/my_Files/Code/Echo/Resources/Fonts/OpenSans-SemiBold.ttf";