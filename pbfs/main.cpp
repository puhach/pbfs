//#include "randomuniforminitializer.h"
#include "randomuniformbuilder.h"
#include "executionstrategy.h"

#include <random>
#include <chrono>


using namespace std;




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
	//Graph<RandomUniformInitializer> g(5000, 0.5, false);
	//Graph g(RandomUniformInitializer< std::mt19937>(5000, 0.5, false));
	//Graph g(RandomUniformInitializer(5000, 0.5, false));

	//GraphBuilder<ExecutionStrategy::Sequential> builder;
	//Graph g = builder.createUniform<std::mt19937>(5000, 0.5, true);

	//RandomUniformBuilder<std::mt19937> builder;
	//Graph g = builder.create<ExecutionStrategy::Sequential>(5000, 0.5, true);
	//RandomUniformBuilder<ExecutionStrategy::Sequential> builder;
	//Graph g = builder.create<std::mt19937>(500, 0.5, true);

	RandomUniformBuilder<ExecutionStrategy::ParallelOmp, std::mt19937> builder;
	Graph g = builder.create(5, 0.5, false);
	cout << g;

	auto start = chrono::system_clock::now();
	std::vector<int> dist = g.pbfs(3);
	chrono::duration<double> dur = chrono::system_clock::now() - start;
	cout << dur.count() << endl;
	copy(dist.begin(), dist.end(), ostream_iterator<int>(cout, " "));

	return 0;
}