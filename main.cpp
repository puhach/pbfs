#include <iostream>
#include <vector>
#include <stdexcept>
#include <random>

using namespace std;

class Graph
{
public:
	Graph(int nVertices, double pEdge, bool directed);

private:
	int nVertices;
	double pEdge;
	bool directed;
	vector<vector<int>> adj;
};

Graph::Graph(int nVertices, double pEdge, bool directed)
	: nVertices(nVertices>0 ? nVertices : throw invalid_argument("The number of vertices must be greater than zero."))
	, pEdge(pEdge>=0 && pEdge<=1 ? pEdge : throw invalid_argument("The probability of an edge must be in range 0..1."))
	, directed(directed)
{
	random_device rd;
	mt19937 gen;
	uniform_real_distribution<double> dist(0, 1);

	adj.resize(nVertices);

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

int main(int argc, char* argv[])
{
	Graph g(3, 0.5, false);
	g = g;
	return 0;
}