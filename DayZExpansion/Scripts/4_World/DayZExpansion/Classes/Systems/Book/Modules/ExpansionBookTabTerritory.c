/**
 * ExpansionBookTabTerritory.c
 *
 * DayZ Expansion Mod
 * www.dayzexpansion.com
 * © 2020 DayZ Expansion Mod Team
 *
 * This work is licensed under the Creative Commons Attribution-NonCommercial-NoDerivatives 4.0 International License. 
 * To view a copy of this license, visit http://creativecommons.org/licenses/by-nc-nd/4.0/.
 *
*/

class ExpansionBookTabTerritory extends ExpansionBookTabBase
{
	protected int COLOR_EXPANSION_TERRITORY_BOOKMARK_ICON = ARGB( 255, 52, 73, 94 );
	
	protected Widget m_territoryPanel;
	protected ButtonWidget m_kickMemberButton;
	protected ButtonWidget m_addMemberButton;
	protected ButtonWidget m_searchPlayerButton;
	protected ButtonWidget m_promotePlayerButton;
	protected ButtonWidget m_demotePlayerButton;
	protected ButtonWidget m_deleteTerritoryButton;
	
	protected Widget m_playerListPanel;
	protected GridSpacerWidget m_playerListGrid;
	protected TextWidget m_playersListInfo;
	
	protected Widget m_membersListPanel;
	protected GridSpacerWidget m_membersListGrid;
	protected TextWidget m_membersListInfo;
	
	protected Widget m_deleteTerritoryPanel;
	protected TextWidget m_deleteTerritoryLabel;
	
	protected Widget m_popupPanel;
	protected TextWidget m_popupPanelTitel;
	protected TextWidget m_popupPanelText;
	protected ButtonWidget m_confirmButton;
	protected ButtonWidget m_cancelButton;
	
	protected Widget m_memberEditPanel;
	protected TextWidget m_memberName;
	protected TextWidget m_memberRank;
	protected ImageWidget m_memberStateIcon;
	protected TextWidget m_memberState;
	protected PlayerPreviewWidget m_memberPreview;
	protected ButtonWidget m_memberPromoteButton;
	protected TextWidget m_memberPromoteButtonLabel;
	protected ButtonWidget m_memberDemoteButton;
	protected TextWidget m_memberDemoteButtonLabel;
	protected ButtonWidget m_memberKickButton;
	protected TextWidget m_memberKickButtonLabel;
	protected ButtonWidget m_memberCancelButton;
	protected TextWidget m_memberCancelButtonLabel;
	
	protected Widget m_territoryInviteListPanel;
	protected TextWidget m_territoryInviteListInfo;
	protected GridSpacerWidget m_territoryInviteListGrid;
	
	protected Widget m_PlayerListFilerPanel;
	protected EditBoxWidget m_PlayerListFilterEditbox;
	protected ButtonWidget m_PlayerListFilterClearButton;
	
	protected ref ExpansionTerritoryModule m_TerritoryModule;
	protected ExpansionTerritoryFlag m_CurrentFlag;
	protected ref ExpansionTerritory m_CurrentTerritory;
	protected ref array<ref ExpansionTerritoryTabPlayerListEntry> m_PlayerListEntrys;
	protected ref array<ref ExpansionTerritoryTabMemberListEntry> m_MemberListEntrys;
	protected ref array<ref ExpansionTerritoryTabInviteListEntry> m_InviteListEntrys;
	
	protected ref ExpansionTerritoryMember m_CurrentMember;
	protected ref ExpansionTerritoryMember m_MyMember;
	
	// ------------------------------------------------------------
	// ExpansionBookTabTerritory Contructor
	// ------------------------------------------------------------
	void ExpansionBookTabTerritory()
	{
		SetButtonIconPath( "set:expansion_iconset image:icon_territory" );
		SetButtonText( "TERRITORY" );
		SetButtonColor( COLOR_EXPANSION_TERRITORY_BOOKMARK_ICON );
		SetLayoutPath( "DayZExpansion/GUI/layouts/book/expansion_book_page_territory.layout" );

		Class.CastTo( m_TerritoryModule, GetModuleManager().GetModule( ExpansionTerritoryModule ) );
		Class.CastTo( m_Player, GetGame().GetPlayer() );
		
		m_PlayerListEntrys = new array<ref ExpansionTerritoryTabPlayerListEntry>;
		m_MemberListEntrys = new array<ref ExpansionTerritoryTabMemberListEntry>;
		m_InviteListEntrys = new array<ref ExpansionTerritoryTabInviteListEntry>;
	}

