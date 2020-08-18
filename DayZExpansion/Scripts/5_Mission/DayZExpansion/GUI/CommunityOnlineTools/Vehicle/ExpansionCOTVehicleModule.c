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

	override ref ExpansionSettingBase GetSettingsInstance()
	{
		return GetExpansionSettings().GetVehicle();
	}

	override void OnRegisterSettings()
	{
		array< string > values_VehicleRequireKeyToStart = { "NO", "YES" };
		CreateEnum( "VehicleRequireKeyToStart", values_VehicleRequireKeyToStart, "Requires Key To Start", "", "" );
		CreateToggle( "VehicleRequireAllDoors", "Key Requires All Doors", "", "" );
		CreateToggle( "VehicleLockedAllowInventoryAccess", "Access Locked Inventory", "", "" );
		CreateToggle( "VehicleLockedAllowInventoryAccessWithoutDoors", "Access Locked Inventory w/o Doors", "", "" );
		CreateToggle( "EnableWindAerodynamics", "Wind Aerodynamics", "", "" );
		CreateToggle( "EnableTailRotorDamage", "Tail Rotor Damage", "", "" );
		CreateToggle( "PlayerAttachment", "Player Attaches To Vehicles", "", "" );	
		CreateToggle( "Towing", "Towing Vehicles", "", "" );	
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

		if ( !GetPermissionsManager().HasPermission( "Expansion.Vehicle.Update", senderRPC ) )
			return;

		UpdateServer( setting );
	}
};