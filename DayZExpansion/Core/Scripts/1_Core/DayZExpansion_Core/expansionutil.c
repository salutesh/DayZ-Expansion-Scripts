class ExpansionUtil<Class T>
{
	static void Swap(inout T a, inout T b)
	{
		T c = a;

		a = b;
		b = c;
	}
}

class ExpansionUtil2<Class T1, Class T2>
{
	static void RefCopy(map<T1, ref T2> from, map<T1, ref T2> to)
	{
		to.Clear();
		foreach (T1 key, T2 value: from)
		{
			to[key] = value;
		}
	}
}
