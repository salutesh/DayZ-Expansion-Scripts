/**
 * ZombieBase.c
 *
 * DayZ Expansion Mod
 * www.dayzexpansion.com
 * © 2020 DayZ Expansion Mod Team
 *
 * This work is licensed under the Creative Commons Attribution-NonCommercial-NoDerivatives 4.0 International License. 
 * To view a copy of this license, visit http://creativecommons.org/licenses/by-nc-nd/4.0/.
 *
*/

modded class ZombieBase
{
	private static ref set< ZombieBase > m_allInfected = new set< ZombieBase >;

	private bool m_SafeZone;

	void ZombieBase()
	{
		m_allInfected.Insert( this );
	}

	void ~ZombieBase()
	{
		int idx = m_allInfected.Find( this );
		if ( idx >= 0 )
		{
			m_allInfected.Remove( idx );
		}
	}
	
	static set< ZombieBase > GetAll()
	{
		return m_allInfected;
	}

	void OnEnterSafeZone()
	{
		#ifdef EXPANSIONEXPRINT
		EXPrint("ZombieBase::OnEnterSafeZone - start");
		#endif

		m_SafeZone = true;

		GetGame().GetCallQueue( CALL_CATEGORY_SYSTEM ).Call( GetGame().ObjectDelete, this ); 

		#ifdef EXPANSIONEXPRINT
		EXPrint("ZombieBase::OnEnterSafeZone - end");
		#endif
	}

	void OnLeftSafeZone()
	{
		#ifdef EXPANSIONEXPRINT
		EXPrint("ZombieBase::OnLeftSafeZone - start");
		#endif

		m_SafeZone = false;

		#ifdef EXPANSIONEXPRINT
		EXPrint("ZombieBase::OnLeftSafeZone - end");
		#endif
	}

	bool IsInSafeZone()
	{
		return m_SafeZone;
	}

	override void RegisterTransportHit(Transport transport)
	{
		if ( transport.IsInherited( ExpansionHelicopterScript ) )
			return;

		super.RegisterTransportHit( transport );
	}
}