/**
 * ExpansionVehicleBase.c
 *
 * DayZ Expansion Mod
 * www.dayzexpansion.com
 * © 2020 DayZ Expansion Mod Team
 *
 * This work is licensed under the Creative Commons Attribution-NonCommercial-NoDerivatives 4.0 International License. 
 * To view a copy of this license, visit http://creativecommons.org/licenses/by-nc-nd/4.0/.
 *
*/

modded class ExpansionVehicleBase
{
	protected bool m_SafeZone;
	protected bool m_SafeZoneSynchRemote;

	// ------------------------------------------------------------
	void ExpansionVehicleBase()
	{
		RegisterNetSyncVariableBool( "m_SafeZoneSynchRemote" );
	}

	// ------------------------------------------------------------
	bool IsInSafeZone()
	{
		return m_SafeZone;
	}

	// ------------------------------------------------------------
	override bool CanBeDamaged()
	{
		if ( GetExpansionSettings().GetSafeZone().Enabled && !GetExpansionSettings().GetSafeZone().EnableVehicleinvincibleInsideSafeZone )
		{
			if ( IsInSafeZone() )
			{
				return false;
			}
		}

		return super.CanBeDamaged();
	}
}
