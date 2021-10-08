/**
 * ExpansionBookMenuTabParty.c
 *
 * DayZ Expansion Mod
 * www.dayzexpansion.com
 * © 2021 DayZ Expansion Mod Team
 *
 * This work is licensed under the Creative Commons Attribution-NonCommercial-NoDerivatives 4.0 International License.
 * To view a copy of this license, visit http://creativecommons.org/licenses/by-nc-nd/4.0/.
 *
*/

#ifdef EXPANSIONMOD
class ExpansionBookMenuTabParty: ExpansionBookMenuTabBase
{
	ref ExpansionPartyModule m_PartyModule;
	ref ExpansionDialog_CreateParty m_CreatePartyDialog;
	ref ExpansionBookMenuTabPartyController m_PartyTabController;
	ref ExpansionPartyData m_Party;
	ref ExpansionDialog_DeleteParty m_DeleteDialog;
	ref ExpansionDialog_LeaveParty m_LeaveDialog;
	ref ExpansionDialog_InviteParty m_InviteDialog;
	ref ExpansionBookMenuTabPartyMemberEdit m_PartyTabMemberEdit;
	
	private TextWidget member_list_info;
	private ButtonWidget party_delete_button;
	private TextWidget party_delete_button_label;
	
	private EditBoxWidget player_list_filter_editbox;
	private ButtonWidget player_list_filter_clear;
	private ImageWidget player_list_filter_clear_icon;
	
	private Widget party_delete_content;
	private Widget party_leave_content;
	private Widget book_menu_content_left;
		
	private int m_PlayerSearchRadius = 25;
	SyncPlayer m_Player;
	
	void ExpansionBookMenuTabParty(ExpansionBookMenu book_menu)
	{
		m_BookMenu = book_menu;
		
		if (!m_PartyModule)
			m_PartyModule = ExpansionPartyModule.Cast(GetModuleManager().GetModule(ExpansionPartyModule));
		
		if (!m_PartyTabController)
			m_PartyTabController = ExpansionBookMenuTabPartyController.Cast(GetController());
		
		RegisterPartyTabs();
	}
	
	void ~ExpansionBookMenuTabParty()
	{
		m_Party = NULL;
	}
	
	void RegisterPartyTabs()
	{
		if (!m_PartyTabMemberEdit)
		{
			m_PartyTabMemberEdit = new ExpansionBookMenuTabPartyMemberEdit(m_BookMenu);
			m_PartyTabMemberEdit.SetParentTab(this);
			m_BookMenu.RegisterTab(m_PartyTabMemberEdit);
		}
	}
	
	ExpansionBookMenuTabPartyMemberEdit GetMemberEditTab()
	{
		return m_PartyTabMemberEdit;
	}
	
	void UnregisterPartyTabs()
	{
		if (m_PartyTabMemberEdit)
		{
			m_BookMenu.UnregisterTab(m_PartyTabMemberEdit);
			delete m_PartyTabMemberEdit;
		}
	}
	
	void GetParty()
	{
		m_Party = m_PartyModule.GetParty();
	}
	
	void SetView()
	{
		if (!m_Party)
			return;
		
		m_PartyTabController.PartyName = m_Party.GetPartyName() + " #STR_EXPANSION_BOOK_MEMBERS";
		m_PartyTabController.NotifyPropertyChanged("PartyName");
	
		#ifdef EXPANSION_PARTY_MODULE_DEBUG
		EXLogPrint("SetView - CanDelete(): " + GetPlayerPartyData().CanDelete());
		EXLogPrint("SetView - CanInvite(): " + GetPlayerPartyData().CanInvite());
		EXLogPrint("SetView - CanKick(): " + GetPlayerPartyData().CanKick());
		EXLogPrint("SetView - CanEdit(): " + GetPlayerPartyData().CanEdit());
		#ifdef EXPANSIONMODMARKET
		EXLogPrint("SetView - CanWithdrawMoney(): " + GetPlayerPartyData().CanWithdrawMoney());
		#endif
		#endif	
		
		if (GetPlayerPartyData().CanDelete())
		{
			party_delete_content.Show(true);
		}
		else
		{
			party_delete_content.Show(false);
		}
		
		if (GetPlayerPartyData().GetID() == m_Party.GetOwnerUID())
		{
			party_leave_content.Show(false);
		}
		else
		{
			party_leave_content.Show(true);			
		}
		
		if (GetPlayerPartyData().CanInvite())
		{
			book_menu_content_left.Show(true);
		}
		else
		{
			book_menu_content_left.Show(false);
		}
	}
	
