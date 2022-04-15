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
		m_Mass = GetGame().ConfigGetFloat(path);

		path = "CfgVehicles " + GetType() + " radius";
		m_Radius = GetGame().ConfigGetFloat(path);

		path = "CfgVehicles " + GetType() + " friction";
		m_Friction = GetGame().ConfigGetFloat(path);

		path = "CfgVehicles " + GetType() + " width";
		m_Width = GetGame().ConfigGetFloat(path);

		path = "CfgVehicles " + GetType() + " tyreRollResistance";
		m_TyreRollResistance = GetGame().ConfigGetFloat(path);

		path = "CfgVehicles " + GetType() + " tyreRollDrag";
		m_TyreRollDrag = GetGame().ConfigGetFloat(path);

		path = "CfgVehicles " + GetType() + " tyreRoughness";
		m_TyreRoughness = GetGame().ConfigGetFloat(path);

		path = "CfgVehicles " + GetType() + " tyreTread";
		m_TyreTread = GetGame().ConfigGetFloat(path);
	}

	override bool Expansion_CarContactActivates()
	{
		return true;
	}
};