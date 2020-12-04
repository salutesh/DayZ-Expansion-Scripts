/**
 * ExpansionCOTBaseBuildingModule.c
 *
 * DayZ Expansion Mod
 * www.dayzexpansion.com
 * © 2020 DayZ Expansion Mod Team
 *
 * This work is licensed under the Creative Commons Attribution-NonCommercial-NoDerivatives 4.0 International License. 
 * To view a copy of this license, visit http://creativecommons.org/licenses/by-nc-nd/4.0/.
 *
*/

class ExpansionCOTBaseBuildingModule: ExpansionCOTModuleBase
{
	void ExpansionCOTBaseBuildingModule()
	{
		GetPermissionsManager().RegisterPermission( "Expansion.BaseBuilding.View" );
		GetPermissionsManager().RegisterPermission( "Expansion.BaseBuilding.Update" );
	}

	override bool HasAccess()
	{
		return GetPermissionsManager().HasPermission( "Expansion.BaseBuilding.View" );
	}

	override string GetLayoutRoot()
	{
		return "DayZExpansion/GUI/layouts/COT/basebuilding/form.layout";
	}

	override string GetTitle()
	{
		return "[EX] BaseBuilding Settings";
	}

	override string GetIconName()
	{
		return "BB";
	}

	override bool ImageIsIcon()
	{
		return false;
	}

	override string GetWebhookTitle()
	{
		return "Expansion BaseBuilding Settings";
	}

	override ref ExpansionSettingBase GetSettingsInstance()
	{
		return GetExpansionSettings().GetBaseBuilding();
	}

	override void OnRegisterSettings()
	{
		CreateToggle( "CanBuildAnywhere", "Can Build Anywhere", "", "" );
		CreateToggle( "AllowBuildingWithoutATerritory", "Allow Building Without A Territory", "", "" );

		CreateToggle( "CanCraftVanillaBasebuilding", "Can Craft Vanilla Basebuilding", "", "" );
		CreateToggle( "CanCraftExpansionBasebuilding", "Can Craft Expansion Basebuilding", "", "" );

		CreateToggle( "DestroyFlagOnDismantle", "Destroy Flag On Dismantle", "", "" );
		
		array< string > values_DismantleFlagRequireTools = { "Owner only", "With Hands", "With Tools" };
		CreateEnum( "DismantleFlagRequireTools", values_DismantleFlagRequireTools, "Dismantle Flag Requires Tools", "", "" );
		CreateToggle( "DismantleOutsideTerritory", "Dismantle Outside Territory", "", "" );
		CreateToggle( "DismantleInsideTerritory", "Dismantle Inside Other Territory", "", "" );
		CreateToggle( "DismantleAnywhere", "Dismantle Anywhere", "", "" );

		CreateToggle( "CodelockActionsAnywhere", "Codelock: Actions Anywhere", "", "" );
		CreateSlider( "CodeLockLength", "Codelock Length", "", "", 1.0, 6.0, 1.0);
		CreateToggle( "DoDamageWhenEnterWrongCodeLock", "Codelock: Damage On Wrong Code", "", "" );
		CreateSlider( "DamageWhenEnterWrongCodeLock", "Codelock: Damage Dealt", "", "", 1.0, 100.0, 1.0);

		CreateToggle( "CanCraftTerritoryFlagKit", "Can Craft Territory Flag Kit", "", "" );

		CreateToggle( "SimpleTerritory", "Simple Territory Creation", "", "" );
		CreateToggle( "AutomaticFlagOnCreation", "Add Flag Item", "", "" );
		array< string > values_EnableFlagMenu = { "Disabled", "Enabled", "No flag choice" };
		CreateEnum( "EnableFlagMenu", values_EnableFlagMenu, "Enable Flag Menu", "", "" );
		CreateToggle( "GetTerritoryFlagKitAfterBuild", "Get Territory FlagKit After Build", "", "" );
	}


	override int GetRPCMin()
	{
		return ExpansionCOTBaseBuildingModuleRPC.INVALID;
	}

	override int GetRPCMax()
	{
		return ExpansionCOTBaseBuildingModuleRPC.COUNT;
	}

	override int GetRPCUpdate()
	{
		return ExpansionCOTBaseBuildingModuleRPC.Update;
	}

	override void OnSend( ref ExpansionSettingBase setting )
	{
		if ( GetGame().IsClient() )
		{
			ScriptRPC rpc = new ScriptRPC();
			setting.OnSend( rpc );
			rpc.Send( NULL, ExpansionCOTBaseBuildingModuleRPC.Update, true, NULL );
		} else if ( IsMissionOffline() )
		{
			GetSettingsInstance().Update( setting );
		}
	}

	override void OnSend_RPC( ref ParamsReadContext ctx, PlayerIdentity senderRPC, Object target )
	{
		if ( !IsMissionHost() )
			return;

		ref ExpansionBaseBuildingSettings setting = new ExpansionBaseBuildingSettings();
		if ( !setting.OnRecieve( ctx ) )
			return;

		JMPlayerInstance inst;
		if ( !GetPermissionsManager().HasPermission( "Expansion.BaseBuilding.Update", senderRPC, inst ) )
			return;

		UpdateServer( setting, inst );
	}
};