	// ------------------------------------------------------------
	// Override OnCreatedLayout
	// ------------------------------------------------------------
	//Layout root var is m_RootLayout
	override void OnCreatedLayout()
	{
		m_territoryPanel			= Widget.Cast( m_RootLayout.FindAnyWidget( "book_territory_tab" ) );
		m_kickMemberButton			= ButtonWidget.Cast( m_RootLayout.FindAnyWidget( "sticky_kick_button" ) );
		m_addMemberButton			= ButtonWidget.Cast( m_RootLayout.FindAnyWidget( "sticky_invite_button" ) );
		m_searchPlayerButton		= ButtonWidget.Cast( m_RootLayout.FindAnyWidget( "sticky_search_button" ) );
		m_promotePlayerButton		= ButtonWidget.Cast( m_RootLayout.FindAnyWidget( "sticky_promote_button" ) );
		m_demotePlayerButton		= ButtonWidget.Cast( m_RootLayout.FindAnyWidget( "sticky_demote_button" ) );
		m_deleteTerritoryPanel		= Widget.Cast( m_RootLayout.FindAnyWidget( "sticky_delete_content" ) );
		m_deleteTerritoryButton		= ButtonWidget.Cast( m_RootLayout.FindAnyWidget( "sticky_delete_button" ) );
		m_deleteTerritoryLabel		= TextWidget.Cast( m_RootLayout.FindAnyWidget( "sticky_delete_button_label" ) );
		
		m_playerListPanel			= Widget.Cast( m_RootLayout.FindAnyWidget( "player_list_scroller" ) );
		m_playerListGrid			= GridSpacerWidget.Cast( m_RootLayout.FindAnyWidget( "player_list_content" ) );
		m_playersListInfo			= TextWidget.Cast( m_RootLayout.FindAnyWidget( "player_list_info" ) );
		
		m_membersListPanel			= Widget.Cast( m_RootLayout.FindAnyWidget( "member_list_scroller" ) );
		m_membersListGrid			= GridSpacerWidget.Cast( m_RootLayout.FindAnyWidget( "member_list_content" ) );
		m_membersListInfo			= TextWidget.Cast( m_RootLayout.FindAnyWidget( "member_list_info" ) );
		
		m_popupPanel				= Widget.Cast( m_RootLayout.FindAnyWidget( "book_dialog" ) );
		m_popupPanelTitel			= TextWidget.Cast( m_RootLayout.FindAnyWidget( "book_dialog_title" ) );
		m_popupPanelText			= TextWidget.Cast( m_RootLayout.FindAnyWidget( "book_dialog_text" ) );
		m_confirmButton				= ButtonWidget.Cast( m_RootLayout.FindAnyWidget( "book_dialog_ok_button" ) );
		m_cancelButton				= ButtonWidget.Cast( m_RootLayout.FindAnyWidget( "book_dialog_cancel_button" ) );
		
		m_memberEditPanel			= Widget.Cast( m_RootLayout.FindAnyWidget( "book_member_edit_tab" ) );
		m_memberName				= TextWidget.Cast( m_RootLayout.FindAnyWidget( "data_name_value" ) );
		m_memberRank				= TextWidget.Cast( m_RootLayout.FindAnyWidget( "data_rank_value" ) );
		m_memberStateIcon			= ImageWidget.Cast( m_RootLayout.FindAnyWidget( "data_status_icon" ) );
		m_memberState				= TextWidget.Cast( m_RootLayout.FindAnyWidget( "data_status_value" ) );
		m_memberPreview				= PlayerPreviewWidget.Cast( m_RootLayout.FindAnyWidget( "book_member_preview" ) );
		
		m_memberPromoteButton		= ButtonWidget.Cast( m_RootLayout.FindAnyWidget( "option_promote_button" ) );
		m_memberPromoteButtonLabel	= TextWidget.Cast( m_RootLayout.FindAnyWidget( "option_promote_lable" ) );
		
		m_memberDemoteButton		= ButtonWidget.Cast( m_RootLayout.FindAnyWidget( "option_demote_button" ) );
		m_memberDemoteButtonLabel	= TextWidget.Cast( m_RootLayout.FindAnyWidget( "option_demote_lable" ) );
		
		m_memberKickButton			= ButtonWidget.Cast( m_RootLayout.FindAnyWidget( "option_kick_button" ) );
		m_memberKickButtonLabel		= TextWidget.Cast( m_RootLayout.FindAnyWidget( "option_kick_lable" ) );
		
		m_memberCancelButton		= ButtonWidget.Cast( m_RootLayout.FindAnyWidget( "option_cancel_button" ) );
		m_memberCancelButtonLabel	= TextWidget.Cast( m_RootLayout.FindAnyWidget( "option_cancel_lable" ) );
		
		m_territoryInviteListPanel 	= Widget.Cast( m_RootLayout.FindAnyWidget( "book_invites_panel" ) );
		m_territoryInviteListGrid 	= GridSpacerWidget.Cast( m_RootLayout.FindAnyWidget( "territory_invites_list_content" ) );
		m_territoryInviteListInfo	= TextWidget.Cast( m_RootLayout.FindAnyWidget( "territory_invites_list_info" ) );
		
		m_PlayerListFilerPanel			= Widget.Cast( m_RootLayout.FindAnyWidget( "player_list_filter" ) );
		m_PlayerListFilterEditbox		= EditBoxWidget.Cast( m_RootLayout.FindAnyWidget( "player_list_filter_editbox" ) );
		m_PlayerListFilterClearButton	= ButtonWidget.Cast( m_RootLayout.FindAnyWidget( "player_list_filter_clear" ) );
		
		m_memberName.SetText( "" );
		m_memberRank.SetText( "" );
		m_memberState.SetText( "" );	
		
		m_CurrentFlag = null;
		m_CurrentTerritory = null;
		m_CurrentMember = null;	
		m_MyMember = null;

		OnRefresh();
	}
	
