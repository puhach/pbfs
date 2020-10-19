#ifndef RANDOMUNIFORMINITIALIZER_H
#define RANDOMUNIFORMINITIALIZER_H

//#include <random>
#include <vector>


//template <class RandomEngine>
class RandomUniformInitializer
{
public:

	enum ExecutionStrategy
	{
		Sequential,
		ParallelOmp
	};

	RandomUniformInitializer(int nVertices, double pEdge, bool directed);

	std::vector<std::vector<int>> getAdjacencyList() const& { return adj; }

	std::vector<std::vector<int>>&& getAdjacencyList()&& { return std::move(adj); }

private:

	template <ExecutionStrategy strategy>
	/*static*/ void init(int nVertices, double pEdge, bool directed, std::vector<std::vector<int>> &adj);

	std::vector<std::vector<int>> adj;
};


#endif	// RANDOMUNIFORMINITIALIZER_H
