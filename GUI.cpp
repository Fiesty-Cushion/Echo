#include "raylib-cpp.hpp"

#include "Button.h"

#define MGRAY CLITERAL(Color) {225, 225, 225, 255}
#define MBG CLITERAL(Color) {240, 240, 240, 255}
#define LIGHTBLUE CLITERAL(Color) {229, 241, 251, 255}
// #define LIGHTGRAY CLITERAL(Color) {173, 173, 173, 255}

// class Screen {
// 	private:
//         float screenWidth = 1000;
//         float screenHeight = 600;
//         raylib::Color textColor = raylib::Color::LightGray();
//         raylib::Window window({1000, 800}, "Echo - Speech To Text");
//         raylib::Font font("./Resources/Fonts/RobotoMono-Regular.ttf"); 


// 	public:
// 		void startScreen() {
            
//             Button button("Start", {100, 40}, RED, MGRAY, font);
//             button.setPosition({ screenWidth / 2, screenHeight / 2 });
        
//         }

//         void choiceScreen() {
            
//             Button b1("O{ screenWidth / 2, screenHeight / 3 });
//             b2.draw();

//             Button b3("Option-3", {100, 40}, RED, MGRAY, font);
//             b3.setPosition({ sption-1", {100, 40}, RED, MGRAY, font);
//             b1.setPosition({ screenWidth / 2, screenHeight / 6 });
//             b1.draw();

//             Button b2("Option-2", {100, 40}, RED, MGRAY, font);
//             b2.setPosition(creenWidth / 2, screenHeight / 2 });
//             b3.draw();

//         }

//         void exitScreen() {
//             Button bn("Thank YOU", {100, 40}, WHITE, BLACK, font);
//             bn.setPosition({screenWidth / 2, screenHeight / 2});
//         }

//         bool isPressed() {

//             if (button.isPressed())
//             {
//             std::cout << "Pressed" << std::endl;
//             return true;
//             }
            
//         }
// };

class Screen {
    private:
        Button button;

    public:
        

};

int main() {
    // Initialization
    //--------------------------------------------------------------------------------------
    float screenWidth = 1000;
    float screenHeight = 600;
    raylib::Color textColor = raylib::Color::LightGray();
    raylib::Window window(screenWidth, screenHeight, "Echo - Speech To Text");
    raylib::Font font("./Resources/Fonts/RobotoMono-Regular.ttf");

    // Screen screen();

    Button button("Start", {100, 40}, RED, MGRAY, font);
    button.setPosition({ screenWidth / 2, screenHeight / 2 });

    Button b1("Option-1", {100, 40}, RED, MGRAY, font);
    b1.setPosition({ screenWidth / 2, screenHeight / 6 });
    
    Button b2("Option-2", {100, 40}, RED, MGRAY, font);
    b2.setPosition({ screenWidth / 2, screenHeight / 3 }); 

    Button b3("Option-3", {100, 40}, RED, MGRAY, font);
    b3.setPosition({ screenWidth / 2, screenHeight / 2 });

    Button bn("Thank YOU", {100, 40}, WHITE, BLACK, font);
    bn.setPosition({screenWidth / 2, screenHeight / 2});


    int k = 0;

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
        switch (k)
        {
            case 0:
                // screen.startScreen();
                button.draw();
                // Check for transition conditions
                if (button.isPressed())
                    k = 1;
                break;

            case 1:
                window.ClearBackground(MBG);
                b1.draw();
                b2.draw();
                b3.draw();

                // screen.choiceScreen();

                // Check for transition conditions
                if (b1.isPressed())
                    k = 2;
                break;

            case 2:

                window.ClearBackground(MBG);
                bn.draw();

                // screen.exitScreen();

                // Check for transition conditions
                if (bn.isPressed())
                    k =0;
                break;
        }
        }
        EndDrawing();
        //----------------------------------------------------------------------------------
    
    }

    return 0;
}