#include "TextBox.h"

TextBox::TextBox(float x, float y, float width, float height)
{
	active = false;
	inputText = "";
	text.SetFont(m_font);
	text.SetFontSize(24);
	text.SetColor(MTEXT);
	text.SetText(inputText);
	text.SetSpacing(1.0f);

    textBox = raylib::Rectangle(x, y, width, height);
    textBoxBorder = raylib::Rectangle(x - 2, y - 2, width + 4, height + 4);
}

float TextBox::getX()
{
    return textBox.GetX();
}

float TextBox::getY()
{
    return textBox.GetY();
}

float TextBox::getWidth()
{
    return textBox.GetWidth();
}

float TextBox::getHeight()
{
    return textBox.GetHeight();
}

void TextBox::Draw()
{
    textBoxBorder.DrawRounded(0.2f, 8, MBORDER);
	textBox.DrawRounded(0.2f, 8, MBG);


	if (!active) {
		text.SetText(inputText);
            DrawTextBoxed(m_font, text.GetText().c_str(), textBox, 0.2, 1, true, RAYWHITE);
	}
	else {
		text.SetText(inputText + '_');
        DrawTextBoxed(m_font, text.GetText().c_str(), textBox, 0.2, 1, true, RAYWHITE);
	}
}

void TextBox::Update()
{
	if ( CheckCollisionPointRec( GetMousePosition(), textBox ) )
	{
		if ( IsMouseButtonPressed(MOUSE_LEFT_BUTTON) )
		{
			active = true;
		}
	}
	else if ( IsMouseButtonPressed(MOUSE_LEFT_BUTTON) )
	{
		active = false;
	}

	if (active)
	{
		int charrr = GetCharPressed();
		int key = GetKeyPressed();
		while (key > 0) 
		{
			if ( (key >= 32) && (key <= 125) ) {
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

void TextBox::DrawTextBoxed(Font font, const char* text, Rectangle rec, float fontSize, float spacing, bool wordWrap, Color tint)
{
    DrawTextBoxedSelectable(font, text, rec, fontSize, spacing, wordWrap, tint, 0, 0, WHITE, WHITE);
}