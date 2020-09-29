/**
 * ExpansionCOTVehiclesModule.c
 *
 * DayZ Expansion Mod
 * www.dayzexpansion.com
 * © 2020 DayZ Expansion Mod Team
 *
 * This work is licensed under the Creative Commons Attribution-NonCommercial-NoDerivatives 4.0 International License. 
 * To view a copy of this license, visit http://creativecommons.org/licenses/by-nc-nd/4.0/.
 *
*/

const int EXVT_NONE = 0;
const int EXVT_CAR = 2;
const int EXVT_BOAT = 4;
const int EXVT_HELICOPTER = 8;
const int EXVT_PLANE = 32;
const int EXVT_BIKE = 512;

const int EXDT_NONE = 0;
const int EXDT_EXPLODED = 2;
const int EXDT_DESTROYED = 4;

class ExpansionVehicleMetaData
{
	int m_NetworkIDLow;
	int m_NetworkIDHigh;

	int m_PersistentIDA;
	int m_PersistentIDB;
	int m_PersistentIDC;
	int m_PersistentIDD;

	string m_ClassName;
	vector m_Position;
	vector m_Orientation;

	int m_VehicleType;
	int m_DestructionType;

	bool m_HasKeys;
	
	static ref ExpansionVehicleMetaData CreateCarScript( CarScript car )
	{
		ref ExpansionVehicleMetaData meta = new ExpansionVehicleMetaData();

		car.GetNetworkID( meta.m_NetworkIDLow, meta.m_NetworkIDHigh );
		
		meta.m_PersistentIDA = car.GetPersistentIDA();
		meta.m_PersistentIDB = car.GetPersistentIDB();
		meta.m_PersistentIDC = car.GetPersistentIDC();
		meta.m_PersistentIDD = car.GetPersistentIDD();

		meta.m_ClassName = car.ClassName();
		meta.m_Position = car.GetPosition();
		meta.m_Orientation = car.GetOrientation();

		meta.m_VehicleType = EXVT_NONE;
		if ( car.IsCar() )
			meta.m_VehicleType |= EXVT_CAR;
		if ( car.IsBoat() )
			meta.m_VehicleType |= EXVT_BOAT;
		if ( car.IsHelicopter() )
			meta.m_VehicleType |= EXVT_HELICOPTER;
		if ( car.IsPlane() )
			meta.m_VehicleType |= EXVT_PLANE;
		
		meta.m_DestructionType = EXDT_NONE;
		if ( car.IsExploded() )
			meta.m_DestructionType |= EXDT_EXPLODED;
		if ( car.IsExploded() )
			meta.m_DestructionType |= EXDT_DESTROYED;

		meta.m_HasKeys = car.HasKey();

		return meta;
	}

	string GetVehicleType()
	{
		string type = "";

		if ( m_VehicleType & EXVT_CAR )
			type += "Car ";
		if ( m_VehicleType & EXVT_BOAT )
			type += "Boat ";
		if ( m_VehicleType & EXVT_HELICOPTER )
			type += "Helicopter ";
		if ( m_VehicleType & EXVT_PLANE )
			type += "Plane ";
		if ( m_VehicleType & EXVT_BIKE )
			type += "Bike ";

		return type;
	}

	string GetVehicleDestructionState()
	{
		string type = "";

		if ( m_DestructionType != EXDT_NONE )
		{
			if ( m_DestructionType & EXDT_DESTROYED )
			{
				type += "Destroyed ";
			}
			if ( m_DestructionType & EXDT_EXPLODED )
			{
				type += "Exploded ";
			}
		} else
		{
			type = "None";
		}

		return type;
	}

	bool IsDestroyed()
	{
		return m_DestructionType & EXDT_DESTROYED;
	}

	bool IsExploded()
	{
		return m_DestructionType & EXDT_EXPLODED;
	}
};

class ExpansionCOTVehiclesModule: JMRenderableModuleBase
{
	private ref array<ref ExpansionVehicleMetaData> m_Vehicles;
	private int m_TimeSinceLastChecked;

	
	// ------------------------------------------------------------
	// ExpansionCOTVehiclesModule ExpansionCOTTerritoriesModule
	// ------------------------------------------------------------	
	void ExpansionCOTVehiclesModule()
	{
		GetPermissionsManager().RegisterPermission( "Expansion.Vehicles.View" );
		
		GetPermissionsManager().RegisterPermission( "Expansion.Vehicles.Delete" );
		GetPermissionsManager().RegisterPermission( "Expansion.Vehicles.Delete.All" );
		GetPermissionsManager().RegisterPermission( "Expansion.Vehicles.Delete.Destroyed" );
		GetPermissionsManager().RegisterPermission( "Expansion.Vehicles.Delete.Unclaimed" );
		GetPermissionsManager().RegisterPermission( "Expansion.Vehicles.Teleport" );
		
		m_Vehicles = new array<ref ExpansionVehicleMetaData>;
	}
	
