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
	static ref ScriptInvoker SI_Airdrop = new ScriptInvoker();
	static ref ScriptInvoker SI_Mission = new ScriptInvoker();

	autoptr ExpansionAirdropSettings m_SettingsAirdrop;
	autoptr ExpansionMissionSettings m_SettingsMission;

	override protected void OnServerInit()
	{
		LoadSetting(m_SettingsAirdrop);
		LoadSetting(m_SettingsMission);

		// m_NetworkedSettings.Insert( "expansionairdropsettings" );
		// m_NetworkedSettings.Insert( "expansionmissionsettings" );

		super.OnServerInit();
	}

	override void Unload()
	{
		super.Unload();

		m_SettingsAirdrop.Unload();
		m_SettingsMission.Unload();
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

		if (!IsSettingLoaded(m_SettingsAirdrop, m_SettingsLoaded))
			return;

		if (!IsSettingLoaded(m_SettingsMission, m_SettingsLoaded))
			return;

		super.CheckSettingsLoaded();
	}

	override void Init()
	{
		m_SettingsAirdrop = new ExpansionAirdropSettings;
		m_SettingsMission = new ExpansionMissionSettings;
		super.Init();
	}

	override void Send(notnull PlayerIdentity identity)
	{
		if (IsMissionClient())
			return;

		super.Send(identity);

		m_SettingsAirdrop.Send(identity);
	}

	override bool OnRPC(PlayerIdentity sender, Object target, int rpc_type, ParamsReadContext ctx)
	{
		if (super.OnRPC(sender, target, rpc_type, ctx))
			return true;

		if (rpc_type <= ExpansionSettingsRPC.INVALID || rpc_type >= ExpansionSettingsRPC.COUNT)
			return false;

		switch (rpc_type)
		{
		case ExpansionSettingsRPC.AirDrop:
		{
			Expansion_Assert_False(m_SettingsAirdrop.OnRecieve(ctx), "Failed reading AirDrop settings");

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
			m_SettingsAirdrop.Save();
			m_SettingsMission.Save();
		}
	}

	ExpansionAirdropSettings GetAirdrop()
	{
		return m_SettingsAirdrop;
	}
	ExpansionMissionSettings GetMission()
	{
		return m_SettingsMission;
	}
};
