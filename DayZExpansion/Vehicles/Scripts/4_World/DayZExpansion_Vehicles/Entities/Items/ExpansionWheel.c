class ExpansionWheelBase : ItemBase
{
	float m_Mass;
	float m_Radius;
	float m_Friction;
	float m_Width;

	float m_TyreRollResistance;
	float m_TyreRollDrag;
	float m_TyreRoughness;
	float m_TyreTread;

	void ExpansionWheelBase()
	{
		string path;

		path = "CfgVehicles " + GetType() + " mass";
		m_Mass = g_Game.ConfigGetFloat(path);

		path = "CfgVehicles " + GetType() + " radius";
		m_Radius = g_Game.ConfigGetFloat(path);

		path = "CfgVehicles " + GetType() + " friction";
		m_Friction = g_Game.ConfigGetFloat(path);

		path = "CfgVehicles " + GetType() + " width";
		m_Width = g_Game.ConfigGetFloat(path);

		path = "CfgVehicles " + GetType() + " tyreRollResistance";
		m_TyreRollResistance = g_Game.ConfigGetFloat(path);

		path = "CfgVehicles " + GetType() + " tyreRollDrag";
		m_TyreRollDrag = g_Game.ConfigGetFloat(path);

		path = "CfgVehicles " + GetType() + " tyreRoughness";
		m_TyreRoughness = g_Game.ConfigGetFloat(path);

		path = "CfgVehicles " + GetType() + " tyreTread";
		m_TyreTread = g_Game.ConfigGetFloat(path);
	}

	override bool Expansion_CarContactActivates()
	{
		return true;
	}
};