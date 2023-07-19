#include "GUI.h"

void GUI::Init()
{
	screenWidth = 1000;
	screenHeight = 600;
	textColor =  raylib::Color::LightGray();
	window = new raylib::Window(screenWidth, screenHeight, "Echo - Speech To Text");
	font =new raylib::Font("D:/Projects/C++/Echo/Resources/Fonts/RobotoMono-Regular.ttf");
	SetTargetFPS(60);

	button1 = Button("Start", { 100, 40 }, RED, MGRAY, *font);
	button2 = Button("Subtitle", { 100, 40 }, BLUE, MGRAY, *font);

	button1.setPosition({ screenWidth / 2, screenHeight / 2 });
	button2.setPosition({ screenWidth / 2, screenHeight / 2 + 50 });
}

void GUI::Draw()
{
	BeginDrawing();
	{
		window->ClearBackground(MBG);
		button1.draw();
		button2.draw();
	}
	EndDrawing();
}

void GUI::HandleEvents()
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

		audio->readPCMFromWav("D:/Projects/C++/Echo/Audio/audio/4507-16021-0012.wav", pcm32fWav, pcmf32sWav, stereo);
		transcriber->TranscribeFromWav(pcm32fWav, 1);
	}
}

void GUI::StartLoop()
{
	while (!window->ShouldClose())
	{
		HandleEvents();
		Draw();
	}
}

void GUI::ShutDown()
{
	delete window;
	delete font;
}
