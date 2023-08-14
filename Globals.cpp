#include "Globals.h"

bool stopRequested = false;
int count = 0;
std::queue<std::vector<float>> pcm32_bufferQueue;
std::vector<float> pcm32_buffer;
std::condition_variable bufferCV;
std::mutex bufferMutex;
std::string transcribedText = "";

// Audio* audio = nullptr;
// Transcriber* transcriber = nullptr;

raylib::Font m_font;