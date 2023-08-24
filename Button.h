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

	Button() {}
	Button(std::string , Vector2 , Color , Color , Font );
	void setTextColor(Color );
	void setPosition(Vector2 );
	void Draw(std::string );
	void setBackgroundColor(Color );

	bool isMouseOver();
	bool isPressed();

};