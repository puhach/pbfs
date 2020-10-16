#include <iostream>
#include <iomanip>
#include <vector>
#include <stdexcept>
#include <random>
#include <chrono>

using namespace std;

class Graph
{
	friend ostream& operator << (ostream& stream, const Graph& g);
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
	, adj(nVertices)
{
	random_device rd;
	mt19937 gen;
	uniform_real_distribution<double> dist(0, 1);

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

ostream& operator << (ostream& stream, const Graph& g)
{
	//for (int v = 0; v < g.nVertices; ++v)
	for (const auto &neighbors : g.adj)
	{
		auto v = &neighbors - &g.adj.front();
		stream << setw(5) << v << " | ";
		copy(neighbors.begin(), neighbors.end(), ostream_iterator<int>(stream, " "));
		stream << endl;
	}

	return stream;
}

int main(int argc, char* argv[])
{
	auto start = chrono::system_clock::now();
	Graph g(5000, 0.5, true);
	chrono::duration<double> dur = chrono::system_clock::now() - start;
	//cout << g;
	cout << dur.count() << endl;
	return 0;
}