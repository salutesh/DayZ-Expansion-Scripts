class ExpansionPolyFlags
{
	static int ToFlag(string name)
	{
		name.ToLower();

		typename type = PGPolyFlags;

		int count = type.GetVariableCount();
		int value;

		for (int i = 0; i < count; i++)
		{
			string variableName = type.GetVariableName(i);
			variableName.ToLower();

			if (type.GetVariableType(i) != int &&type.GetVariableValue(null, i, value) && variableName == name)
			{
				return value;
			}
		}

		return PGPolyFlags.NONE;
	}

	static int ToFlag(TStringArray names)
	{
		int flag = 0;
		foreach (auto name : names)
		{
			flag |= ToFlag(name);
		}

		return flag;
	}
};
