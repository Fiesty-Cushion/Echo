#pragma once
#include "raylib-cpp.hpp"
#include "Audio.h"
#include "Transcriber.h"
#include "Button.h"
#include <string>

class GUI
{
protected:
	float screenWidth;
	float screenHeight;

	const std::string fontFile = "./Resources/Fonts/OpenSans-SemiBold.ttf";

	raylib::Window* window;
	raylib::Font font;

public:
	virtual void Init() = 0;

	virtual void StartLoop() = 0;

	virtual void Draw() = 0;
	virtual void HandleEvents() = 0;

	virtual void ShutDown() = 0;
};