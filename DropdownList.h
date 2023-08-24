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
	DropdownList(int , int , int , int );

	void Draw();
	void Update();

	bool isMouseOver();
};