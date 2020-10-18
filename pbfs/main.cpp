#include <iostream>
#include <iomanip>
#include <vector>
#include <stdexcept>
#include <random>
#include <chrono>

#include <omp.h>

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
		random_device rd;
		mt19937 gen;
		uniform_real_distribution<double> dist(0, 1);

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
	Graph g(5000, 0.5, false);
	//cout << g;
	chrono::duration<double> dur = chrono::system_clock::now() - start;
	cout << dur.count() << endl;
	return 0;
}