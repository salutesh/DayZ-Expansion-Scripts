/**
 * ExpansionClientSettingsModule.c
 *
 * DayZ Expansion Mod
 * www.dayzexpansion.com
 * © 2022 DayZ Expansion Mod Team
 *
 * This work is licensed under the Creative Commons Attribution-NonCommercial-NoDerivatives 4.0 International License. 
 * To view a copy of this license, visit http://creativecommons.org/licenses/by-nc-nd/4.0/.
 *
*/

class ExpansionClientSettingsModule : JMModuleBase
{
	static ref ScriptInvoker m_SettingChanged = new ScriptInvoker;

	override bool IsServer()
	{
		return false;
	}

	override bool IsClient()
	{
		return true;
	}

	override void OnInvokeConnect(PlayerBase player, PlayerIdentity identity)
	{
		if (player)
		{
			GetExpansionClientSettings().Load();
		}

		OnSettingsUpdated();
	}

	override void OnClientLogout(PlayerBase player, PlayerIdentity identity, int logoutTime, bool authFailed)
	{
		if (player)
		{
			GetExpansionClientSettings().Save();
		}
	}

	override void OnSettingsUpdated()
	{
	}
};
