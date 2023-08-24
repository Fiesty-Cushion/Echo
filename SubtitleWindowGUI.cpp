#include "SubtitleWindowGUI.h"
#include "Globals.h"
#include "Button.h"


void SubtitleWindowGUI::Init()
{
	setupDisplayText(subtitleText, "Subtitle Generation");

	subtitleButton = Button("Start", { 100, 40 }, MBG, MTEXT, m_font);
	subtitleButton.setPosition({ 542, 350 });
}

void SubtitleWindowGUI::Draw()
{
	subtitleText.Draw({72, 334});
	subtitleButton.Draw("Start"); // should be disabled when process starts
}

void SubtitleWindowGUI::HandleEvents()
{
	// What to do when this button is Pressed?

}

void SubtitleWindowGUI::ShutDown()
{

}