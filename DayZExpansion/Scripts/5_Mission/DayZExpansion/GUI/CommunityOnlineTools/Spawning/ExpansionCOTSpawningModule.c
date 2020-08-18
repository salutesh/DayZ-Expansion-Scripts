class ExpansionCOTSpawningModule: ExpansionCOTModuleBase
{
	void ExpansionCOTSpawningModule()
	{
		GetPermissionsManager().RegisterPermission( "Expansion.Spawning.View" );
		GetPermissionsManager().RegisterPermission( "Expansion.Spawning.Update" );
	}

	override bool HasAccess()
	{
		return GetPermissionsManager().HasPermission( "Expansion.Spawning.View" );
	}

	override string GetLayoutRoot()
	{
		return "DayZExpansion/GUI/layouts/COT/spawning/form.layout";
	}

	override string GetTitle()
	{
		return "[EX] Spawning Settings";
	}

	override string GetIconName()
	{
		return "S";
	}

	override bool ImageIsIcon()
	{
		return false;
	}

	override ref ExpansionSettingBase GetSettingsInstance()
	{
		return GetExpansionSettings().GetSpawn();
	}

	override void OnRegisterSettings()
	{
		/*array< string > values_SpawningRequireKeyToStart = { "NO", "YES" };
		CreateEnum( "SpawningRequireKeyToStart", values_SpawningRequireKeyToStart, "Requires Key To Start", "", "" );
		CreateToggle( "SpawningRequireAllDoors", "Key Requires All Doors", "", "" );
		CreateToggle( "SpawningLockedAllowInventoryAccess", "Access Locked Inventory", "", "" );
		CreateToggle( "SpawningLockedAllowInventoryAccessWithoutDoors", "Access Locked Inventory w/o Doors", "", "" );
		CreateToggle( "EnableWindAerodynamics", "Wind Aerodynamics", "", "" );
		CreateToggle( "EnableTailRotorDamage", "Tail Rotor Damage", "", "" );*/
	}

	override int GetRPCMin()
	{
		return ExpansionCOTSpawnModuleRPC.INVALID;
	}

	override int GetRPCMax()
	{
		return ExpansionCOTSpawnModuleRPC.COUNT;
	}

	override int GetRPCUpdate()
	{
		return ExpansionCOTSpawnModuleRPC.Update;
	}

	override void OnSend( ref ExpansionSettingBase setting )
	{
		if ( GetGame().IsClient() )
		{
			ScriptRPC rpc = new ScriptRPC();
			setting.OnSend( rpc );
			rpc.Send( NULL, ExpansionCOTSpawnModuleRPC.Update, true, NULL );
		} else if ( IsMissionOffline() )
		{
			GetSettingsInstance().Update( setting );
		}
	}

	override void OnSend_RPC( ref ParamsReadContext ctx, PlayerIdentity senderRPC, Object target )
	{
		if ( !IsMissionHost() )
			return;

		ref ExpansionSpawnSettings setting = new ExpansionSpawnSettings();
		if ( !setting.OnRecieve( ctx ) )
			return;

		if ( !GetPermissionsManager().HasPermission( "Expansion.Spawn.Update", senderRPC ) )
			return;

		UpdateServer( setting );
	}
};