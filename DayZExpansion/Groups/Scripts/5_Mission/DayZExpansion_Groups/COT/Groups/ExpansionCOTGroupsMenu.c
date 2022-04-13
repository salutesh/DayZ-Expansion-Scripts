/**
 * ExpansionCOTGroupsMenu.c
 *
 * DayZ Expansion Mod
 * www.dayzexpansion.com
 * © 2022 DayZ Expansion Mod Team
 *
 * This work is licensed under the Creative Commons Attribution-NonCommercial-NoDerivatives 4.0 International License. 
 * To view a copy of this license, visit http://creativecommons.org/licenses/by-nc-nd/4.0/.
 *
*/

#ifdef JM_COT
enum ExpansionCOTGroupsMenuCallback
{
	GroupsInit = 0
	GroupsUpdate,
	GroupUpdate,
	MemberUpdate,
}

class ExpansionCOTGroupsMenu: JMFormBase
{
	private ExpansionCOTGroupModule m_Module;
	//private ExpansionPartyModule m_GroupsModule;
	
	private ref array<ref ExpansionCOTGroupsListEntry> m_GroupEntries;
	private ref array<ref ExpansionCOTGroupsMembersListEntry> m_MemberEntries;
	private ref array<ref ExpansionCOTGroupsMapMarker> m_GroupMarkers;
	private ref array<ref ExpansionCOTGroupsMarkerListEntry> m_GroupMarkerEntries;
	private ref array<ref ExpansionCOTGroupsPlayersListEntry> m_ServerPlayers;
	
	private ExpansionPartyData m_CurrentSelectedGroup;
	private ExpansionPartyPlayerData m_CurrentSelectedGroupMember;
	private ExpansionPlayerDataCollection m_CurrentSelectedServerPlayer;
	
	private Widget m_GroupsListPanel;	
	private GridSpacerWidget m_GroupsListContent;
	private ButtonWidget m_GroupsListRefreshButton;
	
	private Widget m_GroupInfoPanel;
	
	private TextWidget m_GroupNameLabel;
	private EditBoxWidget m_GroupNameValue;
	private ButtonWidget m_GroupChangeNameButton;
	private TextWidget m_GroupIDValue;
	private TextWidget m_GroupOwnerValue;
	private ButtonWidget m_GroupCopyInfoButton;
	
	private Widget m_GroupButtonsPanel;
	private ButtonWidget m_GroupDeleteButton;
	private ButtonWidget m_GroupInviteSelfButton;
	private ButtonWidget m_GroupEditButton;

#ifdef EXPANSIONMODNAVIGATION
	private Widget m_GroupMapPanel;
	private MapWidget m_GroupMap;
	
	private Widget m_MarkersListPanel;
	private GridSpacerWidget m_MarkersListContent;
#endif
	
	private Widget m_MembersListPanel;	
	private GridSpacerWidget m_MembersListContent;
	private ButtonWidget m_MembersListRefreshButton;
	
	private Widget m_MemberInfoPanel;
	private TextWidget m_MemberNameValue;
	private TextWidget m_MemberUIDValue;
	private TextWidget m_MemberRankValue;
	
	private CheckBoxWidget m_MemberPermInvite;
	private CheckBoxWidget m_MemberPermKick;
	private CheckBoxWidget m_MemberPermEdit;
	private CheckBoxWidget m_MemberPermDelete;
	
	private Widget m_MemberButtonsPanel;
	private ButtonWidget m_MemberSavePermButton;
	private ButtonWidget m_MemberSetOwnerButton;
	private ButtonWidget m_MemberKickButton;
	private ButtonWidget m_MemberEditCancelButton;
	private ButtonWidget m_MemberCopyInfoButton;
	
#ifdef EXPANSIONMODMARKET
	private CheckBoxWidget m_MemberPermWithdraw;
	private Widget m_GroupMoneyPanel;
	private EditBoxWidget m_GroupMoneyValue;
	private ButtonWidget m_GroupChangeMoneyButton;
#endif
	
	private Widget m_PlayersListPanel;
	private GridSpacerWidget m_PlayersListContent;
	private ButtonWidget m_PlayersListRefreshButton;
	
	private Widget m_PlayerInfoPanel;
	private TextWidget m_PlayerInfoNameValue;
	private TextWidget m_PlayerInfoIDValue;
	private TextWidget m_PlayerInfoGroupValue;
	
	private Widget m_PlayerButtonsPanel;
	private ButtonWidget m_PlayerInviteButton;
	private ButtonWidget m_PlayerSetOwnerButton;
	private ButtonWidget m_PlayerEditCancelButton;
	
	private Widget m_EditPanel;
	
	// ------------------------------------------------------------
	// ExpansionCOTGroupsMenu Constructor
	// ------------------------------------------------------------
	void ExpansionCOTGroupsMenu()
	{
		m_GroupEntries = new array<ref ExpansionCOTGroupsListEntry>;
		m_MemberEntries = new array<ref ExpansionCOTGroupsMembersListEntry>;
		m_GroupMarkers  = new array<ref ExpansionCOTGroupsMapMarker>;
		m_GroupMarkerEntries = new array<ref ExpansionCOTGroupsMarkerListEntry>;
		m_ServerPlayers = new array<ref ExpansionCOTGroupsPlayersListEntry>;
	}
	
