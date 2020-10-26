#ifndef BAG_H
#define BAG_H

#include "pennant.h"

#include <vector>
#include <memory>

class Bag
{
public:
	Bag();

	// A convenience method for inserting a single vertex into a bag
	void insert(int vertex);

	//void insert(std::unique_ptr<Pennant> pennant);

private:
	std::vector<std::unique_ptr<Pennant>> pennants;
	//std::vector<Pennant> pennants;
};	// Bag

#endif	// BAG_H