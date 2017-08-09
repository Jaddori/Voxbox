#pragma once

#define ARRAY_DEFAULT_CAPACITY 10

template<typename T>
class Array
{
public:
	Array()
		: size( 0 ), capacity( ARRAY_DEFAULT_CAPACITY )
	{
		data = new T[capacity];
	}

	Array( int cap )
		: size( 0 ), capacity( cap )
	{
		data = new T[capacity];
	}

	~Array()
	{
		delete[] data;
	}

	void add( const T& element )
	{
		if( size >= capacity )
			expand( capacity * 2 );

		data[size] = element;
	}

	T& addInPlace()
	{
		return data[size++];
	}

	void remove( const T& element )
	{
		int index = find( element );
		
		assert( index >= 0 );
		removeAt( index );
	}

	void removeAt( int index )
	{
		assert( index >= 0 && index < size );

		if( index != size-1 )
		{
			data[index] = data[--size];
		}
	}

	int find( const T& element )
	{
		int index = -1;
		for( int i=0; i<size && index < 0; i++ )
			if( data[i] == element )
				index = i;
		return index;
	}

	T& at( int index )
	{
		assert( index >= 0 && index < size );
		return data[index];
	}

private:
	void expand( int newCapacity )
	{
		T* temp = new T[newCapacity];

		for( int i=0; i<size; i++ )
			temp[i] = data[i];

		capacity = newCapacity;
		
		delete[] data;
		data = temp;
	}

	T* data;
	int size, capacity;
};