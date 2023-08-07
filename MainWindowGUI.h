#pragma once
#include "GUI.h"

class MainWindowGUI : public GUI
{
private:
	raylib::Text title_Text;
	raylib::Text model_Text;
	raylib::Rectangle sample_Box;

public:
	void Init();
	void StartLoop();
	void Draw();
	void HandleEvents();
	void ShutDown();
};