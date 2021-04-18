#ifndef GRAPH_H
#define GRAPH_H

#include "bag.h"
#include "executionstrategy.h"

#include <iostream>
#include <iostream>
#include <iomanip>
#include <vector>
#include <stdexcept>


class Graph
{
	friend std::ostream& operator << (std::ostream& stream, const Graph& g);
public:

	Graph(const std::vector<std::vector<int>> &adj): adj(adj) {}

	// Since C++17 vector's move constructor is noexcept
	Graph(std::vector<std::vector<int>>&& adj) noexcept : adj(std::move(adj)) {}

	Graph(const Graph& other) = default;

	Graph(Graph&& other) = default;

	Graph& operator = (const Graph& other) = default;

	Graph& operator = (Graph&& other) = default;

	template <ExecutionStrategy executionStrategy>
	std::vector<int> bfs(int vertex) const;

	//std::vector<int> pbfs(int vertex) const;

	void save(const std::string &fileName) const;

private:

	Bag processLevel(Bag& inBag, int level, std::vector<int>& dist) const;

	void processPennant(Pennant &pennant, Bag &outBag, int level, std::vector<int> &dist) const;

	//void reduce(std::vector<Bag>::iterator a, std::vector<Bag>::iterator b);

	/*int nVertices;
	double pEdge;
	bool directed;*/
	std::vector<std::vector<int>> adj;
};	// Graph



#endif // GRAPH_H