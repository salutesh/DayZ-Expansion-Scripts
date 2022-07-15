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
	static ref ScriptInvoker SI_Party = new ScriptInvoker();

	override void Init()
	{
		super.Init();

		Init(ExpansionPartySettings);
	}

	override bool OnRPC(PlayerIdentity sender, Object target, int rpc_type, ParamsReadContext ctx)
	{
		if (super.OnRPC(sender, target, rpc_type, ctx))
			return true;

		if (rpc_type <= ExpansionSettingsRPC.INVALID || rpc_type >= ExpansionSettingsRPC.COUNT)
			return false;

		switch (rpc_type)
		{
		case ExpansionSettingsRPC.Party:
		{
			Receive(ExpansionPartySettings, ctx);

			return true;
		}
		}

		return false;
	}

	ExpansionPartySettings GetParty(bool checkLoaded = true)
	{
		return ExpansionPartySettings.Cast(Get(ExpansionPartySettings, checkLoaded));
	}
};
