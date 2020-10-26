#include "bag.h"

Bag::Bag()
{

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
			//pennant->merge(pennants[i].release());	
			pennant->merge(pennants[i]);	// destructively reads the pennants[i] and sets it to null
		}
	}

	// If there were no empty slot, append it
	pennants.push_back(std::move(pennant));
}
