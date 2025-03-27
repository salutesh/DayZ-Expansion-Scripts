/**
 * IngameHud.c
 *
 * DayZ Expansion Mod
 * www.dayzexpansion.com
 * © 2022 DayZ Expansion Mod Team
 *
 * This work is licensed under the Creative Commons Attribution-NonCommercial-NoDerivatives 4.0 International License.
 * To view a copy of this license, visit http://creativecommons.org/licenses/by-nc-nd/4.0/.
 *
*/

#ifdef EXPANSIONMODGROUPSHUD
modded class IngameHud
{
	protected ref ExpansionPartyHud m_Expansion_PartyHud;
	protected bool m_ExpansionPartyHUDState;
	protected ExpansionPartyModule m_Expansion_PartyModule;
	protected bool m_Expansion_ShowPartyHUD;
	ref ExpansionPartySettings m_Expansion_PartySettings;

	void IngameHud()
	{
		m_Expansion_PartyModule = ExpansionPartyModule.Cast(CF_ModuleCoreManager.Get(ExpansionPartyModule));
		m_Expansion_PartySettings = GetExpansionSettings().GetParty(false);
	}
	
	void ~IngameHud()
	{
		if (m_Expansion_ShowPartyHUD)
		{
			if (m_Expansion_PartyModule && m_Expansion_PartyModule.m_PartyHUDInvoker)
				m_Expansion_PartyModule.m_PartyHUDInvoker.Remove(UpdatePartyHUD);
		}
	}

	override void Update( float timeslice )
	{
		super.Update(timeslice);
		
		if (!m_Expansion_PartySettings.IsLoaded())
			return;
		
		if (!m_Expansion_ShowPartyHUD)
			m_Expansion_ShowPartyHUD = m_Expansion_PartySettings.ShowPartyMemberHUD;

		if (m_Expansion_ShowPartyHUD)
		{
			if (!m_Expansion_PartyHud)
			{
				m_Expansion_PartyHud = new ExpansionPartyHud();
				if (m_Expansion_PartyModule && m_Expansion_PartyModule.m_PartyHUDInvoker)
					m_Expansion_PartyModule.m_PartyHUDInvoker.Insert(UpdatePartyHUD);
			}
		}
	}

	void UpdatePartyHUD()
	{
		if (!m_Expansion_PartyHud)
			return;

		map<string, string> partyPlayers = new map<string, string>;
		if (m_Expansion_PartyModule)
		{
			ExpansionPartyData partyData = m_Expansion_PartyModule.GetParty();
			if (partyData && partyData.GetPlayers().Count() > 0)
			{
				array<ref ExpansionPartyPlayerData> members = partyData.GetPlayers();
				for (int i = 0; i < members.Count(); ++i)
				{
					partyPlayers.Insert(members[i].UID, members[i].Name);
				}

				if (m_Expansion_PartyHud)
					m_Expansion_PartyHud.UpdateMembers(partyPlayers);
			}
			else if (!partyData && m_Expansion_PartyHud)
			{
				m_Expansion_PartyHud.ClearMembers();
			}
		}
	}

	bool GetPartyHUDState()
	{
		return m_ExpansionPartyHUDState;
	}

	void ShowPartyHUD(bool show)
	{
		m_ExpansionPartyHUDState = show;
		RefreshHudVisibility();
	}

	void PartyHUDShow()
	{
		ShowPartyHUD(true);
	}

	void PartyHUDHide()
	{
		ShowPartyHUD(false);
	}

	override void RefreshHudVisibility()
	{
		super.RefreshHudVisibility();

		if (m_Expansion_PartyHud)
		{
			if (Expansion_CanShowHUDElements() && m_ExpansionPartyHUDState)
				m_Expansion_PartyHud.Show(true);
			else
				m_Expansion_PartyHud.Show(false);
		}
	}
};
#endif