#include "Button.h"


Button::Button(std::string text, Vector2 size, Color bgColor, Color textColor, Font font)
{
	btnText.text = text;
	btnText.font = font;
	btnText.fontSize = 18;
	btnText.color = BLACK;

	button.SetSize(size);

	_color = bgColor;
	_font = font;
}

void Button::setTextColor(Color color)
{
	btnText.color = color;
}

void Button::setPosition(Vector2 pos)
{
	pos = { pos.x - button.GetWidth() / 2,pos.y - button.GetHeight() / 2 };

	button.SetPosition(pos);

	Vector2 textPos = MeasureTextEx(_font,btnText.text.c_str(), btnText.GetFontSize(), btnText.GetSpacing());

	textX = (pos.x + button.GetWidth() / 2) - (textPos.x / 2);
	textY = (pos.y + button.GetHeight() / 2) - (textPos.y / 2);
}

void Button::Draw(std::string buttonText) {
	DrawRectangleLines(button.GetX() - 1,button.GetY() - 1, button.GetWidth() + 2,button.GetHeight() + 2, LIGHTGRAY);
	button.Draw(_color);
	btnText.SetText(buttonText);
	btnText.Draw(textX, textY);
}

bool Button::isMouseOver()
{
	Vector2 mousePos = GetMousePosition();

	bool isMouseOver = CheckCollisionPointRec(mousePos, button);
	return isMouseOver;
}

bool Button::isPressed()
{
	if (IsMouseButtonPressed(0) && isMouseOver())
	{
		return true;
	}
	return false;
}

void Button::setBackgroundColor(Color color)
{
	_color = color;
}