	void LoadMembers()
	{		
		if (m_Party && m_Party.GetPlayers().Count() > 0)
		{
			array<ref ExpansionPartyPlayerData> members = m_Party.GetPlayers();
			for (int i = 0; i < members.Count(); ++i)
			{
				ExpansionPartyPlayerData memberData = members[i];
				ExpansionPartyPlayerData playerData = m_Party.GetPlayer(GetGame().GetPlayer().GetIdentity().GetId());
				
				//! Dont add a entry for the player itself, only his party members
				if (memberData.GetID() == playerData.GetID())
					continue;
				
				ExpansionBookMenuTabPartyMemberEntry entry;
				bool hasEntry = false;
				if (m_PartyTabController.PartyMemberEntrys.Count() > 0)
				{
					for (int e = 0; e < m_PartyTabController.PartyMemberEntrys.Count(); ++e)
					{
						ExpansionBookMenuTabPartyMemberEntry existingEntry = m_PartyTabController.PartyMemberEntrys[e];
						
						if (existingEntry.m_Member.GetID() == memberData.GetID())
							hasEntry = true;
						
						//! If the entry is related to a old member that is no longer the party then remove the entry
						if (members.Find(existingEntry.m_Member) == -1)
							m_PartyTabController.PartyMemberEntrys.Remove(e);
					}
				}
				//! If the party member has a entry already then skip the entry creation
				if (!hasEntry)
				{
					entry = new ExpansionBookMenuTabPartyMemberEntry(this, memberData);
					m_PartyTabController.PartyMemberEntrys.Insert(entry);
				}
			}
		}
		
		if (m_Party && m_Party.GetPlayers().Count() == 1)
		{
			if (member_list_info && !member_list_info.IsVisible())
				member_list_info.Show(true);
			
			if (m_PartyTabController.MemberListInfo && m_PartyTabController.MemberListInfo != "#STR_EXPANSION_BOOK_TERRITORY_ERROR_NO_ONLINE_MEMBERS")
			{
				m_PartyTabController.MemberListInfo = "#STR_EXPANSION_BOOK_TERRITORY_ERROR_NO_ONLINE_MEMBERS";
				m_PartyTabController.NotifyPropertyChanged("MemberListInfo");
			}
		}
		else if (m_Party && m_Party.GetPlayers().Count() > 2)
		{
			if (member_list_info && member_list_info.IsVisible())
				member_list_info.Show(false);
		}
	}
	
	void LoadPlayers(string filter)
	{
		if (!m_Party) 
			return;
		
		PlayerBase player = PlayerBase.Cast(GetGame().GetPlayer());
		if (!player) 
			return;
	
		ClearPlayers();
		
		string filterNormal = filter;
		filter.ToLower();
			
		int nmbPlayer = 0;
		ExpansionBookMenuTabPartyPlayerEntry entry;
		SyncPlayer playerSync;
		string playerName;
		
		if (GetExpansionSettings().GetParty().UseWholeMapForInviteList)
		{
			for (int i = 0; i < ClientData.m_PlayerList.m_PlayerList.Count(); ++i)
			{
				playerSync = ClientData.m_PlayerList.m_PlayerList[i];
				
				if (!playerSync)
					continue;
				
				Print(IsMember(playerSync.m_RUID));
				Print(HasInvite(playerSync.m_RUID));
					
				if (IsMember(playerSync.m_RUID) || HasInvite(playerSync.m_RUID))
				{
					continue;
				}
				
				if (filter != "")
				{
					playerName = playerSync.m_PlayerName;
					playerName.ToLower();
		
					if (playerName.IndexOf(filter) == -1)
						continue;
				}
				
				nmbPlayer++;
				
				AddPlayerEntry(playerSync);
			}
		}
		else
		{
			array<PlayerIdentity> identitys = GetNearbyPlayerIdentitys(player.GetPosition(), m_PlayerSearchRadius);			
			if (!identitys)
			{
				m_PartyTabController.PlayerListInfo = "0";
				m_PartyTabController.NotifyPropertyChanged("PlayerListInfo");
				return;
			}
			
			for (int j = 0; j < identitys.Count(); ++j)
			{					
				for (int k = 0; k < ClientData.m_PlayerList.m_PlayerList.Count(); ++k)
				{					
					playerSync = ClientData.m_PlayerList.m_PlayerList[k];
					
					if (!playerSync)
						continue;
									
					if (IsMember(playerSync.m_RUID) || HasInvite(playerSync.m_RUID))
					{
						continue;
					}
					
					if (filter != "")
					{
						playerName = playerSync.m_PlayerName;
						playerName.ToLower();
			
						if (playerName.IndexOf(filter) == -1)
							continue;
					}
										
					if (playerSync && playerSync.m_RUID == identitys[j].GetId())
					{
						nmbPlayer++;
						
						AddPlayerEntry(playerSync);
					}
				}
			}
		}
		
		if (nmbPlayer > 0)
		{
			m_PartyTabController.PlayerListInfo = "#STR_EXPANSION_BOOK_TERRITORY_INVITE_SELECT";
			m_PartyTabController.NotifyPropertyChanged("PlayerListInfo");
		}
		else
		{
			m_PartyTabController.PlayerListInfo = "#STR_EXPANSION_BOOK_TERRITORY_INVITE_EMPTY";
			m_PartyTabController.NotifyPropertyChanged("PlayerListInfo");
		}
	}
	
