#ifndef PENNANT_H
#define PENNANT_H

#include <cstddef>
#include <memory>
#include <cassert>

class Pennant
{
public:
	constexpr Pennant(int vertex) noexcept
		: vertex(vertex), size(1) {}

	// TODO: if needed, we can implement a copy constructor, but it may be inefficient
	Pennant(const Pennant& other) = delete;

	Pennant(Pennant&& other) = default;

	Pennant& operator = (const Pennant& other) = delete;

	Pennant& operator = (Pennant&& other) = default;

	constexpr int getVertex() const noexcept { assert(size > 0);  return vertex; }

	constexpr std::size_t getSize() const noexcept { return size; }

	std::unique_ptr<Pennant> split();

	void merge(std::unique_ptr<Pennant>& other);

private:
	int vertex;
	std::size_t size;
	std::unique_ptr<Pennant> left, right;
};	// Pennant

#endif // PENNANT_H