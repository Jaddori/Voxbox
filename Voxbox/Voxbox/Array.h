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

	T& operator[]( int index )
	{
		assert( index >= 0 && index < size );
		return data[index];
	}

	const T& operator[]( int index ) const
	{
		assert( index >= 0 && index < size );
		return data[index];
	}

	void add( const T& element )
	{
		if( size >= capacity )
			expand( capacity * 2 );

		data[size++] = element;
	}

	T& append()
	{
		if( size >= capacity )
			expand( capacity * 2 );
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

	void clear()
	{
		size = 0;
	}

	void copy( const Array& ref )
	{
		if( capacity < ref.capacity )
		{
			capacity = ref.capacity;

			delete[] data;
			data = new T[capacity];
		}

		size = ref.size;
		for( int i=0; i<ref.size; i++ )
			data[i] = ref.data[i];
	}

	void fastCopy( const Array& ref )
	{
		if( capacity < ref.capacity )
		{
			capacity = ref.capacity;

			delete[] data;
			data = new T[capacity];
		}

		size = ref.size;
		memcpy( data, ref.data, sizeof(T)*ref.size );
	}

	T& at( int index )
	{
		assert( index >= 0 && index < size );
		return data[index];
	}

	T& first()
	{
		assert( size > 0 );
		return data[0];
	}

	T& last()
	{
		assert( size > 0 );
		return data[size-1];
	}

	T* getData()
	{
		return data;
	}

	const T* getConstData() const
	{
		return data;
	}

	int getSize() const
	{
		return size;
	}

	int getCapacity() const
	{
		return capacity;
	}

	void expand( int newCapacity )
	{
		T* temp = new T[newCapacity];

		for( int i=0; i<size; i++ )
			temp[i] = data[i];

		capacity = newCapacity;

		delete[] data;
		data = temp;
	}

protected:
	T* data;
	int size, capacity;
};