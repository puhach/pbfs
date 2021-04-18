#include "graphbuilder.h"
#include "randomuniformbuilder.h"
#include "executionstrategy.h"
#include "textfileloader.h"

#include <iostream>
#include <iterator>
#include <random>
#include <chrono>

using namespace std;



int main(int argc, char* argv[])
{	
	try
	{
		// Generate a random graph
		GraphBuilder<RandomUniform<ExecutionStrategy::ParallelOmp, std::mt19937>> builder;
		Graph g = builder.create(5000, 0.3, false);
		//g.save("./data/d.txt");
		//cout << g;

		/*GraphBuilder<TextFile> builder;
		Graph g = builder.create("./data/c.txt");
		cout << g;*/

		// Run BFS from the 3rd vertex using the parallel OpenMP strategy
		auto start = chrono::system_clock::now();
		std::vector<int> distP = g.bfs<ExecutionStrategy::ParallelOmp>(3);
		chrono::duration<double> durP = chrono::system_clock::now() - start;
		cout << durP.count() << endl;

		// Run BFS from the 3rd vertex using the sequential strategy
		start = chrono::system_clock::now();
		std::vector<int> distS = g.bfs<ExecutionStrategy::Sequential>(3);
		chrono::duration<double> durS = chrono::system_clock::now() - start;
		cout << endl << durS.count() << endl;
		//copy(distS.begin(), distS.end(), ostream_iterator<int>(cout, " "));

		if (distP == distS)
			cout << "\nOK" << endl;
		else
			cout << "\nMismatch!" << endl;

		return 0;
	}
	catch (const std::exception& e)
	{
		std::cerr << e.what() << std::endl;
		return -1;
	}
}