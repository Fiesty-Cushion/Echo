#include "raylib-cpp.hpp"

#include "Button.h"

#define MGRAY CLITERAL(Color) {225, 225, 225, 255}
#define MBG CLITERAL(Color) {240, 240, 240, 255}
#define LIGHTBLUE CLITERAL(Color) {229, 241, 251, 255}
#define LIGHTGRAY CLITERAL(Color) {173, 173, 173, 255}

int main() {
    // Initialization
    //--------------------------------------------------------------------------------------
    float screenWidth = 1000;
    float screenHeight = 600;
    raylib::Color textColor = raylib::Color::LightGray();
    raylib::Window window(screenWidth, screenHeight, "Echo - Speech To Text");
    raylib::Font font("./Resources/Fonts/RobotoMono-Regular.ttf");    
    
    Button button("Start", {100, 40}, RED, MGRAY, font);
    button.setPosition({ screenWidth / 2, screenHeight / 2 });

    SetTargetFPS(60);
    //--------------------------------------------------------------------------------------

    // Main game loop
    while (!window.ShouldClose()) {   // Detect window close button or ESC key
        // Update
        //----------------------------------------------------------------------------------
        // Update your variables here
        //----------------------------------------------------------------------------------

        if (button.isMouseOver())
        {
            button.setBackgroundColor(LIGHTBLUE);
        }
        else {
            button.setBackgroundColor(MGRAY);
        }

        if (button.isPressed())
        {
            std::cout << "Pressed" << std::endl;
        }

        // Draw
        //----------------------------------------------------------------------------------
        BeginDrawing();
        {
            window.ClearBackground(MBG);
            button.draw();
        }
        EndDrawing();
        //----------------------------------------------------------------------------------
    }

    return 0;
}