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
		CreateToggle( "CanCraftTerritoryFlag", "Can Craft Territory Flag", "", "" );
		CreateToggle( "DestroyFlagOnDismantle", "Destroy Flag On Dismantle", "", "" );
		CreateToggle( "CanDismantleFlag", "Can Dismantle Flag", "", "" );
		CreateToggle( "DismantleOutsideTerritory", "Dismantle Outside Territory", "", "" );
		CreateToggle( "DismantleAnywhere", "Dismantle Anywhere", "", "" );
		CreateToggle( "DoDamageWhenEnterWrongCodeLock", "Do Damage When Enter Wrong Code Lock", "", "" );
		CreateSlider( "DamageWhenEnterWrongCodeLock", "Damage Enter Wrong Code", "", "", 1.0, 100.0, 1.0);
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

		if ( !GetPermissionsManager().HasPermission( "Expansion.BaseBuilding.Update", senderRPC ) )
			return;

		UpdateServer( setting );
	}
};