/*
 * CarScript.c
 *
 * DayZ Expansion Mod
 * www.dayzexpansion.com
 * © 2021 DayZ Expansion Mod Team
 *
 * This work is licensed under the Creative Commons Attribution-NonCommercial-NoDerivatives 4.0 International License. 
 * To view a copy of this license, visit http://creativecommons.org/licenses/by-nc-nd/4.0/.
 *
*/

/**@class		CarScript
 * @brief		
 **/
modded class CarScript
{
	private static ref set< CarScript > m_allVehicles = new set< CarScript >;
	
	// Safezone
	protected bool m_SafeZone;
	
	// ------------------------------------------------------------
	// Constructor
	// ------------------------------------------------------------
	void CarScript()
	{
		#ifdef EXPANSIONEXPRINT
		EXPrint("CarScript::CarScript - Start");
		#endif

		m_allVehicles.Insert( this );
	}
	
	// ------------------------------------------------------------
	// Destructor
	// ------------------------------------------------------------
	void ~CarScript()
	{
		#ifdef EXPANSIONEXPRINT
		EXPrint("CarScript::~CarScript - Start");
		#endif

		int i;

		i = m_allVehicles.Find( this );
		if ( i >= 0 )
		{
			m_allVehicles.Remove( i );
		}
		
		#ifdef EXPANSIONEXPRINT
		EXPrint("CarScript::~CarScript - End");
		#endif
	}
	
	static set< CarScript > GetAll()
	{
		return m_allVehicles;
	}

	array< ExpansionSkin > ExpansionGetSkins()
	{
		return NULL;
	}

	bool ExpansionHasSkin( int skinIndex )
	{
		return false;
	}
	
	void ExpansionSetSkin( int skinIndex )
	{
	}
	
	// ------------------------------------------------------------
	bool IsInSafeZone()
	{
		return m_SafeZone;
	}

	// ------------------------------------------------------------
	bool CanBeDamaged()
	{
		if ( GetExpansionSettings().GetSafeZone().Enabled && IsInSafeZone() )
		{
			return !GetExpansionSettings().GetSafeZone().EnableVehicleinvincibleInsideSafeZone;
		}

		return true;
	}

	// ------------------------------------------------------------
	// Called only server side
	// ------------------------------------------------------------
	void OnEnterSafeZone()
	{
		EXPrint(ToString() + "::OnEnterSafeZone " + GetPosition());

		m_SafeZone = true;

		if ( GetExpansionSettings().GetSafeZone().EnableVehicleinvincibleInsideSafeZone )
			SetAllowDamage(false);
	}

	// ------------------------------------------------------------
	// Called only server side
	// ------------------------------------------------------------
	void OnLeftSafeZone()
	{
		EXPrint(ToString() + "::OnLeftSafeZone " + GetPosition());

		m_SafeZone = false;

		if ( CanBeDamaged() )
			SetAllowDamage(true);
	}
};