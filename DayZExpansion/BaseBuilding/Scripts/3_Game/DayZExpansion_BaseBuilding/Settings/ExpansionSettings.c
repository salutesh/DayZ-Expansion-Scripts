/**
 * ExpansionSettings.c
 *
 * DayZ Expansion Mod
 * www.dayzexpansion.com
 * © 2022 DayZ Expansion Mod Team
 *
 * This work is licensed under the Creative Commons Attribution-NonCommercial-NoDerivatives 4.0 International License. 
 * To view a copy of this license, visit http://creativecommons.org/licenses/by-nc-nd/4.0/.
 *
*/

modded class ExpansionSettings
{
	static ref ScriptInvoker SI_BaseBuilding = new ScriptInvoker();
	static ref ScriptInvoker SI_Raid = new ScriptInvoker();
	static ref ScriptInvoker SI_Territory = new ScriptInvoker();

	override void Init()
	{
		super.Init();

		Init(ExpansionBaseBuildingSettings, true);
		Init(ExpansionRaidSettings, true);
		Init(ExpansionTerritorySettings, true);
	}

	void RPC_BaseBuildingSettings(PlayerIdentity sender, Object target, ParamsReadContext ctx)
	{
		Receive(ExpansionBaseBuildingSettings, ctx);
	}

	void RPC_RaidSettings(PlayerIdentity sender, Object target, ParamsReadContext ctx)
	{
		Receive(ExpansionRaidSettings, ctx);
	}

	void RPC_TerritorySettings(PlayerIdentity sender, Object target, ParamsReadContext ctx)
	{
		Receive(ExpansionTerritorySettings, ctx);
	}

	ExpansionBaseBuildingSettings GetBaseBuilding(bool checkLoaded = true)
	{
		return ExpansionBaseBuildingSettings.Cast(Get(ExpansionBaseBuildingSettings, checkLoaded));
	}

	ExpansionRaidSettings GetRaid(bool checkLoaded = true)
	{
		return ExpansionRaidSettings.Cast(Get(ExpansionRaidSettings, checkLoaded));
	}

	ExpansionTerritorySettings GetTerritory(bool checkLoaded = true)
	{
		return ExpansionTerritorySettings.Cast(Get(ExpansionTerritorySettings, checkLoaded));
	}
};
