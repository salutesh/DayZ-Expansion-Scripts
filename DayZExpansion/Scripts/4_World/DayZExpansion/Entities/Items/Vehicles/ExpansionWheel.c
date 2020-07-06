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

	void ExpansionWheel()
	{
		string mass_path = "CfgVehicles " + GetType() + " mass";
		m_Mass = GetGame().ConfigGetFloat( mass_path );

		string radius_path = "CfgVehicles " + GetType() + " radius";
		m_Radius = GetGame().ConfigGetFloat( radius_path );

		string width_path = "CfgVehicles " + GetType() + " width";
		m_Width = GetGame().ConfigGetFloat( width_path );
	}
}