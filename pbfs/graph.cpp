#include "graph.h"
//#include "pennant.h"

#include <iostream>
#include <utility>
#include <vector>
#include <queue>
#include <fstream>

#include <omp.h>


// Instantiate non-specialized templates
template std::vector<int> Graph::bfs<ExecutionStrategy::ParallelOmp>(int vertex) const;



std::ostream& operator << (std::ostream& stream, const Graph& g)
{
	//for (int v = 0; v < g.nVertices; ++v)
	for (const auto& neighbors : g.adj)
	{
		auto v = &neighbors - &g.adj.front();
		stream << std::setw(5) << v << " | ";
		std::copy(neighbors.begin(), neighbors.end(), std::ostream_iterator<int>(stream, " "));
		stream << std::endl;
	}

	return stream;
}

template <>
std::vector<int> Graph::bfs<ExecutionStrategy::Sequential>(int vertex) const
{
	std::vector<int> dist(this->adj.size(), -1);
	dist[vertex] = 0;

	std::queue<int> frontier;
	frontier.push(vertex);

	while (!frontier.empty())
	{
		int v = frontier.front();
		frontier.pop();

		for (int u : adj[v])
		{
			if (dist[u] < 0)
			{
				dist[u] = dist[v] + 1;
				frontier.push(u);
			}
		}
	}

	return dist;	// NRVO should eliminate copying
}

// Generic Parallel BFS
template <ExecutionStrategy executionStrategy>
std::vector<int> Graph::bfs(int vertex) const
//template <>
//std::vector<int> Graph::bfs<ExecutionStrategy::ParallelOmp>(int vertex) const
{
	std::vector<int> dist(this->adj.size(), -1);
	dist[vertex] = 0;

	int d = 0;

	Bag curBag;
	curBag.insert(vertex);

	while (!curBag.isEmpty())
	{
		Bag nextBag = processLevel(curBag, d++, dist);
		curBag = std::move(nextBag);
	}

	return dist;
}


Bag Graph::processLevel(Bag& inBag, int level, std::vector<int>& dist) const
{
	//std::cout << inBag.getSize() << std::endl;

	//Bag outBag;
	std::vector<Bag> outBags(omp_get_max_threads());

#pragma omp parallel 
	{
		// the paper suggests a parallel for
#pragma omp for schedule(dynamic, 1)
		for (int i = 0; i < inBag.getSize(); ++i)	// the size here means the number of pennants in the bag
		{
//#pragma omp critical
//			{
//				std::cout << i << " " << omp_get_thread_num() << " " << omp_get_num_threads() << std::endl;
//			}

			if (Pennant* pennant = inBag.getPennant(i))		// TODO: consider using a shared pointer
				processPennant(*pennant, outBags[omp_get_thread_num()], level, dist);
		}

		// combine thread-private bags
#pragma omp single
		reduce(outBags.begin(), outBags.end());
	}

	// NRVO doesn't seem to help here
	return std::move(outBags[0]);
}

void Graph::processPennant(Pennant& pennant, Bag& outBag, int level, std::vector<int> &dist) const
{
	if (pennant.getSize() > 1)
	{
		std::unique_ptr<Pennant> other = pennant.split();
		processPennant(*other, outBag, level, dist);		// spawn
		processPennant(pennant, outBag, level, dist);	
		// sync
	}	// pennant size > 1
	else
	{
		int u = pennant.getVertex();	// head of the pennant
		for (int i = 0; i < adj[u].size(); ++i)		// parallel
		{
			int v = adj[u][i];

			if (dist[v] < 0)
			{
				dist[v] = level + 1;	// atomic
				outBag.insert(v);	// may need to use a private bag for each thread
			}
		}
	}	// pennant size <= 1
}


void Graph::save(const std::string& fileName) const
{
	std::ofstream out(fileName);
	if (!out)
		throw std::runtime_error("Failed to open the output file " + fileName);

	out << adj.size() << std::endl;

	for (int i = 0; i < adj.size(); ++i)
	{
		out << i << " ";
		std::copy(adj[i].begin(), adj[i].end(), std::ostream_iterator<int>(out, " "));
		out << std::endl;
	}

	if (!out)
		throw std::runtime_error("Error while writing to the output file " + fileName);
}