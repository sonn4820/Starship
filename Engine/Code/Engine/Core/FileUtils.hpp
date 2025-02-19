#pragma once
#include <vector>
#include "Engine/Core/StringUtils.hpp"


int FileReadToBuffer(std::vector<uint8_t>& outBuffer, const std::string& fileName);
int FileReadToString(std::string& outString, const std::string& fileName);
bool FileWriteFromBuffer(std::vector<uint8_t> const& buffer, std::string const& filePathName);
bool CreateFolder(std::string const& folderPathName);
bool HasFile(std::string const& folderPathName);
	