	// ------------------------------------------------------------
	// Expansion SetTerritory
	// ------------------------------------------------------------
	void SetTerritory()
	{
		PlayerBase player = PlayerBase.Cast( GetGame().GetPlayer() );
		ExpansionTerritoryFlag flag = m_TerritoryModule.FindNearestTerritoryFlag( player );
		m_CurrentFlag = flag;
		
		#ifdef EXPANSIONEXPRINT
		EXPrint("ExpansionBookTabTerritory::SetTerritory 1 : " + m_CurrentFlag );
		#endif
		
		if ( !m_CurrentFlag )
		{
			m_CurrentTerritory = null;
			return;
		}
		
		#ifdef EXPANSIONEXPRINT
		EXPrint("ExpansionBookTabTerritory::SetTerritory 2 : " + m_CurrentFlag );
		#endif
		
		ref map<int, ref ExpansionTerritory> territories = m_TerritoryModule.GetTerritories();
		m_CurrentTerritory = territories.Get( m_CurrentFlag.GetTerritoryID() );
		
		if (!m_CurrentTerritory)
		{
			m_CurrentFlag = null;
			m_CurrentTerritory = null;
		}
	}
	
	// ------------------------------------------------------------
	// Expansion LoadPlayerListEntries
	// ! Adds a entry for each active player in the player list
	// ------------------------------------------------------------
	void LoadPlayerListEntries(string filter)
	{
		#ifdef EXPANSIONEXLOGPRINT
		EXLogPrint("ExpansionBookTabTerritory::LoadPlayerListEntries - Start");
		#endif
		
		if (!m_CurrentTerritory || !m_playerListPanel.IsVisible()) return;
		
		PlayerBase myPlayer = PlayerBase.Cast( GetGame().GetPlayer() );
		if (!myPlayer) return;

		int nmbPlayer = 0;

		if (GetExpansionSettings().GetTerritory().UseWholeMapForInviteList)
		{
			for ( int playerIdx = 0; playerIdx < ClientData.m_PlayerList.m_PlayerList.Count(); ++playerIdx )
			{
				ref SyncPlayer playerSync = ClientData.m_PlayerList.m_PlayerList.Get( playerIdx );
				if ( !playerSync || m_CurrentTerritory.IsMember( playerSync.m_RUID ) || m_CurrentTerritory.HasInvite( playerSync.m_RUID ) || ( filter != "" && !playerSync.m_PlayerName.Contains(filter) ) )
					continue;
				
				nmbPlayer++;
				ExpansionTerritoryTabPlayerListEntry list_entry = new ExpansionTerritoryTabPlayerListEntry( m_playerListGrid, playerSync.m_RUID, playerSync.m_PlayerName, m_CurrentFlag );
				m_PlayerListEntrys.Insert( list_entry );
			}
		}
		else
		{
			array<Object> objects = new array<Object>;
			array<CargoBase> proxyCargos = new array<CargoBase> ;
			GetGame().GetObjectsAtPosition( myPlayer.GetPosition(), 25, objects, proxyCargos );
			
			if ( !objects )
			{
				m_playersListInfo.SetText("No players to invite!");
				return;
			}
			
			for ( int i = 0; i < objects.Count(); ++i )
			{
				PlayerBase player = PlayerBase.Cast( objects.Get( i ) );
				if ( player && player.GetIdentity() )
				{
					#ifdef EXPANSIONEXLOGPRINT
					EXLogPrint( "ExpansionBookTabTerritory::LoadPlayerListEntries - Player found: " + player.GetIdentityUID() + "." );
					#endif	

					// Don't show players that are in this therritory and have a existing invite from this territory already
					if ( !m_CurrentTerritory.IsMember( player.GetIdentityUID() ) && !m_CurrentTerritory.HasInvite( player.GetIdentityUID() ) && ( filter == "" || ( filter != "" && player.GetIdentityName().Contains(filter) ) ) )
					{
						nmbPlayer++;

						#ifdef EXPANSIONEXLOGPRINT
						EXLogPrint( "ExpansionBookTabTerritory::LoadPlayerListEntries - Create entry for player:" + player.GetIdentityUID() + "." );
						#endif

						ExpansionTerritoryTabPlayerListEntry list_entry2 = new ExpansionTerritoryTabPlayerListEntry( m_playerListGrid, player.GetIdentityUID(), player.GetIdentityName(), m_CurrentFlag );
						m_PlayerListEntrys.Insert( list_entry2 );
					}
				}
			}
		}
		
		if (nmbPlayer > 0)
		{
			m_playersListInfo.SetText("Select a player to invite:");
		}
		else
		{
			m_playersListInfo.SetText("No players to invite!");
		}
	
		#ifdef EXPANSIONEXLOGPRINT
		EXLogPrint("ExpansionBookTabTerritory::LoadPlayerListEntries - End");
		#endif
	}
	
