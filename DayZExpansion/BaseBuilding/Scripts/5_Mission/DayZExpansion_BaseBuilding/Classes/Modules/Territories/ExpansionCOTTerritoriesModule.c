/**
 * ExpansionCOTTerritoriesModule.c
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
const int EXDE_NONE = 0;
const int EXDE_DESTROYED = 2;

class ExpansionEntityMetaData
{
	int m_NetworkIDLow;
	int m_NetworkIDHigh;
	string m_ClassName;
	string m_DisplayName;
	int m_ID;
	vector m_Position;
	vector m_Orientation;
	int m_DestructionType;
	bool m_IsTerritoryFlag = false;
	ref array<ref ExpansionEntityMetaData> m_ContainerObj;
	
	void ExpansionEntityMetaData( EntityAI entity )
	{
		entity.GetNetworkID( m_NetworkIDLow, m_NetworkIDHigh );

		m_ClassName = entity.ClassName();
		m_DisplayName = entity.GetDisplayName();
		m_ID = entity.GetID();
		m_Position = entity.GetPosition();
		m_Orientation = entity.GetOrientation();
		
		m_DestructionType = EXDE_NONE;
		if ( entity.IsDamageDestroyed() )
			m_DestructionType |= EXDE_DESTROYED;
		
		m_ContainerObj = new array<ref ExpansionEntityMetaData>;
		
		if ( entity.IsInherited(Container_Base) )
		{
			Container_Base container = Container_Base.Cast(entity);

			array<EntityAI> container_items = new array<EntityAI>;
			container_items.Reserve(container.GetInventory().CountInventory());
			
			container.GetInventory().EnumerateInventory(InventoryTraversalType.PREORDER, container_items);
					
			if ( container_items && container_items.Count() > 0 )
			{
				for ( int k = 0; k < container_items.Count(); k++ )
				{
					EntityAI item = container_items.Get( k );
					if ( item && item != container )
					{
						ExpansionEntityMetaData new_data = new ExpansionEntityMetaData( item );
						m_ContainerObj.Insert( new_data );
					}
				}
			}
		}
		
		TerritoryFlag flag = TerritoryFlag.Cast(entity);
		if ( flag && flag.HasExpansionTerritoryInformation() )
		{
			#ifdef EXPANSION_COT_TERRITORY_MODULE_DEBUG
			EXLogPrint("ExpansionCOTTerritoriesModule::ExpansionEntityMetaData - Entity is territory flag!");
			#endif
			m_IsTerritoryFlag = true;
		}
		
	}
	
	bool IsDamageDestroyed()
	{
		if (m_DestructionType == EXDE_DESTROYED)
			return true;
		
		return false;
	}
	
	bool HasExpansionTerritoryInformation()
	{
		return m_IsTerritoryFlag;
	}
}

class ExpansionCOTTerritoriesModule: JMRenderableModuleBase
{
	// ------------------------------------------------------------
	// ExpansionCOTTerritoriesModule ExpansionCOTTerritoriesModule
	// ------------------------------------------------------------	
	void ExpansionCOTTerritoriesModule()
	{
		#ifdef EXPANSION_COT_TERRITORY_MODULE_DEBUG
		EXLogPrint("ExpansionCOTTerritoriesModule::ExpansionCOTTerritoriesModule - Start");
		#endif
		
		GetPermissionsManager().RegisterPermission( "Expansion.Territories.View" );
		
		GetPermissionsManager().RegisterPermission( "Expansion.Territories.Edit" );
		GetPermissionsManager().RegisterPermission( "Expansion.Territories.Delete" );
		GetPermissionsManager().RegisterPermission( "Expansion.Territories.Delete.All" );
		GetPermissionsManager().RegisterPermission( "Expansion.Territories.Delete.Destroyed" );
		GetPermissionsManager().RegisterPermission( "Expansion.Territories.Teleport" );
		
		#ifdef EXPANSION_COT_TERRITORY_MODULE_DEBUG
		EXLogPrint("ExpansionCOTTerritoriesModule::ExpansionCOTTerritoriesModule - End");
		#endif
	}
	
	override void EnableUpdate()
	{
	}

	override void EnableRPC()
	{
	}

	override void OnInit()
	{
		super.OnInit();

		Expansion_EnableRPCManager();

		Expansion_RegisterServerRPC("RPC_DeleteObject");
		Expansion_RegisterServerRPC("RPC_RequestServerTerritories");
		Expansion_RegisterClientRPC("RPC_SendServerTerritories");
		Expansion_RegisterServerRPC("RPC_RequestTerritoryObjects");
		Expansion_RegisterClientRPC("RPC_SendTerritoryObjects");
		Expansion_RegisterServerRPC("RPC_TeleportToTerritory");
		Expansion_RegisterServerRPC("RPC_RequestUpdateObjectData");
		Expansion_RegisterClientRPC("RPC_SendObjectData");
	}

	// ------------------------------------------------------------
	// ExpansionCOTTerritoriesModule HasAccess
	// ------------------------------------------------------------	
	override bool HasAccess()
	{
		return GetPermissionsManager().HasPermission( "Expansion.Territories.View" );
	}
	
	// ------------------------------------------------------------
	// ExpansionCOTTerritoriesModule GetLayoutRoot
	// ------------------------------------------------------------	
	override string GetLayoutRoot()
	{
		return "DayZExpansion/BaseBuilding/GUI/layouts/COT/territories/Territories_Menu.layout";
	}
	
	// ------------------------------------------------------------
	// ExpansionCOTTerritoriesModule GetTitle
	// ------------------------------------------------------------	
	override string GetTitle()
	{
		return "[EX] Territory Management";
	}
	
	// ------------------------------------------------------------
	// ExpansionCOTTerritoriesModule GetIconName
	// ------------------------------------------------------------	
	override string GetIconName()
	{
		return "T";
	}
	
	// ------------------------------------------------------------
	// ExpansionCOTTerritoriesModule ImageIsIcon
	// ------------------------------------------------------------
	override bool ImageIsIcon()
	{
		return false;
	}
	
	// ------------------------------------------------------------
	// ExpansionCOTTerritoriesModule RequestTerritoryObjects
	// Called from client
	// ------------------------------------------------------------
	void RequestTerritoryObjects( vector pos )
	{
		#ifdef EXPANSION_COT_TERRITORY_MODULE_DEBUG
		EXLogPrint("ExpansionCOTTerritoriesModule::RequestTerritoryObjects - Start");
		#endif
		
		auto rpc = Expansion_CreateRPC("RPC_RequestTerritoryObjects");
		rpc.Write( pos );
		rpc.Expansion_Send(true);
		
		#ifdef EXPANSION_COT_TERRITORY_MODULE_DEBUG
		EXLogPrint("ExpansionCOTTerritoriesModule::RequestTerritoryObjects - End");
		#endif
	}
	
	// ------------------------------------------------------------
	// ExpansionCOTTerritoriesModule RPC_RequestTerritoryObjects
	// Called on Server
	// ------------------------------------------------------------
	void RPC_RequestTerritoryObjects(PlayerIdentity senderRPC, Object target, ParamsReadContext ctx)
	{
		#ifdef EXPANSION_COT_TERRITORY_MODULE_DEBUG
		EXLogPrint("ExpansionCOTTerritoriesModule::RPC_RequestTerritoryObjects - Start");
		#endif

		if ( !GetPermissionsManager().HasPermission( "Expansion.Territories.View", senderRPC ) )
			return;
		
		vector pos;
		if ( !ctx.Read( pos ) )
			return;
		
		Exec_RequestTerritoryObjects( pos, senderRPC );
		
		#ifdef EXPANSION_COT_TERRITORY_MODULE_DEBUG
		EXLogPrint("ExpansionCOTTerritoriesModule::RPC_RequestTerritoryObjects - End");
		#endif
	}
	
	// ------------------------------------------------------------
	// ExpansionCOTTerritoriesModule Exec_RequestTerritoryObjects
	// Called on Server
	// ------------------------------------------------------------
	void Exec_RequestTerritoryObjects(vector pos, PlayerIdentity ident)
	{
		#ifdef EXPANSION_COT_TERRITORY_MODULE_DEBUG
		EXLogPrint("ExpansionCOTTerritoriesModule::Exec_RequestTerritoryObjects - Start");
		#endif
		
		array<ref ExpansionEntityMetaData> objects_datas = new array<ref ExpansionEntityMetaData>;
		array<Object> objects = new array<Object>;
		array<CargoBase> proxyCargos = new array<CargoBase> ;
		GetGame().GetObjectsAtPosition3D( pos, GetExpansionSettings().GetTerritory().TerritorySize, objects, proxyCargos );
		
		if ( objects && objects.Count() > 0 )
		{
			for ( int k = 0; k < objects.Count(); k++ )
			{
				EntityAI obj = EntityAI.Cast( objects.Get( k ) );
				if ( IsBaseBuildingObject( obj ) )
				{
					ExpansionEntityMetaData object_data = new ExpansionEntityMetaData( obj );
					objects_datas.Insert( object_data );
				}
			}
		}
		
		auto rpc = Expansion_CreateRPC("RPC_SendTerritoryObjects");
		rpc.Write( objects_datas );
		rpc.Expansion_Send(true, ident);
		
		#ifdef EXPANSION_COT_TERRITORY_MODULE_DEBUG
		EXLogPrint("ExpansionCOTTerritoriesModule::Exec_RequestTerritoryObjects - End");
		#endif
	}
	
	// ------------------------------------------------------------
	// ExpansionCOTTerritoriesModule RPC_SendTerritoryObjects
	// Called on client
	// ------------------------------------------------------------
	void RPC_SendTerritoryObjects(PlayerIdentity senderRPC, Object target, ParamsReadContext ctx)
	{
		#ifdef EXPANSION_COT_TERRITORY_MODULE_DEBUG
		EXLogPrint("ExpansionCOTTerritoriesModule::RPC_SendTerritoryObjects - Start");
		#endif

		if ( !GetPermissionsManager().HasPermission( "Expansion.Territories.View", senderRPC ) )
			return;
		
		array<ref ExpansionEntityMetaData> objects_datas = new array<ref ExpansionEntityMetaData>;
		if ( !ctx.Read( objects_datas ) )
			return;
		
		Exec_SendTerritoryObjects( objects_datas );
		
		#ifdef EXPANSION_COT_TERRITORY_MODULE_DEBUG
		EXLogPrint("ExpansionCOTTerritoriesModule::RPC_SendTerritoryObjects - End");
		#endif
	}
	
	// ------------------------------------------------------------
	// ExpansionCOTTerritoriesModule Exec_SendTerritoryObjects
	// Called on client
	// ------------------------------------------------------------
	void Exec_SendTerritoryObjects( array<ref ExpansionEntityMetaData> objects_datas )
	{
		#ifdef EXPANSION_COT_TERRITORY_MODULE_DEBUG
		EXLogPrint("ExpansionCOTTerritoriesModule::Exec_SendTerritoryObjects - Start");
		#endif
		
		if (!objects_datas)
			return;
		
		ExpansionCOTTerritoriesMenu.m_COTTerritorySetObjectsInvoker.Invoke( objects_datas );
		
		#ifdef EXPANSION_COT_TERRITORY_MODULE_DEBUG
		EXLogPrint("ExpansionCOTTerritoriesModule::Exec_SendTerritoryObjects - End");
		#endif
	}
	
	// ------------------------------------------------------------
	// ExpansionCOTTerritoriesModule IsBaseBuildingObject
	// ------------------------------------------------------------
	bool IsBaseBuildingObject(EntityAI obj)
	{
		if ( !obj )
			return false;

		if ( obj.IsInherited( BaseBuildingBase ) ) // Basebuilding parts
			return true;
		
		if ( obj.IsInherited( Construction ) ) // Constuctions
			return true;
		
		if ( obj.IsInherited( TentBase ) ) // Not really needed but better be safe
			return true;
		
		if ( obj.IsInherited( UndergroundStash ) )
			return true;
		
		if ( obj.IsInherited( Pot ) ) // cooking pot, often used to store items
			return true;
		
		if ( obj.IsInherited( FireplaceBase ) ) // barrel holes (used a lot by smart players), Fire stuff in general
			return true;
		
		if ( obj.IsInherited( Container_Base ) ) // seachest, tent, box, barrel, bags, safes and so on
			return true;
		
		if ( obj.IsInherited( WorldContainer_Base ) ) // improvised tent, bin, fridge, mostly scrapped items
			return true;
	
		if ( obj.IsInherited( CarScript ) ) // Cars
			return true;
		
		if ( obj.IsInherited( ExpansionGunrack ) ) // Gunrack
			return true;
		
		return false;
	}
	
	// ------------------------------------------------------------
	// ExpansionCOTTerritoriesModule DeleteObject
	// ------------------------------------------------------------	
	void DeleteObject( int netLow, int netHigh )
	{
		#ifdef EXPANSION_COT_TERRITORY_MODULE_DEBUG
		EXLogPrint("ExpansionCOTTerritoriesModule::DeleteObject - Start");
		#endif
		
		if ( IsMissionClient() )
		{
			auto rpc = Expansion_CreateRPC("RPC_DeleteObject");
			rpc.Write( netLow );
			rpc.Write( netHigh );
			rpc.Expansion_Send(true);
		}
		
		#ifdef EXPANSION_COT_TERRITORY_MODULE_DEBUG
		EXLogPrint("ExpansionCOTTerritoriesModule::DeleteObject - End");
		#endif
	}
	
	// ------------------------------------------------------------
	// ExpansionCOTTerritoriesModule RPC_DeleteObject
	// Called on Server
	// ------------------------------------------------------------
	void RPC_DeleteObject(PlayerIdentity senderRPC, Object target, ParamsReadContext ctx)
	{
		#ifdef EXPANSION_COT_TERRITORY_MODULE_DEBUG
		EXLogPrint("ExpansionCOTTerritoriesModule::RPC_DeleteObject - Start");
		#endif

		if ( !GetPermissionsManager().HasPermission( "Expansion.Territories.Delete", senderRPC ) )
			return;
		
		int netLow;
		if ( !ctx.Read( netLow ) )
			return;

		int netHigh;
		if ( !ctx.Read( netHigh ) )
			return;
		
		Object object = GetGame().GetObjectByNetworkId( netLow, netHigh );

		if ( !object )
			return;

		GetGame().ObjectDelete( object );
		
		#ifdef EXPANSION_COT_TERRITORY_MODULE_DEBUG
		EXLogPrint("ExpansionCOTTerritoriesModule::RPC_DeleteObject - End");
		#endif
	}
	
	// ------------------------------------------------------------
	// ExpansionCOTTerritoriesModule RequestServerTerritories
	// Called on client
	// ------------------------------------------------------------
	void RequestServerTerritories()
	{		
		#ifdef EXPANSION_COT_TERRITORY_MODULE_DEBUG
		EXLogPrint("ExpansionCOTTerritoriesModule::RequestServerTerritories - Start");
		#endif
		
		if ( !IsMissionHost() )
		{
			auto rpc = Expansion_CreateRPC("RPC_RequestServerTerritories");
 			rpc.Expansion_Send(true);
		}
		
		#ifdef EXPANSION_COT_TERRITORY_MODULE_DEBUG
		EXLogPrint("ExpansionCOTTerritoriesModule::RequestServerTerritories - End");
		#endif
	}
	
	// ------------------------------------------------------------
	// ExpansionCOTTerritoriesModule RPC_RequestServerTerritories
	// Called on Server
	// ------------------------------------------------------------
	void RPC_RequestServerTerritories(PlayerIdentity senderRPC, Object target, ParamsReadContext ctx)
	{	
		#ifdef EXPANSION_COT_TERRITORY_MODULE_DEBUG
		EXLogPrint("ExpansionCOTTerritoriesModule::RPC_RequestServerTerritories - Start");
		#endif
		
		array< ref ExpansionTerritory > territories = new array< ref ExpansionTerritory >;
		
		ExpansionTerritoryModule territories_module = ExpansionTerritoryModule.Cast( CF_ModuleCoreManager.Get(ExpansionTerritoryModule) );
		
		if (!territories_module)
			return;
	
		map<int, TerritoryFlag> territoryFlags = territories_module.GetAllTerritoryFlags();
		foreach (int territoryID, TerritoryFlag currentFlag: territoryFlags)
		{
			if (!currentFlag) 
			{
				#ifdef EXPANSION_COT_TERRITORY_MODULE_DEBUG
				EXLogPrint("ExpansionCOTTerritoriesModule::RPC_RequestServerTerritories - [ERROR]: Could not recive ExpansionTerritoryFlag from index: " + territories_module.GetAllTerritoryFlags()[i].ToString());
				#endif
				continue;
			}
			
			territories.Insert( currentFlag.GetTerritory() );
		}
		
		auto rpc = Expansion_CreateRPC("RPC_SendServerTerritories");
		rpc.Write( territories );
		rpc.Expansion_Send(true, senderRPC);
		
		#ifdef EXPANSION_COT_TERRITORY_MODULE_DEBUG
		EXLogPrint("ExpansionCOTTerritoriesModule::RPC_RequestServerTerritories - End");
		#endif
	}
	
	
	// ------------------------------------------------------------
	// ExpansionCOTTerritoriesModule RPC_SendServerTerritories
	// Called on Client
	// ------------------------------------------------------------
	void RPC_SendServerTerritories(PlayerIdentity senderRPC, Object target, ParamsReadContext ctx)
	{		
		#ifdef EXPANSION_COT_TERRITORY_MODULE_DEBUG
		EXLogPrint("ExpansionCOTTerritoriesModule::RPC_SendServerTerritories - Start");
		#endif
				
		array<ref ExpansionTerritory> territories = new array<ref ExpansionTerritory>;
		if ( !ctx.Read( territories ) )
		{
			#ifdef EXPANSION_COT_TERRITORY_MODULE_DEBUG
			EXLogPrint("ExpansionCOTTerritoriesModule::RPC_SendServerTerritories - [ERROR]: Could not recive array <ref ExpansionTerritory>");
			#endif
			
			return;
		}
		
		ExpansionCOTTerritoriesMenu.m_COTTerritorySetTerritorysInvoker.Invoke( territories );
		
		#ifdef EXPANSION_COT_TERRITORY_MODULE_DEBUG
		EXLogPrint("ExpansionCOTTerritoriesModule::RPC_SendServerTerritories - End");
		#endif
	}
	
	// ------------------------------------------------------------
	// ExpansionCOTTerritoriesModule RequestTeleportToTerritory
	// Called on client
	// ------------------------------------------------------------
	void RequestTeleportToTerritory( int netLow, int netHigh )
	{
		#ifdef EXPANSION_COT_TERRITORY_MODULE_DEBUG
		EXLogPrint("ExpansionCOTTerritoriesModule::RequestTeleportToTerritory - Start");
		#endif
				
		if ( IsMissionClient() )
		{
			auto rpc = Expansion_CreateRPC("RPC_TeleportToTerritory");
			rpc.Write( netLow );
			rpc.Write( netHigh );
 			rpc.Expansion_Send(true);
		}
		
		#ifdef EXPANSION_COT_TERRITORY_MODULE_DEBUG
		EXLogPrint("ExpansionCOTTerritoriesModule::RequestTeleportToTerritory - End");
		#endif
	}
	
	// ------------------------------------------------------------
	// ExpansionCOTTerritoriesModule RPC_TeleportToTerritory
	// Called on Server
	// ------------------------------------------------------------
	void RPC_TeleportToTerritory(PlayerIdentity senderRPC, Object target, ParamsReadContext ctx)
	{
		#ifdef EXPANSION_COT_TERRITORY_MODULE_DEBUG
		EXLogPrint("ExpansionCOTTerritoriesModule::RPC_TeleportToTerritory - Start");
		#endif

		JMPlayerInstance instance;
		if ( !GetPermissionsManager().HasPermission( "Expansion.Territories.Teleport", senderRPC, instance ) )
			return;

		int netLow;
		if ( !ctx.Read( netLow ) )
			return;

		int netHigh;
		if ( !ctx.Read( netHigh ) )
			return;
		
		PlayerBase player = PlayerBase.GetPlayerByUID(senderRPC.GetId());
		if ( !player )
			return;

		Object object = GetGame().GetObjectByNetworkId( netLow, netHigh );
		if ( !object )
			return;

		vector pos = object.GetPosition();
		pos = Vector( pos[0], pos[1] - 12, pos[2] );
		vector minMax[2];
		object.ClippingInfo( minMax );

		player.SetLastPosition();
		player.SetWorldPosition( pos + minMax[1] );
		
		#ifdef EXPANSION_COT_TERRITORY_MODULE_DEBUG
		EXLogPrint("ExpansionCOTTerritoriesModule::RPC_TeleportToTerritory - End");
		#endif
	}
	
	// ------------------------------------------------------------
	// ExpansionCOTTerritoriesModule RequestUpdateObjectData
	// Called on Client
	// ------------------------------------------------------------
	void RequestUpdateObjectData( int netLow, int netHigh )
	{
		#ifdef EXPANSION_COT_TERRITORY_MODULE_DEBUG
		EXLogPrint("ExpansionCOTTerritoriesModule::RequestUpdateObjectData - Start");
		#endif
		
		if ( IsMissionClient() )
		{
			auto rpc = Expansion_CreateRPC("RPC_RequestUpdateObjectData");
			rpc.Write( netLow );
			rpc.Write( netHigh );
 			rpc.Expansion_Send(true);
		}
		
		#ifdef EXPANSION_COT_TERRITORY_MODULE_DEBUG
		EXLogPrint("ExpansionCOTTerritoriesModule::RequestUpdateObjectData - End");
		#endif
	}
	
	// ------------------------------------------------------------
	// ExpansionCOTTerritoriesModule RPC_RequestUpdateObjectData
	// Called on Server
	// ------------------------------------------------------------
	void RPC_RequestUpdateObjectData(PlayerIdentity senderRPC, Object target, ParamsReadContext ctx)
	{
		#ifdef EXPANSION_COT_TERRITORY_MODULE_DEBUG
		EXLogPrint("ExpansionCOTTerritoriesModule::RPC_RequestUpdateObjectData - Start");
		#endif

		JMPlayerInstance instance;
		if ( !GetPermissionsManager().HasPermission( "Expansion.Territories.View", senderRPC, instance ) )
			return;

		int netLow;
		if ( !ctx.Read( netLow ) )
			return;

		int netHigh;
		if ( !ctx.Read( netHigh ) )
			return;

		Object object = GetGame().GetObjectByNetworkId( netLow, netHigh );
		if ( !object )
			return;

		EntityAI obj_entity = EntityAI.Cast( object );
		if ( !obj_entity )
		{
			#ifdef EXPANSION_COT_TERRITORY_MODULE_DEBUG
			EXLogPrint("ExpansionCOTTerritoriesModule::RPC_RequestUpdateObjectData - [ERROR]: EntityAI is NULL");
			#endif
			return;
		}
		
		ExpansionEntityMetaData object_data = new ExpansionEntityMetaData( obj_entity );
		if ( !object_data )
		{
			#ifdef EXPANSION_COT_TERRITORY_MODULE_DEBUG
			EXLogPrint("ExpansionCOTTerritoriesModule::RPC_RequestUpdateObjectData - [ERROR]: ExpansionEntityMetaData is NULL");
			#endif
			return;
		}

		auto rpc = Expansion_CreateRPC("RPC_SendObjectData");
		rpc.Write( object_data );
 		rpc.Expansion_Send(true, senderRPC);
		
		#ifdef EXPANSION_COT_TERRITORY_MODULE_DEBUG
		EXLogPrint("ExpansionCOTTerritoriesModule::RPC_RequestUpdateObjectData - End");
		#endif
	}
	
	// ------------------------------------------------------------
	// ExpansionCOTTerritoriesModule RPC_SendObjectData
	// Called on Client
	// ------------------------------------------------------------
	void RPC_SendObjectData(PlayerIdentity senderRPC, Object target, ParamsReadContext ctx)
	{
		#ifdef EXPANSION_COT_TERRITORY_MODULE_DEBUG
		EXLogPrint("ExpansionCOTTerritoriesModule::RPC_SendObjectData - Start");
		#endif
		
		ExpansionEntityMetaData object_data;
		if ( !ctx.Read( object_data ) )
		{
			#ifdef EXPANSION_COT_TERRITORY_MODULE_DEBUG
			EXLogPrint("ExpansionCOTTerritoriesModule::RPC_SendObjectData - [ERROR]: ExpansionEntityMetaData is NULL");
			#endif
			return;
		}
		
		ExpansionCOTTerritoriesMenu.m_COTTerritorySetContainerObjectsInvoker.Invoke( object_data );
		
		#ifdef EXPANSION_COT_TERRITORY_MODULE_DEBUG
		EXLogPrint("ExpansionCOTTerritoriesModule::RPC_SendObjectData - End");
		#endif
	}
};
#endif
