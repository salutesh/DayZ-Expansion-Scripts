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
	//! TODO: AllInfected isn't used anywhere, should get rid?
	private static ref set< ZombieBase > m_Expansion_AllInfected = new set< ZombieBase >;
	
	protected autoptr ExpansionZoneActor m_Expansion_SafeZoneInstance = new ExpansionZoneEntity<ZombieBase>(this);

	protected bool m_Expansion_IsInSafeZone;
	
	ref ExpansionNameOverride m_Expansion_NameOverride;

	// ------------------------------------------------------------
	// ZombieBase Consturctor
	// ------------------------------------------------------------	
	void ZombieBase()
	{
		m_Expansion_AllInfected.Insert( this );
	}
	
	// ------------------------------------------------------------
	// ZombieBase Destructor
	// ------------------------------------------------------------	
	void ~ZombieBase()
	{
		if (!GetGame())
			return;

		int idx = m_Expansion_AllInfected.Find( this );
		if ( idx >= 0 )
		{
			m_Expansion_AllInfected.Remove( idx );
		}
	}
	
	// ------------------------------------------------------------
	// ZombieBase GetAll
	// ------------------------------------------------------------	
	static set< ZombieBase > GetAll()
	{
		return m_Expansion_AllInfected;
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
			m_Expansion_IsInSafeZone = true;

			SetHealth(0);
			GetGame().GetCallQueue( CALL_CATEGORY_SYSTEM ).CallLater( GetGame().ObjectDelete, 5000, false, this );
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
			m_Expansion_IsInSafeZone = false;
		}
	}

	// ------------------------------------------------------------
	// ZombieBase IsInSafeZone
	// ------------------------------------------------------------
	bool IsInSafeZone()
	{
		return m_Expansion_IsInSafeZone;
	}

	override bool NameOverride(out string output)
	{
		if (m_Expansion_NameOverride && m_Expansion_NameOverride.Get(output))
			return true;
		else
			return super.NameOverride(output);
	}

	override void OnRPC(PlayerIdentity sender, int rpc_type, ParamsReadContext ctx)
	{
		super.OnRPC(sender, rpc_type, ctx);

		if (m_Expansion_NameOverride)
			m_Expansion_NameOverride.OnRPC(sender, rpc_type, ctx);
	}
}