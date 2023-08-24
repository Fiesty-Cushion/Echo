#include "STTWindowGUI.h"
#include <raygui.h>

void STTWindowGUI::Init()
{
	setupDisplayText(sttText, "Real-Time Speech to Text: ");

	transcribeButton = Button("Start", { 100, 40 }, MBG, MTEXT, m_font);
	transcribeButton.setPosition({ 542, 350 });

	outputTextBox = TextBox(70, 400, 1138, 277);

	panelRec = { outputTextBox.getX(), outputTextBox.getY(), outputTextBox.getWidth(), outputTextBox.getHeight() };
	panelContentRec = { 0, 0, outputTextBox.getWidth(), 500};
}

void STTWindowGUI::Draw()
{
	sttText.Draw({ 72, 334 });

	outputTextBox.Draw();
	
	transcribeButton.Draw(isTranscribing ? "Stop" : "Start");

	GuiScrollPanel(panelRec, NULL, panelContentRec, &panelScroll, &panelView);

	BeginScissorMode(outputTextBox.getX(), outputTextBox.getY(), outputTextBox.getWidth(), outputTextBox.getHeight());
		outputTextBox.DrawTextBoxed(m_font, outputTextBox.inputText.c_str(), Rectangle{ outputTextBox.getX() + 10 + panelScroll.x, outputTextBox.getY() + 10 + panelScroll.y, outputTextBox.getWidth() - 20, 1000 }, 20, 1, true, MTEXT);
	EndScissorMode();

}

void STTWindowGUI::HandleEvents()
{
	outputTextBox.Update();

	//....Event Handlers....//
	if (transcribeButton.isPressed())
	{
		isTranscribing = !isTranscribing;

		// only start the stream once
		if (isInitialClick)
		{
			audio = new Audio();
			audio->StartStream(RealTime);
			transcriber = new Transcriber(*audio);
			isInitialClick = false;
		}

		if (isTranscribing)
			takeMicrophoneInput = true;
		else
			takeMicrophoneInput = false;

	}

	if (isTranscribing)
	{
		const std::vector<transcribed_msg>& transcribedData = transcriber->GetTranscribed();
		if (!transcribedData.empty())
		{
			outputTextBox.inputText += (transcribedData.back().is_partial || (transcribedData.back().text == outputTextBox.inputText)) ? "" : transcribedData.back().text;
			std::cout << outputTextBox.inputText;
			std::cout << std::flush;
		}
	}
}

void STTWindowGUI::ShutDown()
{
	delete audio;
	delete transcriber;
}