#pragma once
#include <fstream>
class FileUtils
{
public:
	static std::vector<char> ReadFile(const std::string& filename);
};

