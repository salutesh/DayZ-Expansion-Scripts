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
	protected ref ExpansionPartyHud m_PartyHUD;
	protected bool m_ExpansionPartyHUDState;
	protected ExpansionPartyModule m_PartyModule;
	protected bool m_ShowPartyHUD;

	void IngameHud()
	{
		m_PartyModule = ExpansionPartyModule.Cast(CF_ModuleCoreManager.Get(ExpansionPartyModule));
	}
	
	void ~IngameHud()
	{
		if (m_ShowPartyHUD)
		{
			if (m_PartyModule && m_PartyModule.m_PartyHUDInvoker)
				m_PartyModule.m_PartyHUDInvoker.Remove(UpdatePartyHUD);
		}
	}

	override void Update( float timeslice )
	{
		super.Update(timeslice);
		
		if (!GetExpansionSettings().GetParty(false).IsLoaded())
			return;
		
		if (!m_ShowPartyHUD)
			m_ShowPartyHUD = GetExpansionSettings().GetParty().ShowPartyMemberHUD;

		if (m_ShowPartyHUD)
		{
			if (!m_PartyHUD)
			{
				m_PartyHUD = new ExpansionPartyHud();
				if (m_PartyModule && m_PartyModule.m_PartyHUDInvoker)
					m_PartyModule.m_PartyHUDInvoker.Insert(UpdatePartyHUD);
			}
		}
	}

	void UpdatePartyHUD()
	{
		if (!m_PartyHUD)
			return;

		map<string, string> partyPlayers = new map<string, string>;
		if (m_PartyModule)
		{
			ExpansionPartyData partyData = m_PartyModule.GetParty();
			if (partyData && partyData.GetPlayers().Count() > 0)
			{
				array<ref ExpansionPartyPlayerData> members = partyData.GetPlayers();
				for (int i = 0; i < members.Count(); ++i)
				{
					partyPlayers.Insert(members[i].UID, members[i].Name);
				}

				if (m_PartyHUD)
					m_PartyHUD.UpdateMembers(partyPlayers);
			}
			else if (!partyData && m_PartyHUD)
			{
				m_PartyHUD.ClearMembers();
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

		if (m_PartyHUD)
		{
			if (m_IsHudVisible && m_ExpansionPartyHUDState)
				m_PartyHUD.Show(true);
			else
				m_PartyHUD.Show(false);
		}
	}
};
#endif