	// ------------------------------------------------------------
	// ExpansionCOTGroupsMenu Constructor
	// ------------------------------------------------------------
	void ~ExpansionCOTGroupsMenu()
	{		
		m_GroupEntries.Clear();
		m_MemberEntries.Clear();
		m_GroupMarkers.Clear();
		m_GroupMarkerEntries.Clear();
		m_ServerPlayers.Clear();
		
		delete layoutRoot;
	}
	
	// ------------------------------------------------------------
	// ExpansionCOTGroupsMenu SetModule
	// ------------------------------------------------------------
	#ifdef COT_BUGFIX_REF
	protected override bool SetModule(JMRenderableModuleBase mdl)
	#else
	protected override bool SetModule(ref JMRenderableModuleBase mdl)
	#endif
	{
		return Class.CastTo(m_Module, mdl);
	}
	
	// ------------------------------------------------------------
	// ExpansionCOTGroupsMenu OnInit
	// ------------------------------------------------------------
	override void OnInit()
	{
		//! Group list
		m_GroupsListPanel = Widget.Cast(layoutRoot.FindAnyWidget("groups_list_panel"));
		m_GroupsListContent = GridSpacerWidget.Cast(layoutRoot.FindAnyWidget("groups_list_content"));
		
		//! Group list refresh button
		m_GroupsListRefreshButton = ButtonWidget.Cast(layoutRoot.FindAnyWidget("groups_refresh_button"));
		
		//! Group info panel
		m_GroupInfoPanel = Widget.Cast(layoutRoot.FindAnyWidget("group_info_panel"));
		
		m_GroupNameLabel = TextWidget.Cast(layoutRoot.FindAnyWidget("info_name_label"));
		m_GroupNameValue = EditBoxWidget.Cast(layoutRoot.FindAnyWidget("info_name_value"));
		m_GroupChangeNameButton = ButtonWidget.Cast(layoutRoot.FindAnyWidget("info_change_name_button"));
		
		m_GroupIDValue = TextWidget.Cast(layoutRoot.FindAnyWidget("info_id_value"));
		m_GroupOwnerValue = TextWidget.Cast(layoutRoot.FindAnyWidget("info_owner_value"));
		m_GroupCopyInfoButton = ButtonWidget.Cast(layoutRoot.FindAnyWidget("copy_group_info_button"));
				
		//! Members list
		m_MembersListPanel = Widget.Cast(layoutRoot.FindAnyWidget("members_list_panel"));
		m_MembersListContent = GridSpacerWidget.Cast(layoutRoot.FindAnyWidget("members_list_content"));
		
		//! Members list refresh button
		m_MembersListRefreshButton = ButtonWidget.Cast(layoutRoot.FindAnyWidget("members_refresh_button"));
		
		//! Group buttons panel
		m_GroupButtonsPanel = Widget.Cast(layoutRoot.FindAnyWidget("group_buttons_panel"));
		m_GroupDeleteButton = ButtonWidget.Cast(layoutRoot.FindAnyWidget("delete_group_button"));
		m_GroupInviteSelfButton = ButtonWidget.Cast(layoutRoot.FindAnyWidget("invite_self_button")); 
		m_GroupEditButton = ButtonWidget.Cast(layoutRoot.FindAnyWidget("group_edit_button"));
		
	#ifdef EXPANSIONMODNAVIGATION
		//! Map
		m_GroupMapPanel = Widget.Cast(layoutRoot.FindAnyWidget("map_window_panel"));
		m_GroupMap = MapWidget.Cast(layoutRoot.FindAnyWidget("group_map"));
		
		//! Markers list
		m_MarkersListPanel = Widget.Cast(layoutRoot.FindAnyWidget("markers_list_panel"));
		m_MarkersListContent = GridSpacerWidget.Cast(layoutRoot.FindAnyWidget("markers_list_content"));
	#endif
		
		//! Member info panel
		m_MemberInfoPanel = Widget.Cast(layoutRoot.FindAnyWidget("member_info_panel"));
		m_MemberNameValue = TextWidget.Cast(layoutRoot.FindAnyWidget("member_info_name_value"));
		m_MemberUIDValue = TextWidget.Cast(layoutRoot.FindAnyWidget("member_info_uid_value"));
		m_MemberRankValue = TextWidget.Cast(layoutRoot.FindAnyWidget("member_info_rank_value"));
		m_MemberPermInvite = CheckBoxWidget.Cast(layoutRoot.FindAnyWidget("member_perm_invite_checkbox"));
		m_MemberPermKick = CheckBoxWidget.Cast(layoutRoot.FindAnyWidget("member_perm_kick_checkbox"));
		m_MemberPermEdit = CheckBoxWidget.Cast(layoutRoot.FindAnyWidget("member_perm_edit_checkbox"));
		m_MemberPermDelete = CheckBoxWidget.Cast(layoutRoot.FindAnyWidget("member_perm_delete_checkbox"));
		
		//! Member buttons panel
		m_MemberButtonsPanel = Widget.Cast(layoutRoot.FindAnyWidget("member_buttons_panel"));
		m_MemberSavePermButton = ButtonWidget.Cast(layoutRoot.FindAnyWidget("save_member_perm_button"));
		m_MemberSetOwnerButton = ButtonWidget.Cast(layoutRoot.FindAnyWidget("setowner_member_perm_button"));
		m_MemberKickButton = ButtonWidget.Cast(layoutRoot.FindAnyWidget("kick_member_button"));
		m_MemberEditCancelButton = ButtonWidget.Cast(layoutRoot.FindAnyWidget("cancel_member_edit_button"));
		m_MemberCopyInfoButton = ButtonWidget.Cast(layoutRoot.FindAnyWidget("copy_member_info_button"));
	
	#ifdef EXPANSIONMODMARKET
		m_MemberPermWithdraw = CheckBoxWidget.Cast(layoutRoot.FindAnyWidget("member_perm_withdraw_checkbox"));
		
		m_GroupMoneyPanel = Widget.Cast(layoutRoot.FindAnyWidget("group_info_money"));
		m_GroupMoneyValue = EditBoxWidget.Cast(layoutRoot.FindAnyWidget("info_money_value"));
		m_GroupChangeMoneyButton = ButtonWidget.Cast(layoutRoot.FindAnyWidget("info_change_money_button"));
	#endif
		
		//! Player list
		m_PlayersListPanel = Widget.Cast(layoutRoot.FindAnyWidget("players_list_panel"));
		m_PlayersListContent = GridSpacerWidget.Cast(layoutRoot.FindAnyWidget("players_list_content")); 
		m_PlayersListRefreshButton = ButtonWidget.Cast(layoutRoot.FindAnyWidget("players_refresh_button"));
		
		//! Player info panel
		m_PlayerInfoPanel = Widget.Cast(layoutRoot.FindAnyWidget("player_info_panel"));
		m_PlayerInfoNameValue = TextWidget.Cast(layoutRoot.FindAnyWidget("player_info_name_value"));
		m_PlayerInfoIDValue = TextWidget.Cast(layoutRoot.FindAnyWidget("player_info_uid_value"));
		m_PlayerInfoGroupValue = TextWidget.Cast(layoutRoot.FindAnyWidget("player_info_group_value"));
		
		//! Player buttons panel
		m_PlayerButtonsPanel = Widget.Cast(layoutRoot.FindAnyWidget("player_buttons_panel"));
		m_PlayerInviteButton = ButtonWidget.Cast(layoutRoot.FindAnyWidget("player_invite_button"));
		m_PlayerSetOwnerButton = ButtonWidget.Cast(layoutRoot.FindAnyWidget("player_set_owner_button"));
		m_PlayerEditCancelButton = ButtonWidget.Cast(layoutRoot.FindAnyWidget("cancel_player_edit_button"));
		
		m_EditPanel = Widget.Cast(layoutRoot.FindAnyWidget("edit_info_panel"));
	}
	
