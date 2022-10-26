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

	override void OnRPC(Class sender, CF_EventArgs args)
	{
		auto rpc = CF_EventRPCArgs.Cast(args);

		switch ( rpc.ID )
		{
		case ExpansionESPModificationModuleRPC.CarUnPair:
			RPC_CarUnPair( rpc.Context, rpc.Sender, rpc.Target );
			return;
		case ExpansionESPModificationModuleRPC.CarUnLock:
			RPC_CarUnLock( rpc.Context, rpc.Sender, rpc.Target );
			return;
		}

		super.OnRPC(sender, args);
	}

	//! ===============================================
	//! CAR KEY ESP FUNCTIONS
	//! ===============================================

	//! Unpair a vehicle
	void CarUnPair( Object target )
	{
		if ( IsMissionOffline() )
		{
			CarScript car = CarScript.Cast( target );
			if ( !car )
				return;

			array< ExpansionCarKey > keys = new array< ExpansionCarKey >;
			ExpansionCarKey.GetKeysForVehicle( car, keys );

			for ( int i = 0; i < keys.Count(); ++i )
				keys[i].Unpair( true );

			car.ResetKeyPairing();

		} else if ( IsMissionClient() )
		{
			auto rpc = ExpansionScriptRPC.Create();
			rpc.Send( target, ExpansionESPModificationModuleRPC.CarUnPair, false, NULL );
		}
	}

	private void RPC_CarUnPair( ParamsReadContext ctx, PlayerIdentity senderRPC, Object target )
	{
		if (!ExpansionScriptRPC.CheckMagicNumber(ctx))
			return;
		
		CarScript car = CarScript.Cast( target );
		if ( !car )
			return;

		JMPlayerInstance instance;
		if ( !GetPermissionsManager().HasPermission( "ESP.Object.Car.Key", senderRPC, instance ) )
			return;

		array< ExpansionCarKey > keys = new array< ExpansionCarKey >;
		ExpansionCarKey.GetKeysForVehicle( car, keys );

		for ( int i = 0; i < keys.Count(); ++i )
			keys[i].Unpair( true );

		car.ResetKeyPairing();
	}
	
	//! Unlock a vehicle
	void CarUnLock( Object target )
	{
		if ( IsMissionOffline() )
		{
			CarScript car = CarScript.Cast( target );
			if ( !car )
				return;

			ExpansionCarAdminKey adminkey;
			car.UnlockCar(adminkey);
		} else if ( IsMissionClient() )
		{
			auto rpc = ExpansionScriptRPC.Create();
			rpc.Send( target, ExpansionESPModificationModuleRPC.CarUnLock, false, NULL );
		}
	}

	private void RPC_CarUnLock( ParamsReadContext ctx, PlayerIdentity senderRPC, Object target )
	{
		if (!ExpansionScriptRPC.CheckMagicNumber(ctx))
			return;
		
		CarScript car = CarScript.Cast( target );
		if ( !car )
			return;

		JMPlayerInstance instance;
		if ( !GetPermissionsManager().HasPermission( "ESP.Object.Car.Key", senderRPC, instance ) )
			return;

		ExpansionCarAdminKey adminkey;
		car.UnlockCar(adminkey);
	}
};
#endif
#endif
