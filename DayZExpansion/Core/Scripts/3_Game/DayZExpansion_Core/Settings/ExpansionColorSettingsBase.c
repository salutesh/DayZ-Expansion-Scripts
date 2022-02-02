/**
 * ExpansionColorSettingsBase.c
 *
 * DayZ Expansion Mod
 * www.dayzexpansion.com
 * © 2022 DayZ Expansion Mod Team
 *
 * This work is licensed under the Creative Commons Attribution-NonCommercial-NoDerivatives 4.0 International License.
 * To view a copy of this license, visit http://creativecommons.org/licenses/by-nc-nd/4.0/.
 *
*/

class ExpansionColorSettingsBase
{
	[NonSerialized()]
	ref map<string, int> m_Colors;

	void ExpansionColorSettingsBase()
	{
		m_Colors = new map<string, int>;
	}

	int Get(string name)
	{
		return m_Colors.Get(name);
	}

	void Set(string name, int color)
	{
		EnScript.SetClassVar(this, name, 0, ExpansionColor.ARGBToHex(color));

		_Set(name, color);
	}

	void Set(string name, string color)
	{
		EnScript.SetClassVar(this, name, 0, color);

		_Set(name, ExpansionColor.ToARGB(color));
	}

	void _Set(string name, int color)
	{
		if (m_Colors.Contains(name))
			m_Colors.Set(name, color);
		else
			m_Colors.Insert(name, color);
	}

	void OnSend(ParamsWriteContext ctx)
	{
		typename type = Type();
		int count = type.GetVariableCount();
		for (int i = 0; i < count; i++)
		{
			if (type.GetVariableType(i) != string)
				continue;

			string color;
			type.GetVariableValue(this, i, color);
			//! Network optimization: We send the color as int
			ctx.Write(ExpansionColor.ToARGB(color));
		}
	}

	void OnReceive(ParamsReadContext ctx)
	{
		typename type = Type();
		int count = type.GetVariableCount();
		for (int i = 0; i < count; i++)
		{
			if (type.GetVariableType(i) != string)
				continue;

			int color;
			ctx.Read(color);
			string name = type.GetVariableName(i);
			Set(name, color);
		}
	}

	void Update()
	{
		typename type = Type();
		int count = type.GetVariableCount();
		for (int i = 0; i < count; i++)
		{
			if (type.GetVariableType(i) != string)
				continue;

			string name = type.GetVariableName(i);
			string color;
			type.GetVariableValue(this, i, color);
			_Set(name, ExpansionColor.ToARGB(color));
		}
	}
}
