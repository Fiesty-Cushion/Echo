#pragma once

#include "GUI.h"
#include "Globals.h"

#include <raylib-cpp/Text.hpp>
#include <raylib-cpp/Font.hpp>
#include <raylib-cpp/Image.hpp>
#include <raylib-cpp/Texture.hpp>
#include "Button.h"
#include "TextBox.h"
#include <nfd.h>
#include "Transcriber.h"


class KaraokeWindowGUI : public GUI
{
private:
	raylib::Text karaokeText;
	raylib::Text karExportText;
	raylib::Text promptText;

	Button karaokeButton;
	Button karaokeImportButton;

	TextBox outDirBox;
	Button outDirBoxButton;

	nfdchar_t* outPath = NULL;
	nfdresult_t result;

	raylib::Image karExportIcon;
	Texture2D karExportIconTexture;

	std::string karaokeInputPath;
	std::string outputDirPath;

	Texture frame;
	Image frameImage;

	//bool displayVideoFrame = false;
	FileStatus fileStatus = FileStatus::Default;

	Audio* audio;
	Transcriber* transcriber;

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

