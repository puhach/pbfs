#ifndef PENNANT_H
#define PENNANT_H

#include <cstddef>
#include <memory>

class Pennant
{
public:
	constexpr Pennant(int vertex) noexcept
		: vertex(vertex), size(1) {}

	// TODO: define copy and move constructors

	constexpr std::size_t getSize() const { return size; }

	std::unique_ptr<Pennant> split();

	void merge(std::unique_ptr<Pennant>& other);

private:
	int vertex;
	std::size_t size;
	std::unique_ptr<Pennant> left, right;
};	// Pennant

#endif // PENNANT_H