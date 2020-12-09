#include "randomuniformbuilder.h"
#include "executionstrategy.h"

#include <omp.h>

#include <random>
#include <vector>

// Instantiate all templates we need (allows for implementation to be separated from declaration).
// Alternatively, we could include the definition (.cpp) into the header file.

// TODO: these are, probably, not required since we still need to instantiate the methods for LLVM clang

//template class GraphBuilder<RandomUniform<ExecutionStrategy::Sequential, std::minstd_rand0>>;
//template class GraphBuilder<RandomUniform<ExecutionStrategy::Sequential, std::minstd_rand>>;
//template class GraphBuilder<RandomUniform<ExecutionStrategy::Sequential, std::mt19937>>;
//template class GraphBuilder<RandomUniform<ExecutionStrategy::Sequential, std::mt19937_64>>;
//template class GraphBuilder<RandomUniform<ExecutionStrategy::Sequential, std::ranlux24_base>>;
//template class GraphBuilder<RandomUniform<ExecutionStrategy::Sequential, std::ranlux48_base>>;
//template class GraphBuilder<RandomUniform<ExecutionStrategy::Sequential, std::ranlux24>>;
//template class GraphBuilder<RandomUniform<ExecutionStrategy::Sequential, std::ranlux48>>;
//template class GraphBuilder<RandomUniform<ExecutionStrategy::Sequential, std::knuth_b>>;
//
//template class GraphBuilder<RandomUniform<ExecutionStrategy::ParallelOmp, std::minstd_rand0>>;
//template class GraphBuilder<RandomUniform<ExecutionStrategy::ParallelOmp, std::minstd_rand>>;
//template class GraphBuilder<RandomUniform<ExecutionStrategy::ParallelOmp, std::mt19937>>;
//template class GraphBuilder<RandomUniform<ExecutionStrategy::ParallelOmp, std::mt19937_64>>;
//template class GraphBuilder<RandomUniform<ExecutionStrategy::ParallelOmp, std::ranlux24_base>>;
//template class GraphBuilder<RandomUniform<ExecutionStrategy::ParallelOmp, std::ranlux48_base>>;
//template class GraphBuilder<RandomUniform<ExecutionStrategy::ParallelOmp, std::ranlux24>>;
//template class GraphBuilder<RandomUniform<ExecutionStrategy::ParallelOmp, std::ranlux48>>;
//template class GraphBuilder<RandomUniform<ExecutionStrategy::ParallelOmp, std::knuth_b>>;

// LLVM clang requires method instantiation rather than class instantiation
template Graph GraphBuilder<RandomUniform<ExecutionStrategy::Sequential, std::minstd_rand0>>::create(int nVertices, double pEdge, bool directed);
template Graph GraphBuilder<RandomUniform<ExecutionStrategy::Sequential, std::minstd_rand>>::create(int nVertices, double pEdge, bool directed);
template Graph GraphBuilder<RandomUniform<ExecutionStrategy::Sequential, std::mt19937>>::create(int nVertices, double pEdge, bool directed);
template Graph GraphBuilder<RandomUniform<ExecutionStrategy::Sequential, std::mt19937_64>>::create(int nVertices, double pEdge, bool directed);
template Graph GraphBuilder<RandomUniform<ExecutionStrategy::Sequential, std::ranlux24_base>>::create(int nVertices, double pEdge, bool directed);
template Graph GraphBuilder<RandomUniform<ExecutionStrategy::Sequential, std::ranlux48_base>>::create(int nVertices, double pEdge, bool directed);
template Graph GraphBuilder<RandomUniform<ExecutionStrategy::Sequential, std::ranlux24>>::create(int nVertices, double pEdge, bool directed);
template Graph GraphBuilder<RandomUniform<ExecutionStrategy::Sequential, std::ranlux48>>::create(int nVertices, double pEdge, bool directed);
template Graph GraphBuilder<RandomUniform<ExecutionStrategy::Sequential, std::knuth_b>>::create(int nVertices, double pEdge, bool directed);

