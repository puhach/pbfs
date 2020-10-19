#ifndef GRAPH_H
#define GRAPH_H

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
	Graph(std::vector<std::vector<int>>&& adj) : adj(std::move(adj)) {}

private:
	/*int nVertices;
	double pEdge;
	bool directed;*/
	std::vector<std::vector<int>> adj;
};	// Graph



#endif // GRAPH_H