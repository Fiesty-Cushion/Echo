#pragma once
#include "raylib-cpp.hpp"
#include "Audio.h"
#include "Transcriber.h"
#include "Button.h"
#include <string>

#define MGRAY CLITERAL(Color) {225, 225, 225, 255}
#define MBG CLITERAL(Color) {240, 240, 240, 255}
#define LIGHTBLUE CLITERAL(Color) {229, 241, 251, 255}
#define LIGHTGRAY CLITERAL(Color) {173, 173, 173, 255}
class GUI
{

private:
	float screenWidth;
	float screenHeight;
	raylib::Color textColor; 
	raylib::Window* window;
	raylib::Font* font;

	void Draw();
	void HandleEvents();


	Button button1;
	Button button2;

	// Just for testing
	bool isRunning = false;
	Audio* audio = nullptr;
	Transcriber* transcriber = nullptr;

public:
	void Init();
	void StartLoop();
	void ShutDown();
};