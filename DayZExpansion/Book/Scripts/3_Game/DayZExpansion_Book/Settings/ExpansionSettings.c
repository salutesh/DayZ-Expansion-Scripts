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
	static ref ScriptInvoker SI_Book = new ScriptInvoker();

	override void Init()
	{
#ifdef EXPANSIONTRACE
		auto trace = CF_Trace_0(ExpansionTracing.SETTINGS, this, "Init");
#endif

		super.Init();

		Init(ExpansionBookSettings, true);
	}

	void RPC_BookSettings(PlayerIdentity sender, Object target, ParamsReadContext ctx)
	{
		Receive(ExpansionBookSettings, ctx);
	}

	ExpansionBookSettings GetBook(bool checkLoaded = true)
	{
		return ExpansionBookSettings.Cast(Get(ExpansionBookSettings, checkLoaded));
	}
};
