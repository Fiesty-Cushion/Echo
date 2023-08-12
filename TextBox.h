#pragma once

#include <raylib-cpp.hpp>
#include "Globals.h"

class TextBox {
private:

	raylib::Rectangle textBox;
	bool active;
	std::string inputText;
	raylib::Text tb_text;

public:

	TextBox() {};
	TextBox(float x, float y, float width, float height) : textBox{x, y, width, height}, active(false), inputText("") 
	{
		tb_text.SetFont(m_font);
		tb_text.SetFontSize(20);
		tb_text.SetColor(MGRAY);
		tb_text.SetText(inputText);
		tb_text.SetSpacing(1.0f);
	}

	void Draw()
	{
		textBox.DrawRounded(0.5f, 8, LGRAY);

		if (!active) {

			tb_text.SetText(inputText);
			tb_text.Draw(static_cast<int>(textBox.x) + 5, static_cast<int>(textBox.y) + 5);
	
		}
		else {

			tb_text.SetText(inputText + '_');
			tb_text.Draw(static_cast<int>(textBox.x) + 5, static_cast<int>(textBox.y) + 5);
			
		}
	}

	void Update()
	{
		if ( CheckCollisionPointRec( GetMousePosition(), textBox ) )
		{
			if ( IsMouseButtonPressed(MOUSE_LEFT_BUTTON) )
			{
				active = true;
				//std::cout << "Clicked!" << std::endl;
			}
		}
		else if ( IsMouseButtonPressed(MOUSE_LEFT_BUTTON) )
		{
			active = false;
			//std::cout << "Clicked Outside!" << std::endl;
		}

		if (active)
		{
			int key = GetKeyPressed();
			while (key > 0) 
			{
				if ((key >= 32) && (key <= 125) && (inputText.length() < 20)) {
					inputText += static_cast<char>(key);
					tb_text.SetText(inputText);
				}
				else if (IsKeyPressed(KEY_BACKSPACE) && (inputText.length() > 0)) {
					inputText.pop_back();
					tb_text.SetText(inputText);
				}
				key = GetKeyPressed();
			}
		}

	}

};