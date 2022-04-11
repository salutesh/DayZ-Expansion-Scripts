/**
 * ZombieBase.c
 *
 * DayZ Expansion Mod
 * www.dayzexpansion.com
 * © 2022 DayZ Expansion Mod Team
 *
 * This work is licensed under the Creative Commons Attribution-NonCommercial-NoDerivatives 4.0 International License. 
 * To view a copy of this license, visit http://creativecommons.org/licenses/by-nc-nd/4.0/.
 *
*/

modded class ZombieBase
{	
	private static ref set< ZombieBase > m_allInfected = new set< ZombieBase >;
	
	protected autoptr ExpansionZoneActor m_Expansion_SafeZoneInstance = new ExpansionZoneEntity<ZombieBase>(this);

	protected bool m_SafeZone;
	
	// ------------------------------------------------------------
	// ZombieBase Consturctor
	// ------------------------------------------------------------	
	void ZombieBase()
	{
		m_allInfected.Insert( this );
	}
	
	// ------------------------------------------------------------
	// ZombieBase Destructor
	// ------------------------------------------------------------	
	void ~ZombieBase()
	{
		if (!GetGame())
			return;

		int idx = m_allInfected.Find( this );
		if ( idx >= 0 )
		{
			m_allInfected.Remove( idx );
		}
	}
	
	// ------------------------------------------------------------
	// ZombieBase GetAll
	// ------------------------------------------------------------	
	static set< ZombieBase > GetAll()
	{
		return m_allInfected;
	}
	
	
	// ------------------------------------------------------------
	// ZombieBase OnEnterZone
	// ------------------------------------------------------------	
	void OnEnterZone(ExpansionZoneType type)
	{
#ifdef EXPANSIONTRACE
		auto trace = CF_Trace_0(ExpansionTracing.ZONES, this, "OnEnterZone");
#endif

		if (type == ExpansionZoneType.SAFE)
		{
			m_SafeZone = true;

			GetGame().GetCallQueue( CALL_CATEGORY_SYSTEM ).Call( GetGame().ObjectDelete, this );
		}
	}
	
	// ------------------------------------------------------------
	// ZombieBase OnLeavingSafeZone
	// ------------------------------------------------------------
	void OnExitZone(ExpansionZoneType type)
	{
#ifdef EXPANSIONTRACE
		auto trace = CF_Trace_0(ExpansionTracing.ZONES, this, "OnExitZone");
#endif

		if (type == ExpansionZoneType.SAFE)
		{
			m_SafeZone = false;
		}
	}

	// ------------------------------------------------------------
	// ZombieBase IsInSafeZone
	// ------------------------------------------------------------
	bool IsInSafeZone()
	{
		return m_SafeZone;
	}
}