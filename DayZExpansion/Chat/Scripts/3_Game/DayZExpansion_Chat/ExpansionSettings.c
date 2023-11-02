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
	static ref ScriptInvoker SI_Chat = new ScriptInvoker();

	override void Init()
	{
		super.Init();

		Init(ExpansionChatSettings, true);
	}

	void RPC_ChatSettings(PlayerIdentity sender, Object target, ParamsReadContext ctx)
	{
		Receive(ExpansionChatSettings, ctx);
	}

	ExpansionChatSettings GetChat(bool checkLoaded = true)
	{
		return ExpansionChatSettings.Cast(Get(ExpansionChatSettings, checkLoaded));
	}
};
