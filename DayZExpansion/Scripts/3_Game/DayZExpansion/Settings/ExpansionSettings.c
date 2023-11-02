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
	static ref ScriptInvoker SI_General = new ScriptInvoker();
	static ref ScriptInvoker SI_PlayerList = new ScriptInvoker();
	static ref ScriptInvoker SI_SocialMedia = new ScriptInvoker();

	override void Init()
	{
		super.Init();

		Init(ExpansionGeneralSettings, true);
		Init(ExpansionPlayerListSettings, true);
		Init(ExpansionSocialMediaSettings, true);
	}

	void RPC_GeneralSettings(PlayerIdentity sender, Object target, ParamsReadContext ctx )
	{
		Receive(ExpansionGeneralSettings, ctx);
	}

	void RPC_PlayerListSettings(PlayerIdentity sender, Object target, ParamsReadContext ctx )
	{
		Receive(ExpansionPlayerListSettings, ctx);
	}

	void RPC_SocialMediaSettings(PlayerIdentity sender, Object target, ParamsReadContext ctx )
	{
		Receive(ExpansionSocialMediaSettings, ctx);
	}

	ExpansionGeneralSettings GetGeneral(bool checkLoaded = true)
	{
		return ExpansionGeneralSettings.Cast(Get(ExpansionGeneralSettings, checkLoaded));
	}

	ExpansionPlayerListSettings GetPlayerList(bool checkLoaded = true)
	{
		return ExpansionPlayerListSettings.Cast(Get(ExpansionPlayerListSettings, checkLoaded));
	}

	ExpansionSocialMediaSettings GetSocialMedia(bool checkLoaded = true)
	{
		return ExpansionSocialMediaSettings.Cast(Get(ExpansionSocialMediaSettings, checkLoaded));
	}
};