	void AddPlayerEntry(SyncPlayer playerSync)
	{
		bool isInList = false;
		for (int i = 0; i < m_PartyTabController.PartyPlayerEntrys.Count(); ++i)
		{
			ExpansionBookMenuTabPartyPlayerEntry entry = m_PartyTabController.PartyPlayerEntrys[i];
			if (entry.m_Player.m_RUID == playerSync.m_RUID)
			{
				isInList = true;
				continue;
			}
		}
		
		if (!isInList)
		{
			ExpansionBookMenuTabPartyPlayerEntry newEntry = new ExpansionBookMenuTabPartyPlayerEntry(this, playerSync);
			m_PartyTabController.PartyPlayerEntrys.Insert(newEntry);
		}
	}
	
	void ClearPlayers()
	{
		for (int i = 0; i < m_PartyTabController.PartyPlayerEntrys.Count(); ++i)
		{
			ExpansionBookMenuTabPartyPlayerEntry entry = m_PartyTabController.PartyPlayerEntrys[i];
			entry.Hide();
			entry = NULL;
		}
		
		m_PartyTabController.PartyPlayerEntrys.Clear();
	}
	
	array<PlayerIdentity> GetNearbyPlayerIdentitys(vector position, int radius)
	{
		array<Object> objects = new array<Object>;
		array<CargoBase> proxyCargos = new array<CargoBase>;
		array<PlayerIdentity> identitys = new array<PlayerIdentity>;
		
		GetGame().GetObjectsAtPosition(position, radius, objects, proxyCargos);
		
		if (objects.Count() > 0)
		{
			foreach (Object obj : objects)
			{
				PlayerBase player;
				if (Class.CastTo(player, obj))
				{
					identitys.Insert(player.GetIdentity());
				}
			}
		}
		
		return identitys;
	}
	
	void InvitePlayer(SyncPlayer player)
	{
		m_Player = player;
		
		if (!m_InviteDialog)
			m_InviteDialog = new ExpansionDialog_InviteParty(this);
		
		m_InviteDialog.Show();
	}
	
	SyncPlayer GetSelectedPlayer()
	{
		return m_Player;
	}
	
	bool IsMember(string uid)
	{
		return m_Party.IsMember(uid);
	}
	
	bool HasInvite(string uid)
	{
		if (m_Party && m_Party.GetPartyInvites().Count() > 0)
		{
			foreach (ExpansionPartyInviteData invite: m_Party.GetPartyInvites())
			{
				if (invite.UID == uid)
					return true;
			}
		}
		
		return false;
	}
	
	ExpansionPartyPlayerData GetPlayerPartyData()
	{
		return m_Party.GetPlayer(GetGame().GetPlayer().GetIdentity().GetId());
	}
	
	void OnDeleteButtonClick()
	{
		if (GetPlayerPartyData().CanDelete())
		{
			if (!m_DeleteDialog)
				m_DeleteDialog = new ExpansionDialog_DeleteParty(this);
				
			m_DeleteDialog.Show();
		}
	}
	
	void OnLeaveButtonClick()
	{
		if (!m_LeaveDialog)
			m_LeaveDialog = new ExpansionDialog_LeaveParty(this);
		
		m_LeaveDialog.Show();
	}
	
	void OnRefreshButtonClick()
	{
		m_PartyTabController.SearchFilter = "#STR_USRACT_SEARCH";
		m_PartyTabController.NotifyPropertyChanged("SearchFilter");
		
		if (GetPlayerPartyData().CanInvite())
			LoadPlayers("");
	}
	
