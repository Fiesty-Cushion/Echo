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

	raylib::Color textColor; 
	raylib::Window* window;
	raylib::Font* font;

	Button button1;
	Button button2;

	// Just for testing
	bool isRunning = false;
	Audio* audio = nullptr;
	Transcriber* transcriber = nullptr;

public:
	virtual void Init() = 0;

	virtual void StartLoop() = 0;

	virtual void Draw() = 0;
	virtual void HandleEvents() = 0;

	virtual void ShutDown() = 0;
};

