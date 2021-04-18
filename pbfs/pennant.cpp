#include "pennant.h"

#include <stdexcept>
#include <cassert>


// The copy constructor and the copy assignment operator are not noexcept because std::make_unique may fail to allocate memory

Pennant::Pennant(const Pennant& other)
    : vertex(other.vertex)
    , size(other.size)
    , left(other.left ? std::make_unique<Pennant>(*other.left) : nullptr)
    , right(other.right ? std::make_unique<Pennant>(*other.right) : nullptr)
{
    //std::cout << "Pennant::Pennant(other) copy" << std::endl;
}

Pennant& Pennant::operator = (const Pennant& other)
{
    this->vertex = other.vertex;
    this->size = other.size;
    this->left = other.left ? std::make_unique<Pennant>(*other.left) : nullptr;
    this->right = other.right ? std::make_unique<Pennant>(*other.right) : nullptr;
    return *this;
}


std::unique_ptr<Pennant> Pennant::split() noexcept
{
    if (this->size < 2)
        return nullptr;

    std::unique_ptr<Pennant> other = std::move(this->left);
    this->left = std::move(other->right);   // other->right will be set to nullptr
    
    this->size >>= 1;   // divide the size by two
    assert(this->size == other->size);

    return other;
}


void Pennant::merge(std::unique_ptr<Pennant> other)
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