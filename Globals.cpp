#include <queue>
#include <vector>
#include <mutex>
#include <string>

#include <raylib.h>
#include <raylib-cpp/Font.hpp>
#include <raylib-cpp/Text.hpp>

#include "Globals.h"

bool abortTranscription = false;
bool takeMicrophoneInput = false;
int count = 0;
std::queue<std::vector<float>> pcm32_bufferQueue;
std::vector<float> pcm32_input;
std::condition_variable bufferCV;
std::mutex bufferMutex;
std::string transcribedText = "";

raylib::Font m_font;
raylib::Font raygui_font;
raylib::Font m_font_title;

void addHoverEffect(raylib::Text text, TextWave* wave, int x, int y)
{
    std::string word = text.GetText();
    float xOffset = 0;
    for (int i = 0; i < wave->numCharacters; i++) {
        char character = word[i];
        float characterWidth = 30;
        DrawTextCodepoint(m_font_title, character, { static_cast<float>(x + xOffset), static_cast<float>(y + wave->characterOffset[i]) }, 48, (i < 4) ? MPINK : MTEXT);
        xOffset += characterWidth;
    }
}