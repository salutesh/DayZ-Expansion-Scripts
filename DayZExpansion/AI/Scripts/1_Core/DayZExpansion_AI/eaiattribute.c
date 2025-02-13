class eAIAttributeBase
{
	static ref map<string, eAIAttributeBase> s_Attributes = new map<string, eAIAttributeBase>;

	string m_Name;

	void eAIAttributeBase(string name)
	{
		m_Name = name;
	}

	void Sync(Class source, Class target)
	{
	}
}

class eAIAttribute<Class T>: eAIAttributeBase
{
	/**
	 * @brief Syncs the attribute between instances `source` and `target`
	 */
	override void Sync(Class source, Class target)
	{
		T value;
		EnScript.GetClassVar(source, m_Name, 0, value);
	#ifdef DIAG_DEVELOPER
		PrintFormat("%1 %2 %3 = %4 -> %5", source, T, m_Name, value, target);
	#endif
		EnScript.SetClassVar(target, m_Name, 0, value);
	}

	static eAIAttribute<T> Register(string name, map<string, eAIAttributeBase> attributes = s_Attributes)
	{
		eAIAttribute<T> attribute;

	/*
		if (attributes.Find(name, attribute))
		{
			EXError.Error(null, "Attribute already registered: " + name);
		}
		else
		{
			attribute = new eAIAttribute<T>(name);
			attributes[name] = attribute;
		}
	*/

		return attribute;
	}
}
