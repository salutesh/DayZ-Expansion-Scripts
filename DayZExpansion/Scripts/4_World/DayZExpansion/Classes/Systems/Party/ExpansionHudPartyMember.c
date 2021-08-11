/**
 * ExpansionPartyHudMember.c
 *
 * DayZ Expansion Mod
 * www.dayzexpansion.com
 * © 2021 DayZ Expansion Mod Team
 *
 * This work is licensed under the Creative Commons Attribution-NonCommercial-NoDerivatives 4.0 International License. 
 * To view a copy of this license, visit http://creativecommons.org/licenses/by-nc-nd/4.0/.
 *
*/

#ifdef EXPANSIONMOD_PARTYHUD_ENABLE
class ExpansionPartyHud extends ExpansionScriptViewBase
{
	ref ExpansionPartyHudController m_PartyHUDController;
	private static autoptr map<string, ref ExpansionPartyHudMember> m_AllPartyHUDMembers = new map<string, ref ExpansionPartyHudMember>;
	
	void ExpansionPartyHud()
	{	
		if (!m_PartyHUDController)
			m_PartyHUDController = ExpansionPartyHudController.Cast(GetController());		
	}
	
	void ~ExpansionPartyHud()
	{
		m_AllPartyHUDMembers.Clear();
		//m_PartyHUDController.PartyHUDMemberElements.Clear();
	}
	
	override string GetLayoutFile() 
	{
		return "DayZExpansion/GUI/layouts/hud/expansion_party_hud_new.layout";
	}
	
	override typename GetControllerType() 
	{
		return ExpansionPartyHudController;
	}
	
	void AddMember(string playerID, string playerName)
	{	
		Print("ExpansionPartyHud::AddMember - Start");
				
		Print("ExpansionPartyHud::AddMember - Step 1");
		
		ref ExpansionPartyHudMember newMember = new ExpansionPartyHudMember(playerID, playerName);
		
		Print("ExpansionPartyHud::AddMember - Step 2");
		
		m_PartyHUDController.PartyHUDMemberElements.Insert(newMember); // <-- Here it crashes?!
		
		Print("ExpansionPartyHud::AddMember - Step 3");
		
		m_AllPartyHUDMembers.Insert(playerID, newMember);
		
		Print("ExpansionPartyHud::AddMember - End");
	}
		
	void RemoveMember(string playerID)
	{
		Print("ExpansionPartyHud::RemoveMember - Start");
		
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
		
		Print("ExpansionPartyHud::RemoveMember - End");
	}
	
	void UpdateMembers(map<string, string> members)
	{
		Print("ExpansionPartyHud::UpdateMembers - Start");
		Print("ExpansionPartyHud::UpdateMembers - Members count: " + members.Count());
		
		array<string> currentMembers = new array<string>;
		foreach (string currentMemberID, ExpansionPartyHudMember currentMember: m_AllPartyHUDMembers)
		{
			Print("ExpansionPartyHud::UpdateMembers - Step 1");
			bool isStillMember = false;
			bool isNewMember = false;
			
			if (members.Contains(currentMemberID))
			{
				Print("ExpansionPartyHud::UpdateMembers - Step 1 - Is still member!");
				isStillMember = true;
				currentMembers.Insert(currentMemberID);
			}
					
			if (!isStillMember)
			{
				Print("ExpansionPartyHud::UpdateMembers - Step 1 - Is no longer member!");
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
			Print("ExpansionPartyHud::UpdateMembers - Step 2");
			int memberIndex = -1;
			memberIndex = currentMembers.Find(playerID);
			if (memberIndex == -1)
			{
				AddMember(playerID, playerName);
			}
		}
		Print("ExpansionPartyHud::UpdateMembers - End");
	}
	
	void ClearMembers()
	{
		Print("ExpansionPartyHud::ClearMembers - Start");
		
		if (!m_PartyHUDController.PartyHUDMemberElements || m_PartyHUDController.PartyHUDMemberElements.Count() == 0)
			return;
		
		/*foreach (ExpansionPartyHudMember member: m_PartyHUDController.PartyHUDMemberElements)
		{
			member = NULL;
			delete member;
		}*/
		
		m_PartyHUDController.PartyHUDMemberElements.Clear();
		m_AllPartyHUDMembers.Clear();
		
		Print("ExpansionPartyHud::ClearMembers - End");
	}
	
	bool HasHUDMembers()
	{
		return (m_PartyHUDController.PartyHUDMemberElements.Count() > 0);
	}
}

class ExpansionPartyHudController: ExpansionViewController
{
	ref ObservableCollection<ref ExpansionPartyHudMember> PartyHUDMemberElements = new ObservableCollection<ref ExpansionPartyHudMember>(this);
}

class ExpansionPartyHudMember: ExpansionScriptViewBase
{
	ref ExpansionPartyHudMemberController m_PartyMemberController;
	ProgressBarWidget PlayerHealth;
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
	
