/**
 * ExpansionIngameHudPartyMember.c
 *
 * DayZ Expansion Mod
 * www.dayzexpansion.com
 * © 2020 DayZ Expansion Mod Team
 *
 * This work is licensed under the Creative Commons Attribution-NonCommercial-NoDerivatives 4.0 International License. 
 * To view a copy of this license, visit http://creativecommons.org/licenses/by-nc-nd/4.0/.
 *
*/

class ExpansionIngameHudPartyMember: ExpansionScriptView
{
	ref ExpansionIngameHud m_ExpansionHUD;
	ref ExpansionIngameHudPartyMemberController m_PartyMemberController;
	ref SyncPlayer m_SyncedPlayer;
	PlayerPreviewWidget PlayerFace;
	ProgressBarWidget PlayerHealth;
	
	int m_CurrentHealth;
	int m_CurrentBlood;
	
	void ExpansionIngameHudPartyMember(ExpansionIngameHud expansionHUD, ref SyncPlayer syncPlayer)
	{
		m_ExpansionHUD = expansionHUD;
		m_SyncedPlayer = syncPlayer;
		
		if (!m_PartyMemberController)
			m_PartyMemberController = ExpansionIngameHudPartyMemberController.Cast(GetController());
		
		ExpansionMonitorModule monitorModule = ExpansionMonitorModule.Cast(GetModuleManager().GetModule(ExpansionMonitorModule));
		if (monitorModule)
		{
			monitorModule.m_StatsInvoker.Insert(this.OnDataRecived);
			monitorModule.RequestPlayerStats(m_SyncedPlayer.m_RUID);
		}
		
		SetView();
	}
	
	void ~ExpansionIngameHudPartyMember()
	{
		ExpansionMonitorModule monitorModule = ExpansionMonitorModule.Cast(GetModuleManager().GetModule(ExpansionMonitorModule));
		if (monitorModule)
		{
			monitorModule.m_StatsInvoker.Remove(this.OnDataRecived);
		}
	}
	
	override string GetLayoutFile() 
	{
		return "DayZExpansion/GUI/layouts/hud/expansion_hud_party_member.layout";
	}
	
	override typename GetControllerType() 
	{
		return ExpansionIngameHudPartyMemberController;
	}
	
	void SetView()
	{
		if (m_PartyMemberController)
		{
			m_PartyMemberController.PlayerName = m_SyncedPlayer.m_PlayerName;
			m_PartyMemberController.NotifyPropertyChanged("PlayerName");
			
			m_ExpansionHUD.GetPartyMembersSpacer().AddChild(GetLayoutRoot(), true);
		}
	}
	
	void OnDataRecived(ref ExpansionSyncedPlayerStats player_stats)
	{
		SetHealth(player_stats);
	}
	
	void SetHealth(ref ExpansionSyncedPlayerStats player_stats)
	{
		if (m_PartyMemberController)
		{
			if (m_CurrentHealth != player_stats.m_Health)
			{
				m_CurrentHealth = player_stats.m_Health;
				
				m_PartyMemberController.PlayerHealth = m_CurrentHealth;
				m_PartyMemberController.NotifyPropertyChanged("PlayerHealth");
						
				m_PartyMemberController.PlayerHealthVal = m_CurrentHealth.ToString() + "%";
				m_PartyMemberController.NotifyPropertyChanged("PlayerHealthVal");
				
				int color;
				if (player_stats.m_Health >= 100)
				{
					color = ARGB(255,39,174,96);
				}
				else if (player_stats.m_Health >= 70)
				{
					color = ARGB(255,46,204,113);
				}
				else if (player_stats.m_Health >= 50)
				{
					color = ARGB(255,241,196,15);
				}
				else if (player_stats.m_Health >= 30)
				{
					color = ARGB(255,211,84,0);
				}
				else if (player_stats.m_Health >= 10)
				{
					color = ARGB(255,192,57,43);
				}
						
				PlayerHealth.SetColor(color);
			}
			
			if (m_CurrentBlood != player_stats.m_Blood)
			{
				m_CurrentBlood = player_stats.m_Blood;
				
				m_PartyMemberController.PlayerBlood = m_CurrentBlood;
				m_PartyMemberController.NotifyPropertyChanged("PlayerBlood");
				
				m_PartyMemberController.PlayerBloodVal = m_CurrentBlood.ToString() + "%";
				m_PartyMemberController.NotifyPropertyChanged("PlayerBloodVal");
			}
		}
	}
	
	/*void SetRankIcon(ref ExpansionSyncedPlayerStats player_stats)
	{
		ExpansionPartyModule partyModule = ExpansionPartyModule.Cast(GetModuleManager().GetModule(ExpansionPartyModule));
		if (partyModule)
		{
			ref ExpansionPartyPlayerData playerData = partyModule.GetParty().GetPlayer(m_SyncedPlayer.m_RUID);
			if (playerData)
			{
				string icon;
				if (partyModule.GetParty().GetOwnerUID() == playerData.UID)
				{
					icon = ExpansionIcons.GetPath("Star");
				}
				else if (playerData.Promoted && partyModule.GetParty().GetOwnerUID() != playerData.UID)
				{
					icon = ExpansionIcons.GetPath("Ellipse");
				}
				else
				{
					icon = ExpansionIcons.GetPath("Square");
				}
				
				m_PartyMemberController.PlayerRangIcon = icon;
				m_PartyMemberController.NotifyPropertyChanged("PlayerRangIcon");
			}
		}
	}*/
	
	override float GetUpdateTickRate()
	{
		return 1.0;
	}
	
	override void Update()
	{
		ExpansionMonitorModule monitorModule = ExpansionMonitorModule.Cast(GetModuleManager().GetModule(ExpansionMonitorModule));
		if (monitorModule)
		{
			monitorModule.RequestPlayerStats(m_SyncedPlayer.m_RUID);
		}
	}
}

class ExpansionIngameHudPartyMemberController: ExpansionViewController
{
	Object PlayerObject;
	int PlayerHealth;
	string PlayerHealthVal;
	int PlayerBlood;
	string PlayerBloodVal;
	string PlayerName;
	string PlayerRangIcon;
}