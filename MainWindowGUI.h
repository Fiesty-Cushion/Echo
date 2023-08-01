#pragma once
#include "GUI.h"

class MainWindowGUI : public GUI
{

public:
	void Init();
	void StartLoop();
	void Draw();
	void HandleEvents();
	void ShutDown();
};