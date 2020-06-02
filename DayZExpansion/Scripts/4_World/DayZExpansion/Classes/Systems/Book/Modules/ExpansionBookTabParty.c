/**
 * ExpansionBookTabParty.c
 *
 * DayZ Expansion Mod
 * www.dayzexpansion.com
 * © 2020 DayZ Expansion Mod Team
 *
 * This work is licensed under the Creative Commons Attribution-NonCommercial-NoDerivatives 4.0 International License. 
 * To view a copy of this license, visit http://creativecommons.org/licenses/by-nc-nd/4.0/.
 *
*/

class ExpansionBookTabParty extends ExpansionBookTabBase
{
	protected int COLOR_EXPANSION_GROUP_BOOKMARK_ICON = ARGB( 255, 41, 128, 185 );
	
	protected EditBoxWidget m_book_PartyClanName;
	protected Widget m_book_TAB_GROUPCREATION;
	protected Widget m_book_TAB_GROUP;
	protected ButtonWidget m_book_PartyCreateGroupButton;
	protected ButtonWidget m_book_PartyExitGroupButton;
	protected TextWidget m_book_PartyExitGroupButton1Lable;
	protected TextWidget m_book_PartyExitGroupButton2Lable;
	protected TextWidget m_book_PartyNameGroup;
	protected TextWidget m_book_PartyMembersCount;
	
	protected Widget m_PlayersListPanel;
	protected TextWidget m_PlayersListInfo;
	protected GridSpacerWidget m_PlayerListEntrysGrid;
	protected Widget m_GroupListPanel;
	protected GridSpacerWidget m_GroupListEntrysGrid;
	protected GridSpacerWidget m_MemberListEntrysGrid;
	
	protected ref ExpansionPartyModule m_PartyModule;
	protected ref array<ref ExpansionPartyTabPlayerListEntry> m_PlayerListEntrys;
	protected ref array<ref ExpansionPartyTabMemberListEntry> m_MemberListEntrys;
	protected ref array<ref ExpansionPartyTabInvitesListEntry> m_PlayerInviteEntrys;
	
	protected bool m_FocusEditbox;
	
	// ------------------------------------------------------------
	// ExpansionBookTabParty Contructor
	// ------------------------------------------------------------
	void ExpansionBookTabParty()
	{
		SetButtonIconPath( "set:expansion_iconset image:icon_group" );
		SetButtonText( "#STR_BOOK_GROUPMENU" );
		SetButtonColor( COLOR_EXPANSION_GROUP_BOOKMARK_ICON );
		SetLayoutPath( "DayZExpansion/GUI/layouts/book/expansion_book_page_group.layout" );
		
		if (!m_Player)
			m_Player = PlayerBase.Cast( GetGame().GetPlayer() );

		if (!m_PartyModule)
			m_PartyModule = ExpansionPartyModule.Cast(GetModuleManager().GetModule(ExpansionPartyModule));
		
		if (!m_PlayerListEntrys)
			m_PlayerListEntrys = new array<ref ExpansionPartyTabPlayerListEntry>;
		
		if (!m_MemberListEntrys)
			m_MemberListEntrys = new array<ref ExpansionPartyTabMemberListEntry>;
		
		if (!m_PlayerInviteEntrys)
			m_PlayerInviteEntrys = new array<ref ExpansionPartyTabInvitesListEntry>;
		
		m_FocusEditbox = false;
	}
	
