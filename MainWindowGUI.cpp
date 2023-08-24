#include "MainWindowGUI.h"
#include "Transcriber.h"
#include "Globals.h"
#include "TextBox.h"
#include "Button.h"
#include "DropdownList.h"

#include <math.h>
#include "Audio.h"


void MainWindowGUI::Init()
{
	screenWidth = 1280;
	screenHeight = 720;

	window = new raylib::Window(screenWidth, screenHeight, "Echo - Speech To Text");
	SetTargetFPS(120);

	m_font = raylib::Font(m_font_path);
	GenTextureMipmaps(&m_font.texture);
	SetTextureFilter(m_font.texture, TEXTURE_FILTER_ANISOTROPIC_8X);

	m_font_title = raylib::Font(m_font_path, 48);

	setupDisplayText(titleText, "Echo - Speech to Text", 48, m_font_title);
	setupDisplayText(modelSelectText, "Select a Language Model: ");
	setupDisplayText(sttText, "Real-Time Speech to Text: ");
	setupDisplayText(featuresText, "Features");

	dropdownList = DropdownList(930, 165, 200, 50);

	transcribeButton = Button("Start", { 100, 40 }, MBG, MTEXT, m_font);
	transcribeButton.setPosition({ 542, 350 });

	modelTextBox = TextBox(72, 170, 506.5, 45);
	outputTextBox = TextBox(70, 400, 1138, 277);

	wave.numCharacters = titleText.GetText().length();
	wave.characterOffset = new float[wave.numCharacters];

	for (int i = 0; i < wave.numCharacters; i++) {
		wave.characterOffset[i] = 0.0f;
	}

	audio = new Audio();
	transcriber = new Transcriber(*audio);

	// TEST ONLY
	karaokeButton = Button("Start", { 100, 40 }, MBG, MTEXT, m_font);
	karaokeButton.setPosition({ 542+100, 350 });

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
		sttText.Draw({ 72, 334 });

		featuresText.Draw({ 977, 110 });
		dropdownList.Draw();

		modelTextBox.Draw();
		outputTextBox.Draw();
		outputTextBox.DrawTextBoxed(m_font, outputTextBox.inputText.c_str(), Rectangle{outputTextBox.getX() + 10, outputTextBox.getY() + 10 , outputTextBox.getWidth() - 20, outputTextBox.getHeight() - 20}, 20, 1, true, MTEXT);

		transcribeButton.Draw(isTranscribing ? "Stop" : "Start");

		// TEST ONLY
		karaokeButton.Draw("Karaoke");

	}
	EndDrawing();

	for (int i = 0; i < wave.numCharacters; i++) {
		wave.characterOffset[i] = sinf(GetTime() * 3.0f + i * 3.0f) * 5.0f; // Adjust amplitude and speed
	}
}

void MainWindowGUI::HandleEvents()
{
	modelTextBox.Update();
	outputTextBox.Update();
	dropdownList.Update();


	//....Event Handlers....//
	if (transcribeButton.isPressed())
	{
		isTranscribing = !isTranscribing;

		// only start the stream once
		if (isInitialClick)
		{
			audio->StartStream(RealTime);
			isInitialClick = false;
		}

		if (isTranscribing)
			takeMicrophoneInput = true;
		else
			takeMicrophoneInput = false;

	}

	if (isTranscribing)
	{
		const std::vector<transcribed_msg>& transcribedData= transcriber->GetTranscribed();
		if (!transcribedData.empty())
		{
			outputTextBox.inputText += (transcribedData.back().is_partial || (transcribedData.back().text == outputTextBox.inputText)) ? "" : transcribedData.back().text;
			std::cout << outputTextBox.inputText;
			std::cout << std::flush;
		}
	}


	// TEST ONLY
	/*if (karaokeButton.isPressed()) {

		std::string inputPath = "./Samples/chacha.wav";
		std::string outputDir = "./Samples";

		transcriber->GenerateKaraoke(inputPath.c_str(), outputDir.c_str());
	}*/

	if (karaokeButton.isPressed())
	{
		std::string videoPath = "./Samples/dsblong.mp4";
		std::string outputDir = "./Samples";

		transcriber->BurnInSubtitles(videoPath.c_str(), outputDir.c_str());
		
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
	delete audio;
	delete transcriber;
	m_font.Unload();
	delete window;
}