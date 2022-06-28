class eAIDynamicPatrolSpawner<Class T>
{
	#ifdef EAI_DEBUG_EVENTPATROL
	ExpansionMarkerModule m_MarkerModule;
	ExpansionMarkerData m_ServerMarker;
	#endif

	eAIDynamicPatrol m_ExpansionAIPatrol;
	bool m_Initialized;

	T m_Object;

	void eAIDynamicPatrolSpawner(T object)
	{
		m_Object = object;
	}

	void Init()
	{
		if (m_Initialized)
			return;

		m_Initialized = true;

		string type = m_Object.GetType();

		m_ExpansionAIPatrol = ExpansionAIPatrolManager.InitObjectPatrol(type, m_Object.GetPosition());
		if (!m_ExpansionAIPatrol)
			return;

		EXTrace.Print(EXTrace.AI, this, "::Init " + type);

		#ifdef EAI_DEBUG_EVENTPATROL
		if (!CF_Modules<ExpansionMarkerModule>.Get(m_MarkerModule))
			return;

		string icon;
		int color;

		//color = m_Object.GetType().Hash();
		//int a, r, g, b;

		//ExpansionStatic.IntToARGB(color, a, b, g, r);

		//float max;

		//if (r > g && r > b)
			//max = r;
		//else if (g > r && g > b)
			//max = g;
		//else
			//max = b;

		//color = ARGB(255, r / max * 255, g / max * 255, b / max * 255);

		if (m_Object.IsInherited(CrashBase))
		{
			icon = "Helicopter";
			color = ARGB(255, 235, 90, 50);
		}
		else if (m_Object.IsInherited(EffectArea))
		{
			icon = "Gas Mask";
			color = ARGB(255, 235, 220, 0);
		}
		else if (type.IndexOf("Land_Wreck_") == 0 && type.Contains("police"))
		{
			icon = "Vehicle Crash";
			color = ARGB(255, 90, 90, 235);
		}
		else
		{
			icon = "Grab";
			color = ARGB(255, 180, 180, 200);
		}

		m_ServerMarker = m_MarkerModule.CreateServerMarker(type, icon, m_Object.GetPosition(), color, true);
		#endif
	}

	void ~eAIDynamicPatrolSpawner()
	{
		if (!GetGame() || !GetGame().IsServer())
			return;

		#ifdef EAI_DEBUG_EVENTPATROL
		if (m_ServerMarker && m_MarkerModule)
			m_MarkerModule.RemoveServerMarker(m_ServerMarker.GetUID());
		#endif

		if (m_ExpansionAIPatrol)
		{
			m_ExpansionAIPatrol.Despawn();
			m_ExpansionAIPatrol.Delete();
		}
	}
};
