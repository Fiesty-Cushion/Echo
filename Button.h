#pragma once

#include <string>
#include <raylib.h>
#include <raylib-cpp/Rectangle.hpp>
#include <raylib-cpp/Text.hpp>


class Button {
private:
	raylib::Rectangle button;
	raylib::Text btnText;
	Color _color;
	Font _font;

public:
	float textX;
	float textY;

	Button() : button({0,0,0,0}), btnText(""), _color(BLANK), _font(GetFontDefault()) { }

	Button(std::string text, Vector2 size, Color bgColor, Color textColor, Font font)
	{
		btnText.text = text;
		btnText.font = font;
		btnText.fontSize = 18;
		btnText.color = BLACK;

		button.SetSize(size);

		_color = bgColor;
		_font = font;
	}

	void setTextColor(Color color)
	{
		btnText.color = color;
	}

	void setPosition(Vector2 pos)
	{
		pos = { pos.x - button.GetWidth() / 2, pos.y - button.GetHeight() / 2 };

		button.SetPosition(pos);

		Vector2 textPos = MeasureTextEx(_font, btnText.text.c_str(), btnText.GetFontSize(), btnText.GetSpacing());

		textX = (pos.x + button.GetWidth() / 2) - (textPos.x / 2);
		textY = (pos.y + button.GetHeight() / 2) - (textPos.y / 2);

	}

	void Draw(std::string buttonText) {
		DrawRectangleLines(button.GetX() - 1, button.GetY() - 1, button.GetWidth() + 2, button.GetHeight() + 2, LIGHTGRAY);
		button.Draw(_color);
		btnText.SetText(buttonText);
		btnText.Draw(textX, textY);
	}

	bool isMouseOver()
	{
		Vector2 mousePos = GetMousePosition();

		bool isMouseOver = CheckCollisionPointRec(mousePos, button);
		return isMouseOver;
	}

	bool isPressed()
	{
		if (IsMouseButtonPressed(0) && isMouseOver())
		{
			return true;
		}
		return false;
	}

	void setBackgroundColor(Color color)
	{
		_color = color;
	}
};