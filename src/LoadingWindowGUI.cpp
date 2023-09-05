#include <iostream>
#include <pl_mpeg.h>

#include "GUI.h"
#include "LoadingWindowGUI.h"


void LoadingWindowGUI::Init()
{
	screenWidth = 1280;
	screenHeight = 720;

	SetConfigFlags(FLAG_WINDOW_UNDECORATED);
	SetConfigFlags(FLAG_WINDOW_TRANSPARENT);

	InitWindow(screenWidth, screenHeight, "Echo - Loading Screen");
	SetTargetFPS(120);

	plm = plm_create_with_filename("../assets/Intro.mpeg");

	if (!plm)
	{
		std::system("pwd");
		std::cerr << "Loading Window Path Not Found !!!" << std::endl;
		std::exit(-1);
	}

	framerate = plm_get_framerate(plm);

	plm_set_loop(plm, 1);

	int width = plm_get_width(plm);
	int height = plm_get_height(plm);

	plm_samples_t* sample = NULL;

	imFrame.width = width;
	imFrame.height = height;
	imFrame.format = PIXELFORMAT_UNCOMPRESSED_R8G8B8;
	imFrame.mipmaps = 1;
	imFrame.data = (unsigned char*)malloc(width * height * 3);

	vid_texture = LoadTextureFromImage(imFrame);

	int framesCounter = 0;

	baseTime = GetTime();    // Time since InitWindow()
	std::cout << std::endl << "init end" << std::endl;

}

void LoadingWindowGUI::StartLoop()
{
	while (!pause)
	{
		HandleEvents();
		Draw();
	}
}

void LoadingWindowGUI::Draw()
{
	BeginDrawing();

	ClearBackground(BLANK);

	DrawTexture(vid_texture, GetScreenWidth() / 2 - vid_texture.width / 2, GetScreenHeight() / 2 - vid_texture.height / 2, WHITE);

	EndDrawing();
}

void LoadingWindowGUI::HandleEvents()
{

	// Video should run at 'framerate' fps => One new frame every 1/framerate
	double time = (GetTime() - baseTime);

	if (time >= (1.0 / framerate))
	{
		baseTime = GetTime();

		// Decode video frame
		frame = plm_decode_video(plm);  // Get frame as 3 planes: Y, Cr, Cb
		if (frame != nullptr)
		{
			plm_frame_to_rgb(frame, static_cast<uint8_t*>(imFrame.data));  // Convert (Y, Cr, Cb) to RGB on the CPU (slow)

			// Update texture
			UpdateTexture(vid_texture, static_cast<uint8_t*>(imFrame.data));
		}
		else
		{
			pause = true;
		}

	}


}

void LoadingWindowGUI::ShutDown()
{
	UnloadImage(imFrame);
	UnloadTexture(vid_texture);

	plm_destroy(plm);

	CloseWindow();
	window->ClearState(FLAG_WINDOW_UNDECORATED);
	window->ClearState(FLAG_WINDOW_TRANSPARENT);
}

