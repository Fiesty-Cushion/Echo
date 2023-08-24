#pragma once

#include "GUI.h"
#include "Globals.h"

#include <raylib-cpp/Text.hpp>
#include <raylib-cpp/Font.hpp>
#include "Button.h"


class KaraokeWindowGUI : public GUI
{
private:
	raylib::Text karaokeText;

	Button karaokeButton;

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

