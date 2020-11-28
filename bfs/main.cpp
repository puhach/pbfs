#include <iostream>
#include <iomanip>
#include <vector>
#include <stdexcept>
#include <random>
#include <chrono>
#include <queue>

using namespace std;

class Graph
{
	friend ostream& operator << (ostream& stream, const Graph& g);
public:
	Graph(int nVertices, double pEdge, bool directed);
	
	vector<int> bfs(int vertex);

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
	mt19937 gen{rd()};
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

vector<int> Graph::bfs(int vertex)
{
	vector<int> dist(nVertices, -1);
	dist[vertex] = 0;

	queue<int> frontier;
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
}	// bfs

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
	Graph g(5, 0.5, true);
	cout << g;
	auto start = chrono::system_clock::now();
	vector<int> dist = g.bfs(3);
	chrono::duration<double> dur = chrono::system_clock::now() - start;
	cout << dur.count() << endl;
	//cout << dist << endl;
	copy(dist.begin(), dist.end(), ostream_iterator<int>(cout, " "));
	return 0;
}