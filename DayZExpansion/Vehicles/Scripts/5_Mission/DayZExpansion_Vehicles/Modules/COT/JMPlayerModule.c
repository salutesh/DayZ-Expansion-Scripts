/**
 * JMPlayerModule.c
 *
 * DayZ Expansion Mod
 * www.dayzexpansion.com
 * © 2021 DayZ Expansion Mod Team
 *
 * This work is licensed under the Creative Commons Attribution-NonCommercial-NoDerivatives 4.0 International License.
 * To view a copy of this license, visit http://creativecommons.org/licenses/by-nc-nd/4.0/.
 *
*/

#ifdef JM_COT
modded class JMPlayerModule
{
	override private void Exec_RepairTransport( array< string > guids, PlayerIdentity ident, JMPlayerInstance instance = NULL  )
	{
		array< Transport > transports = new array< Transport >;

		array< JMPlayerInstance > players = GetPermissionsManager().GetPlayers( guids );

		for ( int i = 0; i < players.Count(); i++ )
		{
			PlayerBase player = PlayerBase.Cast( players[i].PlayerObject );
			if ( player == NULL )
				continue;

			Transport transport;
			if ( !Class.CastTo( transport, player.GetParent() ) )
				continue;

			if ( transports.Find( transport ) > -1 )
				continue;

			transports.Insert( transport );

			ItemBase radiator;
			if ( Class.CastTo( radiator, transport.FindAttachmentBySlotName( "CarRadiator" ) ) )
			{
				radiator.SetHealth01( "", "", 1 );
			}

			transport.SetHealth01( "Engine", "", 1 );
			transport.SetHealth01( "FuelTank", "", 1 );

			CarScript car;
			if ( Class.CastTo( car, transport ) )
			{
				car.Fill( CarFluid.FUEL, car.GetFluidCapacity( CarFluid.FUEL ) );
				car.Fill( CarFluid.OIL, car.GetFluidCapacity( CarFluid.OIL ) );
				car.Fill( CarFluid.BRAKE, car.GetFluidCapacity( CarFluid.BRAKE ) );
				car.Fill( CarFluid.COOLANT, car.GetFluidCapacity( CarFluid.COOLANT ) );
			}

			GetCommunityOnlineToolsBase().Log( ident, "Repaired Transport [guid=" + players[i].GetGUID() + "]" );
			SendWebhook( "Vehicle", instance, "Repaired " + players[i].FormatSteamWebhook() + " vehicle" );

			players[i].Update();
		}
	}
}
#endif
