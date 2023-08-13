#pragma once

#include <raylib-cpp.hpp>
#include "Globals.h"
#include <iostream>

class TextBox {
private:

	raylib::Rectangle textBox;
	bool active;
	std::string inputText;
	raylib::Text text;

public:

	TextBox()
	{
		active = false;
		inputText = "";
		text.SetFont(m_font);
		std::cout<<std::boolalpha;
		std::cout<<m_font.IsReady()<<std::endl;
		text.SetFontSize(20);
		text.SetColor(MGRAY);
		text.SetText(inputText);
		text.SetSpacing(1.0f);
	}

	void Draw(float x, float y, float width, float height)
	{
		textBox = raylib::Rectangle(x, y, width, height);
		textBox.DrawRounded(0.5f, 8, LGRAY);

		if (!active) {
			text.SetText(inputText);
			text.SetFont(m_font);
			text.Draw(static_cast<int>(textBox.x) + 5, static_cast<int>(textBox.y) + 5);
			// m_font.DrawText(text.GetText(), text.MeasureEx(), text.GetFontSize(), text.GetSpacing(), text.GetColor());
			//DrawText(inputText.c_str(), static_cast<int>(textBox.x) + 5, static_cast<int>(textBox.y) + 5, textBox.height - 20, MGRAY);
		}
		else {
			text.SetText(inputText + '_');
			// m_font.DrawText(text.GetText(), text.MeasureEx(), text.GetFontSize(), text.GetSpacing(), text.GetColor());
			text.SetFont(m_font);
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
				std::cout << std::endl << "Clicked!" << std::endl;
			}
		}
		else if ( IsMouseButtonPressed(MOUSE_LEFT_BUTTON) )
		{
			active = false;
			std::cout << "Clicked Outside!" << std::endl;
		}

		if (active)
		{
			int charrr = GetCharPressed();
			int key = GetKeyPressed();
			while (key > 0) 
			{
				if ((key >= 32) && (key <= 125) /*&& (inputText.length() < 20)*/) {
					inputText += static_cast<char>(charrr);
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