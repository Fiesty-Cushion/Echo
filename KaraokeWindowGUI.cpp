#include "KaraokeWindowGUI.h"
#include "Globals.h"
#include "Button.h"
#include "VideoUtils.h"

#include <iostream>
#include <nfd.h>
#include <raygui.h>

void KaraokeWindowGUI::Init()
{
	setupDisplayText(karaokeText, "Karaoke Generation");
	setupDisplayText(promptText, "Import a File to Get Started", 20);
	promptText.SetColor(GRAY);

	karaokeButton = Button("Start", { 100, 40 }, MBG, MTEXT, m_font);
	karaokeButton.setPosition({ 542, 350 });

	outDirBoxButton = Button("", { 45, 45 }, MBG, MTEXT, m_font);
	outDirBoxButton.setPosition({ 1190, 525 });

	karaokeImportButton = Button("", { 500, 300 }, MBG, MTEXT, m_font);
	karaokeImportButton.setPosition({ 320, 540 });

	outDirBox = TextBox(750, 500, 400, 50);

	nfdchar_t* outPath = NULL;
	nfdresult_t result;

	karExportIcon = raylib::Image("./Resources/save.png");
	karExportIcon.Resize(outDirBoxButton.getSize().x - 10, outDirBoxButton.getSize().y - 10);
	karExportIconTexture = LoadTextureFromImage(karExportIcon);
	karExportIcon.Unload();

	setupDisplayText(karExportText, "Save File To");

	audio = new Audio();
	transcriber = new Transcriber(*audio);

}

void KaraokeWindowGUI::Draw()
{
	karaokeText.Draw({ 72, 334 });
	karaokeButton.Draw("Start"); // Disable this when state changes

	//DrawRectangle(70, 410, 500, 270, MBG);
	outDirBox.Draw();
	outDirBox.DrawTextBoxed(m_font, outDirBox.inputText.c_str(), Rectangle{ outDirBox.getX() + 10, outDirBox.getY() + 10 , outDirBox.getWidth() - 20, outDirBox.getHeight() - 20 }, 20, 1, false, MTEXT);

	karExportText.Draw({ 750, 400 });
	outDirBoxButton.Draw("");
	//DrawTexture(karExportIconTexture, outDirBoxButton.getPosition().x + 5, outDirBoxButton.getPosition().y + 5, MPINK);
	if (!displayVideoFrame)
	{
		karaokeImportButton.Draw("");
		GuiDrawIcon(5, karaokeImportButton.getPosition().x + 170, karaokeImportButton.getPosition().y + 50, 10, GRAY);
		promptText.Draw({ 190, 620 });
	}
	else
	{
		frameImage = LoadImageFromTexture(frame);
		ImageResize(&frameImage, karaokeImportButton.getSize().x, karaokeImportButton.getSize().y);
		frame = LoadTextureFromImage(frameImage);
		DrawTexture(frame, karaokeImportButton.getPosition().x, karaokeImportButton.getPosition().y, WHITE);
	}

	GuiDrawIcon(2, outDirBoxButton.getPosition().x + 5, outDirBoxButton.getPosition().y + 5, 2, MTEXT);
}

void KaraokeWindowGUI::HandleEvents()
{
	// What to do when this button is Pressed?
	if (outDirBoxButton.isPressed())
	{
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


	if (karaokeImportButton.isPressed())
	{
		result = NFD_OpenDialog("wav", NULL, &outPath);

		if (result == NFD_OKAY)
		{
			std::cout << outPath << std::endl;
			karaokeInputPath = outPath;
			/*std::string mpegFile = StringUtils::extractFileName(karaokeInputPath) + ".mpeg";
			VideoUtils::ConvertToMpeg(karaokeInputPath, mpegFile);
			frame = VideoUtils::GetFrameFromVideo(mpegFile);*/
			displayVideoFrame = false;
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


	if (karaokeButton.isPressed())
	{
		if (karaokeInputPath == "")
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

			transcriber->BurnInSubtitles(karaokeInputPath.c_str(), outputDirPath.c_str());
		}
	}

}

void KaraokeWindowGUI::ShutDown()
{

}