	// ------------------------------------------------------------
	// Expansion RefreshMembersList
	// ------------------------------------------------------------
	void RefreshMembersList()
	{
		#ifdef EXPANSIONEXLOGPRINT
		EXLogPrint("ExpansionBookTabTerritory::RefreshMembersList - Start");
		#endif
		
		m_MemberListEntrys.Clear();
		
		LoadMemberListEntrys();
		
		#ifdef EXPANSIONEXLOGPRINT
		EXLogPrint("ExpansionBookTabTerritory::RefreshMembersList - End");
		#endif
	}
	
	// ------------------------------------------------------------
	// Expansion LoadMemberListEntrys
	// ------------------------------------------------------------
	void LoadMemberListEntrys()
	{
		#ifdef EXPANSIONEXLOGPRINT
		EXLogPrint("ExpansionBookTabTerritory::LoadMemberListEntrys - Start");
		#endif
		
		#ifdef EXPANSIONEXLOGPRINT
		EXLogPrint("ExpansionBookTabTerritory::LoadMemberListEntrys - Got m_CurrentTerritory " + m_CurrentTerritory + ".");
		#endif
		
		if ( m_CurrentTerritory && m_CurrentTerritory.GetTerritoryMembers().Count() > 0 )
		{
			ref array<ref ExpansionTerritoryMember> members = m_CurrentTerritory.GetTerritoryMembers();
			for (int i = 0; i < members.Count(); ++i)
			{
				ExpansionTerritoryTabMemberListEntry list_entry = new ExpansionTerritoryTabMemberListEntry( m_membersListGrid, this, members[i] );
				m_MemberListEntrys.Insert( list_entry );
			}
		}
		
		if ( m_CurrentTerritory && m_CurrentTerritory.GetTerritoryMembers().Count() == 1 )
		{
			m_membersListInfo.Show( true );
			m_membersListInfo.SetText("Territory has no other members!");
		}
		
		#ifdef EXPANSIONEXLOGPRINT
		EXLogPrint("ExpansionBookTabTerritory::LoadMemberListEntrys - End");
		#endif
	}
	
