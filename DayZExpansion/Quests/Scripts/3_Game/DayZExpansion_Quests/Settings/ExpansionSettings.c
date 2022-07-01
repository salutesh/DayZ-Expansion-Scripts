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
	static ref ScriptInvoker SI_Quest = new ScriptInvoker();

	autoptr ExpansionQuestSettings m_SettingsQuests;

	override protected void OnServerInit()
	{
		LoadSetting(m_SettingsQuests);

		m_NetworkedSettings.Insert( "expansionquestsettings" );

		super.OnServerInit();
	}

	override void Unload()
	{
		super.Unload();

		m_SettingsQuests.Unload();
	}

	override protected void CheckSettingsLoaded()
	{
		if (!IsMissionClient())
		{
			m_SettingsLoaded = true;

			return;
		}

		if (m_SettingsLoaded)
			return;

		if (!IsSettingLoaded(m_SettingsQuests, m_SettingsLoaded))
			return;

		super.CheckSettingsLoaded();
	}

	override void Init()
	{
		m_SettingsQuests = new ExpansionQuestSettings;

		super.Init();
	}

	override void Send(notnull PlayerIdentity identity)
	{
		if (IsMissionClient())
			return;

		super.Send(identity);

		m_SettingsQuests.Send(identity);
	}

	override bool OnRPC(PlayerIdentity sender, Object target, int rpc_type, ParamsReadContext ctx)
	{
		if (super.OnRPC(sender, target, rpc_type, ctx))
			return true;

		if (rpc_type <= ExpansionSettingsRPC.INVALID || rpc_type >= ExpansionSettingsRPC.COUNT)
			return false;

		switch (rpc_type)
		{
			case ExpansionSettingsRPC.Quest:
			{
				Expansion_Assert_False(m_SettingsQuests.OnRecieve(ctx), "Failed reading Quest settings");

				return true;
			}
		}

		return false;
	}

	override void Save()
	{
		super.Save();

		if (IsMissionHost() && GetGame().IsMultiplayer())
		{
			m_SettingsQuests.Save();
		}
	}

	ExpansionQuestSettings GetQuest()
	{
		return m_SettingsQuests;
	}
};
