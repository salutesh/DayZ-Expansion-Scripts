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

	autoptr ExpansionChatSettings m_SettingsChat;

	override protected void OnServerInit()
	{
		LoadSetting(m_SettingsChat);

		m_NetworkedSettings.Insert("expansionchatsettings");

		super.OnServerInit();
	}

	override void Unload()
	{
		super.Unload();

		m_SettingsChat.Unload();
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

		if (!IsSettingLoaded(m_SettingsChat, m_SettingsLoaded))
			return;

		super.CheckSettingsLoaded();
	}

	override void Init()
	{
		m_SettingsChat = new ExpansionChatSettings;

		super.Init();
	}

	override void Send(notnull PlayerIdentity identity)
	{
		if (IsMissionClient())
			return;

		super.Send(identity);

		m_SettingsChat.Send(identity);
	}

	override bool OnRPC(PlayerIdentity sender, Object target, int rpc_type, ParamsReadContext ctx)
	{
		if (super.OnRPC(sender, target, rpc_type, ctx))
			return true;

		if (rpc_type <= ExpansionSettingsRPC.INVALID || rpc_type >= ExpansionSettingsRPC.COUNT)
			return false;

		switch (rpc_type)
		{
		case ExpansionSettingsRPC.Chat:
		{
			Expansion_Assert_False(m_SettingsChat.OnRecieve(ctx), "Failed reading Chat settings");

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
			m_SettingsChat.Save();
		}
	}

	ExpansionChatSettings GetChat()
	{
		return m_SettingsChat;
	}
};