	int m_CurrentHealth;
	int m_CurrentBlood;
	
	string m_PlayerID;
	string m_PlayerName;
	
	protected ref Timer m_UpdateTimer;
	
	void ExpansionPartyHudMember(string playerID, string playerName)
	{	
		EXLogPrint("ExpansionPartyHudMember::ExpansionPartyHudMember - Start");
		m_PlayerID = playerID;
		m_PlayerName = playerName;
			
		if (!m_PartyMemberController)
			m_PartyMemberController = ExpansionPartyHudMemberController.Cast(GetController());
		
		ExpansionMonitorModule monitorModule = ExpansionMonitorModule.Cast(GetModuleManager().GetModule(ExpansionMonitorModule));
		if (monitorModule)
		{
			monitorModule.m_StatsInvoker.Insert(OnDataRecived);
			monitorModule.m_StatesInvoker.Insert(OnStateDataRecived);
		}
		
		CreateUpdateTimer();
		
		SetView();
		
		EXLogPrint("ExpansionPartyHudMember::ExpansionPartyHudMember - End");
	}
	
	void ~ExpansionPartyHudMember()
	{
		EXLogPrint("ExpansionPartyHudMember::~ExpansionPartyHudMember - Start");
		
		ExpansionMonitorModule monitorModule = ExpansionMonitorModule.Cast(GetModuleManager().GetModule(ExpansionMonitorModule));
		if (monitorModule)
		{
			monitorModule.m_StatsInvoker.Remove(OnDataRecived);
			monitorModule.m_StatesInvoker.Remove(OnStateDataRecived);
		}
		
		DestroyUpdateTimer();
		
		EXLogPrint("ExpansionPartyHudMember::~ExpansionPartyHudMember - End");
	}
	
	override string GetLayoutFile() 
	{
		return "DayZExpansion/GUI/layouts/hud/expansion_party_hud_member.layout";
	}
	
	override typename GetControllerType() 
	{
		return ExpansionPartyHudMemberController;
	}
	
	void SetView()
	{
		EXLogPrint("ExpansionPartyHudMember::SetView - Start");
		
		if (m_PartyMemberController)
		{
			m_PartyMemberController.PlayerName = m_PlayerName;
			m_PartyMemberController.NotifyPropertyChanged("PlayerName");
		}
		
		EXLogPrint("ExpansionPartyHudMember::SetView - End");
	}
	
	void OnDataRecived( ExpansionSyncedPlayerStats player_stats)
	{
		EXLogPrint("ExpansionPartyHudMember::OnDataRecived - Start");
		
		if (m_PlayerID != player_stats.m_ID) return;

		SetStats(player_stats);
		
		EXLogPrint("ExpansionPartyHudMember::OnDataRecived - End");
	}
	
	void OnStateDataRecived( ExpansionSyncedPlayerStates player_states)
	{
		EXLogPrint("ExpansionPartyHudMember::OnStateDataRecived - Start");
		
		if (m_PlayerID != player_states.m_ID) return;

		SetStates(player_states);
		
		EXLogPrint("ExpansionPartyHudMember::OnStateDataRecived - End");
	}
	
	void SetStats( ExpansionSyncedPlayerStats player_stats)
	{
		EXLogPrint("ExpansionPartyHudMember::SetStats - Start");
		
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
		
		EXLogPrint("ExpansionPartyHudMember::SetStats - End");
	}
	
