#include "randomuniforminitializer.h"

#include <random>

//template <RandomUniformInitializer::ExecutionStrategy> 
//void init(std::vector<std::vector<int>>& adj);
//
//
//template <>
//void init<RandomUniformInitializer::Sequential>(std::vector<std::vector<int>>& adj)
//{
//
//}

//template <class RandomEngine>
template <>
//void RandomUniformInitializer<RandomEngine>::init<RandomUniformInitializer<RandomEngine>::Sequential>(int nVertices
void RandomUniformInitializer::init<RandomUniformInitializer::Sequential>(int nVertices
	, double pEdge, bool directed, std::vector<std::vector<int>>& adj)
//void RandomUniformInitializer::init<1>(std::vector<std::vector<int>>& adj)
{
	std::random_device rd;
	std::mt19937 gen{rd()};
	std::uniform_real_distribution<double> dist{ 0, 1 };

	//adj.resize(nVertices);

	for (int v = 0; v < nVertices; ++v)
	{
		for (int u = v + 1; u < nVertices; ++u)
		{
			if (dist(gen) < pEdge)
			{
				adj[v].push_back(u);	// forward connection

				// for undirected graphs the connection is symmetric, but for directed graphs we have to roll the dice again
				if (!directed || dist(gen) < pEdge)
					adj[u].push_back(v);	// backward connection
			}
		}	// u
	}	// v
}

//template <class RandomEngine>
template <>
//void RandomUniformInitializer<RandomEngine>::init<RandomUniformInitializer<RandomEngine>::ParallelOmp>(int nVertices
void RandomUniformInitializer::init<RandomUniformInitializer::ParallelOmp>(int nVertices
	, double pEdge, bool directed, std::vector<std::vector<int>>& adj)
{

}

//template <class RandomEngine>
//RandomUniformInitializer<RandomEngine>::RandomUniformInitializer(int nVertices, double pEdge, bool directed)
RandomUniformInitializer::RandomUniformInitializer(int nVertices, double pEdge, bool directed)
	: adj(nVertices)
{
	RandomUniformInitializer::init<RandomUniformInitializer::Sequential>(nVertices, pEdge, directed, adj);
}


