class ExpansionHash
{
	int m_HashA;
	int m_HashB;

	void ExpansionHash(CF_String payload = CF_String.Empty)
	{
		if (payload)
			Update(payload);
	}

	void Update(CF_String payload)
	{
		Set(payload.Hash(), payload.Reverse().Hash());
	}

	void Set(int hashA, int hashB)
	{
		m_HashA = hashA;
		m_HashB = hashB;
	}

	bool IsEqual(ExpansionHash hash)
	{
		if (hash.m_HashA == m_HashA && hash.m_HashB == m_HashB)
			return true;

		return false;
	}

	/**
	 * @brief Returns is object inherited from parent class name hashed in this
	 */
	bool ObjectIsKindOf(Object object)
	{
		TStringArray fullPath = {};
		g_Game.ConfigGetObjectFullPath(object, fullPath);
	
		ExpansionHash tmpHash = new ExpansionHash();
		foreach (string tmp: fullPath)
		{
			tmp.ToLower();
			tmpHash.Update(tmp);
			if (tmpHash.IsEqual(this))
				return true;
		}
	
		return false;
	}

	void Write(ParamsWriteContext ctx)
	{
		ctx.Write(m_HashA);
		ctx.Write(m_HashB);
	}

	bool Read(ParamsReadContext ctx)
	{
		if (!ctx.Read(m_HashA))
			return false;

		if (!ctx.Read(m_HashB))
			return false;

		return true;
	}
}