	// ------------------------------------------------------------
	// Expansion RefreshPlayersList
	// ------------------------------------------------------------
	void RefreshPlayersList(string filter = "")
	{
		#ifdef EXPANSIONEXLOGPRINT
		EXLogPrint("ExpansionBookTabTerritory::RefreshPlayersList - Start");
		#endif
		
		m_PlayerListEntrys.Clear();
		
		LoadPlayerListEntries(filter);
		
		#ifdef EXPANSIONEXLOGPRINT
		EXLogPrint("ExpansionBookTabTerritory::RefreshPlayersList - End");
		#endif
	}
	
	// ------------------------------------------------------------
	// Expansion LoadPlayerTerritoryInvitesEntrys
	// ------------------------------------------------------------
	void LoadPlayerTerritoryInvitesEntrys()
	{
		#ifdef EXPANSIONEXLOGPRINT
		EXLogPrint("ExpansionBookTabTerritory::LoadPlayerTerritoryInvitesEntrys - Start");
		#endif
		
		// For each entry in players intives array player
		ref array<ref ExpansionTerritoryInvite> invites = m_TerritoryModule.GetTerritoryInvites();
		if ( invites.Count() > 0 )
		{
			m_territoryInviteListInfo.Show( true );
			m_territoryInviteListInfo.SetText("");
			
			for ( int i = 0; i < invites.Count(); ++i)
			{ 
				ref ExpansionTerritoryInvite invite = ExpansionTerritoryInvite.Cast( invites[i] ); 
				ExpansionTerritoryTabInviteListEntry list_entry = new ExpansionTerritoryTabInviteListEntry( m_territoryInviteListGrid, invite );
				m_InviteListEntrys.Insert( list_entry );
			}
		}
		else
		{
			m_territoryInviteListInfo.Show( true );
			m_territoryInviteListInfo.SetText("No territory invites");
		}
		
		#ifdef EXPANSIONEXLOGPRINT
		EXLogPrint("ExpansionBookTabTerritory::LoadPlayerTerritoryInvitesEntrys - End");
		#endif
	}
	
	// ------------------------------------------------------------
	// Expansion RefreshInvitesList
	// ------------------------------------------------------------
	void RefreshInvitesList()
	{
		#ifdef EXPANSIONEXLOGPRINT
		EXLogPrint("ExpansionBookTabTerritory::RefreshInvitesList - Start");
		#endif
		
		m_InviteListEntrys.Clear();
		
		LoadPlayerTerritoryInvitesEntrys();
		
		#ifdef EXPANSIONEXLOGPRINT
		EXLogPrint("ExpansionBookTabTerritory::RefreshInvitesList - End");
		#endif
	}
	
	
	// ------------------------------------------------------------
	// Override CanBeShow
	// ------------------------------------------------------------
	override bool CanBeShow()
	{
		if (GetExpansionSettings() && !(GetExpansionSettings().GetBook().EnableTerritoryTab))
		{
			return false;
		}
		
		SetTerritory();
		bool invites = m_TerritoryModule.GetTerritoryInvites().Count() > 0;
		
		return ( (m_CurrentFlag && m_CurrentTerritory) || invites );
	}
	