	// ------------------------------------------------------------
	// ExpansionCOTGroupsMenu SetViewGroupEdit
	// ------------------------------------------------------------	
	void SetViewGroupEdit()
	{		
		m_GroupsListPanel.Show(true);
		m_GroupInfoPanel.Show(true);
		m_GroupButtonsPanel.Show(true);
		m_MembersListPanel.Show(true);
	#ifdef EXPANSIONMODNAVIGATION
		m_GroupMapPanel.Show(true);
		m_MarkersListPanel.Show(true);
	#endif
	}
	
	// ------------------------------------------------------------
	// ExpansionCOTGroupsMenu SetViewMemberEdit
	// ------------------------------------------------------------	
	void SetViewMemberEdit()
	{
		m_GroupsListPanel.Show(false);
		m_GroupInfoPanel.Show(false);
		m_GroupButtonsPanel.Show(false);
		m_MembersListPanel.Show(false);
	#ifdef EXPANSIONMODNAVIGATION
		m_GroupMapPanel.Show(false);
		m_MarkersListPanel.Show(false);
	#endif
		m_PlayerInfoPanel.Show(false);
		m_PlayerButtonsPanel.Show(false);
		m_EditPanel.Show(false);
		
		m_MemberInfoPanel.Show(true);
		m_MemberButtonsPanel.Show(true);
		m_MembersListPanel.Show(true);
	}
	
	// ------------------------------------------------------------
	// ExpansionCOTGroupsMenu SetViewOwnerEdit
	// ------------------------------------------------------------
	void SetViewEdit()
	{
		m_GroupsListPanel.Show(false);
		m_GroupInfoPanel.Show(false);
		m_GroupButtonsPanel.Show(false);
	#ifdef EXPANSIONMODNAVIGATION
		m_GroupMapPanel.Show(false);
		m_MarkersListPanel.Show(false);
	#endif
		
		m_EditPanel.Show(true);
		m_PlayersListPanel.Show(true);
		m_MembersListPanel.Show(true);
	}
	
