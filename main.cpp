#pragma once
#include "GUI.h"
#include "Audio.h"
#include "Transcriber.h"
#include "Button.h"



int main()
{
	GUI ui;
	ui.Init();
	ui.StartLoop();
	ui.ShutDown();
	return 0;
}