	// ------------------------------------------------------------
	// ExpansionBookTabParty OnCreatedLayout
	// ------------------------------------------------------------
	//Layout root var is m_RootLayout
	override void OnCreatedLayout()
	{
		m_book_TAB_GROUP						= m_RootLayout.FindAnyWidget( "book_group_tab" );
		m_book_TAB_GROUPCREATION    			= m_RootLayout.FindAnyWidget( "book_create_group_tab" );

		m_book_PartyClanName 					= EditBoxWidget.Cast( m_RootLayout.FindAnyWidget( "group_name_editbox" ) );
		m_book_PartyCreateGroupButton 			= ButtonWidget.Cast( m_RootLayout.FindAnyWidget( "sticky_create_group_button" ) );
		m_book_PartyExitGroupButton 			= ButtonWidget.Cast( m_RootLayout.FindAnyWidget( "sticky_dissolve_button" ) );
		m_book_PartyExitGroupButton1Lable		= TextWidget.Cast( m_RootLayout.FindAnyWidget( "sticky_dissolve_button_text" ) );
		m_book_PartyExitGroupButton2Lable		= TextWidget.Cast( m_RootLayout.FindAnyWidget( "sticky_dissolve_button_text_1" ) );
		m_book_PartyNameGroup					= TextWidget.Cast( m_RootLayout.FindAnyWidget( "group_info_name_value" ) );
		m_book_PartyMembersCount				= TextWidget.Cast( m_RootLayout.FindAnyWidget( "group_info_members_value" ) );
		
		m_PlayersListPanel						= m_RootLayout.FindAnyWidget( "book_menu_content_left" );
		m_PlayersListInfo 						= TextWidget.Cast( m_RootLayout.FindAnyWidget("player_list_info") );
		m_PlayerListEntrysGrid 					= GridSpacerWidget.Cast( m_RootLayout.FindAnyWidget("player_list_content") );
		
		m_MemberListEntrysGrid 					= GridSpacerWidget.Cast( m_RootLayout.FindAnyWidget("group_list_content") );
		
		m_GroupListPanel						= m_RootLayout.FindAnyWidget( "book_create_group_content_right" );
		m_GroupListEntrysGrid					= GridSpacerWidget.Cast( m_RootLayout.FindAnyWidget("groups_list_content") );
		
		OnRefresh();
	}
		
	// ------------------------------------------------------------
	// Expansion OnCreatePartyButtonClick
	// ------------------------------------------------------------
	void OnCreatePartyButtonClick()
	{
		string party_name = m_book_PartyClanName.GetText();
		TStringArray allCharacters = {"A","B","C","D","E","F","G","H","I","J","K","L","M","N","O","P","Q","R","S","T","U","V","W","X","Y","Z","a","b","c","d","e","f","g","h","i","j","k","l","m","n","o","p","q","r","s","t","u","v","w","x","y","z","0","1","2","3","4","5","6","7","8","9","_"," ","'","\""};

		if ( party_name.Length() < 3 || party_name.Length() > 12 )
		{
			GetNotificationSystem().CreateNotification( new StringLocaliser( "STR_EXPANSION_PARTY_NOTIF_TITLE" ), new StringLocaliser( "STR_EXPANSION_PARTY_SYSTEM_NAME" ), EXPANSION_NOTIFICATION_ICON_ERROR, COLOR_EXPANSION_NOTIFICATION_ERROR, 7 );
			return;
		}

		//Check if they are using character not allowed
		for ( int i = 0; i < party_name.Length(); i++ )
		{
			if ( allCharacters.Find(party_name.Get(i)) == -1 )
			{
				GetNotificationSystem().CreateNotification( new StringLocaliser( "STR_EXPANSION_PARTY_NOTIF_TITLE" ), new StringLocaliser( "STR_EXPANSION_PARTY_SYSTEM_ALPHANUMERIC" ), EXPANSION_NOTIFICATION_ICON_ERROR, COLOR_EXPANSION_NOTIFICATION_ERROR, 7 );
				return;
			}
		}

       	m_PartyModule.Create( party_name );
	}
			
	// ------------------------------------------------------------
	// Expansion OnExitPartyButtonClick
	// ------------------------------------------------------------
	void OnExitPartyButtonClick()
	{
		if ( !Class.CastTo( m_Player, GetGame().GetPlayer() ) )
			return;
			
		if ( m_PartyModule.HasParty() ) 
		{
			if ( m_Player.GetIdentityUID() == m_PartyModule.GetParty().GetOwnerUID() )
			{
				m_PartyModule.Dissolve();
			}
			else
			{
				m_PartyModule.Leave();
			}
		}
	}
	