	void ~ExpansionCOTVehiclesModule()
	{
		delete m_Vehicles;
	}
	
	// ------------------------------------------------------------
	// ExpansionCOTVehiclesModule HasAccess
	// ------------------------------------------------------------	
	override bool HasAccess()
	{
		return GetPermissionsManager().HasPermission( "Expansion.Vehicles.View" );
	}
	
	// ------------------------------------------------------------
	// ExpansionCOTVehiclesModule GetLayoutRoot
	// ------------------------------------------------------------	
	override string GetLayoutRoot()
	{
		return "DayZExpansion/GUI/layouts/COT/vehicles/Vehicles_Menu.layout";
	}
	
	// ------------------------------------------------------------
	// ExpansionCOTVehiclesModule GetTitle
	// ------------------------------------------------------------	
	override string GetTitle()
	{
		return "[EX] Vehicle Management";
	}
	
	// ------------------------------------------------------------
	// ExpansionCOTVehiclesModule GetIconName
	// ------------------------------------------------------------	
	override string GetIconName()
	{
		return "V";
	}
	
	// ------------------------------------------------------------
	// ExpansionCOTVehiclesModule ImageIsIcon
	// ------------------------------------------------------------
	override bool ImageIsIcon()
	{
		return false;
	}
	
	// ------------------------------------------------------------
	// ExpansionTerritoryModule OnMissionLoaded
	// ------------------------------------------------------------
	override void OnMissionLoaded()
	{
		super.OnMissionLoaded();
	}
	
	// ------------------------------------------------------------
	// ExpansionCOTVehiclesModule GetServerVehicle
	// ------------------------------------------------------------
	private void UpdateVehiclesMetaData()
	{
		if ( m_TimeSinceLastChecked + 500 > GetGame().GetTime() )
			return;

		m_TimeSinceLastChecked = GetGame().GetTime();

		if ( m_Vehicles.Count() >= 0 )
			m_Vehicles.Clear();
		
		for ( int i = 0; i < CarScript.GetAll().Count(); i++ )
		{
			CarScript car = CarScript.GetAll()[i];
			if ( !car ) // should not be possible
				continue;

			m_Vehicles.Insert( ExpansionVehicleMetaData.CreateCarScript( car ) );
		}
	}
	
	// ------------------------------------------------------------
	// ExpansionCOTVehiclesModule GetRPCMin
	// ------------------------------------------------------------	
	override int GetRPCMin()
	{
		return ExpansionCOTVehiclesModuleRPC.INVALID;
	}
	
	// ------------------------------------------------------------
	// ExpansionCOTVehiclesModule GetRPCMax
	// ------------------------------------------------------------
	override int GetRPCMax()
	{
		return ExpansionCOTVehiclesModuleRPC.COUNT;
	}
	
	// ------------------------------------------------------------
	// ExpansionCOTVehiclesModule OnRPC
	// ------------------------------------------------------------
	override void OnRPC( PlayerIdentity sender, Object target, int rpc_type, ref ParamsReadContext ctx )
	{
		switch ( rpc_type )
		{
		case ExpansionCOTVehiclesModuleRPC.RequestServerVehicles:
			RPC_RequestServerVehicles( ctx, sender, target );
			break;	
		case ExpansionCOTVehiclesModuleRPC.SendServerVehicles:
			RPC_SendServerVehicles( ctx, sender, target );
			break;
		case ExpansionCOTVehiclesModuleRPC.DeleteVehicle:
			RPC_DeleteVehicle( ctx, sender, target );
			break;
		case ExpansionCOTVehiclesModuleRPC.DeleteVehicleUnclaimed:
			RPC_DeleteVehicleUnclaimed( ctx, sender, target );
			break;
		case ExpansionCOTVehiclesModuleRPC.DeleteVehicleDestroyed:
			RPC_DeleteVehicleDestroyed( ctx, sender, target );
			break;
		case ExpansionCOTVehiclesModuleRPC.DeleteVehicleAll:
			RPC_DeleteVehicleAll( ctx, sender, target );
			break;
		case ExpansionCOTVehiclesModuleRPC.TeleportToVehicle:
			RPC_TeleportToVehicle( ctx, sender, target );
			break;
		}
	}
	
