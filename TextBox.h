#pragma once

#include <raylib-cpp.hpp>
#include "Globals.h"
#include <iostream>

class TextBox {
private:

	raylib::Rectangle textBox;
	bool active;
	
	raylib::Text text;

    // Draw text using font inside rectangle limits with support for text selection
    static void DrawTextBoxedSelectable(Font font, const char* text, Rectangle rec, float fontSize, float spacing, bool wordWrap, Color tint, int selectStart, int selectLength, Color selectTint, Color selectBackTint)
    {
        int length = TextLength(text);  // Total length in bytes of the text, scanned by codepoints in loop

        float textOffsetY = 0;          // Offset between lines (on line break '\n')
        float textOffsetX = 0.0f;       // Offset X to next character to draw

        float scaleFactor = fontSize / (float)font.baseSize;     // Character rectangle scaling factor

        // Word/character wrapping mechanism variables
        enum { MEASURE_STATE = 0, DRAW_STATE = 1 };
        int state = MEASURE_STATE;

        int startLine = -1;         // Index where to begin drawing (where a line begins)
        int endLine = -1;           // Index where to stop drawing (where a line ends)
        int lastk = -1;             // Holds last value of the character position

        for (int i = 0, k = 0; i < length; i++, k++)
        {
            // Get next codepoint from byte string and glyph index in font
            int codepointByteCount = 0;
            int codepoint = GetCodepoint(&text[i], &codepointByteCount);
            int index = GetGlyphIndex(font, codepoint);

            if (codepoint == 0x3f) codepointByteCount = 1;
            i += (codepointByteCount - 1);

            float glyphWidth = 0;
            if (codepoint != '\n')
            {
                glyphWidth = (font.glyphs[index].advanceX == 0) ? font.recs[index].width * scaleFactor : font.glyphs[index].advanceX * scaleFactor;

                if (i + 1 < length) glyphWidth = glyphWidth + spacing;
            }

            if (state == MEASURE_STATE)
            {
                if ((codepoint == ' ')) endLine = i;

                if ((textOffsetX + glyphWidth) > rec.width)
                {
                    endLine = (endLine < 1) ? i : endLine;
                    if (i == endLine) endLine -= codepointByteCount;
                    if ((startLine + codepointByteCount) == endLine) endLine = (i - codepointByteCount);

                    state = !state;
                }
                else if ((i + 1) == length)
                {
                    endLine = i;
                    state = !state;
                }

                if (state == DRAW_STATE)
                {
                    textOffsetX = 0;
                    i = startLine;
                    glyphWidth = 0;

                    // Save character position when we switch states
                    int tmp = lastk;
                    lastk = k - 1;
                    k = tmp;
                }
            }
            else
            {
                if (!wordWrap && ((textOffsetX + glyphWidth) > rec.width))
                {
                    textOffsetY += (font.baseSize + font.baseSize / 2) * scaleFactor;
                    textOffsetX = 0;
                }

                // When text overflows rectangle height limit, just stop drawing
                if ((textOffsetY + font.baseSize * scaleFactor) > rec.height) break;

                // Draw selection background
                bool isGlyphSelected = false;
                if ((selectStart >= 0) && (k >= selectStart) && (k < (selectStart + selectLength)))
                {
                    DrawRectangleRec(Rectangle{ rec.x + textOffsetX - 1, rec.y + textOffsetY, glyphWidth, (float)font.baseSize * scaleFactor }, selectBackTint);
                    isGlyphSelected = true;
                }

                // Draw current character glyph
                if ((codepoint != ' '))
                {
                    DrawTextCodepoint(font, codepoint, Vector2{ rec.x + textOffsetX, rec.y + textOffsetY }, fontSize, isGlyphSelected ? selectTint : tint);
                }


                if (wordWrap && (i == endLine))
                {
                    textOffsetY += (font.baseSize + font.baseSize / 2) * scaleFactor;
                    textOffsetX = 0;
                    startLine = endLine;
                    endLine = -1;
                    glyphWidth = 0;
                    selectStart += lastk - k;
                    k = lastk;

                    state = !state;
                }
            }

            if ((textOffsetX != 0) || (codepoint != ' ')) textOffsetX += glyphWidth;  // avoid leading spaces
        }
    }

public:
	std::string inputText;

	TextBox()
	{
		active = false;
		inputText = "";
		text.SetFont(m_font);
		text.SetFontSize(20);
		text.SetColor(MGRAY);
		text.SetText(inputText);
		text.SetSpacing(1.0f);
	}

	void Draw(float x, float y, float width, float height)
	{
		textBox = raylib::Rectangle(x, y, width, height);
		textBox.DrawRounded(0.2f, 8, LGRAY);

		if (!active) {
			text.SetText(inputText);
			//text.SetFont(m_font);
			//text.Draw(static_cast<int>(textBox.x) + 5, static_cast<int>(textBox.y) + 5);
			 //m_font.DrawText(text.GetText(), static_cast<int>(textBox.x) + 5, static_cast<int>(textBox.y) + 5, text.GetFontSize(), text.GetSpacing(), text.GetColor());
			//DrawText(inputText.c_str(), static_cast<int>(textBox.x) + 5, static_cast<int>(textBox.y) + 5, textBox.height - 20, MGRAY);
		}
		else {
			text.SetText(inputText + '_');
			//m_font.DrawText(text.GetText(), static_cast<int>(textBox.x) + 5, static_cast<int>(textBox.y) + 5, text.GetFontSize(), text.GetSpacing(), text.GetColor());
			
			//text.SetFont(m_font);
			//text.Draw(static_cast<int>(textBox.x) + 5, static_cast<int>(textBox.y) + 5);
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

    static void DrawTextBoxed(Font font, const char* text, Rectangle rec, float fontSize, float spacing, bool wordWrap, Color tint)
    {
        DrawTextBoxedSelectable(font, text, rec, fontSize, spacing, wordWrap, tint, 0, 0, WHITE, WHITE);
    }

};