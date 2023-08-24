#include "KaraokeWindowGUI.h"

void KaraokeWindowGUI::Init()
{
	setupDisplayText(karaokeText, "Karaoke Generation");

	karaokeButton = Button("Start", { 100, 40 }, MBG, MTEXT, m_font);
	karaokeButton.setPosition({ 542, 350 });

    outDirBoxButton = Button("", { 45, 45 }, MBG, MTEXT, m_font);
    outDirBoxButton.setPosition({ 1190, 525 });

    outDirBox = TextBox(750, 500, 400, 50);

    nfdchar_t* outPath = NULL;
    nfdresult_t result;

    karExportIcon = raylib::Image("./Resources/save.png");
    karExportIcon.Resize(outDirBoxButton.getSize().x - 10, outDirBoxButton.getSize().y - 10);
    karExportIconTexture = LoadTextureFromImage(karExportIcon);
    karExportIcon.Unload();

    setupDisplayText(karExportText, "Save File To");

}

void KaraokeWindowGUI::Draw()
{
	karaokeText.Draw({ 72, 334 });
	karaokeButton.Draw("Start"); // Disable this when state changes

    DrawRectangle(70, 410, 500, 270, MBG);
    outDirBox.Draw();
    outDirBox.DrawTextBoxed(m_font, outDirBox.inputText.c_str(), Rectangle{ outDirBox.getX() + 10, outDirBox.getY() + 10 , outDirBox.getWidth() - 20, outDirBox.getHeight() - 20 }, 20, 1, false, MTEXT);

    karExportText.Draw({ 750, 400 });
    outDirBoxButton.Draw("");
    DrawTexture(karExportIconTexture, outDirBoxButton.getPosition().x + 5, outDirBoxButton.getPosition().y + 5, MPINK);
}

void KaraokeWindowGUI::HandleEvents()
{
	// What to do when this button is Pressed?
    if (outDirBoxButton.isPressed())
    {
        result = NFD_SaveDialog("mp4", NULL, &outPath);

        if (result == NFD_OKAY)
        {
            outDirBox.inputText = outPath;
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

}

void KaraokeWindowGUI::ShutDown()
{

}