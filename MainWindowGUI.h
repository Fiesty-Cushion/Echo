#pragma once

#include <string>
#include <raylib-cpp/Rectangle.hpp>
#include <raylib-cpp/Text.hpp>
#include <raylib-cpp/Font.hpp>

#include "Globals.h"
#include "GUI.h"
#include "Modes.h"
#include "TextBox.h"
#include "Transcriber.h"
#include "DropdownList.h"
#include "Button.h"
#include "Audio.h"
#include "STTWindowGUI.h"
#include "SubtitleWindowGUI.h"
#include "KaraokeWindowGUI.h"


class MainWindowGUI : public GUI
{
private:
	Screen screen;
	TextWave wave;

	raylib::Text titleText;
	raylib::Text modelSelectText;
	raylib::Text featuresText;

	DropdownList dropdownList;

	TextBox modelTextBox;

	STTWindowGUI sttScreen;
	SubtitleWindowGUI subtitleScreen;
	KaraokeWindowGUI karaokeScreen;

	void setupDisplayText(raylib::Text& text, std::string message, int fontSize = 32, raylib::Font& font = m_font)
	{
		text.SetText(message);
		text.SetFont(font);
		text.SetFontSize(fontSize);
		text.SetSpacing(1);
		text.SetColor(MTEXT);
	}

public:
	void Init();
	void StartLoop();
	void Draw();
	void HandleEvents();
	void ShutDown();
};