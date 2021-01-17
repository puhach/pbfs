#ifndef BAG_H
#define BAG_H

#include "pennant.h"

#include <vector>
#include <memory>

// TODO: check which methods can be marked noexcept
class Bag
{
public:
	// In C++17 this constructor can't be made constexpr since we are internally using std::vector, whose 
	// constructor is not constexpr until C++20	
	/*constexpr*/ Bag() = default;

	Bag(const Bag& other);	

	Bag(Bag&& other) = default;

	// TODO: define the copy assignment operator
	Bag& operator = (const Bag& other) = delete;

	Bag& operator = (Bag&& other) = default;

	// A convenience method for inserting a single vertex into a bag
	void insert(int vertex);

	//void insert(std::unique_ptr<Pennant> pennant);
	
	bool isEmpty() const { return pennants.empty(); }

	std::size_t getSize() const noexcept { return pennants.size(); }

	// According to C++ core guidelines (resource management rule summary), a raw pointer/reference is not owning
	Pennant* getPennant(int index) const { return pennants[index].get(); }
	//std::weak_ptr<Pennant> getPennant(int index) const { return pennants[index]; }

	Bag& merge(Bag&& other);

private:

	//std::unique_ptr<Pennant> fullAdd(std::unique_ptr<Pennant>& pennant, std::unique_ptr<Pennant>& other, std::unique_ptr<Pennant>& carry) const;
	//std::unique_ptr<Pennant> add(std::unique_ptr<Pennant>& pennant, std::unique_ptr<Pennant>& carry) const;
	void fullAdd(std::unique_ptr<Pennant> &pennant, std::unique_ptr<Pennant> &other, std::unique_ptr<Pennant> &carry) const;
	void add(std::unique_ptr<Pennant>& pennant, std::unique_ptr<Pennant>& carry) const;

	// TODO: consider using a different container
	std::vector<std::unique_ptr<Pennant>> pennants;
};	// Bag

#endif	// BAG_H