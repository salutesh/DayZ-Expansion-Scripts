class CF_VehicleSurface
{
	static ref map<string, ref CF_VehicleSurface> s_VehicleSurfaces = new map<string, ref CF_VehicleSurface>();

	string m_Name;

	float m_NoiseSteer;
	float m_NoiseFrequency;
	float m_Roughness;
	float m_FrictionOffroad;
	float m_FrictionSlick;
	float m_RollResistance;
	float m_RollDrag;

	void CF_VehicleSurface(string name)
	{
		m_Name = name;

		s_VehicleSurfaces[m_Name] = this;

		string path;
		string rootPath = "CfgVehicleSurfaces " + m_Name + " ";

		path = rootPath + "noiseSteer";
		if (GetGame().ConfigIsExisting(path))
			m_NoiseSteer = GetGame().ConfigGetFloat(path);

		path = rootPath + "noiseFrequency";
		if (GetGame().ConfigIsExisting(path))
			m_NoiseFrequency = GetGame().ConfigGetFloat(path);

		path = rootPath + "roughness";
		if (GetGame().ConfigIsExisting(path))
			m_Roughness = GetGame().ConfigGetFloat(path);

		path = rootPath + "frictionOffroad";
		if (GetGame().ConfigIsExisting(path))
			m_FrictionOffroad = GetGame().ConfigGetFloat(path);

		path = rootPath + "frictionSlick";
		if (GetGame().ConfigIsExisting(path))
			m_FrictionSlick = GetGame().ConfigGetFloat(path);

		path = rootPath + "rollResistance";
		if (GetGame().ConfigIsExisting(path))
			m_RollResistance = GetGame().ConfigGetFloat(path);

		path = rootPath + "rollDrag";
		if (GetGame().ConfigIsExisting(path))
			m_RollDrag = GetGame().ConfigGetFloat(path);
	}

	static CF_VehicleSurface Get(string name)
	{
		auto surf = s_VehicleSurfaces[name];
		if (!surf)
			return new CF_VehicleSurface(name);

		return surf;
	}

#ifdef CF_DebugUI
	bool CF_OnDebugUpdate(CF_Debug instance, CF_DebugUI_Type type)
	{
		instance.Add("Name", m_Name);
		instance.Add("Noise Steer", m_NoiseSteer);
		instance.Add("Noise Frequency", m_NoiseFrequency);
		instance.Add("Roughness", m_Roughness);
		instance.Add("Friction Offroad", m_FrictionOffroad);
		instance.Add("Friction Slick", m_FrictionSlick);
		instance.Add("Roll Resistance", m_RollResistance);
		instance.Add("Roll Drag", m_RollDrag);

		return true;
	}
#endif
};
