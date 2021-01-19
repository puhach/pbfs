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


// TODO: find a better place for this function
template <typename Iterator>
Bag& reduce(Iterator first, Iterator last)
//void reduce(Iterator first, Iterator last)
{
	assert(first <= last);

	if (last - first <= 1)		// one item or less
		return *first;
	else // two items or more
	{
		auto k = (last - first) / 2;

		/*
//#pragma omp task
		Bag& bagA = reduce(first, first + k);	// spawn

//#pragma omp task
		Bag& bagB = reduce(first + k, last);	// spawn

		// sync
//#pragma omp taskwait

		return bagA.merge(std::move(bagB));
		*/

#pragma omp task
		reduce(first, first+k);

#pragma omp task
		reduce(first+k, last);

#pragma omp taskwait
		//first->merge(std::move(*(first + k)));
		return first->merge(std::move(*(first+k)));
	}
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

//#pragma omp single
		//{
		Bag outBagOther;
#pragma omp task shared(other, outBagOther, level, dist)
		//processPennant(*other, outBag, level, dist);		// spawn
		processPennant(*other, outBagOther, level, dist);		// spawn

#pragma omp task shared(pennant, outBag, level, dist)
		//processPennant(pennant, outBag, level, dist);			
		processPennant(pennant, outBag, level, dist);
		//}	// sync
#pragma omp taskwait	// sync

		outBag.merge(std::move(outBagOther));
	}	// pennant size > 1
	else
	{
		int u = pennant.getVertex();	// head of the pennant
		for (int i = 0; i < adj[u].size(); ++i)		// parallel 
		{
			int v = adj[u][i];

			// Since multiple threads can be checking the vertex v, we must ensure that reading and writing of dist[v] is synchronized
			int d;
#pragma omp atomic read
			d = dist[v];

			// Even if multiple threads enter the following section, they will try to set dist[v] to the same value. This is due to 
			// the fact that PBFS algorithm processes the graph level-by-level sequentially. 
			if (d < 0)	
			{
#pragma omp atomic write
				dist[v] = level + 1;

				// Each thread has a private bag, so no critical section needed. There is a chance that the same vertex v is added
				// to the next level twice (by different threads), but that will not affect the program correctness.
				outBag.insert(v);	
			}

			// The solution from the paper using double checked locking doesn't seem to guarantee that reading and writing dist[v] 
			// doesn't create a race 

			/*
			// We double check whether the vertex was visited to reduce the number of needless locks.
			// Reading the value of dist[v] outside the critical section is safe as long as we make sure that the same
			// vertex is never added to the next level multiple times. ??
			if (dist[v] < 0)	
			{
				bool vUpdated = false;

#pragma omp critical
				if (dist[v] < 0)
				{
					dist[v] = level + 1;	
					vUpdated = true;
				}	// dist[v] < 0 (critical)

				// Although we use a private bag for each thread and calling insert outside the critical section is safe, 
				// we still have to ensure that the same vertex is not added to the next level twice. This way we eliminate
				// the chance of checking the same v ?? 
				if (vUpdated)
					outBag.insert(v);	
			}	// dist[v] < 0
			*/
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