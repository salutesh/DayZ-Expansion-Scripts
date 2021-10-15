class CF_Surface
{
	static ref map<string, ref CF_Surface> s_Surfaces = new map<string, ref CF_Surface>();

	string m_Name;

	float m_Interior;
	float m_Deflection;
	float m_Friction;
	float m_Restitution;

	string m_SoundEnvironent;
	string m_SoundHit;
	string m_Character;

	float m_FootDamage;
	float m_Audibility;

	string m_Impact;

	bool m_IsDigable;
	bool m_IsFertile;

	float m_ChanceForCatch;
	float m_WindModifier;

	CF_VehicleSurface m_VehicleSurface;

	void CF_Surface(string name)
	{
		m_Name = name;

		s_Surfaces[m_Name] = this;

		string path;
		string rootPath = "CfgSurfaces " + m_Name + " ";

		path = rootPath + "interior";
		if (GetGame().ConfigIsExisting(path))
			m_Interior = GetGame().ConfigGetFloat(path);

		path = rootPath + "deflection";
		if (GetGame().ConfigIsExisting(path))
			m_Deflection = GetGame().ConfigGetFloat(path);

		path = rootPath + "friction";
		if (GetGame().ConfigIsExisting(path))
			m_Friction = GetGame().ConfigGetFloat(path);

		path = rootPath + "restitution";
		if (GetGame().ConfigIsExisting(path))
			m_Restitution = GetGame().ConfigGetFloat(path);

		path = rootPath + "soundEnviron";
		if (GetGame().ConfigIsExisting(path))
			m_SoundEnvironent = GetGame().ConfigGetTextOut(path);

		path = rootPath + "soundHit";
		if (GetGame().ConfigIsExisting(path))
			m_SoundHit = GetGame().ConfigGetTextOut(path);

		path = rootPath + "character";
		if (GetGame().ConfigIsExisting(path))
			m_Character = GetGame().ConfigGetTextOut(path);

		path = rootPath + "footDamage";
		if (GetGame().ConfigIsExisting(path))
			m_FootDamage = GetGame().ConfigGetFloat(path);

		path = rootPath + "audibility";
		if (GetGame().ConfigIsExisting(path))
			m_Audibility = GetGame().ConfigGetFloat(path);

		path = rootPath + "impact";
		if (GetGame().ConfigIsExisting(path))
			m_Impact = GetGame().ConfigGetTextOut(path);

		path = rootPath + "isDigable";
		if (GetGame().ConfigIsExisting(path))
			m_IsDigable = GetGame().ConfigGetInt(path);

		path = rootPath + "isFertile";
		if (GetGame().ConfigIsExisting(path))
			m_IsFertile = GetGame().ConfigGetInt(path);

		path = rootPath + "chanceForCatch";
		if (GetGame().ConfigIsExisting(path))
			m_ChanceForCatch = GetGame().ConfigGetFloat(path);

		path = rootPath + "windModifier";
		if (GetGame().ConfigIsExisting(path))
			m_WindModifier = GetGame().ConfigGetFloat(path);

		path = rootPath + "vpSurface";
		if (GetGame().ConfigIsExisting(path))
			m_VehicleSurface = CF_VehicleSurface.Get(GetGame().ConfigGetTextOut(path));
	}

	static CF_Surface Get(string name)
	{
		auto surf = s_Surfaces[name];
		if (!surf)
			return new CF_Surface(name);

		return surf;
	}

#ifdef CF_DebugUI
	bool CF_OnDebugUpdate(CF_Debug instance, CF_DebugUI_Type type)
	{
		instance.Add("Name", m_Name);
		instance.Add("Interior", m_Interior);
		instance.Add("Deflection", m_Deflection);
		instance.Add("Friction", m_Friction);
		instance.Add("Restitution", m_Restitution);
		instance.Add("Sound Environent", m_SoundEnvironent);
		instance.Add("Sound Hit", m_SoundHit);
		instance.Add("Character", m_Character);
		instance.Add("Foot Damage", m_FootDamage);
		instance.Add("Audibility", m_Audibility);
		instance.Add("Impact", m_Impact);
		instance.Add("Is Digable", m_IsDigable);
		instance.Add("Is Fertile", m_IsFertile);
		instance.Add("Chance For Catch", m_ChanceForCatch);
		instance.Add("Wind Modifier", m_WindModifier);
		instance.Add(m_VehicleSurface);

		return true;
	}
#endif
};
