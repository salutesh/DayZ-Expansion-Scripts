/* TODO: Move to Community Framework */

class __Stack<Class T>
{
	private T _data[256];
	private int _count;
	
	void __Stack()
	{
	}

	T Pop()
	{
		T value;
		
		if ( _count <= 0 )
		{
			_count = 0;
		} else
		{
			value = _data[ _count - 1 ];
			_count--;
		}
		
		return value;
	}

	T Peek()
	{
		return _data[ _count - 1 ];
	}

	T Push( T value )
	{
		_data[ _count ] = value;
				
		_count++;
		
		return value;
	}
	
	array<T> ToArray()
	{
		array<T> arr = new array<T>();
		for ( int i = 0; i < _count; i++ )
			arr.Insert(_data[i]);
		return arr;
	}
	
	void Clear()
	{
		_count = 0;
	}
	
	T Get( int idx )
	{
		return _data[idx];
	}
	
	void Set( int idx, T value )
	{
		_data[idx] = value;
	}
	
	int Count()
	{
		return _count;
	}
};