	// ------------------------------------------------------------
	// Override OnClick
	// ------------------------------------------------------------	
	override bool OnClick( Widget w, int x, int y, int button )
	{
		super.OnClick(w, x, y, button);
		
		ExpansionBook bookMenu;
				
		if ( w == m_deleteTerritoryButton )
		{
			bookMenu = ExpansionBook.Cast( GetGame().GetUIManager().FindMenu( MENU_EXPANSION_BOOK_MENU ) );
			if ( bookMenu )
			{
				bookMenu.HideMenu();
			}
			
			m_territoryPanel.Show( false );
			m_popupPanel.Show( true );
			
			if (m_MyMember.GetRank() != ExpansionTerritoryRank.ADMIN)
			{
				m_popupPanelText.SetText( "Are you sure you want to leave the territory?" );
			}
			else
			{
				m_popupPanelText.SetText( "Are you sure you want to delete the territory?" );
			}
			
			return true;
		}
		
		if ( w == m_confirmButton )
		{
			if ( !m_CurrentMember )
			{
				if (m_MyMember.GetRank() != ExpansionTerritoryRank.ADMIN)
				{
					m_TerritoryModule.Leave( m_CurrentTerritory.GetTerritoryID() );
				}
				else
				{
					m_TerritoryModule.DeleteTerritoryPlayer( m_CurrentFlag );
				}
				
				m_popupPanel.Show( false );
				bookMenu = ExpansionBook.Cast( GetGame().GetUIManager().FindMenu( MENU_EXPANSION_BOOK_MENU ) );
				if ( bookMenu )
					bookMenu.CloseBook();
			}
			else
			{
				//! Promote new owner and admin of the territory
				m_TerritoryModule.PromoteMember( m_CurrentTerritory.GetTerritoryID(), m_CurrentMember );
			}
			
			return true;
		}
		
		if ( w == m_cancelButton )
		{	
			if ( !m_CurrentMember )
			{
				bookMenu = ExpansionBook.Cast( GetGame().GetUIManager().FindMenu( MENU_EXPANSION_BOOK_MENU ) );
				if ( bookMenu )
					bookMenu.ShowMenu();
				
				m_popupPanel.Show( false );
				m_territoryPanel.Show( true );
			}
			else
			{
				bookMenu = ExpansionBook.Cast( GetGame().GetUIManager().FindMenu( MENU_EXPANSION_BOOK_MENU ) );
				if ( bookMenu )
					bookMenu.ShowMenu();
		
				m_territoryPanel.Show( true );
				m_popupPanel.Show( false );
			}
			
			return true;
		}
		
		if ( w == m_memberCancelButton )
		{
			m_territoryPanel.Show( true );
			m_memberEditPanel.Show( false );
			m_memberName.SetText( "" );
			m_memberRank.SetText( "" );
			m_memberState.SetText( "" );
			
			m_CurrentMember = NULL;
			
			return true;
		}
		
		int currentRank;
		if ( w == m_memberPromoteButton )
		{
			if ( m_CurrentMember != NULL )
			{
				currentRank = m_CurrentMember.GetRank();
				
				if ( currentRank == ExpansionTerritoryRank.MEMBER )
				{
					//! Promote to moderator
					m_TerritoryModule.PromoteMember( m_CurrentTerritory.GetTerritoryID(), m_CurrentMember );				
				}
				else if ( currentRank == ExpansionTerritoryRank.MODERATOR )
				{
					GetNotificationSystem().CreateNotification( new StringLocaliser( "STR_EXPANSION_TERRITORY_TITLE" ), new StringLocaliser( "STR_EXPANSION_TERRITORY_CANT_PROMOTE" ), EXPANSION_NOTIFICATION_ICON_ERROR, COLOR_EXPANSION_NOTIFICATION_ERROR, 5, m_Player.GetIdentity() );
				}
				/*else if ( currentRank == ExpansionTerritoryRank.MODERATOR )
				{
					bookMenu = ExpansionBook.Cast( GetGame().GetUIManager().FindMenu( MENU_EXPANSION_BOOK_MENU ) );
					if ( bookMenu )
						bookMenu.HideMenu();
			
					m_memberEditPanel.Show( false );
					m_popupPanel.Show( true );
					m_popupPanelText.SetText( "Are you sure you want to promote this member to the new owner? There is no way to undo this and you will get degraded to a moderator!" );
				}*/
			}
			
			return true;
		}
		
		if ( w == m_memberDemoteButton )
		{
			if ( m_CurrentMember != NULL )
			{
				currentRank = m_CurrentMember.GetRank();
				
				if ( currentRank == ExpansionTerritoryRank.MODERATOR )
				{
					//! Demote to Member
					m_TerritoryModule.DemoteMember( m_CurrentTerritory.GetTerritoryID(), m_CurrentMember );				
				}
			}
			
			return true;
		}
		
		if ( w == m_memberKickButton )
		{
			if ( m_CurrentMember != NULL )
			{
				currentRank = m_CurrentMember.GetRank();
				
				if ( currentRank != ExpansionTerritoryRank.ADMIN )
				{
					//! Demote to Member
					m_TerritoryModule.KickMember( m_CurrentTerritory.GetTerritoryID(), m_CurrentMember );				
				}
			}
			
			return true;
		}
		
		if ( w == m_PlayerListFilterClearButton )
		{
			m_PlayerListFilterEditbox.SetText("");
			RefreshPlayersList();
			return true;
		}
		
		return false;
	}
	
