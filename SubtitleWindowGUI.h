#pragma once

#include "GUI.h"
#include "Globals.h"

#include <raylib-cpp/Text.hpp>
#include <raylib-cpp/Font.hpp>
#include <nfd.h>

#include "Button.h"
#include "TextBox.h"


class SubtitleWindowGUI : public GUI
{
private:
	raylib::Text subtitleText;
	raylib::Text srtExportText;

	Button subtitleButton;
	
	TextBox outDirBox;
	Button outDirBoxButton;

	nfdchar_t* outPath = NULL;
	nfdresult_t result;

	raylib::Image srtExportIcon;
	Texture2D srtExportIconTexture;

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

