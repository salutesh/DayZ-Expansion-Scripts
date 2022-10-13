/**
 * Expansion_Ammo_FlareSupplyBase.c
 *
 * DayZ Expansion Mod
 * www.dayzexpansion.com
 * © 2022 DayZ Expansion Mod Team
 *
 * This work is licensed under the Creative Commons Attribution-NonCommercial-NoDerivatives 4.0 International License. 
 * To view a copy of this license, visit http://creativecommons.org/licenses/by-nc-nd/4.0/.
 *
*/

class Expansion_Ammo_FlareSupplyBase: Ammo_Flare
{
	override void OnWorkStart()
	{
#ifdef EXPANSIONTRACE
		auto trace = CF_Trace_0(ExpansionTracing.MISSIONS, this, "OnWorkStart");
#endif

		super.OnWorkStart();

		if ( IsMissionHost() )
		{
			array< ref ExpansionLootContainer > containers = new array< ref ExpansionLootContainer >;
			ExpansionLootContainer container;

			//! Get all containers enabled for player-called supply drop use
			for ( int i = 0; i < GetExpansionSettings().GetAirdrop().Containers.Count(); i++ )
			{
				container = GetExpansionSettings().GetAirdrop().Containers[i];
				if ( container.Usage == 0 || container.Usage == 2 )
				{
					containers.Insert( container );
				}
			}

			if ( containers.Count() == 0 )
			{
				string errorMsg = "Airdrop settings do not contain compatible container (looked for container with Usage set to 0 or 2)";

				CF_Log.Error( errorMsg );

				ExpansionNotification(new StringLocaliser("STR_EXPANSION_MISSION_NOTIF_TITLE", "ERROR"), new StringLocaliser(errorMsg), EXPANSION_NOTIFICATION_ICON_AIRDROP, COLOR_EXPANSION_NOTIFICATION_MISSION).Create();
				return;
			}

			//! Pick a container (weighted random)
			container = ExpansionLootContainer.GetWeightedRandomContainer( containers );

			CF_Log.Debug("[ExpansionSupplySignal] Selected container: " + container.Container);
					
			int itemCount = container.ItemCount;
			if ( container.ItemCount <= 0 )
				itemCount = GetExpansionSettings().GetAirdrop().ItemCount;  //! Only kept for backwards compatibility, should be set per container

			TStringArray infected;
			int infectedCount;
			if ( container.SpawnInfectedForPlayerCalledDrops )
			{
				infected = container.Infected;
				infectedCount = container.InfectedCount;
			}

			container = new ExpansionLootContainer( container.Container, 2, 1, container.Loot, infected, itemCount, infectedCount );
			
			vector spawnPos = Vector( GetPosition()[0] + Math.RandomFloat( -150, 150 ), 0, GetPosition()[2] + Math.RandomFloat( -150, 150 ) );

			ExpansionAirdropPlane plane = ExpansionAirdropPlane.CreatePlane( Vector( spawnPos[0], 0, spawnPos[2] ), "", GetExpansionSettings().GetAirdrop().Radius, GetExpansionSettings().GetAirdrop().Height, GetExpansionSettings().GetAirdrop().Speed, container, new StringLocaliser( "STR_EXPANSION_MISSION_AIRDROP_CLOSING_ON_PLAYER" ), new StringLocaliser( "STR_EXPANSION_MISSION_AIRDROP_SUPPLIES_DROPPED_PLAYER" ) );

			if ( plane )
			{
				if ( GetExpansionSettings().GetNotification().ShowPlayerAirdropStarted )
					ExpansionNotification(new StringLocaliser("STR_EXPANSION_MISSION_NOTIF_TITLE", "Airdrop"), new StringLocaliser("STR_EXPANSION_MISSION_AIRDROP_HEADING_TOWARDS_PLAYER"), EXPANSION_NOTIFICATION_ICON_AIRDROP, COLOR_EXPANSION_NOTIFICATION_MISSION).Create();			

				if ( GetExpansionSettings().GetLog().MissionAirdrop )
					GetExpansionSettings().GetLog().PrintLog( "[Airdrop] A player called airdrop is heading toward " + Vector( spawnPos[0], 0, spawnPos[2] ) + " with a "+ container.Container );
			}
		}
	}
};
class Expansion_Ammo_FlareSupplyRed: Expansion_Ammo_FlareSupplyBase {};
class Expansion_Ammo_FlareSupplyGreen: Expansion_Ammo_FlareSupplyBase {};
class Expansion_Ammo_FlareSupplyBlue: Expansion_Ammo_FlareSupplyBase {};