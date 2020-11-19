/**
 * ExpansionCOTVehicleModule.c
 *
 * DayZ Expansion Mod
 * www.dayzexpansion.com
 * © 2020 DayZ Expansion Mod Team
 *
 * This work is licensed under the Creative Commons Attribution-NonCommercial-NoDerivatives 4.0 International License. 
 * To view a copy of this license, visit http://creativecommons.org/licenses/by-nc-nd/4.0/.
 *
*/

class ExpansionCOTVehicleModule: ExpansionCOTModuleBase
{
	void ExpansionCOTVehicleModule()
	{
		GetPermissionsManager().RegisterPermission( "Expansion.Vehicle.View" );
		GetPermissionsManager().RegisterPermission( "Expansion.Vehicle.Update" );
	}

	override bool HasAccess()
	{
		return GetPermissionsManager().HasPermission( "Expansion.Vehicle.View" );
	}

	override string GetLayoutRoot()
	{
		return "DayZExpansion/GUI/layouts/COT/vehicle/form.layout";
	}

	override string GetTitle()
	{
		return "[EX] Vehicle Settings";
	}

	override string GetIconName()
	{
		return "V";
	}

	override bool ImageIsIcon()
	{
		return false;
	}

	override string GetWebhookTitle()
	{
		return "Expansion Vehicle Settings";
	}

	override ref ExpansionSettingBase GetSettingsInstance()
	{
		return GetExpansionSettings().GetVehicle();
	}

	override void OnRegisterSettings()
	{
		CreateEnum( "VehicleSync", ExpansionVehicleNetworkMode, "Vehicle Sync", "", "" );
		array< string > values_VehicleRequireKeyToStart = { "NO", "YES" };
		CreateEnum( "VehicleRequireKeyToStart", values_VehicleRequireKeyToStart, "Requires Key To Start", "", "" );
		CreateToggle( "VehicleRequireAllDoors", "Key Requires All Doors", "", "" );
		CreateToggle( "VehicleLockedAllowInventoryAccess", "Access Locked Inventory", "", "" );
		CreateToggle( "VehicleLockedAllowInventoryAccessWithoutDoors", "Access Locked Inventory w/o Doors", "", "" );
		#ifdef EXPANSION_HELI_WIND
		CreateToggle( "EnableWindAerodynamics", "Wind Aerodynamics", "", "" );
		#endif
		CreateToggle( "EnableTailRotorDamage", "Tail Rotor Damage", "", "" );
		CreateToggle( "EnableHelicopterExplosions", "Enable Helicopter Explosions", "", "" );
		#ifdef EXPANSION_PLAYER_ATTACHMENT
		CreateToggle( "PlayerAttachment", "Player Attaches To Vehicles", "", "" );
		#endif
		#ifdef EXPANSION_VEHICLE_TOWING
		CreateToggle( "Towing", "Towing Vehicles", "", "" );	
		#endif
		CreateToggle( "EnableHelicopterExplosions", "Enable Helicopter Explosions", "", "" );	
	}

	override int GetRPCMin()
	{
		return ExpansionCOTVehicleModuleRPC.INVALID;
	}

	override int GetRPCMax()
	{
		return ExpansionCOTVehicleModuleRPC.COUNT;
	}

	override int GetRPCUpdate()
	{
		return ExpansionCOTVehicleModuleRPC.Update;
	}

	override void OnSend( ref ExpansionSettingBase setting )
	{
		if ( GetGame().IsClient() )
		{
			ScriptRPC rpc = new ScriptRPC();
			setting.OnSend( rpc );
			rpc.Send( NULL, ExpansionCOTVehicleModuleRPC.Update, true, NULL );
		} else if ( IsMissionOffline() )
		{
			GetSettingsInstance().Update( setting );
		}
	}

	override void OnSend_RPC( ref ParamsReadContext ctx, PlayerIdentity senderRPC, Object target )
	{
		if ( !IsMissionHost() )
			return;

		ref ExpansionVehicleSettings setting = new ExpansionVehicleSettings();
		if ( !setting.OnRecieve( ctx ) )
			return;

		JMPlayerInstance inst;
		if ( !GetPermissionsManager().HasPermission( "Expansion.Vehicle.Update", senderRPC, inst ) )
			return;

		UpdateServer( setting, inst );
	}
};