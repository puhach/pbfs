#include "pennant.h"

std::unique_ptr<Pennant> Pennant::split()
{
    if (this->size < 2)
        return nullptr;

    std::unique_ptr<Pennant> other = std::move(this->left);
    this->left = std::move(other->right);   // other->right will be set to nullptr
    return other;
}
