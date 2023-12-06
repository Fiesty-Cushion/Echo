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
}

void SubtitleWindowGUI::Draw()
{
	subtitleText.Draw({ 72, 334 });
	subtitleButton.Draw("Start"); // should be disabled when process starts


	outDirBox.Draw();
	outDirBox.DrawTextBoxed(m_font, outDirBox.inputText.c_str(), Rectangle{ outDirBox.getX() + 10, outDirBox.getY() + 10 , outDirBox.getWidth() - 20, outDirBox.getHeight() - 20 }, 20, 1, false, MTEXT);

	srtExportText.Draw({ 750, 400 });
	outDirBoxButton.Draw("");

	subtitleImportButton.Draw("");

	switch (fileStatus)
	{
	case FileStatus::Default:
		setupDisplayText(promptText, "Import a File to Get Started", 20);
		promptText.SetColor(GRAY);
		GuiDrawIcon(5, subtitleImportButton.getPosition().x + 170, subtitleImportButton.getPosition().y + 50, 10, GRAY);
		break;
	case FileStatus::FileReceived:
		setupDisplayText(promptText, subtitleInputPath, 20);
		promptText.SetColor(DARKTURQOUISE);
		GuiDrawIcon(5, subtitleImportButton.getPosition().x + 170, subtitleImportButton.getPosition().y + 50, 10, DARKTURQOUISE);
		break;
	case FileStatus::ProcessComplete:
		setupDisplayText(promptText, "Subtitle Generation Complete!", 20);
		promptText.SetColor(PALEGREEN);
		GuiDrawIcon(5, subtitleImportButton.getPosition().x + 170, subtitleImportButton.getPosition().y + 50, 10, PALEGREEN);
		break;
	case FileStatus::ProcessFailed:
		setupDisplayText(promptText, "Oops something went wrong! :(", 20);
		promptText.SetColor(CORAL);
		GuiDrawIcon(5, subtitleImportButton.getPosition().x + 170, subtitleImportButton.getPosition().y + 50, 10, CORAL);
		break;
	default:
		break;
	}
	promptText.Draw({ subtitleImportButton.getPosition().x + subtitleImportButton.getSize().x / 2 - MeasureTextEx(m_font, promptText.GetText().c_str(),promptText.GetFontSize(), promptText.GetSpacing()).x / 2, 620 });

	GuiDrawIcon(2, outDirBoxButton.getPosition().x + 5, outDirBoxButton.getPosition().y + 5, 2, MTEXT);

}

void SubtitleWindowGUI::HandleEvents()
{
	// What to do when this button is Pressed?
	if (outDirBoxButton.isPressed())
	{
		//result = NFD_SaveDialog("mp4", NULL, &outPath);
		result = NFD_PickFolder(&outPath, NULL);

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
		result = NFD_OpenDialog(&outPath, nullptr, NULL, NULL);

		if (result == NFD_OKAY)
		{
			std::cout << outPath << std::endl;
			subtitleInputPath = outPath;
			fileStatus = FileStatus::FileReceived;
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
		if (outputDirPath == "")
		{
			std::cout << "No Output Directory Selected" << std::endl;
			return;
		}
		Transcriber* transcriber = Transcriber::GetInstance();
		if(transcriber == nullptr)
			return;
			
		std::cout << "Starting Subtitle Generation" << std::endl;

		bool result = transcriber->BurnInSubtitles(subtitleInputPath.c_str(), outputDirPath.c_str());
		fileStatus = result ? FileStatus::ProcessComplete : FileStatus::ProcessFailed;
		
	}

}

void SubtitleWindowGUI::ShutDown()
{

}