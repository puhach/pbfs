#include "pennant.h"

#include <stdexcept>
#include <cassert>

#include <iostream>     // !TEST!


Pennant::Pennant(const Pennant& other)
    : vertex(other.vertex)
    , size(other.size)
    , left(other.left ? std::make_unique<Pennant>(*other.left) : nullptr)
    , right(other.right ? std::make_unique<Pennant>(*other.right) : nullptr)
{
    std::cout << "Pennant::Pennant(other) copy" << std::endl;

}


std::unique_ptr<Pennant> Pennant::split()
{
    if (this->size < 2)
        return nullptr;

    std::unique_ptr<Pennant> other = std::move(this->left);
    this->left = std::move(other->right);   // other->right will be set to nullptr
    
    this->size >>= 1;   // divide the size by two
    assert(this->size == other->size);

    return other;
}


void Pennant::merge(std::unique_ptr<Pennant>& other)
{
    if (!other)
        return;

    if (other->getSize() != this->size)
        throw std::runtime_error("Failed to merge pennants of a different size.");

    assert((this->size << 1) == other->size + this->size);

    //this->left.swap(other);
    //this->left->right = std::move(other);
    other->right = std::move(this->left);
    this->left = std::move(other);
    this->size <<= 1;   // multiply by two
}