	// ------------------------------------------------------------
	// ExpansionCOTGroupsMenu SetViewPlayerEdit
	// ------------------------------------------------------------
	void SetViewPlayerEdit()
	{
		m_MemberInfoPanel.Show(false);
		m_MemberButtonsPanel.Show(false);
		m_EditPanel.Show(false);
		
		m_PlayerInfoPanel.Show(true);
		m_PlayerButtonsPanel.Show(true);
	}
	
	// ------------------------------------------------------------
	// ExpansionCOTGroupsMenu FillGroupList
	// ------------------------------------------------------------
	private void FillGroupList(bool init)
	{
		m_GroupEntries.Clear();
		
		if (!m_Module || m_Module.GetAllGroups().Count() == 0)
			return;
				
		for (int i = 0; i < m_Module.GetAllGroups().Count(); i++)
		{
			ExpansionPartyData group = m_Module.GetAllGroups().GetElement(i);
			if (!group)
				continue;
			
			if (!init && m_CurrentSelectedGroup && m_CurrentSelectedGroup.GetPartyID() == group.GetPartyID())
				m_CurrentSelectedGroup = group;
			
			ExpansionCOTGroupsListEntry listEntry = new ExpansionCOTGroupsListEntry(m_GroupsListContent, this, group);
			m_GroupEntries.Insert(listEntry);
		}
	}
	
	// ------------------------------------------------------------
	// ExpansionCOTGroupsMenu FillPlayerList
	// ------------------------------------------------------------
	private void FillPlayerList()
	{
		ExpansionDataCollectionModule dataCollection;
		if (!CF_Modules<ExpansionDataCollectionModule>.Get(dataCollection))
			return;
		
		m_ServerPlayers.Clear();
		
		for (int i = 0; i < dataCollection.GetAllPlayers().Count(); ++i)
		{
			ExpansionPlayerDataCollection player = dataCollection.GetAllPlayers().GetElement(i);
			if (!player)
				continue;
			
			ExpansionCOTGroupsPlayersListEntry listEntry = new ExpansionCOTGroupsPlayersListEntry(m_PlayersListContent, this, player);
			m_ServerPlayers.Insert(listEntry);
		}
	}
	
	// ------------------------------------------------------------
	// ExpansionCOTGroupsMenu SetPlayerInfo
	// ------------------------------------------------------------	
	void SetPlayerInfo(ExpansionPlayerDataCollection player)
	{
		m_CurrentSelectedServerPlayer = player;
		
		SetViewPlayerEdit();
		
		m_PlayerInfoNameValue.SetText(player.Name);
		m_PlayerInfoIDValue.SetText(player.PlayerUID);
		
		ExpansionPartyPlayerData playerPartyData = m_Module.GetGroupPlayerData(player.PlayerUID);
		if (playerPartyData)
		{
			ExpansionPartyData partyData = playerPartyData.GetParty();
			if (!partyData)
				return;
			
			if (partyData.GetOwnerUID() == player.PlayerUID)
			{
				m_PlayerInfoGroupValue.SetText("IS OWNER OF GROUP: " + partyData.GetPartyID());
				m_PlayerSetOwnerButton.Show(false);
			}
			else
			{
				m_PlayerInfoGroupValue.SetText("IS MEMBER OF GROUP:" + partyData.GetPartyID());
				if (partyData.GetPartyID() == playerPartyData.GetParty().GetPartyID())
					m_PlayerSetOwnerButton.Show(true);
			}
			
			m_PlayerInviteButton.Show(false);
		}
		else
		{
			m_PlayerInfoGroupValue.SetText("NOT IN A GROUP");
			m_PlayerInviteButton.Show(true);
		}
	}
	
	// ------------------------------------------------------------
	// ExpansionCOTGroupsMenu SetGroupInfo
	// ------------------------------------------------------------	
	void SetGroupInfo(ExpansionPartyData group)
	{
		m_CurrentSelectedGroup = group;
		m_CurrentSelectedGroupMember = NULL;
		m_CurrentSelectedServerPlayer = NULL;
		
		SetViewGroupEdit();
		
		m_GroupNameValue.SetText(group.GetPartyName());
		m_GroupIDValue.SetText(group.GetPartyID().ToString());
		m_GroupOwnerValue.SetText(group.GetOwnerName() + " - " + group.GetOwnerUID());
	
	#ifdef EXPANSIONMODMARKET
		m_GroupMoneyPanel.Show(true);
		m_GroupMoneyValue.SetText(group.GetMoneyDeposited().ToString());
	#endif
		
		FillMemberList(true);
	
		string playerUID = GetGame().GetPlayer().GetIdentity().GetId();
		if (m_Module.HasGroup(playerUID))
			m_GroupInviteSelfButton.Show(false);
			
	#ifdef EXPANSIONMODNAVIGATION
		AddGroupMarkers();
	#endif
	}
	