	// ------------------------------------------------------------
	// Expansion LoadPlayerListEntrys
	// ! Adds a entry for each active player in the player list
	// ------------------------------------------------------------
	void LoadPlayerListEntrys()
	{
		#ifdef EXPANSIONEXLOGPRINT
		EXLogPrint("ExpansionBookTabParty::LoadPlayerListEntrys - Start");
		#endif
		
		if ( !m_Player || !m_PartyModule.HasParty() )
			return;
		
		m_PlayersListInfo.Show( true );

		int nmbPlayer = 0;

		if (GetExpansionSettings().GetParty().UseWholeMapForInviteList)
		{
			for ( int playerIdx = 0; playerIdx < ClientData.m_PlayerList.m_PlayerList.Count(); ++playerIdx )
			{
				ref SyncPlayer playerSync = ClientData.m_PlayerList.m_PlayerList.Get( playerIdx );
				if ( !playerSync || m_PartyModule.GetParty().GetPlayer(playerSync.m_RUID) )
					continue;
				
				nmbPlayer++;
				ExpansionPartyTabPlayerListEntry list_entry = new ExpansionPartyTabPlayerListEntry( m_PlayerListEntrysGrid, playerSync.m_PlayerName, playerSync.m_RUID );
				m_PlayerListEntrys.Insert( list_entry );
			}
		}
		else
		{
			array<Object> objects = new array<Object>;
			array<CargoBase> proxyCargos = new array<CargoBase> ;
			GetGame().GetObjectsAtPosition( m_Player.GetPosition(), 25, objects, proxyCargos );
			
			if ( !objects )
			{
				m_PlayersListInfo.SetText("No players to invite!");
				return;
			}
			
			for ( int i = 0; i < objects.Count(); ++i )
			{
				PlayerBase player = PlayerBase.Cast( objects.Get( i ) );
				if ( player && player.GetIdentity() )
				{
					// Only show the players that aren't in our current party
					if (!(m_PartyModule.GetParty().GetPlayer(player.GetIdentityUID())))
					{
						nmbPlayer++;
						ExpansionPartyTabPlayerListEntry list_entry2 = new ExpansionPartyTabPlayerListEntry( m_PlayerListEntrysGrid, player.GetIdentityName(), player.GetIdentityUID() );
						m_PlayerListEntrys.Insert( list_entry2 );
					}
				}
			}
		}
	
		if (nmbPlayer > 0)
	 	{
			m_PlayersListInfo.SetText("Select a player to invite:");
		} else
		{
			m_PlayersListInfo.SetText("No players to invite!");
		}
		
		#ifdef EXPANSIONEXLOGPRINT
		EXLogPrint("ExpansionBookTabParty::LoadPlayerListEntrys - End");
		#endif
	}
	
	// ------------------------------------------------------------
	// Expansion LoadMemberListEntrys
	// ------------------------------------------------------------
	void LoadMemberListEntrys()
	{
		PlayerBase player = PlayerBase.Cast( GetGame().GetPlayer() );
		
		if ( m_PartyModule.HasParty() )
		{
			ref ExpansionPartySaveFormat playerParty = m_PartyModule.GetParty();
			if (!playerParty) return;
			
			ref array< ref ExpansionPartySaveFormatPlayer > players = playerParty.GetPlayers();
			if (!players) return;
			
			for ( int i = 0; i < players.Count(); i++ )
			{
				ref ExpansionPartySaveFormatPlayer playerFormat = players[i];
				if (!playerFormat) return;
				
				ExpansionPartyTabMemberListEntry member_entry = new ExpansionPartyTabMemberListEntry(m_MemberListEntrysGrid, playerFormat);
				m_MemberListEntrys.Insert(member_entry);
			}
		}
	}
	
	// ------------------------------------------------------------
	// Expansion LoadInviteListEntrys
	// ------------------------------------------------------------
	void LoadInviteListEntrys()
	{
		#ifdef EXPANSIONEXLOGPRINT
		EXLogPrint("ExpansionBookTabParty::LoadInviteListEntrys - Start");
		#endif
		
		PlayerBase player = PlayerBase.Cast( GetGame().GetPlayer() );
		
		array<ref ExpansionPartyInvite> invites = m_PartyModule.GetPartyInvites();
		
		if ( invites.Count() > 0 )
		{
			m_GroupListPanel.Show( true );		
			
			for(int i = 0; i < invites.Count(); ++i)
			{
				ExpansionPartyTabInvitesListEntry invite_entry = new ExpansionPartyTabInvitesListEntry( m_GroupListEntrysGrid, invites[i] );
				m_PlayerInviteEntrys.Insert( invite_entry );
			}
		}
		
		#ifdef EXPANSIONEXLOGPRINT
		EXLogPrint("ExpansionBookTabParty::LoadInviteListEntrys - End");
		#endif
	}
	