	// ------------------------------------------------------------
	// ExpansionCOTVehiclesModule RequestServerVehicles
	// Called on client
	// ------------------------------------------------------------
	void RequestServerVehicles()
	{
		if ( IsMissionClient() )
		{
			ScriptRPC rpc = new ScriptRPC();
 			rpc.Send( NULL, ExpansionCOTVehiclesModuleRPC.RequestServerVehicles, true );
		}
	}
	
	// ------------------------------------------------------------
	// ExpansionCOTVehiclesModule RPC_RequestServerVehicles
	// Called on Server
	// ------------------------------------------------------------
	void RPC_RequestServerVehicles( ref ParamsReadContext ctx, PlayerIdentity senderRPC, ref Object target )
	{
		if ( !IsMissionHost() )
			return;

		if ( !GetPermissionsManager().HasPermission( "Expansion.Vehicles.View", senderRPC ) )
			return;
		
		UpdateVehiclesMetaData();
		
		array< ref ExpansionVehicleMetaData > vehicles = new array< ref ExpansionVehicleMetaData >;
			
		if ( m_Vehicles )
		{
			for ( int i = 0; i < m_Vehicles.Count(); ++i )
			{
				ExpansionVehicleMetaData currentData = m_Vehicles.Get( i );
				if ( !currentData )
					continue;
				
				vehicles.Insert( currentData );
			}
		}
		
		ScriptRPC rpc = new ScriptRPC();
		rpc.Write( vehicles );
		rpc.Send( NULL, ExpansionCOTVehiclesModuleRPC.SendServerVehicles, true, senderRPC );
	}
	
	// ------------------------------------------------------------
	// ExpansionCOTVehiclesModule RPC_SendServerVehicles
	// Called on Client
	// ------------------------------------------------------------
	private void RPC_SendServerVehicles( ref ParamsReadContext ctx, PlayerIdentity senderRPC, ref Object target )
	{
		if ( !IsMissionClient() )
			return;
		
		ctx.Read( m_Vehicles );
	}

	// ------------------------------------------------------------
	// ExpansionCOTVehiclesModule DeleteVehicleUnclaimed
	// ------------------------------------------------------------	
	void DeleteVehicleUnclaimed( )
	{
		if ( IsMissionClient() )
		{
			ScriptRPC rpc = new ScriptRPC();
			
			rpc.Send( NULL, ExpansionCOTVehiclesModuleRPC.DeleteVehicleUnclaimed, true );
		}
	}

	// ------------------------------------------------------------
	// ExpansionCOTVehiclesModule DeleteVehicleDestroyed
	// ------------------------------------------------------------	
	void DeleteVehicleDestroyed( )
	{
		if ( IsMissionClient() )
		{
			ScriptRPC rpc = new ScriptRPC();
			
			rpc.Send( NULL, ExpansionCOTVehiclesModuleRPC.DeleteVehicleDestroyed, true );
		}
	}

	// ------------------------------------------------------------
	// ExpansionCOTVehiclesModule DeleteVehicleAll
	// ------------------------------------------------------------	
	void DeleteVehicleAll( )
	{
		if ( IsMissionClient() )
		{
			ScriptRPC rpc = new ScriptRPC();
			
			rpc.Send( NULL, ExpansionCOTVehiclesModuleRPC.DeleteVehicleAll, true );
		}
	}
	
	// ------------------------------------------------------------
	// ExpansionCOTVehiclesModule DeleteVehicle
	// ------------------------------------------------------------	
	void DeleteVehicle( int netLow, int netHigh )
	{
		if ( IsMissionClient() )
		{
			ScriptRPC rpc = new ScriptRPC();
			rpc.Write( netLow );
			rpc.Write( netHigh );
			rpc.Send( NULL, ExpansionCOTVehiclesModuleRPC.DeleteVehicle, true );
		}
	}
	
	// ------------------------------------------------------------
	// ExpansionCOTVehiclesModule RPC_DeleteVehicle
	// Called on Server
	// ------------------------------------------------------------
	private void RPC_DeleteVehicle( ref ParamsReadContext ctx, PlayerIdentity senderRPC, ref Object target )
	{
		if ( !IsMissionHost() )
			return;

		if ( !GetPermissionsManager().HasPermission( "Expansion.Vehicles.Delete", senderRPC ) )
			return;
		
		int netLow;
		if ( !ctx.Read( netLow ) )
			return;

		int netHigh;
		if ( !ctx.Read( netHigh ) )
			return;
		
		CarScript car = CarScript.Cast( GetGame().GetObjectByNetworkId( netLow, netHigh ) );

		if ( !car )
			return;

		GetGame().ObjectDelete( car );
	}