	// ------------------------------------------------------------
	// ExpansionCOTGroupsMenu FillMemberList
	// ------------------------------------------------------------	
	private void FillMemberList(bool init)
	{
		if (!m_CurrentSelectedGroup)
			return;
		
		m_MemberEntries.Clear();
		
		for (int i = 0; i < m_CurrentSelectedGroup.GetPlayers().Count(); i++)
		{
			ExpansionPartyPlayerData playerData = m_CurrentSelectedGroup.GetPlayers()[i];
			if (!playerData)
				continue;
			
			if (!init && m_CurrentSelectedGroupMember && m_CurrentSelectedGroupMember.GetID() == playerData.GetID())
				m_CurrentSelectedGroupMember = playerData;
									
			ExpansionCOTGroupsMembersListEntry listEntry = new ExpansionCOTGroupsMembersListEntry(m_MembersListContent, this, playerData);
			m_MemberEntries.Insert(listEntry);
		}
	}
	
	// ------------------------------------------------------------
	// ExpansionCOTGroupsMenu SetMemberInfo
	// ------------------------------------------------------------	
	void SetMemberInfo(ExpansionPartyPlayerData player)
	{
		if (!m_CurrentSelectedGroup)
			return;
		
		m_CurrentSelectedGroupMember = player;
		
		SetViewMemberEdit();
		
		m_MemberNameValue.SetText(player.GetName());
		m_MemberUIDValue.SetText(player.GetID());
		
		if (m_CurrentSelectedGroup.GetOwnerUID() == m_CurrentSelectedGroupMember.GetID())
		{
			m_MemberSetOwnerButton.Show(false);
			m_MemberRankValue.SetText("Owner");
		}
		else
		{
			m_MemberSetOwnerButton.Show(true);
			m_MemberRankValue.SetText("Member");
		}
		
		m_MemberPermInvite.SetChecked(player.CanInvite());
		m_MemberPermKick.SetChecked(player.CanKick());
		m_MemberPermEdit.SetChecked(player.CanEdit());
		m_MemberPermDelete.SetChecked(player.CanDelete());
	#ifdef EXPANSIONMODMARKET
		m_MemberPermWithdraw.Show(true);
		m_MemberPermWithdraw.SetChecked(player.CanWithdrawMoney());
	#endif
		
		if (m_CurrentSelectedGroup.GetOwnerUID() == m_CurrentSelectedGroupMember.GetID())
			m_MemberSetOwnerButton.Show(false);
		else
			m_MemberSetOwnerButton.Show(true);
	}

#ifdef EXPANSIONMODNAVIGATION
	// ------------------------------------------------------------
	// ExpansionCOTGroupsMenu SetMemberInfo
	// ------------------------------------------------------------
	private void AddGroupMarkers()
	{
		if (!m_CurrentSelectedGroup)
			return;
		
		m_GroupMarkers.Clear();
		m_GroupMarkerEntries.Clear();
				
		array<ref ExpansionMarkerData> markers = new array<ref ExpansionMarkerData>;
		markers = m_CurrentSelectedGroup.GetAllMarkers();		
		for (int i = 0; i < markers.Count(); i++)
		{
			ExpansionMarkerData markerData = markers[i];
			ExpansionCOTGroupsMapMarker marker = new ExpansionCOTGroupsMapMarker(m_GroupMapPanel, m_GroupMap, markerData, this);
			m_GroupMarkers.Insert(marker);
			
			ExpansionCOTGroupsMarkerListEntry markerEntry = ExpansionCOTGroupsMarkerListEntry(m_MarkersListContent, markerData, this);
			m_GroupMarkerEntries.Insert(markerEntry);
		}
	}
	
	// ------------------------------------------------------------
	// ExpansionCOTGroupsMenu SetGroupMapFocus
	// ------------------------------------------------------------
	void SetGroupMapFocus(vector position)
	{
		vector mapPos = m_GroupMap.MapToScreen(position);
		float x, y;
		m_GroupMap.GetScreenPos(x, y);
		vector newPos = Vector(mapPos[0] + x, mapPos[1] + y, 0);
		m_GroupMap.SetScale(0.10);
		m_GroupMap.SetMapPos(newPos);
	}
#endif
	
	// ------------------------------------------------------------
	// ExpansionCOTGroupsMenu EditGroupName
	// ------------------------------------------------------------		
	private void EditGroupName()
	{
		if (!m_CurrentSelectedGroup)
			return;
		
		m_Module.EditGroupName(m_CurrentSelectedGroup.GetPartyID(), m_GroupNameValue.GetText());
	}
	
	// ------------------------------------------------------------
	// ExpansionCOTGroupsMenu ChangeGroupOwner
	// ------------------------------------------------------------	
	private void EditGroup()
	{
		if (!m_CurrentSelectedGroup)
			return;
		
		SetViewEdit();
		
		ExpansionDataCollectionModule dataModule = ExpansionDataCollectionModule.Cast(CF_ModuleCoreManager.Get(ExpansionDataCollectionModule));
		if (!dataModule)
			return;
		
		dataModule.RequestPlayerData();
		FillMemberList(false);
	}
	
