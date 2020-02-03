#include "stdafx.h"
#include "IOManager.h"
#include <filesystem>
#include <fstream>
//-----------------------------------------------------------------------------
namespace fs = std::filesystem;
//-----------------------------------------------------------------------------
bool IOManager::ReadFileToBuffer(const std::string filePath, std::vector<unsigned char>& buffer)
{
	std::ifstream file(filePath, std::ios::binary);
	if ( file.fail() )
		Throw("Can load file : " + filePath);

	//seek to the end
	file.seekg(0, std::ios::end);

	//get the file size
	int fileSize = (int)file.tellg();

	//seek to the beginning
	file.seekg(0, std::ios::beg);

	//reduce the file size by any header bytes that might be present
	fileSize -= (int)file.tellg();

	buffer.resize(fileSize);
	file.read((char*)&(buffer[0]), fileSize);
	file.close();

	return true;
}
//-----------------------------------------------------------------------------
bool IOManager::ReadFileToBuffer(const std::string filePath, std::string& buffer)
{
	std::ifstream file(filePath, std::ios::binary);
	if ( file.fail() )
	{
		perror(filePath.c_str());
		return false;
	}

	//seek to the end
	file.seekg(0, std::ios::end);

	//get the file size
	int fileSize = (int)file.tellg();

	//seek to the beginning
	file.seekg(0, std::ios::beg);

	//reduce the file size by any header bytes that might be present
	fileSize -= (int)file.tellg();

	buffer.resize(fileSize);
	file.read((char*)&(buffer[0]), fileSize);
	file.close();

	return true;
}
//-----------------------------------------------------------------------------
bool IOManager::GetDirectoryEntries(const char* path, std::vector<DirEntry>& rvEntries)
{
	auto dpath = fs::path(path);
	// Must be directory
	if ( !fs::is_directory(dpath) ) return false;

	for ( auto it = fs::directory_iterator(dpath); it != fs::directory_iterator(); ++it )
	{
		rvEntries.emplace_back();
		rvEntries.back().path = it->path().string();
		if ( is_directory(it->path()) )
		{
			rvEntries.back().isDirectory = true;
		}
		else
		{
			rvEntries.back().isDirectory = false;
		}
	}
	return true;
}
//-----------------------------------------------------------------------------
bool IOManager::MakeDirectory(const char* path)
{
	return fs::create_directory(fs::path(path));
}
//-----------------------------------------------------------------------------