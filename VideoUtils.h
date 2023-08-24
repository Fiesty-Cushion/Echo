#pragma once
#include <iostream>
#include <fstream>
#include <cstdlib>
#include <cstring>
#include "ScriptUtils.h"

class VideoUtils
{
private:
    static std::pair<int, int> getWidthHeightFromOutput(const char* filename)
    {
        std::ifstream inputFile(filename);
        if (!inputFile.is_open())
        {
            std::cerr << "Error opening file: " << filename << std::endl;
            return { -1, -1 };
        }

        int width, height;
        char delimiter;
        inputFile >> width >> delimiter >> height;

        inputFile.close();
        return { width, height };
    }

public:
    static std::pair<int, int> GetVideoDimensions(const char* videoPath)
    {
        const char* tempOutputFile = "temp_output.txt";

        std::string command = "ffprobe -v error -select_streams v -show_entries stream=width,height -of csv=p=0:s=x ";
        command += videoPath;
        command += " > ";
        command += tempOutputFile;

        int result = std::system(command.c_str());

        if (result != 0)
        {
            std::cerr << "Error executing command." << std::endl;
            ScriptUtils::RemoveFile(tempOutputFile);
            return { -1, -1 }; // Return an error value
        }

        std::pair<int, int> dimensions = getWidthHeightFromOutput(tempOutputFile);
        ScriptUtils::RemoveFile(tempOutputFile);

        return dimensions;
    }
    static bool Generate16KHzWav(const std::string videoPath, const std::string wavOutput)
    {
        std::string command = "ffmpeg -i " + videoPath + " -vn -acodec pcm_s16le -ar 16000 -ac 2 " + wavOutput + " -y";

        int result = std::system(command.c_str());

        return result == 0;
    }
};
