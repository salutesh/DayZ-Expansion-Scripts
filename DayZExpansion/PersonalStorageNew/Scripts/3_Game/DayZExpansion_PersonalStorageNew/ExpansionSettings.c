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

		Init(ExpansionPersonalStorageNewSettings);
	}

	override bool OnRPC(PlayerIdentity sender, Object target, int rpc_type, ParamsReadContext ctx)
	{
		if (super.OnRPC(sender, target, rpc_type, ctx))
			return true;
		
		switch (rpc_type)
		{
			case ExpansionSettingsRPC.PersonalStorageNew:
			{
				Receive(ExpansionPersonalStorageNewSettings, ctx);
				return true;
			}
		}

		return false;
	}

	ExpansionPersonalStorageNewSettings GetPersonalStorageNew(bool checkLoaded = true)
	{
		return ExpansionPersonalStorageNewSettings.Cast(Get(ExpansionPersonalStorageNewSettings, checkLoaded));
	}
};
