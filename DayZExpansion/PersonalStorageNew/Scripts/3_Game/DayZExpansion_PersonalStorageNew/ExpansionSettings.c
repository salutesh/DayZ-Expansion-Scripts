/**
 * ExpansionSettings.c
 *
 * DayZ Expansion Mod
 * www.dayzexpansion.com
 * © 2023 DayZ Expansion Mod Team
 *
 * This work is licensed under the Creative Commons Attribution-NonCommercial-NoDerivatives 4.0 International License. 
 * To view a copy of this license, visit http://creativecommons.org/licenses/by-nc-nd/4.0/.
 *
*/

modded class ExpansionSettings
{
	static ref ScriptInvoker SI_PersonalStorageNew = new ScriptInvoker();

	override void Init()
	{
		super.Init();

		Init(ExpansionPersonalStorageNewSettings, true);
	}

	void RPC_PersonalStorageNewSettings(PlayerIdentity sender, Object target, ParamsReadContext ctx)
	{
		Receive(ExpansionPersonalStorageNewSettings, ctx);
	}

	ExpansionPersonalStorageNewSettings GetPersonalStorageNew(bool checkLoaded = true)
	{
		return ExpansionPersonalStorageNewSettings.Cast(Get(ExpansionPersonalStorageNewSettings, checkLoaded));
	}
};
