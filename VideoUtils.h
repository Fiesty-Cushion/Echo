#pragma once
#include <iostream>
#include <fstream>
#include <cstdlib>
#include <cstring>
#include "ScriptUtils.h"
#include <pl_mpeg.h>
#include <raylib.h>

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

    static bool ConvertToMpeg(const std::string videoPath, const std::string mpegOutput)
    {
        std::string command = "ffmpeg -i " + videoPath + " -c:v mpeg1video -q:v 0 -c:a mp2 -format mpeg " + mpegOutput + " -y";
        int result = std::system(command.c_str());
        return result == 0;
    }

    static Texture GetFrameFromVideo(const std::string videoPath)
    {
        plm_t* plm = plm_create_with_filename(videoPath.c_str());

        if (!plm) {
            std::cerr << "Failed To Load Video !!!" << std::endl;
            std::exit(-1);
        }

        double framerate = plm_get_framerate(plm);

        int width = plm_get_width(plm);
        int height = plm_get_height(plm);

        plm_samples_t* sample = NULL;

        Image imFrame = { 0 };

        imFrame.width = width;
        imFrame.height = height;
        imFrame.format = PIXELFORMAT_UNCOMPRESSED_R8G8B8;
        imFrame.mipmaps = 1;
        imFrame.data = (unsigned char*)malloc(width * height * 3);

        Texture vid_texture = LoadTextureFromImage(imFrame);
        plm_frame_t* frame;
        for (int i = 0; i < 20; i++)
        {
            frame = plm_skip_video_frame(plm);
        }
        plm_frame_to_rgb(frame, static_cast<uint8_t*>(imFrame.data));
        UpdateTexture(vid_texture, static_cast<uint8_t*>(imFrame.data));

        return vid_texture;
    }
};
