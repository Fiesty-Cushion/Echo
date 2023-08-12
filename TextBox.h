#pragma once

#include <raylib-cpp.hpp>
#include "Globals.h"

class TextBox {
private:

	raylib::Rectangle textBox;
	bool active;
	std::string inputText;
	raylib::Text text;

public:

	TextBox() {};
	TextBox(float x, float y, float width, float height) : textBox{x, y, width, height}, active(false), inputText("") 
	{
		text.SetFont(m_font);
		text.SetFontSize(20);
		text.SetColor(MGRAY);
		text.SetText(inputText);
		text.SetSpacing(1.0f);
	}

	void Draw()
	{
		textBox.DrawRounded(0.5f, 8, LGRAY);

		if (!active) {
			text.Draw(static_cast<int>(textBox.x) + 5, static_cast<int>(textBox.y) + 5);
			//DrawText(inputText.c_str(), static_cast<int>(textBox.x) + 5, static_cast<int>(textBox.y) + 5, textBox.height - 20, MGRAY);
		}
		else {
			text.Draw(static_cast<int>(textBox.x) + 5, static_cast<int>(textBox.y) + 5);
			//DrawText((inputText + "_").c_str(), static_cast<int>(textBox.x) + 5, static_cast<int>(textBox.y) + 5, textBox.height - 20, MGRAY);
		}
	}

	void Update()
	{
		if ( CheckCollisionPointRec( GetMousePosition(), textBox ) )
		{
			if ( IsMouseButtonPressed(MOUSE_LEFT_BUTTON) )
			{
				active = true;
				std::cout << "Clicked!" << std::endl;
			}
		}
		else if ( IsMouseButtonPressed(MOUSE_LEFT_BUTTON) )
		{
			active = false;
			std::cout << "Clicked Outside!" << std::endl;
		}

		if (active)
		{
			int key = GetKeyPressed();
			while (key > 0) 
			{
				if ((key >= 32) && (key <= 125) && (inputText.length() < 20)) {
					inputText += static_cast<char>(key);
					text.SetText(inputText);
				}
				else if (IsKeyPressed(KEY_BACKSPACE) && (inputText.length() > 0)) {
					inputText.pop_back();
					text.SetText(inputText);
				}
				key = GetKeyPressed();
			}
		}

	}

};