	// ------------------------------------------------------------
	// ExpansionCOTGroupsMenu EditGroupMoney
	// ------------------------------------------------------------	
	private void EditGroupMoney()
	{
		if (!m_CurrentSelectedGroup)
			return;
		
		int value = OnlyNumbersCheck(m_GroupMoneyValue.GetText());
		if (value > -1)
			m_Module.ChangeMoney(m_CurrentSelectedGroup.GetPartyID(), value);
	}
	
	// ------------------------------------------------------------
	// ExpansionCOTGroupsMenu OnlyNumbersCheck
	// ------------------------------------------------------------	
	int OnlyNumbersCheck(string text)
	{
		TStringArray allNumbers = {"0","1","2","3","4","5","6","7","8","9"};
		for (int i = 0; i < text.Length(); i++)
		{
			if (allNumbers.Find(text.Get(i)) == -1)
			{
				return -1;
			}
		}

		return text.ToInt();
	}
	
	// ------------------------------------------------------------
	// ExpansionCOTGroupsMenu DeleteGroup
	// ------------------------------------------------------------	
	private void DeleteGroup()
	{
		if (!m_CurrentSelectedGroup)
			return;
		
		m_CurrentSelectedGroup = NULL;
		
		m_Module.DeleteGroup(m_CurrentSelectedGroup.GetPartyID());
		
		m_GroupInfoPanel.Show(false);
		m_GroupButtonsPanel.Show(false);
		m_MembersListPanel.Show(false);
	#ifdef EXPANSIONMODNAVIGATION
		m_GroupMapPanel.Show(false);
		m_MarkersListPanel.Show(false);
	#endif
	}
	
	// ------------------------------------------------------------
	// ExpansionCOTGroupsMenu UpdateMemberPerm
	// ------------------------------------------------------------	
	private void UpdateMemberPerm()
	{
		if (!m_CurrentSelectedGroup || !m_CurrentSelectedGroupMember)
			return;
		
		int perm = m_CurrentSelectedGroupMember.GetPermissions();		
		//! Edit
		if (m_MemberPermEdit.IsChecked())
			perm |= ExpansionPartyPlayerPermissions.CAN_EDIT;
		else
			perm &= ~ExpansionPartyPlayerPermissions.CAN_EDIT;
		
		//! Kick
		if (m_MemberPermKick.IsChecked())
			perm |= ExpansionPartyPlayerPermissions.CAN_KICK;
		else
			perm &= ~ExpansionPartyPlayerPermissions.CAN_KICK;
		
		//! Delete
		if (m_MemberPermDelete.IsChecked())
			perm |= ExpansionPartyPlayerPermissions.CAN_DELETE;
		else
			perm &= ~ExpansionPartyPlayerPermissions.CAN_DELETE;
		
		//! Invite
		if (m_MemberPermInvite.IsChecked())
			perm |= ExpansionPartyPlayerPermissions.CAN_INVITE;
		else
			perm &= ~ExpansionPartyPlayerPermissions.CAN_INVITE;
		
	#ifdef EXPANSIONMODMARKET
		//! Withdraw
		if (m_MemberPermWithdraw.IsChecked())
			perm |= ExpansionPartyPlayerPermissions.CAN_WITHDRAW_MONEY;
		else
			perm &= ~ExpansionPartyPlayerPermissions.CAN_WITHDRAW_MONEY;
	#endif
		
		string playerUID = m_CurrentSelectedGroupMember.GetID();
		int partyID = m_CurrentSelectedGroup.GetPartyID();
		m_Module.UpdatePermissions(playerUID, partyID, perm);
	}
	
	// ------------------------------------------------------------
	// ExpansionCOTGroupsMenu ChangeMemberToGroupOwner
	// ------------------------------------------------------------		
	void ChangeMemberToGroupOwner()
	{
		if (!m_CurrentSelectedGroupMember || !m_CurrentSelectedGroup)
			return;
		
		string playerUID = m_CurrentSelectedGroupMember.GetID();
		int partyID = m_CurrentSelectedGroup.GetPartyID();
		m_Module.ChangeOwner(playerUID, partyID, true);
	}
	
	// ------------------------------------------------------------
	// ExpansionCOTGroupsMenu ChangePlayerToGroupOwner
	// ------------------------------------------------------------		
	void ChangePlayerToGroupOwner()
	{
		if (!m_CurrentSelectedServerPlayer || !m_CurrentSelectedGroup)
			return;
		
		string playerUID = m_CurrentSelectedServerPlayer.PlayerUID;
		int partyID = m_CurrentSelectedGroup.GetPartyID();
		m_Module.ChangeOwner(playerUID, partyID, false);
	}
	
	// ------------------------------------------------------------
	// ExpansionCOTGroupsMenu ChangePlayerToGroupOwner
	// ------------------------------------------------------------		
	void CopyGroupInfo()
	{
		if (!m_CurrentSelectedGroup)
			return;
		
		string ownerUID = m_CurrentSelectedGroup.GetOwnerUID();
		string ownerName = m_CurrentSelectedGroup.GetOwnerName();
		string partyID = m_CurrentSelectedGroup.GetPartyID().ToString();
		GetGame().CopyToClipboard("[COT Expansion Group] Group ID: " + partyID + " | Owner: " + ownerName + " - " + ownerUID);
	}
	