	// ------------------------------------------------------------
	// Expansion RefreshPlayersList
	// ------------------------------------------------------------
	void RefreshPlayersList()
	{
		m_PlayerListEntrys.Clear();
		
		LoadPlayerListEntrys();
	}
	
	// ------------------------------------------------------------
	// Expansion RefreshMembersList
	// ------------------------------------------------------------
	void RefreshMembersList()
	{
		m_MemberListEntrys.Clear();
		
		LoadMemberListEntrys();
	}
		
	// ------------------------------------------------------------
	// Expansion RefreshInvitesList
	// ------------------------------------------------------------
	void RefreshInvitesList()
	{
		#ifdef EXPANSIONEXLOGPRINT
		EXLogPrint("ExpansionBookTabParty::RefreshInvitesList - Start");
		#endif

		m_GroupListPanel.Show( false );
		m_PlayerInviteEntrys.Clear();
		
		LoadInviteListEntrys();
		
		#ifdef EXPANSIONEXLOGPRINT
		EXLogPrint("ExpansionBookTabParty::RefreshInvitesList - End");
		#endif
	}
	
	// ------------------------------------------------------------
	// Override IsFocusable
	// ------------------------------------------------------------
	override bool IsFocusable( Widget w )
	{
		if( w )
		{
			return ( w == m_book_PartyCreateGroupButton || w == m_book_PartyExitGroupButton );
		}
		
		return false;
	}
	
	// ------------------------------------------------------------
	// Override CanBeShow
	// ------------------------------------------------------------
	override bool CanBeShow()
	{
		return GetExpansionSettings().GetBook().EnablePartyTab;
	}
	
