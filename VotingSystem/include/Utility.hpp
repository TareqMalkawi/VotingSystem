#include <iostream>
#include <fstream>
#include <string>
#include <vector>

class Utils
{
public:
	static void WriteFile(std::string fileName , static char userData[64]);
	static void CreateAndWriteFile(std::string fileName , static char userData[64]);
	static void CreateAndWriteFile(std::string fileName , const char userData[64]);
	static void CreateEmptyFile(std::string fileName);
	static void WriteFile(std::string fileName, const char userData[64]);
	static void DeleteAndWriteFile(std::string fileName, const char userData[64]);
	static void ReadFile(std::string fileName , static std::vector<std::string>& storage);
	static void ReadFile(std::string fileName , static std::string& storage);
};
