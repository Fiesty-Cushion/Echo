#pragma once

#include <queue>
#include <mutex>
#include <string>
#include <raylib.h>
#include <raylib-cpp/Font.hpp>
#include <raylib-cpp/Text.hpp>
#include <portaudio.h>


#define MGRAY CLITERAL(Color) {225, 225, 225, 255}
#define MPINK CLITERAL(Color) {255, 102, 196, 255}
#define MTEXT CLITERAL(Color) {29, 43, 83, 255}
#define MBG CLITERAL(Color) {238, 238, 238, 255}
#define MBORDER CLITERAL(Color) {194, 195, 199, 255}
#define LGRAY CLITERAL(Color) {73, 76, 93, 255}
#define LIGHTBLUE CLITERAL(Color) {229, 241, 251, 255}

const double PA_SAMPLE_RATE = 16000;
const unsigned int PA_FRAMES_PER_BUFFER = 512;

// Using extern because Globals is included separately by Audio.h and Transcriber.h and both Audio.h and Transcriber.h are included in Gui.cpp leading to "X is already defined error"
extern bool abortTranscription;
extern bool takeMicrophoneInput;
extern int count;
extern std::queue<std::vector<float>> pcm32_bufferQueue;
extern std::vector<float> pcm32_input;
extern std::condition_variable bufferCV;
extern std::mutex bufferMutex;

extern std::string transcribedText;

//Load the Deafult font
extern raylib::Font m_font;
extern raylib::Font raygui_font;
extern raylib::Font m_font_title;

const std::string m_font_path = "./Resources/Fonts/monogram.ttf";
const std::string MODEL_PATH = "/Users/macbook/my_Files/Code/whisper.cpp/models/ggml-base.en.bin";

// CallBack Function for PortAudio
extern int MyRealTimeCallback(const void* inputBuffer, void* outputBuffer,
	unsigned long framesPerBuffer,
	const PaStreamCallbackTimeInfo* timeInfo,
	PaStreamCallbackFlags statusFlags,
	void* userData);

struct TextWave
{
	int numCharacters;
	float* characterOffset;

};

void addHoverEffect(raylib::Text, TextWave* wave, int x, int y);