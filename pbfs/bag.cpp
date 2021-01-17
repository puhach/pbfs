#include "bag.h"

//#include <iostream>		// TEST!

//Bag::Bag()
//{
//
//}

//Bag::Bag(const Bag& other)
//	: pennants(other.pennants.size())
//{
//	for (std::unique_ptr<Pennant>& pennant : this->pennants)
//	{
//		pennant = std::make_unique<Pennant>();
//	}
//}

Bag::Bag(const Bag& other)
{
	//std::cout << "Bag::Bag(other) copy" << std::endl;	// !TEST!

	this->pennants.reserve(other.pennants.size());
	for (const std::unique_ptr<Pennant>& pennant : other.pennants)
	{
		this->pennants.push_back(pennant ? std::make_unique<Pennant>(*pennant): nullptr);
	}
}


//void Bag::insert(std::unique_ptr<Pennant> pennant)
void Bag::insert(int vertex)
{
	auto pennant = std::make_unique<Pennant>(vertex);

	for (int i = 0; i < pennants.size(); ++i)
	{
		if (!pennants[i])
		{
			pennants[i] = std::move(pennant);
			return;
		}
		else
		{
			///pennant->merge(pennants[i].release());	
			//pennant->merge(pennants[i]);	// destructively reads the pennants[i] and sets it to null
			pennant->merge(std::move(pennants[i]));	
		}
	}

	// If there were no empty slot, append it
	pennants.push_back(std::move(pennant));
}


Bag& Bag::merge(Bag&& other)
{
	std::unique_ptr<Pennant> carry;

	if (getSize() > other.getSize())
	{
		for (auto i = 0; i < other.getSize(); ++i)
		{
			//carry = fullAdd(this->pennants[i], other.pennants[i], carry);
			fullAdd(this->pennants[i], other.pennants[i], carry);
		}

		for (auto i = other.getSize(); carry && i < getSize(); ++i)
		{
			//carry = add(this->pennants[i], carry);
			add(this->pennants[i], carry);
		}

		if (carry)
			this->pennants.push_back(std::move(carry));
	}	// this bag is larger
	else
	{
		for (auto i = 0; i < this->getSize(); ++i)
		{
			//carry = fullAdd(other.pennants[i], this->pennants[i], carry);
			fullAdd(other.pennants[i], this->pennants[i], carry);
		}

		for (auto i = this->getSize(); carry && i < other.getSize(); ++i)
		{
			//carry = add(other.pennants[i], carry);
			add(other.pennants[i], carry);
		}

		if (carry)
			other.pennants.push_back(std::move(carry));

		*this = std::move(other);
	}	// the other bag is larger

	return *this;
}	// merge

void Bag::fullAdd(std::unique_ptr<Pennant>& pennant, std::unique_ptr<Pennant>& other, std::unique_ptr<Pennant>& carry) const
{
	if (pennant)
	{
		if (other)
		{
			if (carry)
			{
				//carry->merge(other);
				carry->merge(std::move(other));
				assert(!other);
			}
			else
			{
				//pennant->merge(other);
				pennant->merge(std::move(other));
				assert(!other);
				carry = std::move(pennant);
				assert(!pennant);
			}	// !carry
		}	// other
		else // !other
		{
			if (carry)
			{
				//carry->merge(pennant);
				carry->merge(std::move(pennant));
				assert(!pennant);
			}

			// otherwise, do nothing
		}	// !other
	}	// pennant
	else // !pennant
	{
		if (other)
		{
			if (carry)
			{
				//carry->merge(other);
				carry->merge(std::move(other));
				assert(!other);
			}
			else
			{
				pennant = std::move(other);
				assert(!other);
			}	// !carry
		}	// other
		else
		{
			pennant = std::move(carry);
			assert(!carry);
		}	// !other
	}	// !pennant
}

//std::unique_ptr<Pennant> Bag::add(std::unique_ptr<Pennant>& pennant, std::unique_ptr<Pennant>& other) const
void Bag::add(std::unique_ptr<Pennant>& pennant, std::unique_ptr<Pennant>& carry) const
{
	if (pennant)
	{
		if (carry)
			carry->merge(std::move(pennant));
	}
	else
	{
		if (carry)
			pennant = std::move(carry);
	}
}