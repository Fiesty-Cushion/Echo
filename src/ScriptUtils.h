#pragma once
#include <string>
#include <iostream>
#include <cstdlib>
#include "StringUtils.h"

#ifdef _WIN32 // Check if compiling for Windows
#include <direct.h>
#define GetCurrentDir _getcwd
#define PATH_SEPARATOR '\\'
#else // Assume Unix-like system
#include <unistd.h>
#define GetCurrentDir getcwd
#define PATH_SEPARATOR '/'
#endif


class ScriptUtils
{
public:
    static bool RunScriptFromCurrentDir(const std::string script)
    {
        char buffer[260];
        if (GetCurrentDir(buffer, sizeof(buffer)) == nullptr)
        {
            std::cerr << "Error getting current directory" << std::endl;
        }
        else
        {
            std::cout << "Current directory: " << buffer << std::endl;
        }
        std::string s = buffer;
        StringUtils::ReplaceAll(s, "\\", "/");

     // using powershell to execute the script from current location
        std::string command = "powershell.exe -ExecutionPolicy Bypass -Command \"cd '" + s + "'; .\\" + script + "\"";

        int result = std::system(command.c_str());

        if (result != 0)
        {
            std::cerr << "Script failed to run" << std::endl;
            return false;
        }

        return true;
    }

    static void RemoveFile(const char* filename)
    {
        std::remove(filename);
    }
};