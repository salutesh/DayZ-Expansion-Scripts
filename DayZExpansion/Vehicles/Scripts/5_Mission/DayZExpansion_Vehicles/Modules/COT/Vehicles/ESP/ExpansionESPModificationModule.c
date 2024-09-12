/**
 * ExpansionESPModificationModule.c
 *
 * DayZ Expansion Mod
 * www.dayzexpansion.com
 * © 2022 DayZ Expansion Mod Team
 *
 * This work is licensed under the Creative Commons Attribution-NonCommercial-NoDerivatives 4.0 International License. 
 * To view a copy of this license, visit http://creativecommons.org/licenses/by-nc-nd/4.0/.
 *
*/

#ifdef JM_COT
#ifdef EXPANSIONCOTESPMODULE
modded class ExpansionESPModificationModule
{
	void ExpansionESPModificationModule()
	{
		GetPermissionsManager().RegisterPermission( "ESP.Object.Car.Key" );
	}

	override void OnInit()
	{
		super.OnInit();

		Expansion_RegisterServerRPC("RPC_CarUnPair");
		Expansion_RegisterServerRPC("RPC_CarUnLock");
	}

	//! ===============================================
	//! CAR KEY ESP FUNCTIONS
	//! ===============================================

	//! Unpair a vehicle
	void CarUnPair( Object target )
	{
		if ( IsMissionOffline() )
		{
			auto vehicle = ExpansionVehicle.Get( target );
			if ( !vehicle )
				return;

			array< ExpansionCarKey > keys = new array< ExpansionCarKey >;
			ExpansionCarKey.GetKeysForVehicle( vehicle, keys );

			for ( int i = 0; i < keys.Count(); ++i )
				keys[i].Unpair( true );

			vehicle.ResetKeyPairing();

		} else if ( IsMissionClient() )
		{
			auto rpc = Expansion_CreateRPC("RPC_CarUnPair");
			rpc.Expansion_Send(target, true, NULL);
		}
	}

	private void RPC_CarUnPair(PlayerIdentity senderRPC, Object target, ParamsReadContext ctx)
	{
		auto vehicle = ExpansionVehicle.Get( target );
		if ( !vehicle )
			return;

		JMPlayerInstance instance;
		if ( !GetPermissionsManager().HasPermission( "ESP.Object.Car.Key", senderRPC, instance ) )
			return;

		array< ExpansionCarKey > keys = new array< ExpansionCarKey >;
		ExpansionCarKey.GetKeysForVehicle( vehicle, keys );

		for ( int i = 0; i < keys.Count(); ++i )
			keys[i].Unpair( true );

		vehicle.ResetKeyPairing();
	}
	
	//! Unlock a vehicle
	void CarUnLock( Object target )
	{
		if ( IsMissionOffline() )
		{
			auto vehicle = ExpansionVehicle.Get( target );
			if ( !vehicle )
				return;

			ExpansionCarAdminKey adminkey;
			vehicle.Unlock(adminkey);
		} else if ( IsMissionClient() )
		{
			auto rpc = Expansion_CreateRPC("RPC_CarUnLock");
			rpc.Expansion_Send(target, true, NULL);
		}
	}

	private void RPC_CarUnLock(PlayerIdentity senderRPC, Object target, ParamsReadContext ctx)
	{
		auto vehicle = ExpansionVehicle.Get( target );
		if ( !vehicle )
			return;

		JMPlayerInstance instance;
		if ( !GetPermissionsManager().HasPermission( "ESP.Object.Car.Key", senderRPC, instance ) )
			return;

		ExpansionCarAdminKey adminkey;
		vehicle.Unlock(adminkey);
	}
};
#endif
#endif