	// ------------------------------------------------------------
	// ExpansionCOTGroupsMenu CopyMemberInfo
	// ------------------------------------------------------------		
	void CopyMemberInfo()
	{
		if (!m_CurrentSelectedGroupMember)
			return;
		
		bool isOwner = false;
		if (m_CurrentSelectedGroupMember.GetParty().GetOwnerUID() == m_CurrentSelectedGroupMember.GetID())
			isOwner = true;
		
		GetGame().CopyToClipboard("[COT Expansion Group] Group Member: " + m_CurrentSelectedGroupMember.GetName() + " [" + m_CurrentSelectedGroupMember.GetID() + "] | Group ID: " + m_CurrentSelectedGroupMember.GetParty().GetPartyID() + " | Permissions: " + m_CurrentSelectedGroupMember.GetPermissions() + " | Owner: " + isOwner);
	}
	
	// ------------------------------------------------------------
	// ExpansionCOTGroupsMenu InvitePlayer
	// ------------------------------------------------------------	
	void InvitePlayer(string playerUID)
	{
		if (!m_CurrentSelectedGroup)
			return;
		
		int partyID = m_CurrentSelectedGroup.GetPartyID();
		m_Module.InvitePlayer(playerUID, partyID);
	}
	
	// ------------------------------------------------------------
	// ExpansionCOTGroupsMenu KickMember
	// ------------------------------------------------------------
	void KickMember()
	{
		if (!m_CurrentSelectedGroupMember || !m_CurrentSelectedGroup)
			return;
		
		string playerUID = m_CurrentSelectedGroupMember.GetID();
		int partyID = m_CurrentSelectedGroup.GetPartyID();
		m_Module.KickMember(partyID, playerUID);
	}
	
	// ------------------------------------------------------------
	// ExpansionCOTGroupsMenu OnClick
	// ------------------------------------------------------------	
	override bool OnClick(Widget w, int x, int y, int button)
	{		
		if (w == m_GroupsListRefreshButton)
		{
			m_Module.RequestGroups(ExpansionCOTGroupsMenuCallback.GroupsUpdate);
			return true;
		}
		else if (w == m_GroupChangeNameButton)
		{
			EditGroupName();
			return true;
		}
		else if (w == m_GroupEditButton)
		{
			EditGroup();
			return true;
		}
		else if (w == m_GroupDeleteButton)
		{
			DeleteGroup();
			return true;
		}
		else if (w == m_GroupChangeMoneyButton)
		{
			EditGroupMoney();
			return true;
		}
		else if (w == m_PlayersListRefreshButton)
		{
			ExpansionDataCollectionModule dataModule = ExpansionDataCollectionModule.Cast(CF_ModuleCoreManager.Get(ExpansionDataCollectionModule));
			if (dataModule)
			{
				dataModule.RequestPlayerData();
				return true;
			}
		}
		else if (w == m_MemberSavePermButton)
		{
			UpdateMemberPerm();
			return true;
		}
		else if (w == m_MemberSetOwnerButton)
		{
			ChangeMemberToGroupOwner();
			return true;
		}
		else if (w == m_PlayerSetOwnerButton)
		{
			ChangePlayerToGroupOwner();
			return true;
		}
		else if (w == m_GroupCopyInfoButton)
		{
			CopyGroupInfo();
			return true;
		}
		else if (w == m_MemberCopyInfoButton)
		{
			CopyMemberInfo();
			return true;
		}
		else if (w == m_GroupInviteSelfButton)
		{
			InvitePlayer(GetGame().GetPlayer().GetIdentity().GetId());
			return true;
		}
		else if (w == m_PlayerInviteButton)
		{
			if (m_CurrentSelectedServerPlayer)
			{
				InvitePlayer(m_CurrentSelectedServerPlayer.PlayerUID);
				return true;
			}
		}
		else if (w == m_MemberEditCancelButton || w == m_PlayerEditCancelButton)
		{
			CancelEdit();
			m_Module.RequestGroups(ExpansionCOTGroupsMenuCallback.GroupUpdate);
			return true;
		}
		else if (w == m_MemberKickButton)
		{
			KickMember();
		}
		
		return false;
	}
	
	// ------------------------------------------------------------
	// ExpansionCOTGroupsMenu CancelEdit
	// ------------------------------------------------------------
	private void CancelEdit()
	{
		m_MemberInfoPanel.Show(false);
		m_MemberButtonsPanel.Show(false);
		m_MembersListPanel.Show(false);
		
		m_PlayerInfoPanel.Show(false);
		m_PlayerButtonsPanel.Show(false);
		m_PlayersListPanel.Show(false);
		
		m_EditPanel.Show(false);
		
		m_GroupsListPanel.Show(true);
		m_GroupInfoPanel.Show(true);
		m_GroupButtonsPanel.Show(true);
		m_MembersListPanel.Show(true);
	#ifdef EXPANSIONMODNAVIGATION
		m_GroupMapPanel.Show(true);
		m_MarkersListPanel.Show(true);
	#endif
	}
	