	// ------------------------------------------------------------
	// ExpansionCOTVehiclesModule RPC_DeleteVehicleUnclaimed
	// Called on Server
	// ------------------------------------------------------------
	private void RPC_DeleteVehicleUnclaimed( ref ParamsReadContext ctx, PlayerIdentity senderRPC, ref Object target )
	{
		if ( !IsMissionHost() )
			return;

		if ( !GetPermissionsManager().HasPermission( "Expansion.Vehicles.Delete.Unclaimed", senderRPC ) )
			return;
		
		for ( int i = 0; i < CarScript.GetAll().Count(); i++ )
		{
			CarScript car = CarScript.GetAll()[i];
			if ( !car )
				return;

			if ( !car.HasKey() )
			{
				GetGame().ObjectDelete( car );
			}
		}
	}

	// ------------------------------------------------------------
	// ExpansionCOTVehiclesModule RPC_DeleteVehicleDestroyed
	// Called on Server
	// ------------------------------------------------------------
	private void RPC_DeleteVehicleDestroyed( ref ParamsReadContext ctx, PlayerIdentity senderRPC, ref Object target )
	{
		if ( !IsMissionHost() )
			return;

		if ( !GetPermissionsManager().HasPermission( "Expansion.Vehicles.Delete.Destroyed", senderRPC ) )
			return;
		
		for ( int i = 0; i < CarScript.GetAll().Count(); i++ )
		{
			CarScript car = CarScript.GetAll()[i];
			if ( !car )
				return;

			if ( car.IsDamageDestroyed() )
			{
				GetGame().ObjectDelete( car );
			}
		}
	}

	// ------------------------------------------------------------
	// ExpansionCOTVehiclesModule RPC_DeleteVehicleAll
	// Called on Server
	// ------------------------------------------------------------
	private void RPC_DeleteVehicleAll( ref ParamsReadContext ctx, PlayerIdentity senderRPC, ref Object target )
	{
		if ( !IsMissionHost() )
			return;

		if ( !GetPermissionsManager().HasPermission( "Expansion.Vehicles.Delete.All", senderRPC ) )
			return;
		
		for ( int i = 0; i < CarScript.GetAll().Count(); i++ )
		{
			CarScript car = CarScript.GetAll()[i];
			if ( !car )
				return;

			GetGame().ObjectDelete( car );
		}
	}
	
	// ------------------------------------------------------------
	// ExpansionCOTVehiclesModule RequestTeleportToVehicle
	// Called on client
	// ------------------------------------------------------------
	void RequestTeleportToVehicle( int netLow, int netHigh )
	{
		if ( IsMissionClient() )
		{
			ScriptRPC rpc = new ScriptRPC();
			rpc.Write( netLow );
			rpc.Write( netHigh );
 			rpc.Send( NULL, ExpansionCOTVehiclesModuleRPC.TeleportToVehicle, true );
		}
	}
	
	// ------------------------------------------------------------
	// ExpansionCOTVehiclesModule RPC_TeleportToVehicle
	// Called on Server
	// ------------------------------------------------------------
	private void RPC_TeleportToVehicle( ref ParamsReadContext ctx, PlayerIdentity senderRPC, ref Object target )
	{
		if ( !IsMissionHost() )
			return;

		JMPlayerInstance instance;
		if ( !GetPermissionsManager().HasPermission( "Expansion.Vehicles.Teleport", senderRPC, instance ) )
			return;

		int netLow;
		if ( !ctx.Read( netLow ) )
			return;

		int netHigh;
		if ( !ctx.Read( netHigh ) )
			return;
		
		PlayerBase player = GetPlayerObjectByIdentity( senderRPC );
		if ( !player )
			return;

		CarScript car = CarScript.Cast( GetGame().GetObjectByNetworkId( netLow, netHigh ) );
		if ( !car )
			return;

		vector pos = car.GetPosition();
		vector minMax[2];
		car.ClippingInfo( minMax );

		player.SetLastPosition();
		player.SetWorldPosition( pos + minMax[1] );
	}
	
	// ------------------------------------------------------------
	// ExpansionCOTVehiclesModule GetServerVehicles
	// Only returns array on Client side
	// ------------------------------------------------------------
	ref array< ref ExpansionVehicleMetaData > GetServerVehicles()
	{
		return m_Vehicles;
	}
};