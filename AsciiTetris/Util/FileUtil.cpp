#include <string>
#include <vector>
#include <fstream>
#include <cassert>

namespace FileUtil
{
	std::vector<std::string> LoadTextLines(const std::string& path)
	{
		std::vector<std::string> lines;

		// Path
		std::string pathstring = std::string(path);

		// File Open and Check
		std::ifstream file(pathstring, std::ios_base::binary);
		assert(file.is_open());

		std::string line;


		while (std::getline(file, line))
		{
			if (line.empty() == false && line.back() == '\r')
			{
				line.pop_back();
			}
			lines.push_back(line);
		}
		file.close();
		return lines;
	}
}