	void SetStates(ExpansionSyncedPlayerStates player_states)
	{
		EXLogPrint("ExpansionPartyHudMember::SetStates - Start");
		
		if (player_states.m_Bones > 0)
		{
			Bones.Show(true);
		}
		else
		{
			Bones.Show(false);
		}
		
		if (player_states.m_Sick > 0 || player_states.m_Cholera > 0 || player_states.m_Infection > 0 || player_states.m_Influenza > 0 || player_states.m_Salmonella > 0)
		{
			Sick.Show(true);
		}
		else
		{
			Sick.Show(false);
		}
	
		if (player_states.m_Poison > 0)
		{
			Poisoned.Show(true);
		}
		else
		{
			Poisoned.Show(false);
		}
		
		if (player_states.m_Cuts > 0)
		{
			Bleeding.Show(true);
			BleedingCount.Show(true);
			BleedingCount.SetText(player_states.m_Cuts.ToString());
		}
		else
		{
			Bleeding.Show(false);
			BleedingCount.Show(false);
			BleedingCount.SetText("");
		}
		
		if (player_states.m_Stance > 0)
		{
			UpdateStance(player_states.m_Stance);
		}
		
		EXLogPrint("ExpansionPartyHudMember::SetStates - End");
	}
	
	void UpdateStance( int stance )
	{
		EXLogPrint("ExpansionPartyHudMember::UpdateStance - Start");
		
		if ( stance == 1 )
		{
			bool is_walking;
			StanceStand.Show(!is_walking);
			StanceStandWalk.Show(is_walking);
			StanceCrouch.Show(false);
			StanceProne.Show(false);
			StanceCar.Show(false);
			StanceHeli.Show(false);
			StanceBoat.Show(false);
		}
		if ( stance == 2 )
		{
			StanceStand.Show(false);
			StanceStandWalk.Show(false);
			StanceCrouch.Show(true);
			StanceProne.Show(false);
			StanceCar.Show(false);
			StanceHeli.Show(false);
			StanceBoat.Show(false);
		}
		if ( stance == 3 )
		{
			StanceStand.Show(false);
			StanceStandWalk.Show(false);
			StanceCrouch.Show(false);
			StanceProne.Show(true);
			StanceCar.Show(false);
			StanceHeli.Show(false);
			StanceBoat.Show(false);			
		}
		if ( stance == 4 )
		{
			StanceStand.Show(false);
			StanceStandWalk.Show(false);
			StanceCrouch.Show(false);
			StanceProne.Show(false);
			StanceCar.Show(true);
			StanceHeli.Show(false);
			StanceBoat.Show(false);
		}
		if ( stance == 5 )
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
		}
		if ( stance == 6 )
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
		}
		
		EXLogPrint("ExpansionPartyHudMember::UpdateStance - End");
	}
		
	float GetUpdateTickRate()
	{
		return 1.0;
	}
	
	void Update()
	{
		EXLogPrint("ExpansionPartyHudMember::Update - Start");
		
		ExpansionMonitorModule monitorModule = ExpansionMonitorModule.Cast(GetModuleManager().GetModule(ExpansionMonitorModule));
		if (monitorModule)
		{
			monitorModule.RequestPlayerStats(m_PlayerID);
			monitorModule.RequestPlayerStates(m_PlayerID);
		}
		
		EXLogPrint("ExpansionPartyHudMember::Update - End");
	}
	
	void CreateUpdateTimer()
	{
		EXLogPrint("ExpansionPartyHudMember::CreateUpdateTimer - Start");
		
		if (!m_UpdateTimer && GetUpdateTickRate() != -1)
		{
			m_UpdateTimer = new Timer(CALL_CATEGORY_GUI);
			m_UpdateTimer.Run(GetUpdateTickRate(), this, "Update", NULL, true);
		}
		
		EXLogPrint("ExpansionPartyHudMember::CreateUpdateTimer - End");
	}
	
	void DestroyUpdateTimer()
	{
		EXLogPrint("ExpansionPartyHudMember::DestroyUpdateTimer - Start");
		
		if (m_UpdateTimer)
		{
			m_UpdateTimer.Stop();
			delete m_UpdateTimer;
		}
		
		EXLogPrint("ExpansionPartyHudMember::DestroyUpdateTimer - End");
	}
}

class ExpansionPartyHudMemberController: ExpansionViewController
{
	Object PlayerObject;
	int PlayerHealth;
	string PlayerHealthVal;
	int PlayerBlood;
	string PlayerBloodVal;
	string PlayerName;
	string PlayerRangIcon;
}
#endif