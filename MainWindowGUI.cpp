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
	
	setupDisplayText(title_Text, "Echo - Speech to Text", 40);
	setupDisplayText(model_Text, "Select a Language Model: ");
	setupDisplayText(display_Text, "Real-Time Text to Speech: ");
	setupDisplayText(subtitle_Text, "Subtitle Generation: ");
	setupDisplayText(lyrics_Text, "Lyrics Generation: ");
	setupDisplayText(karaoke_Text, "Karaoke Generation: ");


	subtitle_Box.SetPosition({ 926.7, 189.3 });
	subtitle_Box.SetSize({ 281.3, 54.6 });
	lyrics_Box.SetPosition({ 926.7, 330.1 });
	lyrics_Box.SetSize({ 281.3, 54.6 });
	karaoke_Box.SetPosition({ 926.7, 471 });
	karaoke_Box.SetSize({ 281.3, 54.6 });

	


	

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
		display_Text.Draw({ 72, 308.3 });
		subtitle_Text.Draw({ 926.7, 142.8 });
		lyrics_Text.Draw({ 926.7, 283.9 });
		karaoke_Text.Draw({ 926.7, 424.8 });

		// std::cout << (screenWidth / 2) - (title_Text.MeasureEx().GetX() / 2) << std::endl;

		model_Text.Draw({72, 142.8});

		modelTextBox.Draw( 72, 194.7 , 250, 50);
		// outputTextBox.Draw( 506.5, 45 , 50, 50);

		subtitle_Box.Draw(LGRAY);
		lyrics_Box.Draw(LGRAY);
		karaoke_Box.Draw(LGRAY);

		DrawLine(0, screenHeight / 2, screenWidth, screenHeight / 2, RED);
		DrawLine(screenWidth / 2, 0, screenWidth / 2, screenHeight, RED);
		
		// button1.draw();
		// button2.draw();
	}
	EndDrawing();
}

void MainWindowGUI::HandleEvents()
{

	modelTextBox.Update();
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
	m_font.Unload();
	delete window;
}