	// ------------------------------------------------------------
	// Override OnRefresh
	// ------------------------------------------------------------
	override void OnRefresh()
	{
		super.OnRefresh();

		if ( !Class.CastTo( m_Player, GetGame().GetPlayer() ) )
			return;

		if ( m_PartyModule.HasParty() )
		{
			#ifdef EXPANSIONEXLOGPRINT
			EXLogPrint("ExpansionBookTabParty::OnRefresh - Player in party! Start");
			#endif
			
			m_book_TAB_GROUPCREATION.Show( false );
			m_book_TAB_GROUP.Show( true );
			
			ref ExpansionPartySaveFormat party = m_PartyModule.GetParty();
			
			#ifdef EXPANSIONEXLOGPRINT
			EXLogPrint("ExpansionBookTabParty::OnRefresh - party : " + party);
			#endif

			if ( party.GetOwnerUID() != m_Player.GetIdentityUID() )
			{
				m_book_PartyExitGroupButton1Lable.SetText("#STR_EXPANSION_BOOK_GROUP_QUIT");
				m_book_PartyExitGroupButton2Lable.SetText("#STR_EXPANSION_BOOK_GROUP_GROUP");
			}
			else
			{
				m_book_PartyExitGroupButton1Lable.SetText("#STR_EXPANSION_BOOK_GROUP_DISSOLVE");
				m_book_PartyExitGroupButton2Lable.SetText("#STR_EXPANSION_BOOK_GROUP_GROUP");
			}
			
			#ifdef EXPANSIONEXLOGPRINT
			EXLogPrint("ExpansionBookTabParty::OnRefresh - before GetPlayer");
			#endif

			ref ExpansionPartySaveFormatPlayer player = party.GetPlayer( m_Player.GetIdentityUID() );

			#ifdef EXPANSIONEXLOGPRINT
			EXLogPrint("ExpansionBookTabParty::OnRefresh - player : " + player);
			#endif

			#ifdef EXPANSIONEXLOGPRINT
			EXLogPrint("ExpansionBookTabParty::OnRefresh - player.Promoted : " + player.Promoted);
			#endif
			
			#ifdef EXPANSIONEXLOGPRINT
			EXLogPrint("ExpansionBookTabParty::OnRefresh - before m_PlayersListPanel");
			#endif

			#ifdef EXPANSIONEXLOGPRINT
			EXLogPrint("ExpansionBookTabParty::OnRefresh - m_PlayersListPanel : " + m_PlayersListPanel);
			#endif

			m_PlayersListPanel.Show( player.Promoted );
			
			#ifdef EXPANSIONEXLOGPRINT
			EXLogPrint("ExpansionBookTabParty::OnRefresh - after m_PlayersListPanel : " + m_PlayersListPanel);
			#endif
			
			#ifdef EXPANSIONEXLOGPRINT
			EXLogPrint("ExpansionBookTabParty::OnRefresh - party.GetPartyName() : " + party.GetPartyName());
			#endif

			m_book_PartyNameGroup.SetText( party.GetPartyName() );
			
			#ifdef EXPANSIONEXLOGPRINT
			EXLogPrint("ExpansionBookTabParty::OnRefresh - party.GetPlayers() : " + party.GetPlayers());
			#endif

			#ifdef EXPANSIONEXLOGPRINT
			EXLogPrint("ExpansionBookTabParty::OnRefresh - party.GetPlayers().Count() : " + party.GetPlayers().Count());
			#endif

			#ifdef EXPANSIONEXLOGPRINT
			EXLogPrint("ExpansionBookTabParty::OnRefresh - party.GetPlayers().Count().ToString() : " + party.GetPlayers().Count().ToString());
			#endif

			m_book_PartyMembersCount.SetText( party.GetPlayers().Count().ToString() );

			//RefreshGroupMembersList();

			#ifdef EXPANSIONEXLOGPRINT
			EXLogPrint("ExpansionBookTabParty::OnRefresh - Player in party! Before RefreshPlayersList");
			#endif

			RefreshPlayersList();

			#ifdef EXPANSIONEXLOGPRINT
			EXLogPrint("ExpansionBookTabParty::OnRefresh - Player in party! Before RefreshMembersList");
			#endif

			RefreshMembersList();

			#ifdef EXPANSIONEXLOGPRINT
			EXLogPrint("ExpansionBookTabParty::OnRefresh - Player in party! End");
			#endif

		} else
		{
			#ifdef EXPANSIONEXLOGPRINT
			EXLogPrint("ExpansionBookTabParty::OnRefresh - Player not in party!");
			#endif
			
			m_book_TAB_GROUP.Show( false );
			m_book_TAB_GROUPCREATION.Show( true );
			
			RefreshInvitesList();
		}
	}
	
	// ------------------------------------------------------------
	// Override OnClick
	// ------------------------------------------------------------
	override bool OnClick( Widget w, int x, int y, int button )
	{
		super.OnClick(w, x, y, button);
		
		if ( w == m_book_PartyCreateGroupButton )
		{
			OnCreatePartyButtonClick();
		}
		else if ( w == m_book_PartyExitGroupButton )
		{
			OnExitPartyButtonClick();
		}
		
		return false;
	}
	
	// ------------------------------------------------------------
	// Override OnFocus
	// ------------------------------------------------------------
	override bool OnMouseEnter(Widget w, int x, int y)
	{
		if ( w == m_book_PartyClanName )
		{
			#ifdef EXPANSIONEXLOGPRINT
			EXLogPrint("ExpansionBookTabParty::OnMouseEnter - m_book_PartyClanName");
			#endif

			ExpansionBook book = ExpansionBook.Cast( GetGame().GetUIManager().GetMenu() );
			book.SetHasFocus( true );
			
			return true;
		}
		
		return false;
	}	
	
	// ------------------------------------------------------------
	// Override OnFocus
	// ------------------------------------------------------------
	override bool OnMouseLeave(Widget w, Widget enterW, int x, int y)
	{
		if ( w == m_book_PartyClanName )
		{
			#ifdef EXPANSIONEXLOGPRINT
			EXLogPrint("ExpansionBookTabParty::OnMouseLeave - m_book_PartyClanName");
			#endif
			
			ExpansionBook book = ExpansionBook.Cast( GetGame().GetUIManager().GetMenu() );
			book.SetHasFocus( false );
			
			return true;
		}
		return false;
	}
}
