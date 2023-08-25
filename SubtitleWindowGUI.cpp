#include "SubtitleWindowGUI.h"
#include "Globals.h"
#include "Button.h"
#include "VideoUtils.h"

#include <iostream>
#include <nfd.h>
#include <raygui.h>


void SubtitleWindowGUI::Init()
{
	setupDisplayText(subtitleText, "Subtitle Generation");
	setupDisplayText(promptText, "Import a File to Get Started", 20);
	promptText.SetColor(GRAY);

	subtitleButton = Button("Start", { 100, 40 }, MBG, MTEXT, m_font);
	subtitleButton.setPosition({ 542, 350 });

	outDirBoxButton = Button("", { 45, 45 }, MBG, MTEXT, m_font);
	outDirBoxButton.setPosition({ 1190, 525 });

	subtitleImportButton = Button("", { 500, 300 }, MBG, MTEXT, m_font);
	subtitleImportButton.setPosition({ 320, 540 });

	outDirBox = TextBox(750, 500, 400, 50);

	nfdchar_t* outPath = NULL;
	nfdresult_t result;

	setupDisplayText(srtExportText, "Save File To");
	audio = new Audio();
	transcriber = new Transcriber(*audio);
}

void SubtitleWindowGUI::Draw()
{
	subtitleText.Draw({ 72, 334 });
	subtitleButton.Draw("Start"); // should be disabled when process starts


	outDirBox.Draw();
	outDirBox.DrawTextBoxed(m_font, outDirBox.inputText.c_str(), Rectangle{ outDirBox.getX() + 10, outDirBox.getY() + 10 , outDirBox.getWidth() - 20, outDirBox.getHeight() - 20 }, 20, 1, false, MTEXT);

	srtExportText.Draw({ 750, 400 });
	outDirBoxButton.Draw("");

	if (!displayVideoFrame)
	{
		subtitleImportButton.Draw("");
		GuiDrawIcon(5, subtitleImportButton.getPosition().x + 170, subtitleImportButton.getPosition().y + 50, 10, GRAY);
		promptText.Draw({ 190, 620 });
	}
	else
	{
		frameImage = LoadImageFromTexture(frame);
		ImageResize(&frameImage, subtitleImportButton.getSize().x, subtitleImportButton.getSize().y);
		frame = LoadTextureFromImage(frameImage);
		DrawTexture(frame, subtitleImportButton.getPosition().x, subtitleImportButton.getPosition().y, WHITE);
	}

	GuiDrawIcon(2, outDirBoxButton.getPosition().x + 5, outDirBoxButton.getPosition().y + 5, 2, MTEXT);

}

void SubtitleWindowGUI::HandleEvents()
{
	// What to do when this button is Pressed?
	if (outDirBoxButton.isPressed())
	{
		//result = NFD_SaveDialog("mp4", NULL, &outPath);
		result = NFD_PickFolder("", &outPath);

		if (result == NFD_OKAY)
		{
			outDirBox.inputText = outPath;
			outputDirPath = outPath;
		}
		else if (result == NFD_CANCEL)
		{
			return;
		}
		else
		{
			printf("Error: %s\n", NFD_GetError());
		}
	}

	if (subtitleImportButton.isPressed())
	{
		result = NFD_OpenDialog("mp4", NULL, &outPath);

		if (result == NFD_OKAY)
		{
			std::cout << outPath << std::endl;
			subtitleInputPath = outPath;
			std::string mpegFile = StringUtils::extractFileName(subtitleInputPath) + ".mpeg";
			VideoUtils::ConvertToMpeg(subtitleInputPath, mpegFile);
			frame = VideoUtils::GetFrameFromVideo(mpegFile);
			displayVideoFrame = true;
			//std::cout << "Input File: " << outPath << std::endl;
		}
		else if (result == NFD_CANCEL)
		{
			return;
		}
		else
		{
			printf("Error: %s\n", NFD_GetError());
		}
	}

	if (subtitleButton.isPressed())
	{
		if (subtitleInputPath == "")
		{
			std::cout << "No Input File Selected" << std::endl;
			return;
		}
		else if (outputDirPath == "")
		{
			std::cout << "No Output Directory Selected" << std::endl;
			return;
		}
		else
		{
			std::cout << "Starting Subtitle Generation" << std::endl;

			transcriber->BurnInSubtitles(subtitleInputPath.c_str(), outputDirPath.c_str());
		}
	}

}

void SubtitleWindowGUI::ShutDown()
{

}