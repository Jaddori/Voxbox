#pragma once

#include "Swap.h"
#include "Array.h"

template<typename T>
class SwapArray : public Swap<Array<T>>
{
public:
	void swap() override
	{
		data[SWAP_READ].fastCopy( data[SWAP_WRITE] );
	}
};