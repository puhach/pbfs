#include "pennant.h"

#include <stdexcept>


std::unique_ptr<Pennant> Pennant::split()
{
    if (this->size < 2)
        return nullptr;

    std::unique_ptr<Pennant> other = std::move(this->left);
    this->left = std::move(other->right);   // other->right will be set to nullptr
    return other;
}


void Pennant::merge(std::unique_ptr<Pennant>& other)
{
    if (!other)
        return;

    if (other->getSize() != this->size)
        throw std::runtime_error("Failed to merge pennants of a different size.");

    //this->left.swap(other);
    //this->left->right = std::move(other);
    other->right = std::move(this->left);
    this->left = std::move(other);
}