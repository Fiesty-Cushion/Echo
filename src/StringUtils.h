#pragma once
#include <string>
#include <regex>

class StringUtils
{
public:
	static std::string RemoveSpecialCharacters(std::string& input)
	{
		// replacing the music character with [Music]
		ReplaceAll(input, "\xE2\x99", "[Music]");
		ReplaceAll(input, "\xC2\xAA", "");
		std::regex pattern("[^A-Za-z0-9!><@#%$^&*(_+).,;~`=-?|}{\": \\t/\\\\\\[\\]]");
		return std::regex_replace(input, pattern, "");
	}
	static void ReplaceAll(std::string& s, const std::string& search, const std::string& replace)
	{
		for (size_t pos = 0; ; pos += replace.length())
		{
			pos = s.find(search, pos);
			if (pos == std::string::npos) break;
			s.erase(pos, search.length());
			s.insert(pos, replace);
		}
	}
	static std::string ExtractFileName(const std::string& input)
	{
		size_t lastSlashPos = input.find_last_of("/\\");
		size_t startPos = (lastSlashPos != std::string::npos) ? (lastSlashPos + 1) : 0;
		size_t lastDotPos = input.find_last_of(".");
		size_t endPos = (lastDotPos != std::string::npos && lastDotPos > startPos) ? lastDotPos : input.length();
		return input.substr(startPos, endPos - startPos);
	}
	static std::string JoinPaths(const std::string& directory, const std::string& fileName)
	{
		char lastChar = directory.back();
		if (lastChar != '/' && lastChar != '\\')
		{
			return directory + '/' + fileName;
		}

		return directory + fileName;
	}
	static std::string GetFileExtension(const std::string& input) {
    size_t lastDotPos = input.find_last_of(".");
    if (lastDotPos != std::string::npos) {
        return input.substr(lastDotPos + 1);
    }
    return ""; // No file extension found
}
};
