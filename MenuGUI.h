#include "GUI.h"

class MenuGUI : public GUI
{
public:
     void Draw() override {
        BeginDrawing();
	    {
		window->ClearBackground(MBG);
		button1.draw();
		button2.draw();
        DrawLine(0,  screenHeight / 2,  screenWidth, screenHeight / 2, RED);
        DrawLine(screenWidth / 2, 0 ,  screenWidth / 2,  screenHeight,  RED);
	    }
	    EndDrawing();
    }

};