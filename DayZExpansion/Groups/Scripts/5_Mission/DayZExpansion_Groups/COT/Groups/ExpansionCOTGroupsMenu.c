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
	MarkersUpdate
}

class ExpansionCOTGroupsMenu: JMFormBase
{
	private ExpansionCOTGroupModule m_Module;

	private ref array<ref ExpansionCOTGroupsListEntry> m_GroupEntries;
	private ref array<ref ExpansionCOTGroupsMembersListEntry> m_MemberEntries;
	private ref array<ref ExpansionCOTGroupsPlayersListEntry> m_ServerPlayers;

#ifdef EXPANSIONMODNAVIGATION
	private ExpansionMarkerModule m_MarkerModule;
	private ref array<ref ExpansionCOTGroupsMarkerListEntry> m_GroupMarkerEntries;
	private ref array<ref ExpansionCOTGroupsMapMarker> m_GroupMarkers;
	private ref array<ref ExpansionCOTGroupsIconListEntry> m_MarkerIcons;
	private ExpansionCOTGroupsMapMarker m_CurrentSelectedMarker;
	private ExpansionIcon m_CurrentSelectedIcon;
	private string m_CurrentIconPath;
	private int m_PrimaryColor;
	private bool m_IsEdittingMarker = false;
	private bool m_IsCreatingMarker = false;
	private int m_CreationPosX;
	private int m_CreationPosY;
#endif

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
	private TextWidget m_GroupOwnerNameValue;
	private TextWidget m_GroupOwnerUIDValue;

	private Widget m_GroupButtonsPanel;
	private ButtonWidget m_GroupDeleteButton;
	private ButtonWidget m_GroupInviteSelfButton;
	private ButtonWidget m_GroupEditButton;
	private ButtonWidget m_GroupCopyInfoButton;

#ifdef EXPANSIONMODNAVIGATION
	private Widget m_GroupMapPanel;
	private MapWidget m_GroupMap;

	private Widget m_MarkersListPanel;
	private GridSpacerWidget m_MarkersListContent;
	private ButtonWidget m_MarkersListRefreshButton;

	private Widget m_MarkerEditPanel;
	private SliderWidget m_ColorRedSlider;
	private SliderWidget m_ColorGreenSlider;
	private SliderWidget m_ColorBlueSlider;
	private WrapSpacerWidget m_MarkerIconsContent;
	private Widget m_Marker3DCheckboxPanel;
	private CheckBoxWidget m_Marker3DCheckbox;
	private CheckBoxWidget m_MarkerLockedCheckbox;
	private EditBoxWidget m_MarkerNameEditbox;
	private ButtonWidget m_MarkerSaveButton;
	private ButtonWidget m_MarkerCreateButton;
	private ButtonWidget m_MarkerDeleteButton;
	private ButtonWidget m_MarkerCancelButton;
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
		m_ServerPlayers = new array<ref ExpansionCOTGroupsPlayersListEntry>;

	#ifdef EXPANSIONMODNAVIGATION
		m_GroupMarkerEntries = new array<ref ExpansionCOTGroupsMarkerListEntry>;
		m_GroupMarkers  = new array<ref ExpansionCOTGroupsMapMarker>;
		m_MarkerIcons = new array<ref ExpansionCOTGroupsIconListEntry>;
		CF_Modules<ExpansionMarkerModule>.Get(m_MarkerModule);
	#endif

		ExpansionDataCollectionModule dataModule = ExpansionDataCollectionModule.Cast(CF_ModuleCoreManager.Get(ExpansionDataCollectionModule));
		if (!dataModule)
			return;

