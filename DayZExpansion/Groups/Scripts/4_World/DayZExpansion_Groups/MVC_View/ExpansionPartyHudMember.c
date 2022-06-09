/**
 * ExpansionPartyHudMember.c
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
class ExpansionPartyHudMember: ExpansionScriptViewBase
{
	ref ExpansionPartyHudMemberController m_PartyMemberController;
	ProgressBarWidget PlayerHealth;
	Widget PlayerBloodPanel;
	Widget PlayerStates;
	Widget PlayerStances;
	
	ImageWidget Bones;
	ImageWidget Sick;
	ImageWidget Poisoned;
	ImageWidget Bleeding;
	TextWidget BleedingCount;

	ImageWidget StanceProne;
	ImageWidget StanceCrouch;
	ImageWidget StanceStand;
	ImageWidget StanceStandWalk;
	ImageWidget StanceCar;
	ImageWidget StanceHeli;
	ImageWidget StanceBoat;
	ImageWidget StanceDead;
	ImageWidget StanceUncon;
	
	int m_CurrentHealth;
	int m_CurrentBlood;
	
	string m_PlayerID;
	string m_PlayerName;
	
	protected ref Timer m_UpdateTimer;
	
	void ExpansionPartyHudMember(string playerID, string playerName)
	{
		m_PlayerID = playerID;
		m_PlayerName = playerName;
		
		m_PartyMemberController = ExpansionPartyHudMemberController.Cast(GetController());
		
#ifdef EXPANSIONMONITORMODULE
		ExpansionMonitorModule monitorModule = ExpansionMonitorModule.Cast(CF_ModuleCoreManager.Get(ExpansionMonitorModule));
		if (monitorModule)
		{
			monitorModule.m_StatsInvoker.Insert(OnDataRecived);
			if (GetExpansionSettings().GetParty().ShowHUDMemberStates || GetExpansionSettings().GetParty().ShowHUDMemberStance)
				monitorModule.m_StatesInvoker.Insert(OnStateDataRecived);
		}
#endif
		
		CreateUpdateTimer();
		
		SetView();
	}
	
	void ~ExpansionPartyHudMember()
	{
#ifdef EXPANSIONMONITORMODULE
		ExpansionMonitorModule monitorModule = ExpansionMonitorModule.Cast(CF_ModuleCoreManager.Get(ExpansionMonitorModule));
		if (monitorModule)
		{
			monitorModule.m_StatsInvoker.Remove(OnDataRecived);
			if (GetExpansionSettings().GetParty().ShowHUDMemberStates || GetExpansionSettings().GetParty().ShowHUDMemberStance)
				monitorModule.m_StatesInvoker.Remove(OnStateDataRecived);
		}
#endif
		
		DestroyUpdateTimer();
	}
	
	override string GetLayoutFile() 
	{
		return "DayZExpansion/Groups/GUI/layouts/expansion_party_hud_member.layout";
	}
	
	override typename GetControllerType() 
	{
		return ExpansionPartyHudMemberController;
	}
	
	void SetView()
	{
		if (GetExpansionSettings().GetParty().ShowHUDMemberBlood)
			PlayerBloodPanel.Show(true);
		
		if (GetExpansionSettings().GetParty().ShowHUDMemberStates)
			PlayerStates.Show(true);
		
		if (GetExpansionSettings().GetParty().ShowHUDMemberStance)
			PlayerStances.Show(true);
		
		m_PartyMemberController.PlayerName = m_PlayerName;
		m_PartyMemberController.NotifyPropertyChanged("PlayerName");
	}
	
	void OnDataRecived( ExpansionSyncedPlayerStats player_stats)
	{
		if (m_PlayerID != player_stats.m_ID) return;

		SetStats(player_stats);
	}
	
	void OnStateDataRecived( ExpansionSyncedPlayerStates player_states)
	{
		if (m_PlayerID != player_states.m_ID) 
			return;
		
		SetStates(player_states);
	}
	
	void SetStats( ExpansionSyncedPlayerStats player_stats)
	{
		if (m_CurrentHealth != player_stats.m_Health)
		{
			m_CurrentHealth = player_stats.m_Health;
			
			m_PartyMemberController.PlayerHealth = m_CurrentHealth;
			m_PartyMemberController.NotifyPropertyChanged("PlayerHealth");
		
			m_PartyMemberController.PlayerHealthVal = m_CurrentHealth.ToString() + "%";
			m_PartyMemberController.NotifyPropertyChanged("PlayerHealthVal");
			
			int color;
			color = GetHealthColor(player_stats.m_Health);
			PlayerHealth.SetColor(color);
		}
	
		if (GetExpansionSettings().GetParty().ShowHUDMemberBlood && m_CurrentBlood != player_stats.m_Blood)
		{
			m_CurrentBlood = player_stats.m_Blood;
			
			m_PartyMemberController.PlayerBlood = m_CurrentBlood;
			m_PartyMemberController.NotifyPropertyChanged("PlayerBlood");
			
			m_PartyMemberController.PlayerBloodVal = m_CurrentBlood.ToString() + "%";
			m_PartyMemberController.NotifyPropertyChanged("PlayerBloodVal");
		}
	}
	
	int GetHealthColor(int health)
	{
		int color;
		if (health >= 100)
		{
			color = ARGB(255,39,174,96);
		}
		else if (health >= 70)
		{
			color = ARGB(255,46,204,113);
		}
		else if (health >= 50)
		{
			color = ARGB(255,241,196,15);
		}
		else if (health >= 30)
		{
			color = ARGB(255,211,84,0);
		}
		else if (health >= 10)
		{
			color = ARGB(255,192,57,43);
		}
		
		return color;
	}
	
	void SetStates(ExpansionSyncedPlayerStates player_states)
	{
		if (GetExpansionSettings().GetParty().ShowHUDMemberStates)
		{
			if (Bones && player_states.m_Bones > 0)
			{
				Bones.Show(true);
			}
			else if (Bones && player_states.m_Bones <= 0)
			{
				Bones.Show(false);
			}
	
			if (Sick && (player_states.m_Sick > 0 || player_states.m_Cholera > 0 || player_states.m_Infection > 0 || player_states.m_Influenza > 0 || player_states.m_Salmonella > 0))
			{
				Sick.Show(true);
			}
			else if (Sick && (player_states.m_Sick <= 0 || player_states.m_Cholera <= 0 || player_states.m_Infection <= 0 || player_states.m_Influenza <= 0 || player_states.m_Salmonella <= 0))
			{
				Sick.Show(false);
			}
		
			if (Poisoned && player_states.m_Poison > 0)
			{
				Poisoned.Show(true);
			}
			else if (Poisoned && player_states.m_Poison <= 0)
			{
				Poisoned.Show(false);
			}
			
			if (Bleeding && BleedingCount && player_states.m_Cuts > 0)
			{
				Bleeding.Show(true);
				BleedingCount.Show(true);
				BleedingCount.SetText(player_states.m_Cuts.ToString());
			}
			else if (Bleeding && BleedingCount && player_states.m_Cuts <= 0)
			{
				Bleeding.Show(false);
				BleedingCount.Show(false);
				BleedingCount.SetText("");
			}
		}
		
		if (GetExpansionSettings().GetParty().ShowHUDMemberStance && player_states.m_Stance > 0)
		{
			UpdateStance(player_states.m_Stance);
		}
	}
	
	void UpdateStance(int stance)
	{		
		if (stance == 1)
		{
			StanceStand.Show(true);
			StanceStandWalk.Show(false);
			StanceCrouch.Show(false);
			StanceProne.Show(false);
			StanceCar.Show(false);
			StanceHeli.Show(false);
			StanceBoat.Show(false);
			StanceDead.Show(false);
			StanceUncon.Show(false);
		}
		else if (stance == 2)
		{
			StanceStand.Show(false);
			StanceStandWalk.Show(false);
			StanceCrouch.Show(true);
			StanceProne.Show(false);
			StanceCar.Show(false);
			StanceHeli.Show(false);
			StanceBoat.Show(false);
			StanceDead.Show(false);
			StanceUncon.Show(false);
		}
		else if (stance == 3)
		{
			StanceStand.Show(false);
			StanceStandWalk.Show(false);
			StanceCrouch.Show(false);
			StanceProne.Show(true);
			StanceCar.Show(false);
			StanceHeli.Show(false);
			StanceBoat.Show(false);
			StanceDead.Show(false);
			StanceUncon.Show(false);
		}
		else if (stance == 4)
		{
			StanceStand.Show(false);
			StanceStandWalk.Show(false);
			StanceCrouch.Show(false);
			StanceProne.Show(false);
			StanceCar.Show(true);
			StanceHeli.Show(false);
			StanceBoat.Show(false);
			StanceDead.Show(false);
			StanceUncon.Show(false);
		}
		else if (stance == 5)
		{
			StanceStand.Show(false);
			StanceStandWalk.Show(false);
			StanceCrouch.Show(false);
			StanceProne.Show(false);
			StanceCar.Show(false);
			StanceHeli.Show(true);
			StanceHeli.LoadImageFile(0, ExpansionIcons.GetPath("Helicopter"));
			StanceHeli.SetImage(0);
			StanceBoat.Show(false);
			StanceDead.Show(false);
			StanceUncon.Show(false);
		}
		else if (stance == 6)
		{
			StanceStand.Show(false);
			StanceStandWalk.Show(false);
			StanceCrouch.Show(false);
			StanceProne.Show(false);
			StanceCar.Show(false);
			StanceHeli.Show(false);
			StanceBoat.Show(true);
			StanceBoat.LoadImageFile(0, ExpansionIcons.GetPath("Boat"));
			StanceBoat.SetImage(0);
			StanceDead.Show(false);
			StanceUncon.Show(false);
		}
		else if (stance == 7)
		{
			StanceStand.Show(false);
			StanceStandWalk.Show(false);
			StanceCrouch.Show(false);
			StanceProne.Show(false);
			StanceCar.Show(false);
			StanceHeli.Show(false);
			StanceBoat.Show(false);
			StanceDead.Show(true);
			StanceUncon.Show(false);
		}
		else if (stance == 8)
		{
			StanceStand.Show(false);
			StanceStandWalk.Show(false);
			StanceCrouch.Show(false);
			StanceProne.Show(false);
			StanceCar.Show(false);
			StanceHeli.Show(false);
			StanceBoat.Show(false);
			StanceDead.Show(false);
			StanceUncon.Show(true);
		}
	}
		
	float GetUpdateTickRate()
	{
		return 1.0;
	}
	
	void Update()
	{				
#ifdef EXPANSIONMONITORMODULE
		ExpansionMonitorModule monitorModule = ExpansionMonitorModule.Cast(CF_ModuleCoreManager.Get(ExpansionMonitorModule));
		if (monitorModule)
		{
			monitorModule.RequestPlayerStats(m_PlayerID);
			monitorModule.RequestPlayerStates(m_PlayerID);
		}
#endif
	}
	
	void CreateUpdateTimer()
	{
		if (!m_UpdateTimer && GetUpdateTickRate() != -1)
		{
			m_UpdateTimer = new Timer(CALL_CATEGORY_GUI);
			m_UpdateTimer.Run(GetUpdateTickRate(), this, "Update", NULL, true);
		}
	}
	
	void DestroyUpdateTimer()
	{
		if (m_UpdateTimer)
		{
			m_UpdateTimer.Stop();
			delete m_UpdateTimer;
		}
	}
};
#endif