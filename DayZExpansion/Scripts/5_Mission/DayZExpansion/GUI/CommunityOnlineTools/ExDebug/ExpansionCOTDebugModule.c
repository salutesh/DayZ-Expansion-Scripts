/**
 * ExpansionCOTDebugModule.c
 *
 * DayZ Expansion Mod
 * www.dayzexpansion.com
 * © 2020 DayZ Expansion Mod Team
 *
 * This work is licensed under the Creative Commons Attribution-NonCommercial-NoDerivatives 4.0 International License. 
 * To view a copy of this license, visit http://creativecommons.org/licenses/by-nc-nd/4.0/.
 *
*/

class ExpansionCOTDebugModule: ExpansionCOTModuleBase
{
	void ExpansionCOTDebugModule()
	{
		GetPermissionsManager().RegisterPermission( "Expansion.Debug.View" );
		GetPermissionsManager().RegisterPermission( "Expansion.Debug.Update" );
	}

	override bool HasAccess()
	{
		return GetPermissionsManager().HasPermission( "Expansion.Debug.View" );
	}

	override string GetLayoutRoot()
	{
		return "DayZExpansion/GUI/layouts/COT/exdebug/form.layout";
	}

	override string GetTitle()
	{
		return "[EX] Debug Settings";
	}

	override string GetIconName()
	{
		return "D";
	}

	override bool ImageIsIcon()
	{
		return false;
	}

	override string GetWebhookTitle()
	{
		return "Expansion Debug Settings";
	}

	override ExpansionSettingBase GetSettingsInstance()
	{
		return GetExpansionSettings().GetDebug();
	}

	override void OnRegisterSettings()
	{
		CreateToggle( "ShowVehicleDebugMarkers", "Show Vehicle Debug Markers", "", "" );

		array< string > values_DebugVehicleTransformSet = { "SET", "PHYSICS" };
		CreateEnum( "DebugVehicleTransformSet", values_DebugVehicleTransformSet, "Debug Vehicle Transform", "", "" );

		array< string > values_DebugVehicleSync = { "CLIENT ONLY", "ALWAYS" };
		CreateEnum( "DebugVehicleSync", values_DebugVehicleSync, "Physics Sync", "", "" );

		array< string > values_DebugVehiclePlayerNetworkBubbleMode = { "NONE", "CREATE", "CREATE/DESTROY" };
		CreateEnum( "DebugVehiclePlayerNetworkBubbleMode", values_DebugVehiclePlayerNetworkBubbleMode, "Network Bubble", "", "" );
	}

	override int GetRPCMin()
	{
		return ExpansionCOTDebugModuleRPC.INVALID;
	}

	override int GetRPCMax()
	{
		return ExpansionCOTDebugModuleRPC.COUNT;
	}

	override int GetRPCUpdate()
	{
		return ExpansionCOTDebugModuleRPC.Update;
	}

	override void OnSend(  ExpansionSettingBase setting )
	{
		if ( GetGame().IsClient() )
		{
			ScriptRPC rpc = new ScriptRPC();
			setting.OnSend( rpc );
			rpc.Send( NULL, ExpansionCOTDebugModuleRPC.Update, true, NULL );
		} else if ( IsMissionOffline() )
		{
			GetSettingsInstance().Update( setting );
		}
	}

	override void OnSend_RPC( ParamsReadContext ctx, PlayerIdentity senderRPC, Object target )
	{
		if ( !IsMissionHost() )
			return;

		ExpansionDebugSettings setting = new ExpansionDebugSettings();
		if ( !setting.OnRecieve( ctx ) )
			return;

		JMPlayerInstance inst;
		if ( !GetPermissionsManager().HasPermission( "Expansion.Debug.Update", senderRPC, inst ) )
			return;

		UpdateServer( setting, inst );
	}
};