		m_Module.m_COTGroupModuleSI.Insert(MenuCallback);
		dataModule.m_ModuleSI.Insert(OnPlayerDataRecived);
	}

	// ------------------------------------------------------------
	// ExpansionCOTGroupsMenu Constructor
	// ------------------------------------------------------------
	void ~ExpansionCOTGroupsMenu()
	{
		m_GroupEntries.Clear();
		m_MemberEntries.Clear();
		m_ServerPlayers.Clear();

	#ifdef EXPANSIONMODNAVIGATION
		m_GroupMarkerEntries.Clear();
		m_GroupMarkers.Clear();
		m_MarkerIcons.Clear();
	#endif

		if (m_Module)
			m_Module.m_COTGroupModuleSI.Remove(MenuCallback);

		ExpansionDataCollectionModule dataModule = ExpansionDataCollectionModule.Cast(CF_ModuleCoreManager.Get(ExpansionDataCollectionModule));
		if (dataModule)
			dataModule.m_ModuleSI.Remove(OnPlayerDataRecived);

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
		m_GroupsListRefreshButton = ButtonWidget.Cast(layoutRoot.FindAnyWidget("groups_refresh_button"));

		//! Group info panel
		m_GroupInfoPanel = Widget.Cast(layoutRoot.FindAnyWidget("group_info_panel"));
		m_GroupNameLabel = TextWidget.Cast(layoutRoot.FindAnyWidget("info_name_label"));
		m_GroupNameValue = EditBoxWidget.Cast(layoutRoot.FindAnyWidget("info_name_value"));
		m_GroupChangeNameButton = ButtonWidget.Cast(layoutRoot.FindAnyWidget("info_change_name_button"));
		m_GroupIDValue = TextWidget.Cast(layoutRoot.FindAnyWidget("info_id_value"));
		m_GroupOwnerNameValue = TextWidget.Cast(layoutRoot.FindAnyWidget("group_owner info_name_value"));
		m_GroupOwnerUIDValue = TextWidget.Cast(layoutRoot.FindAnyWidget("group_owner uid_name_value"));

		//! Group buttons panel
		m_GroupButtonsPanel = Widget.Cast(layoutRoot.FindAnyWidget("group_buttons_panel"));
		m_GroupCopyInfoButton = ButtonWidget.Cast(layoutRoot.FindAnyWidget("copy_group_info_button"));
		m_GroupDeleteButton = ButtonWidget.Cast(layoutRoot.FindAnyWidget("delete_group_button"));
		m_GroupInviteSelfButton = ButtonWidget.Cast(layoutRoot.FindAnyWidget("invite_self_button"));
		m_GroupEditButton = ButtonWidget.Cast(layoutRoot.FindAnyWidget("group_edit_button"));

		//! Members list
		m_MembersListPanel = Widget.Cast(layoutRoot.FindAnyWidget("members_list_panel"));
		m_MembersListContent = GridSpacerWidget.Cast(layoutRoot.FindAnyWidget("members_list_content"));
		m_MembersListRefreshButton = ButtonWidget.Cast(layoutRoot.FindAnyWidget("members_refresh_button"));

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

	#ifdef EXPANSIONMODNAVIGATION
		//! Map
		m_GroupMapPanel = Widget.Cast(layoutRoot.FindAnyWidget("map_window_panel"));
		m_GroupMap = MapWidget.Cast(layoutRoot.FindAnyWidget("group_map"));

		//! Markers list
		m_MarkersListPanel = Widget.Cast(layoutRoot.FindAnyWidget("markers_list_panel"));
		m_MarkersListContent = GridSpacerWidget.Cast(layoutRoot.FindAnyWidget("markers_list_content"));
		m_MarkersListRefreshButton = ButtonWidget.Cast(layoutRoot.FindAnyWidget("markers_refresh_button"));

		//! Marker edit panel
		m_MarkerEditPanel = Widget.Cast(layoutRoot.FindAnyWidget("marker_edit_panel"));
		m_ColorRedSlider = SliderWidget.Cast(layoutRoot.FindAnyWidget("marker_color_red_slider"));
		m_ColorGreenSlider = SliderWidget.Cast(layoutRoot.FindAnyWidget("marker_color_green_slider"));
		m_ColorBlueSlider = SliderWidget.Cast(layoutRoot.FindAnyWidget("marker_color_blue_slider"));
		m_MarkerIconsContent = WrapSpacerWidget.Cast(layoutRoot.FindAnyWidget("marker_icons_content"));
		m_Marker3DCheckboxPanel = Widget.Cast(layoutRoot.FindAnyWidget("marker_state_3d_container"));
		m_Marker3DCheckbox = CheckBoxWidget.Cast(layoutRoot.FindAnyWidget("marker_state_3d_checkbox"));
		m_MarkerLockedCheckbox = CheckBoxWidget.Cast(layoutRoot.FindAnyWidget("marker_state_lock_checkbox"));
		m_MarkerNameEditbox = EditBoxWidget.Cast(layoutRoot.FindAnyWidget("marker_name"));
		m_MarkerSaveButton = ButtonWidget.Cast(layoutRoot.FindAnyWidget("marker_save_button"));
		m_MarkerCreateButton = ButtonWidget.Cast(layoutRoot.FindAnyWidget("marker_create_button"));
		m_MarkerDeleteButton = ButtonWidget.Cast(layoutRoot.FindAnyWidget("marker_delete_button"));
		m_MarkerCancelButton = ButtonWidget.Cast(layoutRoot.FindAnyWidget("marker_cancel_button"));
	#endif

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

		//! Edit panel
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
		m_MarkerEditPanel.Show(false);
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
		m_MarkerEditPanel.Show(false);
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

		map<int, ref ExpansionPartyData> groups = m_Module.GetAllGroups();
		foreach (int partyID, ExpansionPartyData group: groups)
		{
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

		map <string, ref ExpansionPlayerDataCollection> players = dataCollection.GetAllPlayers();
		foreach (string playerUID, ExpansionPlayerDataCollection player: players)
		{
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
		m_GroupOwnerNameValue.SetText(group.GetOwnerName());
		m_GroupOwnerUIDValue.SetText(group.GetOwnerUID());

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

#ifdef EXPANSIONMODMARKET
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
#endif

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
		m_MarkerEditPanel.Show(false);
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

		if (m_CurrentSelectedGroup.GetAllMarkers().Count() == 0)
			return;

		for (int i = 0; i < m_CurrentSelectedGroup.GetAllMarkers().Count(); i++)
		{
			ExpansionMarkerData markerData = m_CurrentSelectedGroup.GetAllMarkers()[i];
			ExpansionCOTGroupsMapMarker marker = new ExpansionCOTGroupsMapMarker(m_GroupMapPanel, m_GroupMap, false);			
			marker.Init();
			m_GroupMarkers.Insert(marker);
			marker.SetCOTGroupsMenu(this);
			marker.SetMarkerData(markerData);
			marker.Show();

			ExpansionCOTGroupsMarkerListEntry markerEntry = ExpansionCOTGroupsMarkerListEntry(m_MarkersListContent, marker, this);
			m_GroupMarkerEntries.Insert(markerEntry);
		}
	}

	// ------------------------------------------------------------
	// ExpansionCOTGroupsMenu EditMarker
	// ------------------------------------------------------------
	void EditMarker(ExpansionCOTGroupsMapMarker marker)
	{
		ResetMarkerEditView();

		m_CurrentSelectedMarker = marker;
		m_MarkerIcons.Clear();
		SetIsEdittingMarker(true);
		m_MarkerEditPanel.Show(true);

		if (!IsCreatingMarker())
		{
			m_MarkerCreateButton.Show(false);
			m_MarkerSaveButton.Show(true);
			
			m_CurrentSelectedIcon = m_CurrentSelectedMarker.GetMarkerData().GetExpansionIcon();
			SetPrimaryMarkerColor(m_CurrentSelectedMarker.GetMarkerData().GetColor());
			m_Marker3DCheckboxPanel.Show(GetExpansionSettings().GetMap().CanCreate3DMarker);
			m_Marker3DCheckbox.SetChecked(m_CurrentSelectedMarker.GetMarkerData().Is3D());
			m_MarkerLockedCheckbox.SetChecked(m_CurrentSelectedMarker.GetMarkerData().GetLockState());
			m_MarkerNameEditbox.SetText(m_CurrentSelectedMarker.GetMarkerData().GetName());
		}
		else
		{
			m_MarkerCreateButton.Show(true);
			m_MarkerSaveButton.Show(false);
			m_CurrentSelectedIcon = ExpansionIcons.Get(0);
		}

		array<ExpansionIcon> iconsSorted = ExpansionIcons.Sorted();
		for (int i = 0; i < iconsSorted.Count(); ++i)
		{
			ExpansionCOTGroupsIconListEntry iconEntry = new ExpansionCOTGroupsIconListEntry(m_MarkerIconsContent, i, this);
			if (!IsCreatingMarker())
				iconEntry.SetColor(m_CurrentSelectedMarker.GetMarkerData().GetColor());
			else
				iconEntry.SetColor(ARGB(255, 255, 255, 255));
			
			m_MarkerIcons.Insert(iconEntry);
		}
	}

	// ------------------------------------------------------------
	// ExpansionCOTGroupsMenu SetPrimaryMarkerColor
	// ------------------------------------------------------------
	void SetPrimaryMarkerColor(int color)
	{
		if (!m_CurrentSelectedMarker)
			return;

		m_PrimaryColor = color;

		int a = (color >> 24) & 0xFF;
		int r = (color >> 16) & 0xFF;
		int g = (color >> 8) & 0xFF;
		int b = (color)&0xFF;

		m_ColorRedSlider.SetCurrent(r);
		m_ColorGreenSlider.SetCurrent(g);
		m_ColorBlueSlider.SetCurrent(b);

		m_CurrentSelectedMarker.SetPrimaryColor(m_PrimaryColor);

		for (int i = 0; i < m_MarkerIcons.Count(); ++i)
		{
			m_MarkerIcons[i].SetColor(m_PrimaryColor);
		}
	}
	
	// ------------------------------------------------------------
	// ExpansionCOTGroupsMenu SetSelectedIcon
	// ------------------------------------------------------------
	void SetSelectedIcon(int iconIndex)
	{
		m_CurrentSelectedIcon = ExpansionIcons.Get(iconIndex);
		m_CurrentSelectedMarker.SetIcon(m_CurrentSelectedIcon);
	}	
	
	// ------------------------------------------------------------
	// ExpansionCOTGroupsMenu SaveMarker
	// ------------------------------------------------------------
	void SaveMarker()
	{
		if (!m_CurrentSelectedMarker || !m_MarkerModule)
			return;

		if (m_MarkerNameEditbox.GetName() != string.Empty)
			m_CurrentSelectedMarker.GetMarkerData().SetName(m_MarkerNameEditbox.GetText());

		m_CurrentSelectedMarker.GetMarkerData().SetIcon(m_CurrentSelectedIcon);
		m_CurrentSelectedMarker.GetMarkerData().SetColor(m_PrimaryColor);
		m_CurrentSelectedMarker.GetMarkerData().SetPosition(m_CurrentSelectedMarker.GetPosition());
		m_CurrentSelectedMarker.GetMarkerData().Set3D(m_Marker3DCheckbox.IsChecked());
		m_CurrentSelectedMarker.GetMarkerData().SetLockState(m_MarkerLockedCheckbox.IsChecked());

		if (m_MarkerModule.UpdateMarker(m_CurrentSelectedMarker.GetMarkerData()))
			m_Module.RequestGroups(ExpansionCOTGroupsMenuCallback.MarkersUpdate);
	}

	// ------------------------------------------------------------
	// ExpansionCOTGroupsMenu DeleteMarker
	// ------------------------------------------------------------
	void DeleteMarker()
	{
		if (!m_CurrentSelectedMarker || !m_MarkerModule)
			return;
		
		if (!IsCreatingMarker())
		{
			string uid = m_CurrentSelectedMarker.GetMarkerData().GetUID();
			if (!m_MarkerModule.GetData().PartyRemove(uid))
				return;
		}

		int i;
		for (i = 0; i < m_GroupMarkerEntries.Count(); ++i)
		{
			if (m_GroupMarkerEntries[i].GetMarker().GetMarkerData().GetUID() == uid)
				m_GroupMarkerEntries.Remove(i);
		}

		for (i = 0; i < m_GroupMarkers.Count(); ++i)
		{
			if (m_GroupMarkers[i].GetMarkerData().GetUID() == uid)
				m_GroupMarkers.Remove(i);
		}

		m_Module.RequestGroups(ExpansionCOTGroupsMenuCallback.GroupUpdate);
		CancelMarkerEdit();
	}

	// ------------------------------------------------------------
	// ExpansionCOTGroupsMenu CancelMarkerEdit
	// ------------------------------------------------------------
	void CancelMarkerEdit()
	{
		if (IsCreatingMarker())
		{
			DeleteMarker(m_CurrentSelectedMarker);
		}

		m_CurrentSelectedMarker = NULL;
		SetIsEdittingMarker(false);
		m_MarkerEditPanel.Show(false);
		SetGroupInfo(m_CurrentSelectedGroup);
	}

	// ------------------------------------------------------------
	// ExpansionCOTGroupsMenu DeleteMarker
	// ------------------------------------------------------------
	void DeleteMarker(ExpansionCOTGroupsMapMarker marker)
	{
		if (marker == NULL)
			return;

		if (marker.GetMarkerData() != NULL)
		{
			string uid = marker.GetMarkerData().GetUID();
			for (int i = 0; i < m_GroupMarkers.Count(); ++i)
			{
				if (m_GroupMarkers[i].GetMarkerData().GetUID() == uid)
				{
					m_GroupMarkers.Remove(i);
					m_MarkerModule.GetData().PartyRemove(uid);
					continue;
				}
			}
		}
	}

	// ------------------------------------------------------------
	// ExpansionCOTGroupsMenu ResetMarkerEditView
	// ------------------------------------------------------------
	void ResetMarkerEditView()
	{
		m_Marker3DCheckbox.SetChecked(false);
		m_MarkerLockedCheckbox.SetChecked(false);
		m_PrimaryColor = ARGB(255, 255, 255, 255);
		m_CurrentSelectedIcon = NULL;
		m_MarkerNameEditbox.SetText("");
	}

	// ------------------------------------------------------------
	// ExpansionCOTGroupsMenu CreateNewMarker
	// ------------------------------------------------------------
	void CreateNewMarker(int x, int y)
	{
		SetIsCreatingMarker(true);

		m_CreationPosX = x;
		m_CreationPosY = y;

		ExpansionCOTGroupsMapMarker marker = new ExpansionCOTGroupsMapMarker(m_GroupMapPanel, m_GroupMap, false);
		marker.Init();

		marker.SetCreation(true);
		marker.SetPosition(m_CreationPosX, m_CreationPosY);
		marker.SetIcon(ExpansionIcons.Get(0));
		marker.SetCOTGroupsMenu(this);

		m_GroupMarkers.Insert(marker);

		m_CurrentSelectedMarker = marker;

		EditMarker(m_CurrentSelectedMarker);
	}

	// ------------------------------------------------------------
	// ExpansionCOTGroupsMenu OnCreateNewMarkerButtonClick
	// ------------------------------------------------------------
	void OnCreateNewMarkerButtonClick()
	{
		if (!m_CurrentSelectedMarker || !m_CurrentSelectedGroup)
			return;

		ExpansionMarkerData markerData = ExpansionMarkerData.Create(ExpansionMapMarkerType.PARTY);
		markerData.SetName(m_MarkerNameEditbox.GetText());
		markerData.SetIcon(m_CurrentSelectedIcon);
		markerData.SetColor(m_PrimaryColor);
		markerData.SetPosition(m_CurrentSelectedMarker.GetPosition());
		markerData.Set3D(m_Marker3DCheckbox.IsChecked());
		markerData.SetLockState(m_MarkerLockedCheckbox.IsChecked());
		m_CurrentSelectedMarker.SetMarkerData(markerData);

		m_Module.CreateGroupMarker(markerData, m_CurrentSelectedGroup.GetPartyID());

		SetIsCreatingMarker(false);
		CancelMarkerEdit();
	}

	// ------------------------------------------------------------
	// ExpansionCOTGroupsMenu IsRGBSlider
	// ------------------------------------------------------------
	bool IsRGBSlider(Widget w)
	{
		return (w == m_ColorRedSlider || w == m_ColorGreenSlider || w == m_ColorBlueSlider);
	}

	// ------------------------------------------------------------
	// ExpansionCOTGroupsMenu OnClick
	// ------------------------------------------------------------
	override bool OnChange(Widget w, int x, int y, bool finished)
	{
		if (w != NULL && IsRGBSlider(w))
		{
			SetPrimaryMarkerColor(ARGB(255, m_ColorRedSlider.GetCurrent(), m_ColorGreenSlider.GetCurrent(), m_ColorBlueSlider.GetCurrent()));
			return true;
		}

		return super.OnChange(w, x, y, finished);
	}

	// ------------------------------------------------------------
	// Override OnDoubleClick
	// ------------------------------------------------------------
	override bool OnDoubleClick( Widget w, int x, int y, int button )
	{
		if (button == MouseState.LEFT)
		{
			if (w == m_GroupMap && !IsEdittingMarker() && !IsCreatingMarker())
			{
				int mouse_x, mouse_y;
				GetGame().GetMousePos(mouse_x, mouse_y);
				CreateNewMarker(mouse_x, mouse_y);
				return true;
			}
		}
		return false;
	}

	// ------------------------------------------------------------
	// ExpansionCOTGroupsMenu IsEdittingMarker
	// ------------------------------------------------------------
	bool IsEdittingMarker()
	{
		return m_IsEdittingMarker;
	}

	// ------------------------------------------------------------
	// ExpansionCOTGroupsMenu SetIsEdittingMarker
	// ------------------------------------------------------------
	void SetIsEdittingMarker(bool state)
	{
		m_IsEdittingMarker = state;
	}

	// ------------------------------------------------------------
	// ExpansionCOTGroupsMenu IsCreatingMarker
	// ------------------------------------------------------------
	bool IsCreatingMarker()
	{
		return m_IsCreatingMarker;
	}

	// ------------------------------------------------------------
	// ExpansionCOTGroupsMenu SetIsCreatingMarker
	// ------------------------------------------------------------
	void SetIsCreatingMarker(bool state)
	{
		m_IsCreatingMarker = state;
	}
#endif

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
		else if (w == m_PlayersListRefreshButton)
		{
			ExpansionDataCollectionModule dataModule = ExpansionDataCollectionModule.Cast(CF_ModuleCoreManager.Get(ExpansionDataCollectionModule));
			if (dataModule)
			{
				dataModule.RequestPlayerData();
				return true;
			}
		}
		else if (w == m_MembersListRefreshButton)
		{
			m_Module.RequestGroups(ExpansionCOTGroupsMenuCallback.GroupUpdate);
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
			return true;
		}

	#ifdef EXPANSIONMODMARKET
		if (w == m_GroupChangeMoneyButton)
		{
			EditGroupMoney();
			return true;
		}
	#endif

	#ifdef EXPANSIONMODNAVIGATION
		if (w == m_MarkerSaveButton)
		{
			SaveMarker();
			return true;
		}
		else if (w == m_MarkerDeleteButton)
		{
			DeleteMarker();
			return true;
		}
		else if (w == m_MarkerCancelButton)
		{
			CancelMarkerEdit();
			return true;
		}
		else if (w == m_MarkerCreateButton)
		{
			OnCreateNewMarkerButtonClick();
			return true;
		}
		else if (w == m_MarkersListRefreshButton)
		{
			m_Module.RequestGroups(ExpansionCOTGroupsMenuCallback.GroupUpdate);
			return true;
		}
	#endif

		return super.OnClick(w, x, y, button);
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

		m_Module.m_COTGroupModuleSI.Insert(MenuCallback);
		dataModule.m_ModuleSI.Insert(OnPlayerDataRecived);
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

		m_Module.m_COTGroupModuleSI.Remove(MenuCallback);
		dataModule.m_ModuleSI.Remove(OnPlayerDataRecived);
	}

	// ------------------------------------------------------------
	// ExpansionCOTGroupsMenu MenuCallback
	// ------------------------------------------------------------
	void MenuCallback(int type)
	{
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
		#ifdef EXPANSIONMODNAVIGATION
			case ExpansionCOTGroupsMenuCallback.MarkersUpdate:
			{
				FillGroupList(false);
				if (m_CurrentSelectedGroup)
					SetGroupInfo(m_CurrentSelectedGroup);
				break;
			}
		#endif
		}
	}

	// ------------------------------------------------------------
	// ExpansionCOTGroupsMenu HighlightWidget
	// ------------------------------------------------------------
	void HighlightWidget(Widget w)
	{
		if (w.GetName().Contains("_button"))
		{
			ButtonWidget button = ButtonWidget.Cast(w);
			if (button)
				button.SetColor(ARGB(255, 255, 255, 255));

			TextWidget label = TextWidget.Cast(w.FindAnyWidget(w.GetName() + "_label"));
			if (label)
				label.SetColor(ARGB(255, 0, 0, 0));
		}

		if (w.GetName().Contains("_checkbox"))
		{
			CheckBoxWidget checkbox = CheckBoxWidget.Cast(w);
			if (checkbox)
				checkbox.SetColor(ARGB(255, 41, 128, 185));
		}
	}

	// ------------------------------------------------------------
	// ExpansionCOTGroupsMenu NormaliseWidget
	// ------------------------------------------------------------
	void NormaliseWidget(Widget w)
	{
		if (w.GetName().Contains("_button"))
		{
			ButtonWidget button = ButtonWidget.Cast(w);
			if (button)
				button.SetColor(ARGB(255, 0, 0, 0));

			TextWidget label = TextWidget.Cast(w.FindAnyWidget(w.GetName() + "_label"));
			if (label)
				label.SetColor(ARGB(255, 255, 255, 255));
		}

		if (w.GetName().Contains("_checkbox"))
		{
			CheckBoxWidget checkbox = CheckBoxWidget.Cast(w);
			if (checkbox)
				checkbox.SetColor(ARGB(255, 255, 255, 255));
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
	// ExpansionCOTGroupsMenu IsFocusWidget
	// ------------------------------------------------------------
	bool IsFocusWidget(Widget w)
	{
		if (w == m_GroupsListRefreshButton || w == m_GroupChangeNameButton || w == m_GroupCopyInfoButton || w == m_GroupDeleteButton || w == m_GroupInviteSelfButton || w == m_GroupEditButton || w == m_MembersListRefreshButton)
			return true;

		if (w == m_MemberPermInvite || w == m_MemberPermKick || w == m_MemberPermEdit || w == m_MemberPermDelete)
			return true;

		if (w == m_MemberSavePermButton || w == m_MemberSetOwnerButton || w == m_MemberKickButton || w == m_MemberEditCancelButton || w == m_MemberCopyInfoButton)
			return true;

		if (w == m_PlayersListRefreshButton || w == m_PlayerInviteButton || w == m_PlayerSetOwnerButton || w == m_PlayerEditCancelButton)
			return true;
	#ifdef EXPANSIONMODNAVIGATION
		if (w == m_MarkerSaveButton || w == m_MarkerDeleteButton || w == m_MarkerCancelButton || w == m_MarkerCreateButton || w == m_MarkersListRefreshButton)
			return true;
	#endif

	#ifdef EXPANSIONMODMARKET
		if (w == m_MemberPermWithdraw || w == m_GroupChangeMoneyButton)
			return true;
	#endif

		return false;
	}

	// ------------------------------------------------------------
	// ExpansionCOTGroupsMenu OnMouseEnter
	// ------------------------------------------------------------
	override bool OnMouseEnter(Widget w, int x, int y)
	{
		super.OnMouseEnter(w, x, y);

		if (IsFocusWidget(w))
		{
			HighlightWidget(w);
			return true;
		}

		return false;
	}

	// ------------------------------------------------------------
	// ExpansionCOTGroupsMenu OnMouseLeave
	// ------------------------------------------------------------
	override bool OnMouseLeave(Widget w, Widget enterW, int x, int y)
	{
		super.OnMouseEnter(w, x, y);

		if (IsFocusWidget(w))
		{
			NormaliseWidget(w);
			return true;
		}

		return false;
	}

#ifdef EXPANSIONMODNAVIGATION
	// ------------------------------------------------------------
	// ExpansionCOTGroupsMenu OnMouseLeave
	// ------------------------------------------------------------
	override void Update()
	{
		//! Update group map markers
		for (int i = 0; i < m_GroupMarkers.Count(); ++i)
		{
			if (m_GroupMarkers[i])
			{
				m_GroupMarkers[i].Update(0.1);
			}
		}
	}
#endif
};
#endif
