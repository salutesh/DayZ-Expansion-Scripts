/**
 * ExpansionBookMenuTabPlayerProfile.c
 *
 * DayZ Expansion Mod
 * www.dayzexpansion.com
 * © 2022 DayZ Expansion Mod Team
 *
 * This work is licensed under the Creative Commons Attribution-NonCommercial-NoDerivatives 4.0 International License.
 * To view a copy of this license, visit http://creativecommons.org/licenses/by-nc-nd/4.0/.
 *
*/

class ExpansionBookMenuTabPlayerProfile: ExpansionBookMenuTabBase
{
	ref ExpansionBookMenuTabPlayerProfileController m_PlayerProfileController;

	Widget hardline_reputation_spacer;

	Widget hab_suicides_spacer;
	Widget hab_affinity_spacer;
	Widget hab_level_spacer;
	Widget hab_humanity_spacer;
	TextWidget hab_humanity_label;
	Widget hab_medic_spacer;
	Widget hab_raid_spacer;
	Widget profile_player_faction_spacer;
	Widget profile_blood_spacer;
	
	vector m_PlayerPreviewOrientation = vector.Zero;
	int m_PlayerPreviewRotationX = 0;
	int m_PlayerPreviewRotationY = 0;
	PlayerPreviewWidget player_preview;

	protected bool m_MouseButtonIsDown;

	bool m_ShowHaBStats;
	bool m_Updated;

	bool m_ShowHardlineStats;
	bool m_ShowPlayerFaction;

	void ExpansionBookMenuTabPlayerProfile(ExpansionBookMenu book_menu)
	{
		if (!m_PlayerProfileController)
			m_PlayerProfileController = ExpansionBookMenuTabPlayerProfileController.Cast(GetController());

	#ifdef EXPANSIONMONITORMODULE
		ExpansionMonitorModule monitorModule = ExpansionMonitorModule.Cast(CF_ModuleCoreManager.Get(ExpansionMonitorModule));
		if (!monitorModule)
			return;

		monitorModule.m_StatsInvoker.Insert(SetStats);
	#endif

	#ifdef HEROESANDBANDITSMOD
		//! https://feedback.bistudio.com/T173348
		if (g_HeroesAndBanditsPlayer && GetExpansionSettings().GetBook().ShowHaBStats)
			m_ShowHaBStats = true;
		else
			m_ShowHaBStats = false;
	#endif

	#ifdef EXPANSIONMODHARDLINE
		//! https://feedback.bistudio.com/T173348
		if (GetExpansionSettings().GetHardline(false).IsLoaded() && GetExpansionSettings().GetHardline().UseReputation)
			m_ShowHardlineStats = true;
		else
			m_ShowHardlineStats = false;
	#endif
	#ifdef EXPANSIONMODAI
		m_ShowPlayerFaction = GetExpansionSettings().GetBook().ShowPlayerFaction;
	#endif

		UpdateUIElements();
	}

	void ~ExpansionBookMenuTabPlayerProfile()
	{
#ifdef EXPANSIONMONITORMODULE
		ExpansionMonitorModule monitorModule = ExpansionMonitorModule.Cast(CF_ModuleCoreManager.Get(ExpansionMonitorModule));
		if (!monitorModule)
			return;

		monitorModule.m_StatsInvoker.Remove(SetStats);
#endif
	}

