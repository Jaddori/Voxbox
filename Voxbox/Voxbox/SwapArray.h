#pragma once

#include "Swap.h"
#include "Array.h"

template<typename T>
class SwapArray : public Swap<Array<T>>
{
public:
	void swap() override
	{
		Swap<Array<T>>::data[SWAP_READ].fastCopy( Swap<Array<T>>::data[SWAP_WRITE] );
	}
};
