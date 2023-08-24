#pragma once

#include <string>
#include <raylib.h>
#include <raygui.h>
#include <raylib-cpp/Rectangle.hpp>
#include <raylib-cpp/Font.hpp>

#include "Globals.h"


class DropdownList
{
private:
	raylib::Rectangle dropdownList;

    int active = 0;
    bool editMode = false;
	int selected = 0;

public:

	DropdownList() { }

	DropdownList(int x, int y, int width, int height) : dropdownList(x, y, width, height)
	{
	}

	Screen getSelected()
	{
		if (active == 0)
		{
			return RealTime_Screen;
		}
		else if (active == 1)
		{
			return Subtitle_Screen;
		}
		else
		{
			return Karaoke_Screen;
		}
	}

	void Draw()
	{
        selected = GuiDropdownBox(Rectangle{ dropdownList.GetX(), dropdownList.GetY(), dropdownList.GetWidth(), dropdownList.GetHeight()}, "Real Time; Subtitle; Karaoke", &active, editMode);
	}

	bool isMouseOver()
	{
		Vector2 mousePos = GetMousePosition();

		bool isMouseOver = CheckCollisionPointRec(mousePos, dropdownList);
		return isMouseOver;
	}

	void Update()
	{
		if (isMouseOver() && IsMouseButtonPressed(0))
		{
			editMode = !editMode;
		}
	}
};