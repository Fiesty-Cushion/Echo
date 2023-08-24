#pragma once

#include <string>
#include <raylib-cpp/Rectangle.hpp>
#include <raylib-cpp/Text.hpp>
#include <raylib-cpp/Font.hpp>

#include "Globals.h"
#include "GUI.h"
#include "TextBox.h"
#include "Transcriber.h"
#include "DropdownList.h"
#include "Button.h"
#include "Audio.h"


class MainWindowGUI : public GUI
{
private:
	TextWave wave;

	raylib::Text titleText;
	raylib::Text modelSelectText;
	raylib::Text sttText;
	raylib::Text featuresText;

	DropdownList dropdownList;

	Audio* audio;
	Transcriber* transcriber;

	Button transcribeButton;
	bool isInitialClick = true;

	bool isTranscribing = false;

	TextBox modelTextBox;
	TextBox outputTextBox;

	raylib::Rectangle subtitle_Box;
	raylib::Rectangle lyrics_Box;
	raylib::Rectangle karaoke_Box;

	void setupDisplayText(raylib::Text& text, std::string message, int fontSize = 32, raylib::Font& font = m_font)
	{
		text.SetText(message);
		text.SetFont(font);
		text.SetFontSize(fontSize);
		text.SetSpacing(1);
		text.SetColor(MTEXT);
	}
	// TEST ONLY
	Button karaokeButton;


public:
	void Init();
	void StartLoop();
	void Draw();
	void HandleEvents();
	void ShutDown();
};