#ifdef EXPANSION_MODSTORAGE
enum Expansion_ModStorageDataType
{
	INVALID = -1,
	BOOL,
	INT,
	FLOAT,
	VECTOR,
	STRING

	//! TODO
	//ARRAY_BOOL,
	//ARRAY_INT,
	//ARRAY_FLOAT,
	//ARRAY_VECTOR,
	//ARRAY_STRING
};

class Expansion_ModStorageDataBase
{
	typename ValueType()
	{
		return typename;
	}

	string ValueString()
	{
		return "";
	}
};

class Expansion_ModStorageData<Class T>: Expansion_ModStorageDataBase
{
	T m_Value;

	void Expansion_ModStorageData(T value)
	{
		m_Value = value;
	}

	T Get()
	{
		return m_Value;
	}

	override typename ValueType()
	{
		return T;
	}

	override string ValueString()
	{
		return string.ToString(m_Value);
	}
};
#endif
