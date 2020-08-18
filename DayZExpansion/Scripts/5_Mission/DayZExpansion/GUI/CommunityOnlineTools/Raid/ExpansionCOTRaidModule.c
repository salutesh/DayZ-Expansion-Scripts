class ExpansionCOTRaidModule: ExpansionCOTModuleBase
{
	void ExpansionCOTRaidModule()
	{
		GetPermissionsManager().RegisterPermission( "Expansion.Raid.View" );
		GetPermissionsManager().RegisterPermission( "Expansion.Raid.Update" );
	}

	override bool HasAccess()
	{
		return GetPermissionsManager().HasPermission( "Expansion.Raid.View" );
	}

	override string GetLayoutRoot()
	{
		return "DayZExpansion/GUI/layouts/COT/raid/form.layout";
	}

	override string GetTitle()
	{
		return "[EX] Raid Settings";
	}

	override string GetIconName()
	{
		return "R";
	}

	override bool ImageIsIcon()
	{
		return false;
	}

	override ref ExpansionSettingBase GetSettingsInstance()
	{
		return GetExpansionSettings().GetRaid();
	}

	override void OnRegisterSettings()
	{		
		CreateSlider( "ExplosionTime", "Explosion Time", "", "", 30.0, 300.0, 1.0); 
		CreateToggle( "EnableExplosiveWhitelist", "Enable Explosive Whitelist", "", "" );
		CreateSlider( "ExplosionDamageMultiplier", "Explosion Damage Multiplier", "", "", 1.0, 100.0, 1.0);
		CreateSlider( "ProjectileDamageMultiplier", "Projectile Damage Multiplier", "", "", 1.0, 100.0, 1.0);
		CreateToggle( "CanRaidSafes", "Can Raid Safes", "", "" );
		CreateSlider( "SafeExplosionDamageMultiplier", "Safe Explosion Damage Multiplier", "", "", 1.0, 100.0, 1.0);
		CreateSlider( "SafeProjectileDamageMultiplier", "Safe Projectile Damage Multiplier", "", "", 1.0, 100.0, 1.0);
	}

	override int GetRPCMin()
	{
		return ExpansionCOTRaidModuleRPC.INVALID;
	}

	override int GetRPCMax()
	{
		return ExpansionCOTRaidModuleRPC.COUNT;
	}

	override int GetRPCUpdate()
	{
		return ExpansionCOTRaidModuleRPC.Update;
	}

	override void OnSend( ref ExpansionSettingBase setting )
	{
		if ( GetGame().IsClient() )
		{
			ScriptRPC rpc = new ScriptRPC();
			setting.OnSend( rpc );
			rpc.Send( NULL, ExpansionCOTRaidModuleRPC.Update, true, NULL );
		} else if ( IsMissionOffline() )
		{
			GetSettingsInstance().Update( setting );
		}
	}

	override void OnSend_RPC( ref ParamsReadContext ctx, PlayerIdentity senderRPC, Object target )
	{
		if ( !IsMissionHost() )
			return;

		ref ExpansionRaidSettings setting = new ExpansionRaidSettings();
		if ( !setting.OnRecieve( ctx ) )
			return;

		if ( !GetPermissionsManager().HasPermission( "Expansion.Raid.Update", senderRPC ) )
			return;

		UpdateServer( setting );
	}
};