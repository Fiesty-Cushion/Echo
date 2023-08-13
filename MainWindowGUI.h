#pragma once
#include "GUI.h"
#include "TextBox.h"


class MainWindowGUI : public GUI
{
private:
	raylib::Text title_Text;
	raylib::Text model_Text;
	raylib::Text display_Text;
	raylib::Text subtitle_Text;
	raylib::Text lyrics_Text;
	raylib::Text karaoke_Text;

	TextBox modelTextBox;
	TextBox outputTextBox;

	raylib::Rectangle subtitle_Box;
	raylib::Rectangle lyrics_Box;
	raylib::Rectangle karaoke_Box;

	void setupDisplayText(raylib::Text& text, std::string message, int fontSize = 25){
		text.SetText(message);
		text.SetFont(m_font);
		text.SetFontSize(fontSize);
		text.SetSpacing(3);
	}

public:
	void Init();
	void StartLoop();
	void Draw();
	void HandleEvents();
	void ShutDown();
};