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
		CreateSlider( "ExplosionTime", "Timer before Explosion", "", "", 1.0, 60.0, 1.0); 
		
		CreateToggle( "EnableExplosiveWhitelist", "Enable Explosive Whitelist", "", "" );

		CreateSlider( "ExplosionDamageMultiplier", "Explosion Damage Multiplier", "", "", 0.0, 100.0, 1.0);
		CreateSlider( "ProjectileDamageMultiplier", "Projectile Damage Multiplier", "", "", 0.0, 100.0, 1.0);

		CreateToggle( "CanRaidSafes", "Can Raid Safes", "", "" );
		CreateSlider( "SafeExplosionDamageMultiplier", "Safe Explosion Damage Multiplier", "", "", 0.0, 100.0, 1.0);
		CreateSlider( "SafeProjectileDamageMultiplier", "Safe Projectile Damage Multiplier", "", "", 0.0, 100.0, 1.0);

		CreateToggle( "CanRaidSafes", "Can Raid Safes", "", "" );
		CreateSlider( "SafeExplosionDamageMultiplier", "Safe Explosion Damage Multiplier", "", "", 0.0, 100.0, 1.0);
		CreateSlider( "SafeProjectileDamageMultiplier", "Safe Projectile Damage Multiplier", "", "", 0.0, 100.0, 1.0);

		CreateSlider( "SafeRaidToolTimeSeconds", "Safe Raid Tool Time Seconds", "", "", 0.0, 720.0, 1.0);
		CreateSlider( "SafeRaidToolCycles", "Safe Raid Tool Cycles", "", "", 0.0, 20.0, 1.0);
		CreateSlider( "SafeRaidToolDamagePercent", "Safe Raid Tool Damage Percent", "", "", 0.0, 100.0, 1.0);

		CreateToggle( "CanRaidBarbedWire", "Can Raid BarbedWire", "", "" );
		CreateSlider( "BarbedWireRaidToolTimeSeconds", "BarbedWire Raid Tool Time Seconds", "", "", 0.0, 720.0, 1.0);
		CreateSlider( "BarbedWireRaidToolCycles", "BarbedWire Raid Tool Cycles", "", "", 0.0, 20.0, 1.0);
		CreateSlider( "BarbedWireRaidToolDamagePercent", "BarbedWire Raid Tool Damage Percent", "", "", 0.0, 100.0, 1.0);
		
		CreateEnum( "CanRaidLocksOnWalls", RaidLocksOnWallsEnum, "Can Raid Locks On Walls", "", "" );
		CreateToggle( "CanRaidLocksOnFences", "Can Raid Locks On Fences", "", "" );
		CreateToggle( "CanRaidLocksOnTents", "Can Raid Locks On Tents", "", "" );
		
		CreateSlider( "LockOnWallRaidToolTimeSeconds", "Lock On Wall Raid Tool Time Seconds", "", "", 0.0, 720.0, 1.0);
		CreateSlider( "LockOnFenceRaidToolTimeSeconds", "Lock On Fence Raid Tool Time Seconds", "", "", 0.0, 720.0, 1.0);
		CreateSlider( "LockOnTentRaidToolTimeSeconds", "Lock On Tent Raid Tool Time Seconds", "", "", 0.0, 720.0, 1.0);
		CreateSlider( "LockRaidToolCycles", "Lock Raid Tool Cycles", "", "", 0.0, 20.0, 1.0);
		CreateSlider( "LockRaidToolDamagePercent", "Lock Raid Tool Damage Percent", "", "", 0.0, 100.0, 1.0);

		CreateEnum( "BaseBuildingRaidMode", BaseBuildingRaidEnum, "BaseBuilding Raid Mode", "", "" );
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