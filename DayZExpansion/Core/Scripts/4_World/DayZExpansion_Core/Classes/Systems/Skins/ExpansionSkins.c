/**
 * ExpansionSkins.c
 *
 * DayZ Expansion Mod
 * www.dayzexpansion.com
 * © 2022 DayZ Expansion Mod Team
 *
 * This work is licensed under the Creative Commons Attribution-NonCommercial-NoDerivatives 4.0 International License. 
 * To view a copy of this license, visit http://creativecommons.org/licenses/by-nc-nd/4.0/.
 *
*/

class ExpansionSkins : Managed
{
	private ref map< string, ref ExpansionSkin > m_Skins;
	private ref array< string > m_Order;

	private string m_DefaultSkin;
	
	// ------------------------------------------------------------
	// ExpansionSkins Constructor
	// ------------------------------------------------------------	
	void ExpansionSkins( string defaultSkin )
	{
#ifdef EXPANSIONTRACE
		auto trace = CF_Trace_0(ExpansionTracing.SKIN, this, "ExpansionSkins");
#endif

		m_DefaultSkin = defaultSkin;
		m_Skins = new map< string, ref ExpansionSkin >;
		m_Order = new array< string >;
	}
	
	// ------------------------------------------------------------
	// ExpansionSkins AddSkin
	// ------------------------------------------------------------	
	void AddSkin( string name, string path )
	{
#ifdef EXPANSIONTRACE
		auto trace = CF_Trace_0(ExpansionTracing.SKIN, this, "AddSkin");
#endif

		ExpansionSkin skin = new ExpansionSkin;
		JsonFileLoader< ExpansionSkin >.JsonLoadFile( path, skin );
		
		m_Skins.Insert( name, skin );
		m_Order.Insert( name );
	}
	
	// ------------------------------------------------------------
	// ExpansionSkins Count
	// ------------------------------------------------------------
	int Count()
	{
		return m_Skins.Count();
	}
	
	// ------------------------------------------------------------
	// ExpansionSkins Find
	// ------------------------------------------------------------	
	int Find( string name )
	{
		return m_Order.Find( name );
	}
	
	// ------------------------------------------------------------
	// ExpansionSkins GetName
	// ------------------------------------------------------------
	string GetName( int index )
	{
		return m_Order[index];
	}
	
	// ------------------------------------------------------------
	// ExpansionSkins Get
	// ------------------------------------------------------------
	ExpansionSkin Get( int index )
	{
		return m_Skins[GetName(index)];
	}
	
	// ------------------------------------------------------------
	// ExpansionSkins Get
	// ------------------------------------------------------------
	ExpansionSkin Get( string name )
	{
		return m_Skins[name];
	}
	
	// ------------------------------------------------------------
	// ExpansionSkins GetDefaultSkin
	// ------------------------------------------------------------
	string GetDefaultSkin()
	{
		return m_DefaultSkin;
	}
	
	// ------------------------------------------------------------
	// ExpansionSkins Sort
	// ------------------------------------------------------------
	void Sort()
	{
		m_Order.Sort();
	}
};