#ifndef RANDOMUNIFORMBUILDER_H
#define RANDOMUNIFORMBUILDER_H

#include "graph.h"
#include "graphbuilder.h"
#include "executionstrategy.h"


template <ExecutionStrategy, class RandomEngine>
class RandomUniform;


template <class RandomEngine>
class GraphBuilder<RandomUniform<ExecutionStrategy::Sequential, RandomEngine>>
{
public:
	Graph create(int nVertices, double pEdge, bool directed);
};


template <class RandomEngine>
class GraphBuilder<RandomUniform<ExecutionStrategy::ParallelOmp, RandomEngine>>
{
public:
	Graph create(int nVertices, double pEdge, bool directed);
};


#endif // RANDOMUNIFORMBUILDER_H