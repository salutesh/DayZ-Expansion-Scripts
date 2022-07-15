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

		Init(ExpansionGeneralSettings);
		Init(ExpansionPlayerListSettings);
		Init(ExpansionSocialMediaSettings);
	}

	override bool OnRPC( PlayerIdentity sender, Object target, int rpc_type, ParamsReadContext ctx )
	{
#ifdef EXPANSIONTRACE
		auto trace = CF_Trace_0(ExpansionTracing.SETTINGS, this, "OnRPC");
#endif

		if (super.OnRPC(sender, target, rpc_type, ctx))
			return true;

		if ( rpc_type <= ExpansionSettingsRPC.INVALID || rpc_type >= ExpansionSettingsRPC.COUNT )
			return false;

		switch ( rpc_type )
		{
			case ExpansionSettingsRPC.General:
			{
				Receive(ExpansionGeneralSettings, ctx);
				return true;
			}

			case ExpansionSettingsRPC.PlayerList:
			{
				Receive(ExpansionPlayerListSettings, ctx);
				return true;
			}

			case ExpansionSettingsRPC.SocialMedia:
			{
				Receive(ExpansionSocialMediaSettings, ctx);
				return true;
			}
		}

		return false;
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
