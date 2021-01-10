#include<string>
#include<iostream>
#include<filesystem>
#include <vector>
#include <fstream>
#include <sstream>
#include <set>
#include <tchar.h>
#include <windows.h>
using namespace std;
namespace fs = filesystem;

void GetFiles(const string drName, vector<string>& dirFiles, bool isRecursive)
{
	if (!fs::exists(drName))
	{
		return;
	}
	if (!fs::is_directory(drName))
	{
		dirFiles.push_back(drName);
		return;
	}
	if (isRecursive)
	{
		for (const auto& entry : fs::recursive_directory_iterator(drName))
		{
			if (!fs::is_directory(entry))
				dirFiles.push_back(entry.path().string());
		}

	}
	else
	{
		for (const auto& entry : fs::directory_iterator(drName))
		{
			if (!fs::is_directory(entry))
				dirFiles.push_back(entry.path().string());

		}
	}
}

bool SearChText(vector<string> files, string searchText)
{
	bool bResult = false;
	for (auto& item : files)
	{
		string fileName = item;
		std::string filedata;
		{
			std::ifstream fin(fileName.c_str());
			std::stringstream ss;
			ss << fin.rdbuf();
			filedata = ss.str();
		}

		auto result = std::search(std::begin(filedata), std::end(filedata),
			std::begin(searchText), std::end(searchText));
		if (std::end(filedata) != result) {
			bResult = true;
			std::cout << "Found In:" << fileName.c_str() << std::endl;
		}
	}
	return bResult;
}
vector<string> split(const string& str, const string& delim)
{
	vector<string> tokens;
	size_t prev = 0, pos = 0;
	do
	{
		pos = str.find(delim, prev);
		if (pos == string::npos) pos = str.length();
		string token = str.substr(prev, pos - prev);
		if (!token.empty()) tokens.push_back(token);
		prev = pos + delim.length();
	} while (pos < str.length() && prev < str.length());
	return tokens;
}
void Init()
{
#define INFO_BUFFER_SIZE 32767
	TCHAR  infoBuf[INFO_BUFFER_SIZE];
	DWORD  bufCharCount = INFO_BUFFER_SIZE;

	// Get and display the name of the computer.
	if (!GetComputerName(infoBuf, &bufCharCount))
		std::wcout << (TEXT("GetComputerName"));
	std::wcout << "Computer name:      " << infoBuf << std::endl;

	// Get and display the user name.
	if (!GetUserName(infoBuf, &bufCharCount))
		std::wcout << (TEXT("GetUserName"));
	std::wcout << "User name:          " << infoBuf << std::endl;
}

int main(int argc, const char* argv[])
{
	if (argc < 4) //1= Exe Name, 2=Folder Paths, separated by | character, to search in, 3=Search Text, 4=Search Subfolders
	{
		std::cout << "Need 3 arguments 1=Folder Paths, separated by | character, to search in, 2=Search Text, 3=Search Subfolders" << std::endl;
		return 0;
	}
	string path(argv[1]);
	string searchText(argv[2]);
	bool isRecursive = strcmp(argv[3], "1") == 0 ? true : false;

	if (path.size() <= 0)
	{
		std::cout << "Please provide Valid Path" << std::endl;
		return 0;
	}
	Init();
	std::cout << std::endl;
	std::vector<string> dirfolders = split(path, "|");
	if (dirfolders.size() > 0)
	{
		sort(dirfolders.begin(), dirfolders.end());
		dirfolders.erase(unique(dirfolders.begin(), dirfolders.end()), dirfolders.end());
		//std::set<string> uniqueDirs(dirfolders.begin(), dirfolders.end());
		bool bResult = false;
		for (auto& dir : dirfolders)
		{
			vector<string> files;
			GetFiles(dir, files, isRecursive);

			if (SearChText(files, searchText))
				bResult = true;

		}
		if (!bResult)
			std::cout << "No matches were found" << std::endl;
	}
	return 0;
}
