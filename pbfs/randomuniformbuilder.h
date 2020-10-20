#ifndef GRAPHBUILDER_H
#define GRAPHBUILDER_H

#include "graph.h"
#include "executionstrategy.h"

//template <class RandomEngine>
//class RandomUniformBuilder
//{
//public:
//	RandomUniformBuilder() = default;
//
//	template <ExecutionStrategy>
//	Graph create(int nVertices, double pEdge, bool directed);
//};
//
//template <class RandomEngine>
//Graph RandomUniformBuilder<RandomEngine>::create<ExecutionStrategy::Sequential>(int nVertices, double pEdge, bool directed)
////Graph RandomUniformBuilder::create<ExecutionStrategy::Sequential>(int nVertices, double pEdge, bool directed)
//{
//	std::vector<std::vector<int>> adj;
//	return Graph(adj);
//}

template <ExecutionStrategy, class RandomEngine>
class RandomUniformBuilder;


template <class RandomEngine>
class RandomUniformBuilder<ExecutionStrategy::Sequential, RandomEngine>
{
public:
	//RandomUniformBuilder() = default;

	Graph create(int nVertices, double pEdge, bool directed);
};


template <class RandomEngine>
class RandomUniformBuilder<ExecutionStrategy::ParallelOmp, RandomEngine>
{
public:
	Graph create(int nVertices, double pEdge, bool directed);
};

#endif // GRAPHBUILDER_H