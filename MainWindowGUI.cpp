#include "MainWindowGUI.h"
#include "Globals.h"
#include "TextBox.h"
#include "DropdownList.h"

#include <math.h>


void MainWindowGUI::Init()
{
	screenWidth = 1280;
	screenHeight = 720;

	window = new raylib::Window(screenWidth, screenHeight, "Echo - Speech To Text");
	SetTargetFPS(120);

	screen = RealTime_Screen;

	m_font = raylib::Font(m_font_path);
	GenTextureMipmaps(&m_font.texture);
	SetTextureFilter(m_font.texture, TEXTURE_FILTER_ANISOTROPIC_8X);

	m_font_title = raylib::Font(m_font_path, 48);

	setupDisplayText(titleText, "Echo - Speech to Text", 48, m_font_title);
	setupDisplayText(modelSelectText, "Select a Language Model: ");
	setupDisplayText(featuresText, "Features");

	dropdownList = DropdownList(930, 165, 200, 50);

	modelTextBox = TextBox(72, 170, 506.5, 45);

	wave.numCharacters = titleText.GetText().length();
	wave.characterOffset = new float[wave.numCharacters];

	for (int i = 0; i < wave.numCharacters; i++) {
		wave.characterOffset[i] = 0.0f;
	}

	sttScreen.Init();
	subtitleScreen.Init();
	karaokeScreen.Init();
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
		window->ClearBackground(RAYWHITE);

		addHoverEffect(titleText, &wave, (screenWidth / 2 - titleText.MeasureEx().GetX() / 2) - 20, 10);

		modelSelectText.Draw({ 72, 114 });
		modelTextBox.Draw();
		
		featuresText.Draw({ 977, 110 });
		dropdownList.Draw();

		screen = dropdownList.getSelected();

		if (screen == RealTime_Screen)
		{
			sttScreen.Draw();
		} 
		else if (screen == Subtitle_Screen)
		{
			subtitleScreen.Draw();
		}
		else if (screen == Karaoke_Screen)
		{
			karaokeScreen.Draw();
		}
		

	}
	EndDrawing();

	for (int i = 0; i < wave.numCharacters; i++) {
		wave.characterOffset[i] = sinf(GetTime() * 3.0f + i * 3.0f) * 5.0f; // Adjust amplitude and speed
	}
}

void MainWindowGUI::HandleEvents()
{
	modelTextBox.Update();
	dropdownList.Update();

	if (screen == RealTime_Screen)
	{
		sttScreen.HandleEvents();
	}
	else if (screen == Subtitle_Screen)
	{
		subtitleScreen.HandleEvents();
	}
	else if (screen == Karaoke_Screen)
	{
		karaokeScreen.HandleEvents();		
	}

	// FOR TRANSCRIPTION FROM WAV FILE //
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