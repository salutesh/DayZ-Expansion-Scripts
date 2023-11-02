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
	static ref ScriptInvoker SI_Hardline = new ScriptInvoker();

	override void Init()
	{
		super.Init();

		Init(ExpansionHardlineSettings, true);
	}

	void RPC_HardlineSettings(PlayerIdentity sender, Object target, ParamsReadContext ctx)
	{
		Receive(ExpansionHardlineSettings, ctx);
	}

	ExpansionHardlineSettings GetHardline(bool checkLoaded = true)
	{
		return ExpansionHardlineSettings.Cast(Get(ExpansionHardlineSettings, checkLoaded));
	}
};
