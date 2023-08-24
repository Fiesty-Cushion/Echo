#include "STTWindowGUI.h"

void STTWindowGUI::Init()
{
	setupDisplayText(sttText, "Real-Time Speech to Text: ");

	transcribeButton = Button("Start", { 100, 40 }, MBG, MTEXT, m_font);
	transcribeButton.setPosition({ 542, 350 });

	outputTextBox = TextBox(70, 400, 1138, 277);
}

void STTWindowGUI::Draw()
{
	sttText.Draw({ 72, 334 });

	outputTextBox.Draw();
	outputTextBox.DrawTextBoxed(m_font, outputTextBox.inputText.c_str(), Rectangle{ outputTextBox.getX() + 10, outputTextBox.getY() + 10 , outputTextBox.getWidth() - 20, outputTextBox.getHeight() - 20 }, 20, 1, true, MTEXT);

	transcribeButton.Draw(isTranscribing ? "Stop" : "Start");

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