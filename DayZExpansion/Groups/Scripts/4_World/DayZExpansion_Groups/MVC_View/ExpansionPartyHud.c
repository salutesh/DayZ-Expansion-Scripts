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
class ExpansionPartyHud extends ExpansionScriptViewBase
{
	ref ExpansionPartyHudController m_PartyHUDController;
	private static autoptr map<string, ref ExpansionPartyHudMember> m_AllPartyHUDMembers;
	private Widget m_Parent;
	
	void ExpansionPartyHud(Widget parent)
	{
		m_Parent =parent;
		
		if (!m_PartyHUDController)
			m_PartyHUDController = ExpansionPartyHudController.Cast(GetController());		
		
		if (!m_AllPartyHUDMembers)
			m_AllPartyHUDMembers = new map<string, ref ExpansionPartyHudMember>;
		
		m_Parent.AddChild(GetLayoutRoot());
	}
	
	void ~ExpansionPartyHud()
	{
		//m_AllPartyHUDMembers.Clear();
		//m_PartyHUDController.PartyHUDMemberElements.Clear();
	}
	
	override string GetLayoutFile() 
	{
		return "DayZExpansion/Groups/GUI/layouts/expansion_party_hud_new.layout";
	}
	
	override typename GetControllerType() 
	{
		return ExpansionPartyHudController;
	}
	
	void AddMember(string playerID, string playerName)
	{
		ref ExpansionPartyHudMember newMember = new ExpansionPartyHudMember(playerID, playerName);
		m_PartyHUDController.PartyHUDMemberElements.Insert(newMember); // <-- Here it crashes?!
		m_AllPartyHUDMembers.Insert(playerID, newMember);
	}
		
	void RemoveMember(string playerID)
	{
		ExpansionPartyHudMember member; 
		m_AllPartyHUDMembers.Find(playerID, member);
		
		if (member)
		{
			int index = -1;
			index = m_PartyHUDController.PartyHUDMemberElements.Find(member);
			if (index > -1)
			{
				m_PartyHUDController.PartyHUDMemberElements[index] = NULL;
				m_PartyHUDController.PartyHUDMemberElements.Remove(index);
				
				delete m_PartyHUDController.PartyHUDMemberElements[index];
			}
			
			m_AllPartyHUDMembers.Remove(playerID);
		}
	}
	
	void UpdateMembers(map<string, string> members)
	{
		array<string> currentMembers = new array<string>;
		foreach (string currentMemberID, ExpansionPartyHudMember currentMember: m_AllPartyHUDMembers)
		{
			bool isStillMember = false;
			if (members.Contains(currentMemberID))
			{
				isStillMember = true;
				currentMembers.Insert(currentMemberID);
			}
			
			if (!isStillMember || !IsMemberOnline(currentMemberID))
			{
				int index = -1;
				index = m_PartyHUDController.PartyHUDMemberElements.Find(currentMember);
				if (index > -1)
				{
					RemoveMember(currentMemberID);
				}
			}
		}
		
		foreach (string playerID, string playerName: members)
		{
			int memberIndex = -1;
			memberIndex = currentMembers.Find(playerID);
			if (memberIndex == -1 && playerID != GetGame().GetPlayer().GetIdentity().GetId() && IsMemberOnline(playerID))
			{
				AddMember(playerID, playerName);
			}
		}
	}
	
	bool IsMemberOnline(string uid)
	{
		foreach (SyncPlayer player : ClientData.m_PlayerList.m_PlayerList)
		{
			if (player.m_RUID == uid)
			{
				return true;
			}
		}
		
		return false;
	}
	
	void ClearMembers()
	{
		if (!m_PartyHUDController.PartyHUDMemberElements || m_PartyHUDController.PartyHUDMemberElements.Count() == 0)
			return;
		
		for (int i = 0; i < m_PartyHUDController.PartyHUDMemberElements.Count(); i++)
		{
			ExpansionPartyHudMember member = m_PartyHUDController.PartyHUDMemberElements[i];
			if (member)
			{
				member = NULL;
				delete member;
			}
		}
		
		m_PartyHUDController.PartyHUDMemberElements.Clear();
		m_PartyHUDController.NotifyPropertyChanged("PartyHUDMemberElements");
		m_AllPartyHUDMembers.Clear();
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