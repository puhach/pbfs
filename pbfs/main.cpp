#include "randomuniforminitializer.h"

#include <iostream>
#include <iomanip>
#include <vector>
#include <stdexcept>
#include <random>
#include <chrono>

#include <omp.h>

using namespace std;


//template <class Initializer>
class Graph
{
	friend ostream& operator << (ostream& stream, const Graph& g);
public:
	//Graph(int nVertices, double pEdge, bool directed);

	//template <typename ... Args>
	//Graph(Args ... args): adj(std::move(Initializer(std::forward<Args>(args)...)).getAdjacencyList()) {}

	template <class Initializer>
	Graph(Initializer initializer) : adj(std::move(initializer).getAdjacencyList()) {}

private:
	/*int nVertices;
	double pEdge;
	bool directed;*/
	vector<vector<int>> adj;
};


//class Graph
//{
//	friend ostream& operator << (ostream& stream, const Graph& g);
//public:
//	Graph(int nVertices, double pEdge, bool directed);
//	
//
//private:
//	int nVertices;
//	double pEdge;
//	bool directed;
//	vector<vector<int>> adj;
//};

int main(int argc, char* argv[])
{
	auto start = chrono::system_clock::now();
	//Graph<RandomUniformInitializer> g(5000, 0.5, false);
	//Graph g(RandomUniformInitializer< std::mt19937>(5000, 0.5, false));
	Graph g(RandomUniformInitializer(5000, 0.5, false));

	//cout << g;
	//chrono::duration<double> dur = chrono::system_clock::now() - start;
	//cout << dur.count() << endl;
	return 0;
}