/**
 * ExpansionPartyHud.c
 *
 * DayZ Expansion Mod
 * www.dayzexpansion.com
 * © 2021 DayZ Expansion Mod Team
 *
 * This work is licensed under the Creative Commons Attribution-NonCommercial-NoDerivatives 4.0 International License. 
 * To view a copy of this license, visit http://creativecommons.org/licenses/by-nc-nd/4.0/.
 *
*/
#ifdef EXPANSIONMODGROUPSHUD
class ExpansionPartyHud: ExpansionScriptViewBase
{
	protected ref ExpansionPartyHudController m_PartyHUDController;
	
	void ExpansionPartyHud()
	{		
		if (!m_PartyHUDController)
			m_PartyHUDController = ExpansionPartyHudController.Cast(GetController());		
	}
		
	override string GetLayoutFile() 
	{
		return "DayZExpansion/Groups/GUI/layouts/expansion_party_hud.layout";
	}
	
	override typename GetControllerType() 
	{
		return ExpansionPartyHudController;
	}
	
	void AddMember(string playerID, string playerPlainID, string playerName)
	{
		ExpansionPartyHudMember newMember = new ExpansionPartyHudMember(playerID, playerPlainID, playerName);
		m_PartyHUDController.PartyHUDMemberElements.Insert(newMember);
	}
	
	void UpdateMembers(map<string, string> members)
	{
		array<string> currentMembers = new array<string>;
		for (int i = m_PartyHUDController.PartyHUDMemberElements.Count() - 1; i >= 0; i--)
		{
			string currentMemberID = m_PartyHUDController.PartyHUDMemberElements[i].m_PlayerID;

			bool isStillMember = false;
			if (members.Contains(currentMemberID))
			{
				isStillMember = true;
				currentMembers.Insert(currentMemberID);
			}
			
			if (!isStillMember || !PlayerBase.Expansion_IsOnline(currentMemberID))
			{
				m_PartyHUDController.PartyHUDMemberElements.RemoveOrdered(i);
			}
		}

		foreach (string playerID, string playerName: members)
		{
			int memberIndex = currentMembers.Find(playerID);
			if (memberIndex == -1 && playerID != GetGame().GetPlayer().GetIdentity().GetId())
			{
				SyncPlayer syncPlayer = SyncPlayer.Expansion_GetByID(playerID);
				if (syncPlayer)
					AddMember(playerID, syncPlayer.m_UID, playerName);
			}
		}
	}
	
	void ClearMembers()
	{
		if (!m_PartyHUDController.PartyHUDMemberElements || m_PartyHUDController.PartyHUDMemberElements.Count() == 0)
			return;
		
		m_PartyHUDController.PartyHUDMemberElements.Clear();
	}
	
	override bool CanShow()
	{
		return true;
	}
	
	void Show(bool state)
	{
		GetLayoutRoot().Show(state);
	}
};
#endif