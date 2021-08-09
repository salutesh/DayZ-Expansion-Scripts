/**
 * ExpansionPartyHudMember.c
 *
 * DayZ Expansion Mod
 * www.dayzexpansion.com
 * © 2020 DayZ Expansion Mod Team
 *
 * This work is licensed under the Creative Commons Attribution-NonCommercial-NoDerivatives 4.0 International License. 
 * To view a copy of this license, visit http://creativecommons.org/licenses/by-nc-nd/4.0/.
 *
*/

#ifdef EXPANSIONMOD_PARTYHUD_ENABLE
class ExpansionPartyHud: ExpansionScriptViewBase
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
		//m_PartyHUDController.PartyMembers.Clear();
	}
	
	override string GetLayoutFile() 
	{
		return "DayZExpansion/GUI/layouts/hud/expansion_party_hud.layout";
	}
	
	override typename GetControllerType() 
	{
		return ExpansionPartyHudController;
	}
	
	void AddMember(string playerID, string playerName)
	{	
		Print("ExpansionPartyHud::AddMember - Start");
		
		if (!m_PartyHUDController)
			m_PartyHUDController = ExpansionPartyHudController.Cast(GetController());
		
		Print("ExpansionPartyHud::AddMember - Step 1");
		
		ExpansionPartyHudMember newMember = new ExpansionPartyHudMember(playerID, playerName);
		
		Print("ExpansionPartyHud::AddMember - Step 2");
		
		m_PartyHUDController.PartyMembers.Insert(newMember);
		
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
			index = m_PartyHUDController.PartyMembers.Find(member);
			if (index > -1)
			{
				m_PartyHUDController.PartyMembers.Remove(index);
			}
			
			m_AllPartyHUDMembers.Remove(playerID);
		}
		
		Print("ExpansionPartyHud::RemoveMember - End");
	}
	
	void UpdateMembers(map<string, string> members)
	{
		Print("ExpansionPartyHud::UpdateMembers - Start");
		Print("ExpansionPartyHud::UpdateMembers - Members count: " + members.Count());
						
		if (!m_AllPartyHUDMembers || m_AllPartyHUDMembers.Count() == 0)
			return;
		
		array<string> currentMembers = new array<string>;
		foreach (string currentMemberID, ExpansionPartyHudMember currentMember: m_AllPartyHUDMembers)
		{
			bool isStillMember = false;
			bool isNewMember = false;
			
			if (members.Contains(currentMemberID))
			{
				isStillMember = true;
				currentMembers.Insert(currentMemberID);
			}
					
			if (!isStillMember)
			{
				int index = -1;
				index = m_PartyHUDController.PartyMembers.Find(currentMember);
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
		
		if (!m_PartyHUDController.PartyMembers || m_PartyHUDController.PartyMembers.Count() == 0)
			return;
		
		m_PartyHUDController.PartyMembers.Clear();
		
		Print("ExpansionPartyHud::ClearMembers - End");
	}
}

class ExpansionPartyHudController: ExpansionViewController
{
	ref ObservableCollection<ref ExpansionPartyHudMember> PartyMembers = new ObservableCollection<ref ExpansionPartyHudMember>(this);
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
	
	void ExpansionPartyHudMember(string playerID, string playerName)
	{	
		m_PlayerID = playerID;
		m_PlayerName = playerName;
			
		if (!m_PartyMemberController)
			m_PartyMemberController = ExpansionPartyHudMemberController.Cast(GetController());
		
		/*ExpansionMonitorModule monitorModule = ExpansionMonitorModule.Cast(GetModuleManager().GetModule(ExpansionMonitorModule));
		if (monitorModule)
		{
			monitorModule.m_StatsInvoker.Insert(this.OnDataRecived);
			monitorModule.RequestPlayerStats(m_PlayerID);
			
			monitorModule.m_StatesInvoker.Insert(this.OnStateDataRecived);
			monitorModule.RequestPlayerStates(m_PlayerID);
		}*/
		
		SetView();
	}
	
	void ~ExpansionPartyHudMember()
	{
		ExpansionMonitorModule monitorModule = ExpansionMonitorModule.Cast(GetModuleManager().GetModule(ExpansionMonitorModule));
		if (monitorModule)
		{
			monitorModule.m_StatsInvoker.Remove(this.OnDataRecived);
			monitorModule.m_StatesInvoker.Remove(this.OnStateDataRecived);
		}
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
		if (m_PartyMemberController)
		{
			m_PartyMemberController.PlayerName = m_PlayerName;
			m_PartyMemberController.NotifyPropertyChanged("PlayerName");
		}
	}
	
	void OnDataRecived( ExpansionSyncedPlayerStats player_stats)
	{
		if (m_PlayerID != player_stats.m_ID) return;

		SetStats(player_stats);
	}
	
	void OnStateDataRecived( ExpansionSyncedPlayerStates player_states)
	{
		if (m_PlayerID != player_states.m_ID) return;

		SetStates(player_states);
	}
	
	void SetStats( ExpansionSyncedPlayerStats player_stats)
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
	
	void SetStates(ExpansionSyncedPlayerStates player_states)
	{
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
	}
	
	void UpdateStance( int stance )
	{
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
	}
		
	override float GetUpdateTickRate()
	{
		return 1.0;
	}
	
	override void Update()
	{
		ExpansionMonitorModule monitorModule = ExpansionMonitorModule.Cast(GetModuleManager().GetModule(ExpansionMonitorModule));
		if (monitorModule)
		{
			monitorModule.RequestPlayerStats(m_PlayerID);
			monitorModule.RequestPlayerStates(m_PlayerID);
		}
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