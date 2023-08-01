#include "MainWindowGUI.h"

void MainWindowGUI::Init()
{
	screenWidth = 1280;
	screenHeight = 720;

	textColor = raylib::Color::LightGray();
	window = new raylib::Window(screenWidth, screenHeight, "Echo - Speech To Text");
	
	
	font = new raylib::Font("./Resources/Fonts/RobotoMono-Regular.ttf");

	SetTargetFPS(60);

	button1 = Button("Let's Begin", { 100, 40 }, RED, MGRAY, *font);
	button2 = Button("Exit", { 100, 40 }, BLUE, MGRAY, *font);

	button1.setPosition({ screenWidth / 2, screenHeight / 2 });
	button2.setPosition({ screenWidth / 2, screenHeight / 2 + 50 });
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
		button1.draw();
		button2.draw();
	}
	EndDrawing();
}

void MainWindowGUI::HandleEvents()
{
	//....Hover Effect....//
	if (button1.isMouseOver())
	{
		button1.setBackgroundColor(LIGHTBLUE);
	}
	else
	{
		button1.setBackgroundColor(MGRAY);
	}

	if (button2.isMouseOver())
	{
		button2.setBackgroundColor(LIGHTBLUE);
	}
	else
	{
		button2.setBackgroundColor(MGRAY);
	}


	//....Event Handlers....//
	if (button1.isPressed())
	{
		std::cout << "Pressed" << std::endl;
		// For testing only
		if (!isRunning)
		{
			audio = new Audio();
			transcriber = new Transcriber();
			audio->StartStream(RealTime);
			transcriber->BeginRealTimeTranscription();
			isRunning = true;
		}
		else
		{
			delete audio;
			delete transcriber;
		}
	}
	if (button2.isPressed()) {
		std::cout << "Subtitles Button Pressed" << std::endl;

		std::vector<float> pcm32fWav;
		std::vector<std::vector<float>> pcmf32sWav;
		bool stereo = false;

		audio = new Audio();
		transcriber = new Transcriber();

		audio->readPCMFromWav("C:/Users/USER/Downloads/rain.wav", pcm32fWav, pcmf32sWav, stereo);
		transcriber->TranscribeFromWav(pcm32fWav, 1);
	}
}

void MainWindowGUI::ShutDown()
{
	delete window;
	delete font;
}