	// ------------------------------------------------------------
	// Override OnRefresh
	// ------------------------------------------------------------		
	override void OnRefresh()
	{
		super.OnRefresh();
		
		if ( !Class.CastTo( m_Player, GetGame().GetPlayer() ) )
			return;
		
		SetTerritory();
			
		if ( !m_CurrentTerritory && !m_CurrentFlag && m_TerritoryModule.GetTerritoryInvites().Count() == 0 )
		{
			m_ParentMenu.CloseTab();
			return;
		}
		
		m_MyMember = m_CurrentTerritory.GetMember( m_Player.GetIdentityUID() );
		
		if ( m_CurrentMember )
		{
			m_CurrentMember = m_CurrentTerritory.GetMember( m_CurrentMember.GetID() );
			SetMemberInfo(m_CurrentMember);
		}
		
		RefreshVisibility();
		
		if ( m_CurrentFlag )
		{
			RefreshPlayersList();
			RefreshMembersList();
		}
		
		RefreshInvitesList();
		
		if (m_MyMember.GetRank() != ExpansionTerritoryRank.ADMIN)
		{
			m_deleteTerritoryLabel.SetText("Leave");
		}
		else
		{
			m_deleteTerritoryLabel.SetText("Delete");
		}
	}
	
	// ------------------------------------------------------------
	// Expansion RefreshVisibility
	// ------------------------------------------------------------	
	void RefreshVisibility()
	{
		if ( m_CurrentTerritory && m_CurrentMember )
		{
			m_territoryPanel.Show( false );
			m_memberEditPanel.Show( true );
			
			m_memberPromoteButton.Show( true );
			m_memberDemoteButton.Show( true );
			m_memberKickButton.Show( true );
			
			if (m_CurrentMember.GetID() == m_MyMember.GetID())
			{
				m_memberPromoteButton.Show( false );
				m_memberDemoteButton.Show( false );
				m_memberKickButton.Show( false );
			}
			else if (m_MyMember.GetRank() == ExpansionTerritoryRank.MEMBER)
			{
				m_memberPromoteButton.Show( false );
				m_memberDemoteButton.Show( false );
				m_memberKickButton.Show( false );
			}
			else if (m_MyMember.GetRank() == ExpansionTerritoryRank.MODERATOR)
			{
				if ( m_CurrentMember.GetRank() != ExpansionTerritoryRank.MEMBER )
				{
					m_memberPromoteButton.Show( false );
					m_memberDemoteButton.Show( false );
					m_memberKickButton.Show( false );
				}
			}
		}
		else if (m_CurrentTerritory)
		{
			m_memberEditPanel.Show(false);
			m_territoryPanel.Show(true);
			m_playerListPanel.Show( m_MyMember.GetRank() != ExpansionTerritoryRank.MEMBER );
		}
		else
		{
			m_memberEditPanel.Show(false);
			m_territoryPanel.Show(false);
		}
		
		m_territoryInviteListPanel.Show( ( m_TerritoryModule.GetTerritoryInvites().Count() > 0 ) );
	}
	
	// ------------------------------------------------------------
	// Override IsFocusable
	// ------------------------------------------------------------
	override bool IsFocusable( Widget w )
	{
		if ( w )
		{
			return ( w == m_kickMemberButton || w == m_addMemberButton || w == m_searchPlayerButton  || w == m_promotePlayerButton || w == m_demotePlayerButton || w == m_deleteTerritoryButton );
		}
		
		return false;
	}
	
