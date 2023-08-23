#pragma once

#include "raylib-cpp.hpp"
#include "Audio.h"
#pragma once

#include <raylib-cpp/Window.hpp>


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
