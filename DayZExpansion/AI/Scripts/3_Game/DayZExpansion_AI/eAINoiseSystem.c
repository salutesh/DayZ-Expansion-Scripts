enum eAINoiseType
{
	SHOT,
	SOUND,
	EXPLOSION,
	BULLETIMPACT
}

class eAINoiseParams: Class
{
	string m_Path;
	float m_Strength;
	eAINoiseType m_Type;

	void eAINoiseParams(string path, eAINoiseType type = -1)
	{
		m_Path = path;
		m_Strength = g_Game.ConfigGetFloat(path + " strength");

		if (type == -1)
		{
			switch (g_Game.ConfigGetTextOut(path + " type"))
			{
				case "shot":
					m_Type = eAINoiseType.SHOT;
					break;
				default:
					m_Type = eAINoiseType.SOUND;
					break;
			}
		}
		else
		{
			m_Type = type;
		}

		switch (m_Type)
		{
			case eAINoiseType.SHOT:
				m_Strength = Math.Min(m_Strength * 13.75, 1100);  //! Hmm... scale strength (range) so strength 100 = 1375 but clamp to 1100
				break;

			case eAINoiseType.BULLETIMPACT:
				m_Strength *= 2;
				break;
		}

		EXTrace.Print(EXTrace.AI, this, path + " " + m_Strength + " " + typename.EnumToString(eAINoiseType, m_Type));
	}
}

class eAINoiseSystem
{
	static ref ScriptInvoker SI_OnNoiseAdded = new ScriptInvoker;

	static ref map<string, eAINoiseParams> s_NoiseParams = new map<string, eAINoiseParams>;

	static eAINoiseParams GetNoiseParams(string path, eAINoiseType type = -1)
	{
		eAINoiseParams params;
		if (!s_NoiseParams.Find(path, params))
		{
			params = new eAINoiseParams(path, type);
			s_NoiseParams.Insert(path, params);
		}

		return params;
	}

	static void AddNoise(EntityAI source, string path, float strengthMultiplier = 1.0, eAINoiseType type = -1)
	{
		eAINoiseParams params = GetNoiseParams(path, type);
		SI_OnNoiseAdded.Invoke(source, vector.Zero, -1, params, strengthMultiplier);
	}

	static void AddNoise(EntityAI source, vector position, string path, float strengthMultiplier = 1.0, eAINoiseType type = -1)
	{
		eAINoiseParams params = GetNoiseParams(path, type);
		SI_OnNoiseAdded.Invoke(source, position, -1, params, strengthMultiplier);
	}

	static void AddNoise(vector position, float lifetime, string path, float strengthMultiplier = 1.0, eAINoiseType type = -1)
	{
		eAINoiseParams params = GetNoiseParams(path, type);
		SI_OnNoiseAdded.Invoke(null, position, lifetime, params, strengthMultiplier);
	}

	static void AddNoiseEx(EntityAI source, eAINoiseParams params, float strengthMultiplier = 1.0)
	{
		SI_OnNoiseAdded.Invoke(source, vector.Zero, -1, params, strengthMultiplier);
	}

	static void AddNoiseEx(EntityAI source, vector position, eAINoiseParams params, float strengthMultiplier = 1.0)
	{
		SI_OnNoiseAdded.Invoke(source, position, -1, params, strengthMultiplier);
	}

	static void AddNoiseEx(vector position, float lifetime, eAINoiseParams params, float strengthMultiplier = 1.0)
	{
		SI_OnNoiseAdded.Invoke(null, position, lifetime, params, strengthMultiplier);
	}

	[CF_EventSubscriber(ScriptCaller.Create(eAINoiseSystem.OnGameDestroy), CF_LifecycleEvents.OnGameDestroy)]
	static void OnGameDestroy()
	{
		if (s_NoiseParams)
		{
			foreach (eAINoiseParams params: s_NoiseParams)
			{
				delete params;
			}
		}
	}
}