	void SetStats(ExpansionSyncedPlayerStats stats)
	{
		if (!stats || stats.m_PlainID != string.Empty || !stats.m_HasBaseStats || !stats.m_HasRegisteredStats)
			return;

		PlayerBase player = PlayerBase.Cast(GetGame().GetPlayer());

		stats.Acquire(player);

		//! Profile stats
		m_PlayerProfileController.ProfileTimePlayed = ExpansionStatic.GetTimeString(stats.m_Playtime);
		m_PlayerProfileController.ProfileLongestShot = ExpansionStatic.GetDistanceString(stats.m_LongestShot);
		m_PlayerProfileController.ProfilePlayerKills = ExpansionStatic.GetValueString(stats.m_PlayersKilled) + " Kills";
		m_PlayerProfileController.ProfileZombieKills = ExpansionStatic.GetValueString(stats.m_InfectedKilled) + " Kills";
		m_PlayerProfileController.ProfileAnimalKills = ExpansionStatic.GetValueString(stats.m_AnimalsKilled) + " Kills";

	#ifdef HEROESANDBANDITSMOD
		if (m_ShowHaBStats)
		{
			m_PlayerProfileController.HaB_Affinity = g_HeroesAndBanditsPlayer.getAffinity().DisplayName;
			m_PlayerProfileController.HaB_Level = g_HeroesAndBanditsPlayer.getLevel().Name;
			if ( GetHeroesAndBanditsSettings().Mode != 1 )
			{
				m_PlayerProfileController.HaB_Humanity = ExpansionStatic.GetValueString(g_HeroesAndBanditsPlayer.getHumanity());
			}
			else
			{
				m_PlayerProfileController.HaB_Humanity = ExpansionStatic.GetValueString(g_HeroesAndBanditsPlayer.getAffinityPoints( g_HeroesAndBanditsPlayer.getAffinityName() ));
			}
			if ( !GetHeroesAndBanditsSettings().HideKillsInGUI )
				m_PlayerProfileController.ProfilePlayerKills = ExpansionStatic.GetValueString(stats.m_PlayersKilled) + " (" + ExpansionStatic.GetValueString( g_HeroesAndBanditsPlayer.getStat("Kill") ) + ") Kills";
			
			m_PlayerProfileController.ProfileZombieKills = ExpansionStatic.GetValueString(stats.m_InfectedKilled) + " (" + ExpansionStatic.GetValueString( g_HeroesAndBanditsPlayer.getStat("ZombieKill") ) + ") Kills";
			m_PlayerProfileController.ProfileAnimalKills = ExpansionStatic.GetValueString(stats.m_AnimalsKilled) + " (" + ExpansionStatic.GetValueString( g_HeroesAndBanditsPlayer.getStat("Hunt") ) + ") Kills";
			m_PlayerProfileController.HaB_Medic = ExpansionStatic.GetValueString( g_HeroesAndBanditsPlayer.getStat("Medic") );
			m_PlayerProfileController.HaB_Raid = ExpansionStatic.GetValueString( g_HeroesAndBanditsPlayer.getStat("Raid") );
			m_PlayerProfileController.HaB_Suicides = ExpansionStatic.GetValueString( g_HeroesAndBanditsPlayer.getStat("Sucide") );

			m_PlayerProfileController.NotifyPropertiesChanged({"HaB_Affinity", "HaB_Level", "HaB_Humanity", "HaB_Medic", "HaB_Raid", "HaB_Suicides"});
		}
	#endif

		m_PlayerProfileController.ProfileDistanceTraveled = ExpansionStatic.GetDistanceString(stats.m_Distance);
		m_PlayerProfileController.ProfileWeight = ExpansionStatic.GetWeightString(stats.m_Weight);

		array<string> profile_properties = {"ProfileTimePlayed", "ProfileLongestShot", "ProfilePlayerKills", "ProfileZombieKills", "ProfileAnimalKills", "ProfileDistanceTraveled", "ProfileWeight"};
		
		if (player.HasBloodTypeVisible())
		{
			string type;
			bool positive;
			m_PlayerProfileController.PlayerBloodType = BloodTypes.GetBloodTypeName(player.GetBloodType(), type, positive);
			profile_properties.Insert("PlayerBloodType");
		}
		else
		{
			profile_blood_spacer.Show(false);
		}

		m_PlayerProfileController.NotifyPropertiesChanged(profile_properties);

		//! Player preview
		if (!m_PlayerProfileController.PlayerCharacter)
		{
			m_PlayerProfileController.PlayerCharacter = GetGame().GetPlayer();
			m_PlayerProfileController.NotifyPropertyChanged("PlayerCharacter");
		}
		
		//! Player conditions
		m_PlayerProfileController.PlayerHealth = stats.m_Health;
		m_PlayerProfileController.PlayerHealthValue = stats.m_Health.ToString() + "%";
		m_PlayerProfileController.PlayerBlood = stats.m_Blood;
		m_PlayerProfileController.PlayerBloodValue = stats.m_Blood.ToString() + "%";
		m_PlayerProfileController.PlayerWater = stats.m_Water;
		m_PlayerProfileController.PlayerWaterValue = stats.m_Water.ToString() + "%";
		m_PlayerProfileController.PlayerEnergy = stats.m_Energy;
		m_PlayerProfileController.PlayerEnergyValue = stats.m_Energy.ToString() + "%";
		m_PlayerProfileController.PlayerStamina = stats.m_Stamina;
		m_PlayerProfileController.PlayerStaminaValue = stats.m_Stamina.ToString() + "%";

		array<string> player_properties = {"PlayerHealth", "PlayerHealthValue", "PlayerBlood", "PlayerBloodValue", "PlayerWater", "PlayerWaterValue", "PlayerEnergy", "PlayerEnergyValue", "PlayerStamina", "PlayerStaminaValue"};
		m_PlayerProfileController.NotifyPropertiesChanged(player_properties);

	#ifdef EXPANSIONMODHARDLINE
		if (m_ShowHardlineStats)
		{
			m_PlayerProfileController.Hardline_Reputation = player.Expansion_GetReputation().ToString();
			m_PlayerProfileController.NotifyPropertyChanged("Hardline_Reputation");
		}
	#endif

	#ifdef EXPANSIONMODAI
		if (m_ShowPlayerFaction)
		{
			string factionName = "#STR_EXPANSION_NONE";
			eAIGroup group = player.GetGroup();
			if (group)
			{
				eAIFaction playerFaction = group.GetFaction();
				if (playerFaction)
					factionName = playerFaction.GetDisplayName();
			}
		
			m_PlayerProfileController.ProfilePlayerFaction = factionName;
			m_PlayerProfileController.NotifyPropertyChanged("ProfilePlayerFaction");
		}
	#endif

		UpdateUIElements();
	}

