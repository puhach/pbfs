#include "graph.h"

#include <iostream>

std::ostream& operator << (std::ostream& stream, const Graph& g)
{
	//for (int v = 0; v < g.nVertices; ++v)
	for (const auto& neighbors : g.adj)
	{
		auto v = &neighbors - &g.adj.front();
		stream << std::setw(5) << v << " | ";
		copy(neighbors.begin(), neighbors.end(), std::ostream_iterator<int>(stream, " "));
		stream << std::endl;
	}

	return stream;
}

std::vector<int> Graph::pbfs(int vertex) const
{
	std::vector<int> dist(this->adj.size(), -1);
	dist[vertex] = 0;

	int d = 0;

	Bag curBag;
	curBag.insert(vertex);

	while (!curBag.isEmpty())
	{
		Bag nextBag = processLevel(curBag, d++, dist);
	}

	return dist;
}

Bag Graph::processLevel(Bag& inBag, int level, std::vector<int>& dist) const
{
	Bag outBag;

	// TODO: the paper suggests a parallel for
	for (int i = 0; i < inBag.getSize(); ++i)	// the size here means the number of pennants in the bag
	{
		if (Pennant* pennant = inBag.getPennant(i))
			processPennant(pennant, outBag, level);
	}

	return outBag;
}
