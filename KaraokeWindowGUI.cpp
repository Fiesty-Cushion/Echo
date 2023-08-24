#include "KaraokeWindowGUI.h"

void KaraokeWindowGUI::Init()
{
	setupDisplayText(karaokeText, "Karaoke Generation");

	karaokeButton = Button("Start", { 100, 40 }, MBG, MTEXT, m_font);
	karaokeButton.setPosition({ 542, 350 });

}

void KaraokeWindowGUI::Draw()
{
	karaokeText.Draw({ 72, 334 });
	karaokeButton.Draw("Start"); // Disable this when state changes
}

void KaraokeWindowGUI::HandleEvents()
{
	// What to do when this button is Pressed?

}

void KaraokeWindowGUI::ShutDown()
{

}