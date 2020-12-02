#ifndef BAG_H
#define BAG_H

#include "pennant.h"

#include <vector>
#include <memory>

class Bag
{
public:
	// TODO: consider making this constructor constexpr and noexcept (as well as some other methods)
	Bag();

	// TODO: if needed, we can implement a copy constructor, but that will likely involve cloning pennants
	Bag(const Bag& other) = delete;	

	Bag(Bag&& other) = default;

	Bag& operator = (const Bag& other) = delete;

	Bag& operator = (Bag&& other) = default;

	// A convenience method for inserting a single vertex into a bag
	void insert(int vertex);

	//void insert(std::unique_ptr<Pennant> pennant);
	
	bool isEmpty() const { return pennants.empty(); }

	std::size_t getSize() const { return pennants.size(); }

	// According to C++ core guidelines (resource management rule summary), a raw pointer/reference is not owning
	Pennant* getPennant(int index) const { return pennants[index].get(); }
	//std::weak_ptr<Pennant> getPennant(int index) const { return pennants[index]; }

private:
	// TODO: consider using a different container
	std::vector<std::unique_ptr<Pennant>> pennants;
};	// Bag

#endif	// BAG_H