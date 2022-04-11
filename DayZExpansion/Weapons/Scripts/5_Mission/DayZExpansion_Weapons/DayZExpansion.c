/**
 * DayzExpansion.c
 *
 * DayZ Expansion Mod
 * www.dayzexpansion.com
 * © 2022 DayZ Expansion Mod Team
 *
 * This work is licensed under the Creative Commons Attribution-NonCommercial-NoDerivatives 4.0 International License.
 * To view a copy of this license, visit http://creativecommons.org/licenses/by-nc-nd/4.0/.
 *
*/

/**@class		DayZExpansion
 * @brief		
 **/
modded class DayZExpansion
{
	void DayZExpansion()
	{
#ifdef EXPANSIONTRACE
		auto trace = CF_Trace_0(ExpansionTracing.GLOBAL, this, "DayZExpansion");
#endif

		//! Confirm firing
		GetRPCManager().AddRPC( "DayZExpansion", "ConfirmWeaponFire", this, SingeplayerExecutionType.Server );
	}

	void ConfirmWeaponFire( CallType type, ParamsReadContext ctx, PlayerIdentity sender, Object target )
	{
#ifdef EXPANSIONTRACE
		auto trace = CF_Trace_0(ExpansionTracing.GLOBAL, this, "ConfirmWeaponFire");
#endif

		Param1< vector > data;
		if ( !ctx.Read( data ) )
			return;

		if ( type == CallType.Server )
		{
			Weapon_Base weapon = Weapon_Base.Cast( target );
			if ( !weapon )
				return;

			PlayerBase player;

			if ( GetGame().IsMultiplayer() )
			{
				if ( !sender )
					return;

				player = PlayerBase.GetPlayerByUID( sender.GetId() );
			} 
			else
			{
				player = PlayerBase.Cast( GetGame().GetPlayer() );
			}

			if ( !player || player != weapon.GetHierarchyRootPlayer() )
				return;

			weapon.ExpansionFire( player, data.param1 );
		}
	}
};
