#include "Engine/Core/FileUtils.hpp"
#include "Engine/Core/ErrorWarningAssert.hpp"
#include "Engine/Window/Window.hpp"
#include <windows.h>

int FileReadToBuffer(std::vector<uint8_t>& outBuffer, const std::string& fileName)
{
	FILE* filePointer;
	fopen_s(&filePointer, fileName.c_str(), "rb");

	if (!filePointer)
	{
		ERROR_AND_DIE("FILE IS NOT FOUND");
	}

	fseek(filePointer, 0, SEEK_END);
	long length = ftell(filePointer);
	fseek(filePointer, 0, SEEK_SET);

	outBuffer.resize(length);
	fread(outBuffer.data(), length, 1, filePointer);
	fclose(filePointer);

	return length;
}

int FileReadToString(std::string& outString, const std::string& fileName)
{
	std::vector<uint8_t> buffer;
	int result = FileReadToBuffer(buffer, fileName);
	buffer.push_back('\0');
	outString.assign(buffer.begin(), buffer.end());
	return result;
}

bool FileWriteFromBuffer(std::vector<uint8_t> const& buffer, std::string const& filePathName)
{
	FILE* file = nullptr;
	errno_t result = fopen_s(&file, filePathName.c_str(), "wb");
	if (result != 0 || file == nullptr)
	{
		return false;
	}
	fwrite(buffer.data(), 1, buffer.size(), file);
	fclose(file);
	return true;
}

bool CreateFolder(std::string const& folderPathName)
{
	return CreateDirectoryA(folderPathName.c_str(), nullptr);
}

bool HasFile(std::string const& folderPathName)
{
	struct stat sb;

	if (stat(folderPathName.c_str(), &sb) == 0 && !(sb.st_mode & S_IFDIR))
	{
		return true;
	}
	
	return false;
}