template Graph GraphBuilder<RandomUniform<ExecutionStrategy::ParallelOmp, std::minstd_rand0>>::create(int nVertices, double pEdge, bool directed);
template Graph GraphBuilder<RandomUniform<ExecutionStrategy::ParallelOmp, std::minstd_rand>>::create(int nVertices, double pEdge, bool directed);
template Graph GraphBuilder<RandomUniform<ExecutionStrategy::ParallelOmp, std::mt19937>>::create(int nVertices, double pEdge, bool directed);
template Graph GraphBuilder<RandomUniform<ExecutionStrategy::ParallelOmp, std::mt19937_64>>::create(int nVertices, double pEdge, bool directed);
template Graph GraphBuilder<RandomUniform<ExecutionStrategy::ParallelOmp, std::ranlux24_base>>::create(int nVertices, double pEdge, bool directed);
template Graph GraphBuilder<RandomUniform<ExecutionStrategy::ParallelOmp, std::ranlux48_base>>::create(int nVertices, double pEdge, bool directed);
template Graph GraphBuilder<RandomUniform<ExecutionStrategy::ParallelOmp, std::ranlux24>>::create(int nVertices, double pEdge, bool directed);
template Graph GraphBuilder<RandomUniform<ExecutionStrategy::ParallelOmp, std::ranlux48>>::create(int nVertices, double pEdge, bool directed);
template Graph GraphBuilder<RandomUniform<ExecutionStrategy::ParallelOmp, std::knuth_b>>::create(int nVertices, double pEdge, bool directed);


template <class RandomEngine>
Graph GraphBuilder<RandomUniform<ExecutionStrategy::Sequential, RandomEngine>>::create(int nVertices, double pEdge, bool directed)
{	
	std::random_device rd;
	//mt19937 gen;
	RandomEngine gen{rd()};
	std::uniform_real_distribution<double> dist(0, 1);
	std::vector<std::vector<int>> adj(nVertices);

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

	return Graph(std::move(adj));
}	// sequential create


template <class RandomEngine>
Graph GraphBuilder<RandomUniform<ExecutionStrategy::ParallelOmp, RandomEngine>>::create(int nVertices, double pEdge, bool directed)
{
	std::vector<std::vector<int>> adj(nVertices);
	
	/*
vector<unsigned long long> numForward(nVertices);

#pragma omp parallel
	{
		// these instances will be private for each thread
		random_device rd;
		mt19937 gen;
		uniform_real_distribution<double> dist(0, 1);

		//cout << omp_get_thread_num() << endl;

		// add forward edges
#pragma omp for
		for (int v = 0; v < nVertices; ++v)
		{
			for (int u = v + 1; u < nVertices; ++u)
			{
				if (dist(gen) < pEdge)
					adj[v].push_back(u);
			}	// u

			numForward[v] = adj[v].size();
		}	// v

		// add backward edges
		if (directed)
		{
#pragma omp for
			for (int v = 0; v < nVertices; ++v)
			{
				for (int u = 0; u < v; ++u)
				{
					if (dist(gen) < pEdge)
						adj[v].push_back(u);
				} // u
			} // v
		} // directed
		else
		{
			for (int v = 0; v < nVertices; ++v)
			{
				auto& neighbors = adj[v];

//#pragma omp critical
//				{
//					cout << "begin: " << omp_get_thread_num() << " : " << v << " " << numForward[v] << endl;
//				}

#pragma omp for
				//for (int j = 0; j < neighbors.size(); ++j)
				for (int j = 0; j < numForward[v]; ++j)
				{
//#pragma omp critical
//					{
//						cout << "innner: " << omp_get_thread_num() << " : " << j << " " << v << endl;
//					}

					int u = neighbors[j];
					//if (u > v)	// ignore backward edges that we have just added
					adj[u].push_back(v);	// all neighbors are different and not equal to v
				} // for j

//#pragma omp critical
//				{
//					cout << "end: " << omp_get_thread_num() << " : " << v << endl;
//				}
			} // for v
		} // undirected
	}	// omp parallel
	*/


#pragma omp parallel 
	{
		// these instances will be private for each thread
		std::random_device rd;
		//mt19937 gen(rd());
		RandomEngine gen{rd()};
		std::uniform_real_distribution<double> dist{ 0, 1 };

		if (directed)
		{
#pragma omp for
			for (int v = 0; v < nVertices; ++v)
			{
				for (int u = 0; u < nVertices; ++u)
				{
					if (u != v && dist(gen) < pEdge)
						adj[v].push_back(u);
				} // u
			} // v
		} // directed
		else
		{
#pragma omp for
			for (int v = 0; v < nVertices; ++v)
			{
				for (int u = v + 1; u < nVertices; ++u)
				{
					if (dist(gen) < pEdge)
						adj[v].push_back(u);
				} // u
			} // v

			for (int v = 0; v < nVertices; ++v)
			{
				const auto& neighbors = adj[v];

#pragma omp for
				for (int j = 0; j < neighbors.size(); ++j)
				{
					//#pragma omp critical 
					//					{
					//						cout << omp_get_thread_num() << " " << v << endl;
					//					}
					int u = neighbors[j];
					if (u > v)	// ignore backward edges that we have just added
						adj[u].push_back(v);
				}
			} // v

		}	// undirected
	}	// parallel

	return Graph(std::move(adj));
}	// OMP create