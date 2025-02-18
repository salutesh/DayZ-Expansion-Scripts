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
	
	string m_PlayerID;
	protected string m_PlayerPlainID;
	protected string m_PlayerName;
	protected vector m_CurrentPosition;
	
	protected ref Timer m_UpdateTimer;
	#ifdef EXPANSIONMONITORMODULE
	protected ProgressBarWidget PlayerHealth;
	protected ImageWidget Bones;
	protected ImageWidget Sickness;
	protected ImageWidget Poisoned;
	protected ImageWidget Bleeding;
	protected TextWidget BleedingCount;
	protected ImageWidget Stance;
	protected GridSpacerWidget PlayerStates;
	protected WrapSpacerWidget PlayerStances;
	protected TextWidget PlayerDistance;
	protected Widget PlayerBloodPanel;

	protected int m_CurrentHealth;
	protected int m_CurrentBlood;
	protected ExpansionMonitorModule m_MonitorModule;
	ref ExpansionPartySettings m_Settings;
	#endif

	void ExpansionPartyHudMember(string playerID, string playerPlainID, string playerName)
	{
		#ifdef EXTRACE
		auto trace = EXTrace.Start(EXTrace.GROUPS, this, playerPlainID, playerName);
		#endif

		m_PlayerID = playerID;
		m_PlayerPlainID = playerPlainID;
		m_PlayerName = playerName;
		
		m_PartyMemberController = ExpansionPartyHudMemberController.Cast(GetController());

		#ifdef EXPANSIONMONITORMODULE
		m_Settings = GetExpansionSettings().GetParty(false);  //! We cannot gurantee at this point that settings are already received on client
		
		m_MonitorModule = ExpansionMonitorModule.Cast(CF_ModuleCoreManager.Get(ExpansionMonitorModule));
		if (m_MonitorModule)
		{
			m_MonitorModule.m_StatsInvoker.Insert(OnDataRecived);
			m_MonitorModule.m_StatesInvoker.Insert(OnStateDataRecived);
		}
		#endif

		SetView();
		CreateUpdateTimer();
	}
	
	void ~ExpansionPartyHudMember()
	{
		#ifdef EXTRACE
		auto trace = EXTrace.Start(EXTrace.GROUPS, this);
		#endif

		#ifdef EXPANSIONMONITORMODULE
		if (m_MonitorModule)
		{
			m_MonitorModule.m_StatsInvoker.Remove(OnDataRecived);
			m_MonitorModule.m_StatesInvoker.Remove(OnStateDataRecived);
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
				
		#ifdef EXPANSIONMONITORMODULE
		PlayerDistance.Show(false);
		PlayerBloodPanel.Show(false);
		PlayerStances.Show(false);
		PlayerStates.Show(false);
		
		//! IMPORTANT: Valid index for LoadImageFile is 0-7!
		Stance.LoadImageFile(0, "set:dayz_gui image:stance_stand");
		Stance.LoadImageFile(1, "set:dayz_gui image:stance_crouch");
		Stance.LoadImageFile(2, "set:dayz_gui image:stance_prone");
		Stance.LoadImageFile(3, "set:dayz_gui image:stance_car");
		Stance.LoadImageFile(4, "dayzexpansion/core/gui/icons/misc/T_Helicopter_256x256.edds");
		Stance.LoadImageFile(5, "dayzexpansion/core/gui/icons/misc/T_Boat_256x256.edds");
		Stance.LoadImageFile(6, "dayzexpansion/core/gui/icons/misc/T_HumanSkull_256x256.edds");
		Stance.LoadImageFile(7, "dayzexpansion/core/gui/icons/misc/T_Medic_Box_256x256.edds");
		#endif
	}
	
	void OnDataRecived(ExpansionSyncedPlayerStats player_stats)
	{
		#ifdef EXTRACE
		auto trace = EXTrace.Start(EXTrace.PLAYER_MONITOR, this, player_stats.m_PlainID);
		#endif

		if (m_PlayerPlainID != player_stats.m_PlainID || !player_stats.m_HasBaseStats) 
			return;
		
		SetStats(player_stats);
	}
	
	void OnStateDataRecived(ExpansionSyncedPlayerStates player_states)
	{
		#ifdef EXTRACE
		auto trace = EXTrace.Start(EXTrace.PLAYER_MONITOR, this, player_states.m_PlainID);
		#endif

		if (m_PlayerPlainID != player_states.m_PlainID) 
			return;
		
		SetStates(player_states);
	}
	
	void SetStats(ExpansionSyncedPlayerStats player_stats)
	{
		#ifdef EXPANSIONMONITORMODULE
		DayZPlayer player = GetGame().GetPlayer();
		if (m_Settings.ShowHUDMemberDistance && player)
		{
			vector playerPos = player.GetPosition();
			float distance = vector.Distance(player_stats.m_Position, playerPos);
			float round = Math.Round(distance);
			m_PartyMemberController.PlayerDistance = round.ToString() + " m";
			m_PartyMemberController.NotifyPropertyChanged("PlayerDistance");
			PlayerDistance.Show(true);
		}

		if (m_CurrentHealth != player_stats.m_Health)
		{
			m_CurrentHealth = player_stats.m_Health;
			
			m_PartyMemberController.PlayerHealth = m_CurrentHealth;
			m_PartyMemberController.NotifyPropertyChanged("PlayerHealth");
			
			int color;
			color = GetHealthColor(player_stats.m_Health);
			PlayerHealth.SetColor(color);
		}
	
		PlayerBloodPanel.Show(m_Settings.ShowHUDMemberBlood);
		if (m_Settings.ShowHUDMemberBlood && m_CurrentBlood != player_stats.m_Blood)
		{
			m_CurrentBlood = player_stats.m_Blood;
			
			m_PartyMemberController.PlayerBlood = m_CurrentBlood;
			m_PartyMemberController.NotifyPropertyChanged("PlayerBlood");
		}
		#endif
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
		#ifdef EXPANSIONMONITORMODULE
		if (m_Settings.ShowHUDMemberStates)
		{
			bool brokenBones = player_states.m_Bones;
			bool isSick;
			if (player_states.m_Cholera || player_states.m_Infection || player_states.m_Influenza || player_states.m_Salmonella)
				isSick = true;
			bool isPoisoned = player_states.m_Poison;
			bool isBleeding;
			if (player_states.m_Cuts > 0)
				isBleeding = true;
			
			Bones.Show(brokenBones);
			Sickness.Show(isSick);
			Poisoned.Show(isPoisoned);
			Bleeding.Show(isBleeding);
			BleedingCount.Show(isBleeding);
			string bleedingCount;
			
			if (isBleeding)
			{
				bleedingCount = player_states.m_Cuts.ToString();
			}
			
			BleedingCount.SetText(bleedingCount);

			bool showStatesPanel = (brokenBones || isSick || isPoisoned || isBleeding);
			PlayerStates.Show(showStatesPanel);
		}
		
		if (m_Settings.ShowHUDMemberStance)
		{
			PlayerStances.Show(true);
			UpdateStance(player_states.m_Stance);
		}
		#endif
	}
	
	void UpdateStance(ExpansionPlayerStanceStatus stance)
	{
		#ifdef EXPANSIONMONITORMODULE
		switch (stance)
		{
		case ExpansionPlayerStanceStatus.CROUCH:
			Stance.SetImage(1);
			break;
		case ExpansionPlayerStanceStatus.PRONE:
			Stance.SetImage(2);
			break;
		case ExpansionPlayerStanceStatus.CAR:
			Stance.SetImage(3);
			break;
		case ExpansionPlayerStanceStatus.HELICOPTER:
			Stance.SetImage(4);
			break;
		case ExpansionPlayerStanceStatus.BOAT:
			Stance.SetImage(5);
			break;
		case ExpansionPlayerStanceStatus.DEAD:
			Stance.SetImage(6);
			break;
		case ExpansionPlayerStanceStatus.UNCONSCIOUS:
			Stance.SetImage(7);
			break;
		default:
			Stance.SetImage(0);
			break;
		}
		#endif
	}
		
	float GetUpdateTickRate()
	{
		return 0.5;
	}
	
	void Expansion_Update()
	{
		#ifdef EXPANSIONMONITORMODULE
		if (!m_Settings.IsLoaded())
			return;

		if (m_MonitorModule && m_Settings.ShowHUDMemberStates)
		{
			if (!m_Settings.ShowHUDMemberStance)
			{
				m_MonitorModule.RequestPlayerStats(m_PlayerPlainID);
			}
			else
			{
				m_MonitorModule.RequestPlayerStatsAndStates(m_PlayerPlainID);
			}
		}
		#endif
	}
	
	void CreateUpdateTimer()
	{
		#ifdef EXTRACE
		auto trace = EXTrace.Start(EXTrace.GROUPS, this);
		#endif
		
		if (!m_UpdateTimer && GetUpdateTickRate() != -1)
		{
			m_UpdateTimer = new Timer(CALL_CATEGORY_GUI);
			m_UpdateTimer.Run(GetUpdateTickRate(), this, "Expansion_Update", NULL, true);
		}
	}
	
	void DestroyUpdateTimer()
	{
		if (m_UpdateTimer)
		{
			m_UpdateTimer.Stop();
			m_UpdateTimer = null;
		}
	}
}
#endif