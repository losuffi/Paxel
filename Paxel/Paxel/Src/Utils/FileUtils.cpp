#include "PXPCH.h"
#include "FileUtils.h"
#pragma once

std::vector<char> FileUtils::ReadFile(const std::string& filename)
{
	std::ifstream File(filename, std::ios::ate | std::ios::binary);

	PX_ENSURE_RET_VAL(File.is_open(), std::vector<char>{}, "Failed to open file!");

	size_t FileSize = static_cast<size_t>(File.tellg());
	std::vector<char> Buffer(FileSize);
	File.seekg(0);
	File.read(Buffer.data(), FileSize);
	File.close();
	
	return Buffer;
}
