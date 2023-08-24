#include "DropdownList.h"

DropdownList::DropdownList(int x, int y, int width, int height) : dropdownList(x, y, width, height)
{
	raygui_font = raylib::Font(m_font_path.c_str(), 24);
	GuiSetFont(raygui_font);
	GuiSetStyle(DROPDOWNBOX, BASE_COLOR_NORMAL, 0xEEEEEEFF);
	GuiSetStyle(DROPDOWNBOX, TEXT_COLOR_NORMAL, 0x1D2B53FF);
	GuiSetStyle(DROPDOWNBOX, BASE_COLOR_FOCUSED, 0x00000000);
	GuiSetStyle(DROPDOWNBOX, BASE_COLOR_PRESSED, 0x00000000);
	GuiSetStyle(DROPDOWNBOX, TEXT_ALIGNMENT, TEXT_ALIGN_CENTER);
	GuiSetStyle(DROPDOWNBOX, ARROW_PADDING, 30);

}

void DropdownList::Draw()
{
    selected = GuiDropdownBox(Rectangle{ dropdownList.GetX(), dropdownList.GetY(), dropdownList.GetWidth(), dropdownList.GetHeight()}, "Real Time; Subtitle;Karaoke", &active, editMode);
}

bool DropdownList::isMouseOver()
{
	Vector2 mousePos = GetMousePosition();

	bool isMouseOver = CheckCollisionPointRec(mousePos, dropdownList);
	return isMouseOver;
}

void DropdownList::Update()
{
	if (isMouseOver() && IsMouseButtonPressed(0))
	{
	editMode = !editMode;
	}
}