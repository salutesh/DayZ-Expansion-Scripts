/**
 * ExpansionCOTTerritoriesModule.c
 *
 * DayZ Expansion Mod
 * www.dayzexpansion.com
 * © 2020 DayZ Expansion Mod Team
 *
 * This work is licensed under the Creative Commons Attribution-NonCommercial-NoDerivatives 4.0 International License. 
 * To view a copy of this license, visit http://creativecommons.org/licenses/by-nc-nd/4.0/.
 *
*/

class ExpansionCOTTerritoriesModule: JMRenderableModuleBase
{
	// ------------------------------------------------------------
	// ExpansionCOTTerritoriesModule ExpansionCOTTerritoriesModule
	// ------------------------------------------------------------	
	void ExpansionCOTTerritoriesModule()
	{
		GetPermissionsManager().RegisterPermission( "Expansion.Territories.View" );
		GetPermissionsManager().RegisterPermission( "Expansion.Territories.Apply" );
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
		return "DayZExpansion/GUI/layouts/COT/territories/Territories_Menu.layout";
	}
	
	// ------------------------------------------------------------
	// ExpansionCOTTerritoriesModule GetTitle
	// ------------------------------------------------------------	
	override string GetTitle()
	{
		return "Territories";
	}
	
	// ------------------------------------------------------------
	// ExpansionCOTTerritoriesModule GetIconName
	// ------------------------------------------------------------	
	override string GetIconName()
	{
		return "EX";
	}
	
	// ------------------------------------------------------------
	// ExpansionCOTTerritoriesModule ImageIsIcon
	// ------------------------------------------------------------
	override bool ImageIsIcon()
	{
		return false;
	}
	
	// ------------------------------------------------------------
	// ExpansionCOTTerritoriesModule GetRPCMin
	// ------------------------------------------------------------	
	override int GetRPCMin()
	{
		return ExpansionCOTTerritoriesModuleRPC.INVALID;
	}
	
	// ------------------------------------------------------------
	// ExpansionCOTTerritoriesModule GetRPCMax
	// ------------------------------------------------------------
	override int GetRPCMax()
	{
		return ExpansionCOTTerritoriesModuleRPC.COUNT;
	}
	
	// ------------------------------------------------------------
	// ExpansionCOTTerritoriesModule OnRPC
	// ------------------------------------------------------------
	override void OnRPC( PlayerIdentity sender, Object target, int rpc_type, ref ParamsReadContext ctx )
	{
		switch ( rpc_type )
		{
		case ExpansionCOTTerritoriesModuleRPC.DeleteObject:
			RPC_DeleteObject( ctx, sender, target );
			break;
		}
	}
	
	// ------------------------------------------------------------
	// ExpansionCOTTerritoriesModule DeleteObject
	// Called on client
	// ------------------------------------------------------------
	void DeleteObject(Object obj)
	{
		#ifdef EXPANSIONEXLOGPRINT
		EXLogPrint("ExpansionCOTTerritoriesModule::DeleteObject - Start");
		#endif
		
		if ( IsMissionClient() )
		{
			ScriptRPC rpc = new ScriptRPC();
			rpc.Write( obj );
 			rpc.Send( null, ExpansionCOTTerritoriesModuleRPC.DeleteObject, true );
		}
		
		#ifdef EXPANSIONEXLOGPRINT
		EXLogPrint("ExpansionCOTTerritoriesModule::RequestServerTerritories - End");
		#endif
	}
	
	// ------------------------------------------------------------
	// ExpansionCOTTerritoriesModule RPC_DeleteObject
	// Called on Server
	// ------------------------------------------------------------
	void RPC_DeleteObject( ref ParamsReadContext ctx, PlayerIdentity senderRPC, ref Object target )
	{
		#ifdef EXPANSIONEXLOGPRINT
		EXLogPrint("ExpansionCOTTerritoriesModule::RPC_DeleteObject - Start");
		#endif
		
		if (!senderRPC || !IsMissionHost()) return;
		
		Object obj;
		ctx.Read( obj );
		
		GetGame().ObjectDelete( obj );
		
		#ifdef EXPANSIONEXLOGPRINT
		EXLogPrint("ExpansionCOTTerritoriesModule::RPC_DeleteObject - End");
		#endif
	}
}