	// ------------------------------------------------------------
	// Override SetMemberInfo
	// ------------------------------------------------------------
	void SetMemberInfo( ExpansionTerritoryMember member )
	{
		if ( !member )
			return;
		
		m_memberName.SetText( member.GetName() );
		m_memberRank.SetText( member.GetRankName() );
		
		if ( IsOnline( member.GetID() ) )
		{
			m_memberStateIcon.SetColor( ARGB( 255, 39, 174, 96 ) );
			m_memberState.SetText( "Online" );
			
			PlayerBase player = PlayerBase.GetPlayerByUID(member.GetID());
			if ( player )
			{
				#ifdef EXPANSIONEXLOGPRINT
				EXLogPrint("ExpansionbookTabTerritory::SetMemberInfo:: - Got DayZ Player: [" + player.ToString() + "].");
				#endif

				m_memberPreview.Show( true );
				m_memberPreview.SetPlayer( player );
			}
		}
		else
		{
			m_memberPreview.Show( false );
			m_memberStateIcon.SetColor( ARGB( 255, 192, 57, 43 ) );
			m_memberState.SetText( "Offline" );
		}
		
		m_CurrentMember = member;
		
		RefreshVisibility();
	}
	
	// ------------------------------------------------------------
	// ExpansionTerritoriesCOTMemberListEntry IsOnline
	// ------------------------------------------------------------
	bool IsOnline( string uid )
	{
		for ( int i = 0; i < ClientData.m_PlayerList.m_PlayerList.Count(); i++ )
		{
			ref SyncPlayer player = ClientData.m_PlayerList.m_PlayerList.Get( i );

			if ( player )
			{
				if ( player.m_RUID == uid )
				{
					return true;
				}
			}
		}
		
		return false;
	}
	
	// ------------------------------------------------------------
	// Override OnMouseEnter
	// ------------------------------------------------------------	
	override bool OnMouseEnter( Widget w, int x, int y )
	{
		if ( w == m_memberPromoteButton )
		{
			m_memberPromoteButtonLabel.SetColor( ARGB( 255, 255, 255, 255 ) );		
			return true;
		}
		
		if ( w == m_memberDemoteButton )
		{
			m_memberDemoteButtonLabel.SetColor( ARGB( 255, 255, 255, 255 ) );		
			return true;
		}
		
		if ( w == m_memberKickButton )
		{
			m_memberKickButtonLabel.SetColor( ARGB( 255, 255, 255, 255 ) );		
			return true;
		}
		
		if ( w == m_memberCancelButton )
		{
			m_memberCancelButtonLabel.SetColor( ARGB( 255, 255, 255, 255 ) );		
			return true;
		}
		
		return false;
	}
	
	// ------------------------------------------------------------
	// Override OnMouseLeave
	// ------------------------------------------------------------	
	override bool OnMouseLeave( Widget w, Widget enterW, int x, int y )
	{
		if ( w == m_memberPromoteButton )
		{
			m_memberPromoteButtonLabel.SetColor( ARGB( 255, 0, 0, 0 ) );		
			return true;
		}
		
		if ( w == m_memberDemoteButton )
		{
			m_memberDemoteButtonLabel.SetColor( ARGB( 255, 0, 0, 0 ) );		
			return true;
		}
		
		if ( w == m_memberKickButton )
		{
			m_memberKickButtonLabel.SetColor( ARGB( 255, 0, 0, 0 ) );		
			return true;
		}
		
		if ( w == m_memberCancelButton )
		{
			m_memberCancelButtonLabel.SetColor( ARGB( 255, 0, 0, 0 ) );		
			return true;
		}
		
		return false;
	}
	
	// ------------------------------------------------------------
	// Override OnChange
	// ------------------------------------------------------------
	override bool OnChange(Widget w, int x, int y, bool finished)
	{
		if ( w == m_PlayerListFilterEditbox )
		{
			RefreshPlayersList( m_PlayerListFilterEditbox.GetText() );
			return true;
		}
		
		return false;
	}
}