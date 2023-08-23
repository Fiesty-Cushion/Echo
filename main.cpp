#define PL_MPEG_IMPLEMENTATION
#define _CRT_SECURE_NO_WARNINGS

#pragma once
#include "LoadingWindowGUI.h"
#include "MainWindowGUI.h"


int main()
{
	LoadingWindowGUI lGUI;
	lGUI.Init();
	lGUI.StartLoop();
	lGUI.ShutDown();

	MainWindowGUI mGUI;
	mGUI.Init();
	mGUI.StartLoop();
	mGUI.ShutDown();

	// MenuGUI opGUI;
	// opGUI.Draw();

	return 0;
}