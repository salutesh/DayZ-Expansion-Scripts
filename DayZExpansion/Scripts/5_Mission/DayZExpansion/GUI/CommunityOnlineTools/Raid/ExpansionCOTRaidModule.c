/**
 * ExpansionCOTRaidModule.c
 *
 * DayZ Expansion Mod
 * www.dayzexpansion.com
 * © 2020 DayZ Expansion Mod Team
 *
 * This work is licensed under the Creative Commons Attribution-NonCommercial-NoDerivatives 4.0 International License. 
 * To view a copy of this license, visit http://creativecommons.org/licenses/by-nc-nd/4.0/.
 *
*/

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

	override string GetWebhookTitle()
	{
		return "Expansion Raid Settings";
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

		JMPlayerInstance inst;
		if ( !GetPermissionsManager().HasPermission( "Expansion.Raid.Update", senderRPC, inst ) )
			return;

		UpdateServer( setting, inst );
	}
};