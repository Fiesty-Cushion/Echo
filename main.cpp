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

	return 0;
}