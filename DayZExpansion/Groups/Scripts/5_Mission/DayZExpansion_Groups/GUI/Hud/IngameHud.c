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

	void ~IngameHud()
	{
		if (GetExpansionSettings().GetParty().ShowPartyMemberHUD)
		{
			ExpansionPartyModule partyModule = ExpansionPartyModule.Cast(CF_ModuleCoreManager.Get(ExpansionPartyModule));
			if (partyModule && partyModule.m_PartyHUDInvoker)
				partyModule.m_PartyHUDInvoker.Remove(UpdatePartyHUD);
		}
	}

	override void Update( float timeslice )
	{
		super.Update(timeslice);

		if (GetExpansionSettings().GetParty(false).IsLoaded() && GetExpansionSettings().GetParty().ShowPartyMemberHUD)
		{
			if (!m_PartyHUD)
			{
				m_PartyHUD = new ExpansionPartyHud();
				ExpansionPartyModule partyModule = ExpansionPartyModule.Cast(CF_ModuleCoreManager.Get(ExpansionPartyModule));
				if (partyModule && partyModule.m_PartyHUDInvoker)
					partyModule.m_PartyHUDInvoker.Insert(UpdatePartyHUD);
			}
		}
	}

	void UpdatePartyHUD()
	{
		if (!m_PartyHUD)
			return;

		map<string, string> partyPlayers = new map<string, string>;
		ExpansionPartyModule partyModule = ExpansionPartyModule.Cast(CF_ModuleCoreManager.Get(ExpansionPartyModule));
		if (partyModule)
		{
			ExpansionPartyData partyData = partyModule.GetParty();
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
			bool showPartyHUD = m_HudState && m_ExpansionPartyHUDState;
			m_PartyHUD.Show(showPartyHUD);
		}
	}
};
#endif