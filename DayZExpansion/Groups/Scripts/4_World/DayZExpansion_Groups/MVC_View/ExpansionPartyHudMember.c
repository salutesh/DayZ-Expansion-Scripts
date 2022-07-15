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
	protected ref ExpansionPartyHudMemberController m_PartyMemberController;
	protected ProgressBarWidget PlayerHealth;
	protected Widget PlayerBloodPanel;
	protected Widget PlayerStates;
	protected Widget PlayerStances;
	
	protected ImageWidget Bones;
	protected ImageWidget Sick;
	protected ImageWidget Poisoned;
	protected ImageWidget Bleeding;
	protected TextWidget BleedingCount;

	protected ImageWidget StanceProne;
	protected ImageWidget StanceCrouch;
	protected ImageWidget StanceStand;
	protected ImageWidget StanceStandWalk;
	protected ImageWidget StanceCar;
	protected ImageWidget StanceHeli;
	protected ImageWidget StanceBoat;
	protected ImageWidget StanceDead;
	protected ImageWidget StanceUncon;
	
	protected int m_CurrentHealth;
	protected int m_CurrentBlood;
	
	protected string m_PlayerPlainID;
	protected string m_PlayerName;
	
	protected ref Timer m_UpdateTimer;
	
	void ExpansionPartyHudMember(string playerPlainID, string playerName)
	{
		EXTrace.Start(EXTrace.PLAYER_MONITOR, this, playerPlainID, playerName);

		m_PlayerPlainID = playerPlainID;
		m_PlayerName = playerName;
		
		m_PartyMemberController = ExpansionPartyHudMemberController.Cast(GetController());
		
#ifdef EXPANSIONMONITORMODULE
		ExpansionMonitorModule monitorModule = ExpansionMonitorModule.Cast(CF_ModuleCoreManager.Get(ExpansionMonitorModule));
		if (monitorModule)
		{
			monitorModule.m_StatsInvoker.Insert(OnDataRecived);
			monitorModule.m_StatesInvoker.Insert(OnStateDataRecived);
		}
#endif
		
		CreateUpdateTimer();
		
		SetView();
	}
	
	void ~ExpansionPartyHudMember()
	{
#ifdef EXPANSIONMONITORMODULE
		if (!GetExpansionSettings().IsLoaded(ExpansionPartySettings))
			return;

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
		m_PartyMemberController.PlayerName = m_PlayerName;
		m_PartyMemberController.NotifyPropertyChanged("PlayerName");
	}
	
	void OnDataRecived( ExpansionSyncedPlayerStats player_stats)
	{
		EXTrace.Start(EXTrace.PLAYER_MONITOR, this, player_stats.m_PlainID);

		if (m_PlayerPlainID != player_stats.m_PlainID) return;

		SetStats(player_stats);
	}
	
	void OnStateDataRecived( ExpansionSyncedPlayerStates player_states)
	{
		EXTrace.Start(EXTrace.PLAYER_MONITOR, this, player_states.m_PlainID);

		if (m_PlayerPlainID != player_states.m_PlainID) 
			return;
		
		SetStates(player_states);
	}
	
	void SetStats( ExpansionSyncedPlayerStats player_stats)
	{
		PlayerBloodPanel.Show(GetExpansionSettings().GetParty().ShowHUDMemberBlood);

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
		PlayerStates.Show(GetExpansionSettings().GetParty().ShowHUDMemberStates);
		PlayerStances.Show(GetExpansionSettings().GetParty().ShowHUDMemberStance);

		if (GetExpansionSettings().GetParty().ShowHUDMemberStates)
		{
			Bones = ImageWidget.Cast(GetLayoutRoot().FindAnyWidget("Bones"));
			if (Bones && player_states.m_Bones > 0)
			{
				Bones.Show(true);
			}
			else if (Bones && player_states.m_Bones <= 0)
			{
				Bones.Show(false);
			}
	
			Sick = ImageWidget.Cast(GetLayoutRoot().FindAnyWidget("Sick"));
			if (Sick && (player_states.m_Sick > 0 || player_states.m_Cholera > 0 || player_states.m_Infection > 0 || player_states.m_Influenza > 0 || player_states.m_Salmonella > 0))
			{
				Sick.Show(true);
			}
			else if (Sick && (player_states.m_Sick <= 0 || player_states.m_Cholera <= 0 || player_states.m_Infection <= 0 || player_states.m_Influenza <= 0 || player_states.m_Salmonella <= 0))
			{
				Sick.Show(false);
			}
		
			Poisoned = ImageWidget.Cast(GetLayoutRoot().FindAnyWidget("Poisoned"));
			if (Poisoned && player_states.m_Poison > 0)
			{
				Poisoned.Show(true);
			}
			else if (Poisoned && player_states.m_Poison <= 0)
			{
				Poisoned.Show(false);
			}
			
			Bleeding = ImageWidget.Cast(GetLayoutRoot().FindAnyWidget("Bleeding"));
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
	
	void UpdateStance(ExpansionPlayerStanceStatus stance)
	{
		StanceStand = ImageWidget.Cast(GetLayoutRoot().FindAnyWidget("StanceStand"));
		StanceStandWalk = ImageWidget.Cast(GetLayoutRoot().FindAnyWidget("StanceStandWalk"));
		StanceCrouch = ImageWidget.Cast(GetLayoutRoot().FindAnyWidget("StanceCrouch"));
		StanceProne = ImageWidget.Cast(GetLayoutRoot().FindAnyWidget("StanceProne"));
		StanceCar = ImageWidget.Cast(GetLayoutRoot().FindAnyWidget("StanceCar"));
		StanceHeli = ImageWidget.Cast(GetLayoutRoot().FindAnyWidget("StanceHeli"));
		StanceBoat = ImageWidget.Cast(GetLayoutRoot().FindAnyWidget("StanceBoat"));
		StanceDead = ImageWidget.Cast(GetLayoutRoot().FindAnyWidget("StanceDead"));
		StanceUncon = ImageWidget.Cast(GetLayoutRoot().FindAnyWidget("StanceUncon"));
			
		switch (stance)
		{
		case ExpansionPlayerStanceStatus.STAND:
			StanceStand.Show(true);
			StanceStandWalk.Show(false);
			StanceCrouch.Show(false);
			StanceProne.Show(false);
			StanceCar.Show(false);
			StanceHeli.Show(false);
			StanceBoat.Show(false);
			StanceDead.Show(false);
			StanceUncon.Show(false);
			break;
		case ExpansionPlayerStanceStatus.CROUCH:
			StanceStand.Show(false);
			StanceStandWalk.Show(false);
			StanceCrouch.Show(true);
			StanceProne.Show(false);
			StanceCar.Show(false);
			StanceHeli.Show(false);
			StanceBoat.Show(false);
			StanceDead.Show(false);
			StanceUncon.Show(false);
			break;
		case ExpansionPlayerStanceStatus.PRONE:
			StanceStand.Show(false);
			StanceStandWalk.Show(false);
			StanceCrouch.Show(false);
			StanceProne.Show(true);
			StanceCar.Show(false);
			StanceHeli.Show(false);
			StanceBoat.Show(false);
			StanceDead.Show(false);
			StanceUncon.Show(false);
			break;
		case ExpansionPlayerStanceStatus.CAR:
			StanceStand.Show(false);
			StanceStandWalk.Show(false);
			StanceCrouch.Show(false);
			StanceProne.Show(false);
			StanceCar.Show(true);
			StanceHeli.Show(false);
			StanceBoat.Show(false);
			StanceDead.Show(false);
			StanceUncon.Show(false);
			break;
		case ExpansionPlayerStanceStatus.HELICOPTER:
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
			break;
		case ExpansionPlayerStanceStatus.BOAT:
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
			break;
		case ExpansionPlayerStanceStatus.DEAD:
			StanceStand.Show(false);
			StanceStandWalk.Show(false);
			StanceCrouch.Show(false);
			StanceProne.Show(false);
			StanceCar.Show(false);
			StanceHeli.Show(false);
			StanceBoat.Show(false);
			StanceDead.Show(true);
			StanceUncon.Show(false);
			break;
		case ExpansionPlayerStanceStatus.UNCONSCIOUS:
			StanceStand.Show(false);
			StanceStandWalk.Show(false);
			StanceCrouch.Show(false);
			StanceProne.Show(false);
			StanceCar.Show(false);
			StanceHeli.Show(false);
			StanceBoat.Show(false);
			StanceDead.Show(false);
			StanceUncon.Show(true);
			break;
		}
	}
		
	float GetUpdateTickRate()
	{
		return 1.0;
	}
	
	void Update()
	{
#ifdef EXPANSIONMONITORMODULE
		if (!GetExpansionSettings().IsLoaded(ExpansionPartySettings))
			return;

		ExpansionMonitorModule monitorModule = ExpansionMonitorModule.Cast(CF_ModuleCoreManager.Get(ExpansionMonitorModule));
		if (monitorModule)
		{
			if (GetExpansionSettings().GetParty().ShowHUDMemberStates || GetExpansionSettings().GetParty().ShowHUDMemberStance)
				monitorModule.RequestPlayerStatsAndStates(m_PlayerPlainID);
			else
				monitorModule.RequestPlayerStats(m_PlayerPlainID);
		}
#endif
	}
	
	void CreateUpdateTimer()
	{
		EXTrace.Start(EXTrace.PLAYER_MONITOR, this);

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