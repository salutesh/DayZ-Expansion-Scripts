/**
 * ExpansionESPModificationModule.c
 *
 * DayZ Expansion Mod
 * www.dayzexpansion.com
 * © 2021 DayZ Expansion Mod Team
 *
 * This work is licensed under the Creative Commons Attribution-NonCommercial-NoDerivatives 4.0 International License. 
 * To view a copy of this license, visit http://creativecommons.org/licenses/by-nc-nd/4.0/.
 *
*/

modded class JMESPModule
{
	JMESPMeta EXP_GetMeta( Object target )
	{
		return m_MappedESPObjects.Get( target );
	}
};

class ExpansionESPModificationModule : JMModuleBase
{
	private JMESPModule m_BaseModule;

	void ExpansionESPModificationModule()
	{
		GetPermissionsManager().RegisterPermission( "ESP.Object.BaseBuilding.Codelock" );
		GetPermissionsManager().RegisterPermission( "ESP.Object.Car.Key" );
	}

	override void OnMissionStart()
	{
		super.OnMissionStart();
		
		m_BaseModule = JMESPModule.Cast( GetModuleManager().GetModule( JMESPModule ) );
	}

	override int GetRPCMin()
	{
		return ExpansionESPModificationModuleRPC.INVALID;
	}

	override int GetRPCMax()
	{
		return ExpansionESPModificationModuleRPC.COUNT;
	}

	#ifdef CF_BUGFIX_REF
	override void OnRPC( PlayerIdentity sender, Object target, int rpc_type, ParamsReadContext ctx )
	#else
	override void OnRPC( PlayerIdentity sender, Object target, int rpc_type, ref ParamsReadContext ctx )
	#endif
	{
		switch ( rpc_type )
		{
		case ExpansionESPModificationModuleRPC.RequestCode:
			RPC_RequestCode( ctx, sender, target );
			break;
		case ExpansionESPModificationModuleRPC.RemoveCode:
			RPC_RemoveCode( ctx, sender, target );
			break;
		case ExpansionESPModificationModuleRPC.CarUnPair:
			RPC_CarUnPair( ctx, sender, target );
			break;
		case ExpansionESPModificationModuleRPC.CarUnLock:
			RPC_CarUnLock( ctx, sender, target );
			break;
		}
	}

	//! ===============================================
	//! CODELOCK ESP FUNCTIONS
	//! ===============================================

	void RequestCode( Object target )
	{
		if ( IsMissionOffline() )
		{
			ItemBase item = ItemBase.Cast( target );
			Container_Base container = Container_Base.Cast( target );
			if ( !item && !container )
				return;
			if( !item )
				Exec_RequestCode( container, container.GetCode() );
			else
				Exec_RequestCode( item, item.GetCode() );
		} else if ( IsMissionClient() )
		{
			ScriptRPC rpc = new ScriptRPC();
			rpc.Send( target, ExpansionESPModificationModuleRPC.RequestCode, false, NULL );
		}
	}

	private void Exec_RequestCode( ItemBase target, string code )
	{
		if ( !target )
			return;

		JMESPMetaBaseBuilding meta = JMESPMetaBaseBuilding.Cast( m_BaseModule.EXP_GetMeta( target ) );
		if ( meta )
		{
			meta.SetCode( code );
		} else {
			JMESPMetaTents metat = JMESPMetaTents.Cast( m_BaseModule.EXP_GetMeta( target ) );
			if ( metat )
			{
				metat.SetCode( code );
			} else {
				JMESPMetaContainer metac = JMESPMetaContainer.Cast( m_BaseModule.EXP_GetMeta( target ) );
				if ( metac )
				{
					metac.SetCode( code );
				}
			}
		}
	}

	private void RPC_RequestCode( ParamsReadContext ctx, PlayerIdentity senderRPC, Object target )
	{
		ItemBase item = ItemBase.Cast( target );
		if ( !item )
			return;

		if ( GetGame().IsServer() )
		{
			JMPlayerInstance instance;
			if ( !GetPermissionsManager().HasPermission( "ESP.Object.BaseBuilding.Codelock", senderRPC, instance ) )
				return;

			ScriptRPC rpc = new ScriptRPC();
			rpc.Write( item.GetCode() );
			rpc.Send( target, ExpansionESPModificationModuleRPC.RequestCode, false, NULL );
		} else
		{
			string code;
			if ( !ctx.Read( code ) )
				return;

			Exec_RequestCode( item, code );
		}
	}

	void RemoveCode( Object target )
	{
		if ( IsMissionOffline() )
		{
			ItemBase item = ItemBase.Cast( target );
			if ( !item )
				return;

			item.SetCode( "" );
		} else if ( IsMissionClient() )
		{
			ScriptRPC rpc = new ScriptRPC();
			rpc.Send( target, ExpansionESPModificationModuleRPC.RemoveCode, false, NULL );
		}
	}

	private void RPC_RemoveCode( ParamsReadContext ctx, PlayerIdentity senderRPC, Object target )
	{
		ItemBase item = ItemBase.Cast( target );
		if ( !item )
			return;

		JMPlayerInstance instance;
		if ( !GetPermissionsManager().HasPermission( "ESP.Object.BaseBuilding.Codelock", senderRPC, instance ) )
			return;

		item.SetCode( "" );

		ScriptRPC rpc = new ScriptRPC();
		rpc.Write( "" );
		rpc.Send( target, ExpansionESPModificationModuleRPC.RequestCode, false, NULL );
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
			ScriptRPC rpc = new ScriptRPC();
			rpc.Send( target, ExpansionESPModificationModuleRPC.CarUnPair, false, NULL );
		}
	}

	private void RPC_CarUnPair( ParamsReadContext ctx, PlayerIdentity senderRPC, Object target )
	{
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
			ScriptRPC rpc = new ScriptRPC();
			rpc.Send( target, ExpansionESPModificationModuleRPC.CarUnLock, false, NULL );
		}
	}

	private void RPC_CarUnLock( ParamsReadContext ctx, PlayerIdentity senderRPC, Object target )
	{
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