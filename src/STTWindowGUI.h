#pragma once

#include <raylib-cpp/Text.hpp>
#include "GUI.h"
#include "Audio.h"
#include "Transcriber.h"
#include "Button.h"
#include "TextBox.h"

class STTWindowGUI : public GUI
{
private:
	raylib::Text sttText;

	bool isInitialClick = true;
	bool isTranscribing = false;

	Button transcribeButton;

	TextBox outputTextBox;

	Rectangle panelRec;
	Rectangle panelContentRec;
	Rectangle panelView = { 0 };
	Vector2 panelScroll = { 99, -20 };

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
	void Draw();
	void HandleEvents();
	void ShutDown();
};

