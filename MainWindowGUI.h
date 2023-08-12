#pragma once
#include "GUI.h"
#include "TextBox.h"

class MainWindowGUI : public GUI
{
private:
	raylib::Text title_Text;
	raylib::Text model_Text;
	TextBox text_box;
	

public:
	void Init();
	void StartLoop();
	void Draw();
	void HandleEvents();
	void ShutDown();
};