	// ------------------------------------------------------------
	// ExpansionCOTGroupsMenu OnShow
	// ------------------------------------------------------------
	override void OnShow()
	{
		super.OnShow();
		
		if (!m_Module)
			return;
				
		ExpansionDataCollectionModule dataModule = ExpansionDataCollectionModule.Cast(CF_ModuleCoreManager.Get(ExpansionDataCollectionModule));
		if (!dataModule)
			return;
		
		m_Module.GetModuleSI().Insert(MenuCallback);
		dataModule.GetModuleSI().Insert(OnPlayerDataRecived);
		m_Module.RequestGroups(ExpansionCOTGroupsMenuCallback.GroupsInit);
	}
	
	// ------------------------------------------------------------
	// ExpansionCOTGroupsMenu OnHide
	// ------------------------------------------------------------
	override void OnHide()
	{
		super.OnHide();
		
		if (!m_Module)
			return;
		
		ExpansionDataCollectionModule dataModule = ExpansionDataCollectionModule.Cast(CF_ModuleCoreManager.Get(ExpansionDataCollectionModule));
		if (!dataModule)
			return;
		
		m_Module.GetModuleSI().Remove(MenuCallback);		
		dataModule.GetModuleSI().Remove(OnPlayerDataRecived);
	}
	
	// ------------------------------------------------------------
	// ExpansionCOTGroupsMenu MenuCallback
	// ------------------------------------------------------------
	void MenuCallback(int type)
	{
		Print("ExpansionCOTGroupsMenu::MenuCallback - Start");
		Print("ExpansionCOTGroupsMenu::MenuCallback - Type: " + type);
		
		switch (type)
		{
			case ExpansionCOTGroupsMenuCallback.GroupsInit:
			{
				FillGroupList(true);
				break;
			}
			case ExpansionCOTGroupsMenuCallback.GroupsUpdate:
			{
				FillGroupList(false);
				if (m_CurrentSelectedGroup)				
					SetGroupInfo(m_CurrentSelectedGroup);
				break;
			}
			case ExpansionCOTGroupsMenuCallback.GroupUpdate:
			{
				CancelEdit();
				FillGroupList(false);
				if (m_CurrentSelectedGroup)				
					SetGroupInfo(m_CurrentSelectedGroup);
				break;
			}
			case ExpansionCOTGroupsMenuCallback.MemberUpdate:
			{
				CancelEdit();
				FillGroupList(false);
				if	(m_CurrentSelectedGroupMember)
					SetMemberInfo(m_CurrentSelectedGroupMember);
				break;
			}
		}
		
		Print("ExpansionCOTGroupsMenu::MenuCallback - End");
	}
	
	// ------------------------------------------------------------
	// ExpansionCOTGroupsMenu HighlightWidget
	// ------------------------------------------------------------		
	void HighlightWidget(Widget w)
	{		
		if (w.GetName().Contains("_button"))	
		{
			ButtonWidget button = ButtonWidget.Cast(w.FindAnyWidget(w.GetName())); 
			TextWidget label = TextWidget.Cast(w.FindAnyWidget(w.GetName() + "_label"));
			
			button.SetColor(ARGB(255, 255, 255, 255));
			label.SetColor(ARGB(255, 0, 0, 0));
		}	
	}
	
	// ------------------------------------------------------------
	// ExpansionCOTGroupsMenu NormaliseWidget
	// ------------------------------------------------------------	
	void NormaliseWidget(Widget w)
	{
		if (w.GetName().Contains("_button"))	
		{
			ButtonWidget button = ButtonWidget.Cast(w.FindAnyWidget(w.GetName() + "_button")); 
			TextWidget label = TextWidget.Cast(w.FindAnyWidget(w.GetName() + "_label"));
			
			button.SetColor(ARGB(255, 0, 0, 0));
			label.SetColor(ARGB(255, 255, 255, 255));
		}
	}
		
	// ------------------------------------------------------------
	// ExpansionCOTGroupsMenu OnPartyDataRecived
	// ------------------------------------------------------------	
	void OnPlayerDataRecived()
	{
		FillPlayerList();
	}
	
	// ------------------------------------------------------------
	// ExpansionCOTGroupsMenu OnMouseEnter
	// ------------------------------------------------------------
	override bool OnMouseEnter(Widget w, int x, int y)
	{	
		super.OnMouseEnter(w, x, y);
		
		HighlightWidget(w);
		
		return false;
	}
	
	// ------------------------------------------------------------
	// ExpansionCOTGroupsMenu OnMouseLeave
	// ------------------------------------------------------------
	override bool OnMouseLeave(Widget w, Widget enterW, int x, int y)
	{		
		super.OnMouseEnter(w, x, y);
		
		NormaliseWidget(w);
		
		return false;
	}
};
#endif
