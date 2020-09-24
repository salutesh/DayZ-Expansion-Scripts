class ProxyExpansionWheel: ProxySubpart
{

}

#ifdef EXPANSION_HELI_TEMP
class ExpansionWheel: CarWheel
#else
class ExpansionWheel: ItemBase
#endif
{
	float m_Mass;
	float m_Radius;
	float m_Width;

	float m_TyreRollResistance;
	float m_TyreTread;

	void ExpansionWheel()
	{
		string path;

		path = "CfgVehicles " + GetType() + " mass";
		m_Mass = GetGame().ConfigGetFloat( path );

		path = "CfgVehicles " + GetType() + " radius";
		m_Radius = GetGame().ConfigGetFloat( path );

		path = "CfgVehicles " + GetType() + " width";
		m_Width = GetGame().ConfigGetFloat( path );

		path = "CfgVehicles " + GetType() + " tyreRollResistance";
		m_TyreRollResistance = GetGame().ConfigGetFloat( path );

		path = "CfgVehicles " + GetType() + " tyreTread";
		m_TyreTread = GetGame().ConfigGetFloat( path );
	}
}