#include "Utility.hpp"

void Utils::WriteFile(std::string fileName , static char userData[64])
{
	std::fstream file;
	file.open(fileName, std::ios::out | std::ios::in | std::ios::app);

	try
	{
		if (file.is_open())
		{
			file << userData << std::endl;
		}
	}
	catch (std::fstream::failure e)
	{
		std::cout << e.code() << "\n";
	}

	file.close();
}

void Utils::WriteFile(std::string fileName, const char userData[64])
{
	std::fstream file;
	file.open(fileName, std::ios::out | std::ios::in | std::ios::app);

	try
	{
		if (file.is_open())
		{
			file << userData << std::endl;
		}
	}
	catch (std::fstream::failure e)
	{
		std::cout << e.code() << "\n";
	}

	file.close();
}

void Utils::CreateAndWriteFile(std::string fileName, static char userData[64])
{
	std::fstream file(fileName);
	file.open(fileName, std::ios::out | std::ios::in | std::ios::app);

	try
	{
		if (file.is_open())
		{
			file << userData << std::endl;
		}
	}
	catch (std::fstream::failure e)
	{
		std::cout << e.code() << "\n";
	}

	file.close();
}

void Utils::CreateAndWriteFile(std::string fileName, const char userData[64])
{
	std::fstream file(fileName);
	file.open(fileName, std::ios::out | std::ios::in | std::ios::app);

	try
	{
		if (file.is_open())
		{
			file << userData << std::endl;
		}
	}
	catch (std::fstream::failure e)
	{
		std::cout << e.code() << "\n";
	}

	file.close();
}

void Utils::DeleteAndWriteFile(std::string fileName, const char userData[64])
{
	std::fstream file;
	file.open(fileName, std::ios::out | std::ios::in);

	try
	{
		if (file.is_open())
		{
			file << userData << std::endl;
		}
	}
	catch (std::fstream::failure e)
	{
		std::cout << e.code() << "\n";
	}

	file.close();
}

void Utils::ReadFile(std::string fileName, static std::vector<std::string>& storage)
{
	static char data[64];
	
	std::fstream file;
	file.open(fileName, std::ios::out | std::ios::in);

	try
	{
		if (file.is_open())
		{
			while (file.getline(data, 50000))
			{
				storage.push_back(data);
			}
		}
	}
	catch (std::fstream::failure e)
	{
		std::cout << e.code() << "\n";
	}

	file.close();
}

void Utils::CreateEmptyFile(std::string fileName)
{
	std::fstream file(fileName);
	file.open(fileName, std::ios::out | std::ios::in | std::ios::app);
}

void Utils::ReadFile(std::string fileName, static std::string& storage)
{
	static char data[64];

	std::fstream file;
	file.open(fileName, std::ios::out | std::ios::in);

	try
	{
		if (file.is_open())
		{
			while (file.getline(data, 50000))
			{
				storage = data;
			}
		}
	}
	catch (std::fstream::failure e)
	{
		std::cout << e.code() << "\n";
	}

	file.close();
}