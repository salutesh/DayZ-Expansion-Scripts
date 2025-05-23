class EXType: Managed
{
	typename GetValueType()
	{
		return typename;
	}

	string ToString(bool type = false, bool name = false, bool quotes = true)
	{
		return string.ToString(string.Empty, type, name, quotes);
	}
}

class EXTypeT<Class T>: EXType
{
	T m_Value;

	void EXTypeT(T value)
	{
		m_Value = value;
	}

	T GetValue()
	{
		return m_Value;
	}

	override typename GetValueType()
	{
		return T;
	}

	override string ToString(bool type = false, bool name = false, bool quotes = true)
	{
		return string.ToString(m_Value, type, name, quotes);
	}
}

typedef EXTypeT<bool> EXBool;

typedef EXTypeT<float> EXFloat;

typedef EXTypeT<int> EXInt;

typedef EXTypeT<string> EXString;

typedef EXTypeT<vector> EXVector;

//! Legacy
typedef EXType ExpansionPrimitive;
class ExpansionPrimitiveT<Class T>: ExpansionPrimitive
{
	T m_Value;

	void ExpansionPrimitiveT(T value)
	{
		m_Value = value;
	}

	T GetValue()
	{
		return m_Value;
	}

	override typename GetValueType()
	{
		return T;
	}

	override string ToString(bool type = false, bool name = false, bool quotes = true)
	{
		return string.ToString(m_Value, type, name, quotes);
	}
}
