#include "textfileloader.h"

//#include <filesystem>
#include <fstream>
#include <sstream>
#include <cassert>

Graph GraphBuilder<TextFile>::create(const std::string &fileName)
{
	//if (!std::filesystem::exists(fileName))
	//	throw std::runtime_error(std::string("Input file doesn't exist: ").append(fileName));

	std::ifstream in(fileName, std::ios::in);
	//assert(in.is_open());
	if (!in)
		throw std::runtime_error(std::string("Failed to open the input file: ").append(fileName));

	int n;
	if (!(in >> n))
		throw std::runtime_error("Failed to read the number of vertices.");

	std::vector<std::vector<int>> adj(n);

	for (int i = 0, v; i < n && in >> v; ++i)
	{
		std::string s;
		std::getline(in, s);

		std::istringstream lineReader(s);
		for (int u; lineReader >> u; )
		{
			adj.at(v).push_back(u);
		}			
	}

	if (!in)
		throw std::runtime_error("Error while reading the input file.");

	return Graph(move(adj));
}