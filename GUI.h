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

	raylib::Window* window;

public:
	virtual void Init() = 0;

	virtual void StartLoop() = 0;

	virtual void Draw() = 0;
	virtual void HandleEvents() = 0;

	virtual void ShutDown() = 0;
};
