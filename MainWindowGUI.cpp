#include "MainWindowGUI.h"
#include "TextBox.h"
#include "Globals.h"
#include "raylib-cpp.hpp"

void MainWindowGUI::Init()
{
	screenWidth = 1280;
	screenHeight = 720;

	window = new raylib::Window(screenWidth, screenHeight, "Echo - Speech To Text");
	SetTargetFPS(60);
	m_font = raylib::Font(m_font_path);
	//raylib::Font m_font = raylib::Font(m_font_path);

	title_Text.SetText("Echo - Speech to Text");
	title_Text.SetFont(m_font);
	title_Text.SetFontSize(40);
	title_Text.SetSpacing(5);

	model_Text.SetText("Select a Language Model: ");
	model_Text.SetFont(m_font);
	model_Text.SetFontSize(25);
	model_Text.SetSpacing(3);

	//sample_Box.SetPosition({ 72, 194.7 });
	//sample_Box.SetSize({ 506.5, 45 });

	text_box = TextBox(72, 194.7, 506.5, 45);



	// button1 = Button("Let's Begin", { 100, 40 }, RED, MGRAY, *font);
	// button2 = Button("Exit", { 100, 40 }, BLUE, MGRAY, *font);

	// button1.setPosition({ screenWidth / 2, 2 * screenHeight / 3 });
	// button2.setPosition({ screenWidth / 2, screenHeight / 2 + 50 });
}

void MainWindowGUI::StartLoop()
{
	while (!window->ShouldClose())
	{
		HandleEvents();
		Draw();
	}
}

void MainWindowGUI::Draw()
{
	BeginDrawing();
	{

		window->ClearBackground(MBG);

		title_Text.Draw({ static_cast<float>( screenWidth/2 - title_Text.MeasureEx().GetX()/2 ), 56.2 });
		model_Text.Draw({ 72, 142.8 });

		text_box.Draw();

		DrawLine(0, screenHeight / 2, screenWidth, screenHeight / 2, RED);
		DrawLine(screenWidth / 2, 0, screenWidth / 2, screenHeight, RED);

		// button1.draw();
		// button2.draw();
	}
	EndDrawing();
}

void MainWindowGUI::HandleEvents()
{
	text_box.Update();
	// //....Hover Effect....//
	// if (button1.isMouseOver())
	// {
	// 	button1.setBackgroundColor(LIGHTBLUE);
	// }
	// else
	// {
	// 	button1.setBackgroundColor(MGRAY);
	// }

	// if (button2.isMouseOver())
	// {
	// 	button2.setBackgroundColor(LIGHTBLUE);
	// }
	// else
	// {
	// 	button2.setBackgroundColor(MGRAY);
	// }


	//....Event Handlers....//
	// if (button1.isPressed())
	// {
	// 	std::cout << "Pressed" << std::endl;
	// 	// For testing only
	// 	if (!isRunning)
	// 	{
	// 		audio = new Audio();
	// 		transcriber = new Transcriber();
	// 		audio->StartStream(RealTime);
	// 		transcriber->BeginRealTimeTranscription();
	// 		isRunning = true;
	// 	}
	// 	else
	// 	{
	// 		delete audio;
	// 		delete transcriber;
	// 	}
	// }
	// if (button2.isPressed()) {
	// 	std::cout << "Subtitles Button Pressed" << std::endl;

	// 	std::vector<float> pcm32fWav;
	// 	std::vector<std::vector<float>> pcmf32sWav;
	// 	bool stereo = false;

	// 	audio = new Audio();
	// 	transcriber = new Transcriber();

	// 	audio->readPCMFromWav("/Users/macbook/my_Files/Code/Echo/Audio/Recording.wav", pcm32fWav, pcmf32sWav, stereo);
	// 	transcriber->TranscribeFromWav(pcm32fWav, 1);
	// }
}

void MainWindowGUI::ShutDown()
{
	delete window;
	m_font.Unload();
}