#ifndef PENNANT_H
#define PENNANT_H

#include <cstddef>

class Pennant
{
public:
	Pennant(int vertex)
		: vertex(vertex), size(1) {}

	constexpr std::size_t getSize() const { return size; }

private:
	int vertex;
	std::size_t size;
};	// Pennant

#endif // PENNANT_H