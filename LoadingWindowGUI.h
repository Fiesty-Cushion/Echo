#pragma once
#include "GUI.h"
#include "pl_mpeg.h"

class LoadingWindowGUI : public GUI
{
protected:
	int screenWidth;
	int screenHeight;
	bool pause = false;

	plm_t* plm;
	plm_frame_t* frame = NULL;

	Texture texture;
	Image imFrame = { 0 };

	double framerate;
	double baseTime;

public:
	void Init();
	void StartLoop();
	void Draw();
	void HandleEvents();
	void ShutDown();
};