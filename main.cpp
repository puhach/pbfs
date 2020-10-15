#include <iostream>
#include <vector>
#include <stdexcept>

using namespace std;

class Graph
{
public:
	Graph(int nVertices, double pEdge, bool directed);

private:
	int nVertices;
	double pEdge;
	bool directed;
};

Graph::Graph(int nVertices, double pEdge, bool directed)
	: nVertices(nVertices > 0 ? nVertices : throw invalid_argument("The number of vertices must be greater than zero."))
	, pEdge(pEdge >= 0 && pEdge <= 1 ? pEdge : throw invalid_argument("The probability of an edge must be in range 0..1."))
	, directed(directed)
{

}

int main(int argc, char* argv[])
{
	return 0;
}