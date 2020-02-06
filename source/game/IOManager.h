#pragma once

struct DirEntry
{
	std::string path;
	bool isDirectory;
};

class IOManager
{
public:
	static bool ReadFileToBuffer(std::string_view filePath, std::vector<unsigned char> &buffer);
	static bool ReadFileToBuffer(std::string_view filePath, std::string &buffer);

	static bool GetDirectoryEntries(const char *path, std::vector<DirEntry> &rvEntries);

	static bool MakeDirectory(const char *path);
};