	void UpdateUIElements()
	{
		//! Heroes and bandits
		hab_suicides_spacer.Show(m_ShowHaBStats);
		hab_affinity_spacer.Show(m_ShowHaBStats);
		hab_level_spacer.Show(m_ShowHaBStats);
		hab_humanity_spacer.Show(m_ShowHaBStats);
		hab_medic_spacer.Show(m_ShowHaBStats);
		hab_raid_spacer.Show(m_ShowHaBStats);

		//! Hardline
		hardline_reputation_spacer.Show(m_ShowHardlineStats);

		//! Factions
		profile_player_faction_spacer.Show(m_ShowPlayerFaction);
	}

	void UpdatePlayerPreviewRotation(int mouse_x, int mouse_y, bool is_dragging)
	{
		vector orientation = m_PlayerPreviewOrientation;
		orientation[1] = orientation[1] - (m_PlayerPreviewRotationX - mouse_x);
		orientation[2] = orientation[2] - (m_PlayerPreviewRotationY - mouse_y);
		player_preview.SetModelOrientation(orientation);

		if (!is_dragging)
		{
			m_PlayerPreviewOrientation = orientation;
		}
	}

	override string GetLayoutFile()
	{
		return "DayZExpansion/Book/GUI/layouts/tabs/expansion_book_tab_playerProfile.layout";
	}

	override typename GetControllerType()
	{
		return ExpansionBookMenuTabPlayerProfileController;
	}

	override string GetTabIconName()
	{
		return "Persona";
	}

	override string GetTabName()
	{
		return "#STR_EXPANSION_BOOK_STATUS_CHARACTER_TABTITLE";
	}

	override int GetTabColor()
	{
		return ARGB(255,0,0,0);
	}

	override bool OnMouseButtonDown( Widget w, int x, int y, int button )
	{
		if (w == player_preview && !m_MouseButtonIsDown)
		{
			//! For some reason, OnMouseButtonDown gets called twice when holding down the mouse button, which fucks with player preview rotation.
			//! Set a flag so we can check whether mouse button is still down.
			m_MouseButtonIsDown = true;
			GetMousePos(m_PlayerPreviewRotationX, m_PlayerPreviewRotationY);
			GetGame().GetDragQueue().Call(this, "UpdatePlayerPreviewRotation");
		}

		return super.OnMouseButtonDown(w, x, y, button);
	}

	override bool OnMouseButtonUp( Widget w, int x, int y, int button )
	{
		m_MouseButtonIsDown = false;

		return super.OnMouseButtonUp(w, x, y, button);
	}

	override bool CanShow()
	{
		return GetExpansionSettings().GetBook().EnableStatusTab;
	}

	override bool IsParentTab()
	{
		return true;
	}

	override float GetUpdateTickRate()
	{
		if (!IsVisible())
			return -1;

		return 1.0;
	}

	override void OnShow()
	{
		super.OnShow();

		Expansion_Update();  //! 1st update immediately after first shown
		m_Updated = true;

		if (!GetExpansionSettings().GetMonitoring().Enabled)
			return;

		CreateUpdateTimer();
	}

	override void Expansion_Update()
	{
	#ifdef EXPANSIONMONITORMODULE
		if (m_Updated)
		{
			m_Updated = false;
			return
		}

		if (!IsVisible())
			return;

		ExpansionMonitorModule monitorModule = ExpansionMonitorModule.Cast(CF_ModuleCoreManager.Get(ExpansionMonitorModule));
		if (monitorModule)
		{
			monitorModule.RequestPlayerStats("", true);
		}
	#endif
	}
};

class ExpansionBookMenuTabPlayerProfileController: ExpansionViewController
{
	string ProfileTimePlayed;
	string ProfileLongestShot;
	string ProfilePlayerKills;
	string ProfileZombieKills;
	string ProfileAnimalKills;
	string ProfileDistanceTraveled;
	string ProfileWeight;
	string PlayerBloodType;

	DayZPlayer PlayerCharacter;
	float PlayerHealth;
	string PlayerHealthValue;
	float PlayerBlood;
	string PlayerBloodValue;
	float PlayerWater;
	string PlayerWaterValue;
	float PlayerEnergy;
	string PlayerEnergyValue;
	float PlayerStamina;
	string PlayerStaminaValue;
	
	string Hardline_Reputation;
	string ProfilePlayerFaction;

	//! Heros and Bandits
	string HaB_Affinity;
	string HaB_Level;
	string HaB_Humanity;
	string HaB_Medic;
	string HaB_Raid;
	string HaB_Suicides;
};