	override string GetLayoutFile() 
	{
		return "DayZExpansion/Book/GUI/layouts/tabs/party/expansion_book_tab_party.layout";
	}
	
	override typename GetControllerType() 
	{
		return ExpansionBookMenuTabPartyController;
	}
	
	override string GetTabIconName()
	{
		return "Group";
	}
	
	override string GetTabName()
	{
		return "#STR_EXPANSION_BOOK_TAB_PARTY_MANAGER";
	}
	
	override int GetTabColor()
	{
		return ARGB(255,0,0,0);
	}
	
	override bool CanShow()
	{
		return GetExpansionSettings().GetBook().EnablePartyTab;
	}
	
	override void OnShow()
	{
		super.OnShow();
		
		if (!m_PartyModule.HasParty())
		{
			if (!m_CreatePartyDialog)
				m_CreatePartyDialog = new ExpansionDialog_CreateParty(this);
			
			m_CreatePartyDialog.Show();
			GetLayoutRoot().Show(false);
		}
		else
		{
			GetParty();
			SetView();
			
			LoadMembers();
			
			if (GetPlayerPartyData().CanInvite())
				LoadPlayers("");
		}
		
		SwitchMovementLockState(true);
	}
	
	override void OnHide()
	{
		super.OnHide();
		
		if (m_CreatePartyDialog && m_CreatePartyDialog.IsVisible())
			m_CreatePartyDialog.Hide();
		
		if (m_InviteDialog && m_InviteDialog.IsVisible())
			m_InviteDialog.Hide();
		
		if (m_DeleteDialog && m_DeleteDialog.IsVisible())
			m_DeleteDialog.Hide();
		
		SwitchMovementLockState(false);
		
		m_Party = NULL;
	}
	
	override bool IsParentTab()
	{
		return true;
	}
	
	override bool OnMouseEnter(Widget w, int x, int y)
	{
		switch (w)
		{
		case party_delete_button:
			party_delete_button_label.SetColor(ARGB(255, 220, 220, 220));
			break;
		case player_list_filter_clear:
			player_list_filter_clear_icon.SetColor(ARGB(255, 220, 220, 220));
			break;
		}
		
		return super.OnMouseEnter(w, x, y);;
	}
	
	override bool OnMouseLeave(Widget w, Widget enterW, int x, int y)
	{
		switch (w)
		{
		case party_delete_button:
			party_delete_button_label.SetColor(ARGB(255, 0, 0, 0));
			break;
		case player_list_filter_clear:
			player_list_filter_clear_icon.SetColor(ARGB(255, 0, 0, 0));
			break;
		}
		
		return super.OnMouseLeave(w, enterW, x, y);
	}
	
	override void OnBackButtonClick()
	{
		super.OnBackButtonClick();
		
		Hide();
	}
	
	override bool OnChange(Widget w, int x, int y, bool finished)
	{
		if (w == player_list_filter_editbox)
		{
			if (GetPlayerPartyData().CanInvite())
			{
				LoadPlayers(player_list_filter_editbox.GetText());
				return true;
			}
		}
		
		return false;
	}
	
	override void Refresh()
	{
		Print("ExpansionBookMenuTabParty::Refresh - Start");
		if (!m_PartyModule.HasParty())
		{
			Print("ExpansionBookMenuTabParty::Refresh - Player has no party!");
			m_Party = NULL;
			Hide();
			
			if (m_PartyTabController.PartyMemberEntrys.Count())
				m_PartyTabController.PartyMemberEntrys.Clear();
		}
		else
		{
			Print("ExpansionBookMenuTabParty::Refresh - Player has party!");
			GetParty();
			SetView();
			
			LoadMembers();
			
			if (GetPlayerPartyData().CanInvite())
				LoadPlayers("");
		}
		Print("ExpansionBookMenuTabParty::Refresh - End");
	}
};

class ExpansionBookMenuTabPartyController: ExpansionViewController 
{
	ref ObservableCollection<ref ExpansionBookMenuTabPartyMemberEntry> PartyMemberEntrys = new ObservableCollection<ref ExpansionBookMenuTabPartyMemberEntry>(this);
	ref ObservableCollection<ref ExpansionBookMenuTabPartyPlayerEntry> PartyPlayerEntrys  = new ObservableCollection<ref ExpansionBookMenuTabPartyPlayerEntry>(this);
	string PartyName;
	string MemberListInfo;
	string PlayerListInfo;
	string SearchFilter;
};
#endif
