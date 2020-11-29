#ifndef GRAPH_H
#define GRAPH_H

#include "bag.h"
#include "executionstrategy.h"

#include <iostream>
#include <iostream>
#include <iomanip>
#include <vector>
#include <stdexcept>


//template <class Initializer>
class Graph
{
	friend std::ostream& operator << (std::ostream& stream, const Graph& g);
public:
	//Graph(int nVertices, double pEdge, bool directed);

	//template <typename ... Args>
	//Graph(Args ... args): adj(std::move(Initializer(std::forward<Args>(args)...)).getAdjacencyList()) {}

	//template <class Initializer>
	//Graph(Initializer initializer);// : adj(std::move(initializer).getAdjacencyList()) {}

	Graph(const std::vector<std::vector<int>> &adj): adj(adj) {}

	// Since C++17 vector's move constructor is noexcept
	Graph(std::vector<std::vector<int>>&& adj) noexcept : adj(std::move(adj)) {}

	// TODO: define copy/move constructors and copy/move assignment operators

	template <ExecutionStrategy executionStrategy>
	std::vector<int> bfs(int vertex) const;

	//std::vector<int> pbfs(int vertex) const;

	void save(const std::string &fileName) const;

private:

	Bag processLevel(Bag& inBag, int level, std::vector<int>& dist) const;

	void processPennant(Pennant &pennant, Bag &outBag, int level, std::vector<int> &dist) const;

	/*int nVertices;
	double pEdge;
	bool directed;*/
	std::vector<std::vector<int>> adj;
};	// Graph



#endif // GRAPH_H