/**
 * ExpansionBook.c
 *
 * DayZ Expansion Mod
 * www.dayzexpansion.com
 * © 2020 DayZ Expansion Mod Team
 *
 * This work is licensed under the Creative Commons Attribution-NonCommercial-NoDerivatives 4.0 International License. 
 * To view a copy of this license, visit http://creativecommons.org/licenses/by-nc-nd/4.0/.
 *
*/

/**@class		ExpansionBook
 * @brief		
 **/
class ExpansionBook extends UIScriptedMenu
{
	static const int							NumberOfBookmarksOnOnePage = 10;
	static const int							NumberOfBookmarksOnTwoPages = 20;
	static ref ExpansionBookTabBase 			LastOpenedTab = NULL;
	static int									LastPageOpened = 1;
	
	protected autoptr array< ref ExpansionBookTabBase > m_Tabs;

	protected int 								m_CurrentPage;
	protected ref ExpansionBookTabBase			m_CurrentOpenTab;
	
	protected Widget							m_back_Button_Panel;
	protected ButtonWidget 						m_back_Button;
	protected TextWidget						m_back_Button_Label;
	protected ButtonWidget						m_book_PageNextButton;
	protected ButtonWidget						m_book_PageBackButton;
	protected TextWidget						m_book_CurrentPageText_PageLeft;
	protected TextWidget						m_book_CurrentPageText_PageRight;
	
	protected Widget 							m_book_Panel_Bookmarks_left;
	protected GridSpacerWidget 					m_book_GridSpacerWidgetBookmarks_left;
	protected Widget 							m_book_Panel_Bookmarks_right;	
	protected GridSpacerWidget					m_book_GridSpacerWidgetBookmarks_right;
	
	protected Widget 							m_book_TAB_BOOKMARKS;
	protected GridSpacerWidget					m_Book_Button_Left_Spacer;
	protected GridSpacerWidget					m_Book_Button_Right_Spacer;
	
	protected ImageWidget						m_Book_Pages_Background;
	protected ImageWidget						m_Book_Cover_Background;
	
	protected PlayerBase						m_Player;
	protected EffectSound 						m_Sound;
	
	protected float 							m_OpenBookTime = 0;
	protected bool 								m_HasFocus;
	
	// ------------------------------------------------------------
	// ExpansionBook Destructor
	// ------------------------------------------------------------
	void ExpansionBook()
	{
		if (GetGame() && (GetGame().IsClient() || !GetGame().IsMultiplayer())) 
		{
			m_Player = PlayerBase.Cast( GetGame().GetPlayer() );
		}
		
		m_HasFocus = false;
	}
	
	// ------------------------------------------------------------
	// ExpansionBook Destructor
	// ------------------------------------------------------------
	void ~ExpansionBook()
	{
		if (GetGame() && (GetGame().IsClient() || !GetGame().IsMultiplayer())) 
		{
			if (m_Sound) 
                m_Sound.SoundStop();
		}
	}
	
	// ------------------------------------------------------------
	// Override  Widget Init
	// ------------------------------------------------------------
	override Widget Init()
	{
		super.Init();
		
		layoutRoot 								= GetGame().GetWorkspace().CreateWidgets("DayZExpansion/GUI/layouts/book/expansion_book.layout");
		SetID(MENU_EXPANSION_BOOK_MENU);
		
		// BOOK TABS
		m_book_TAB_BOOKMARKS					= Widget.Cast( layoutRoot.FindAnyWidget( "bookmarks_content_left" ) );
		m_book_TAB_BOOKMARKS.Show( true );
		
		m_book_PageNextButton 					= ButtonWidget.Cast( layoutRoot.FindAnyWidget( "page_next_button" ) );
		m_book_PageBackButton 					= ButtonWidget.Cast( layoutRoot.FindAnyWidget( "page_back_button" ) );
		m_book_PageBackButton.Show( false );
		m_book_CurrentPageText_PageLeft			= TextWidget.Cast( layoutRoot.FindAnyWidget( "page_number_left" ) );
		m_book_CurrentPageText_PageRight		= TextWidget.Cast( layoutRoot.FindAnyWidget( "page_number_right" ) );
		
		m_book_Panel_Bookmarks_left 			= Widget.Cast( layoutRoot.FindAnyWidget( "bookmarks_content_left" ) );
		m_book_GridSpacerWidgetBookmarks_left 	= GridSpacerWidget.Cast( layoutRoot.FindAnyWidget( "bookmarks_spacer_left" ) );
		m_book_Panel_Bookmarks_right 			= Widget.Cast( layoutRoot.FindAnyWidget( "bookmarks_content_right" ) );
		m_book_GridSpacerWidgetBookmarks_right 	= GridSpacerWidget.Cast( layoutRoot.FindAnyWidget( "bookmarks_spacer_right" ) );
		m_Book_Button_Left_Spacer				= GridSpacerWidget.Cast( layoutRoot.FindAnyWidget( "book_menu_left_spacer" ) );
		m_Book_Button_Right_Spacer				= GridSpacerWidget.Cast( layoutRoot.FindAnyWidget( "book_menu_right_spacer" ) );

		m_back_Button_Panel						= Widget.Cast( layoutRoot.FindAnyWidget( "back_button_panel" ) );
		m_back_Button 							= ButtonWidget.Cast( layoutRoot.FindAnyWidget( "back_button" ) );
		m_back_Button_Label 					= TextWidget.Cast( layoutRoot.FindAnyWidget( "back_button_label" ) );
		
		m_Book_Pages_Background					= ImageWidget.Cast( layoutRoot.FindAnyWidget( "book_pages_background" ) );
		m_Book_Cover_Background					= ImageWidget.Cast( layoutRoot.FindAnyWidget( "book_cover_background" ) );
		
		ExpansionBookConstructor bookCstr = new ExpansionBookConstructor;
		bookCstr.GenerateTabs( m_Tabs );

		for ( int i = 0; i < m_Tabs.Count(); i++ )
		{
			m_Tabs[i].InitModule( this, layoutRoot );
		}
		
		if ( GetNumberOfPages() == 1 )
		{
			m_book_PageNextButton.Show( false );
		}

		//layoutRoot.SetAlpha(0);
		
		return layoutRoot;
	}

	// ------------------------------------------------------------
	// Override UseKeyboard
	// ------------------------------------------------------------
	override bool UseKeyboard() 
	{ 
		return true; 
	}

	// ------------------------------------------------------------
	// Override UseMouse
	// ------------------------------------------------------------
	override bool UseMouse()
	{
		return true;
	}
	
	// ------------------------------------------------------------
	// Override OnShow
	// ------------------------------------------------------------
	override void OnShow()
	{
		super.OnShow();

		m_OpenBookTime = 0;
		
		SetFocus( layoutRoot );
	
		m_CurrentPage = LastPageOpened;
		ShowCurrentPage( m_CurrentPage );
		ShowCurrentNumberOfPages();
		
		UpdatePageButton();

		GetGame().GetMission().PlayerControlDisable(INPUT_EXCLUDE_ALL);
		
		PPEffects.SetBlurInventory( 0.5 );
		
		if ( LastOpenedTab && LastOpenedTab.CanBeShow() )
		{
			LastOpenedTab.OpenTab();
		}
		
		PlayOpenBookSound();
	}
	
	// ------------------------------------------------------------
	// Override OnHide
	// ------------------------------------------------------------
	override void OnHide()
	{
		super.OnHide();

		GetGame().GetMission().PlayerControlEnable(true);
		
		PPEffects.SetBlurInventory( 0.0 );
	}
	
	// ------------------------------------------------------------
	// Override OnClick
	// ------------------------------------------------------------
	override bool OnClick( Widget w, int x, int y, int button )
	{
		super.OnClick(w, x, y, button);
		
		if ( m_book_PageNextButton && w == m_book_PageNextButton )
		{
			OnPageNext();
			return true;
		}
		else if ( m_book_PageBackButton && w == m_book_PageBackButton )
		{
			OnPageBack();
			return true;
		}
		else if ( m_back_Button && w == m_back_Button )
		{
			if ( m_CurrentOpenTab )
			{
				CloseTab();
			}
			else
			{
				CloseBook();
			}
			return true;
		}
		
		return false;
	}
	
	// ------------------------------------------------------------
	// Override Update
	// ------------------------------------------------------------
	override void Update(float timeslice)
	{
		m_OpenBookTime += timeslice;
		
		Input input = GetGame().GetInput();
		if ( input.LocalPress( "UAUIBack", false ) )
		{
			Hide();
			Close();
			return;
		}
		
		if ( input.LocalPress( "UAExpansionBookToggle", false ) && m_OpenBookTime > 0.75 && !m_HasFocus )
		{
			Hide();
			Close();
			return;
		}

		if ( m_CurrentOpenTab )
		{
			m_CurrentOpenTab.OnUpdate( timeslice );
		}
	}
	
	// ------------------------------------------------------------
	// Expansion Refresh
	// ------------------------------------------------------------
	override void Refresh()
	{
		if ( !m_CurrentOpenTab )
		{
			HideCurrentPage( m_CurrentPage );
			ShowCurrentPage( m_CurrentPage );
		}
		else
		{
			m_CurrentOpenTab.OnRefresh();
		}
	}
	
	// ------------------------------------------------------------
	// Expansion OnPageNext
	// ------------------------------------------------------------
	void OnPageNext()
	{
		if ( m_CurrentPage + 1 <= GetNumberOfPages() )
		{
			HideCurrentPage( m_CurrentPage );
			m_CurrentPage++;
			
			ShowCurrentPage( m_CurrentPage );
			
			UpdatePageButton();
			
			ShowCurrentNumberOfPages();
			
			LastPageOpened = m_CurrentPage;
			
			PlaySwitchPageSound();
		}
	}
	
	// ------------------------------------------------------------
	// Expansion OnPageBack
	// ------------------------------------------------------------
	void OnPageBack()
	{
		if ( m_CurrentPage - 1 >= 1 )
		{
			HideCurrentPage( m_CurrentPage );
			m_CurrentPage--;
			
			ShowCurrentPage( m_CurrentPage );
			
			UpdatePageButton();
			
			ShowCurrentNumberOfPages();
			
			LastPageOpened = m_CurrentPage;
			
			PlaySwitchPageSound();
		}
	}
	
	// ------------------------------------------------------------
	// Expansion UpdatePageButton
	// ------------------------------------------------------------
	void UpdatePageButton()
	{
		m_book_PageBackButton.Show( m_CurrentPage >= 2 );
		
		m_book_PageNextButton.Show( m_CurrentPage <= ( GetNumberOfPages() - 1 ) );
	}
	
	// ------------------------------------------------------------
	// Expansion ShowCurrentNumberOfPages
	// ------------------------------------------------------------
	void ShowCurrentNumberOfPages()
	{
		if ( m_book_CurrentPageText_PageLeft )
		{
			m_book_CurrentPageText_PageLeft.SetText( "" + ( m_CurrentPage * 2 - 1 ) );
		}
		
		if ( m_book_CurrentPageText_PageRight )
		{
			m_book_CurrentPageText_PageRight.SetText( "" + ( m_CurrentPage * 2 ) );
		}
	}
	
	// ------------------------------------------------------------
	// Expansion OnResizeScreen
	// ------------------------------------------------------------
	void OnResizeScreen()
	{
		if ( m_CurrentOpenTab )
		{
			m_CurrentOpenTab.OnResizeScreen();
		}
	}
	
	// ------------------------------------------------------------
	// Expansion GetNumberOfPages
	// ------------------------------------------------------------
	int GetNumberOfPages()
	{
		return ( Math.Floor( m_Tabs.Count() / NumberOfBookmarksOnTwoPages ) + 1 );
	}
	
	// ------------------------------------------------------------
	// Expansion ShowCurrentPage
	// ------------------------------------------------------------
	void ShowCurrentPage( int page )
	{
		int nmbOfBookmarksShown = 0;
		int currentStartIteration = Math.Floor( ( page - 1 ) * NumberOfBookmarksOnTwoPages );

		ref ExpansionBookTabBase currentModule;
		for (int i = currentStartIteration; i < m_Tabs.Count(); ++i)
		{
			currentModule = m_Tabs.Get(i);
			
			if ( currentModule.CanBeShow() )
			{
				if ( nmbOfBookmarksShown < NumberOfBookmarksOnOnePage )
				{
					currentModule.CreateButton( m_book_GridSpacerWidgetBookmarks_left, m_Book_Button_Left_Spacer );
				}
				else
				{
					currentModule.CreateButton( m_book_GridSpacerWidgetBookmarks_right, m_Book_Button_Right_Spacer );
				}
				
				nmbOfBookmarksShown++;
				
				if ( nmbOfBookmarksShown >= NumberOfBookmarksOnTwoPages )
				{
					break;
				}
			}
			else
			{
				currentModule.HideButton( true );
			}
		}
	}
	
	// ------------------------------------------------------------
	// Expansion HideCurrentPage
	// ------------------------------------------------------------
	void HideCurrentPage( int page )
	{
		int currentStartIteration = Math.Floor( ( page - 1 ) * NumberOfBookmarksOnTwoPages );
		
		ref ExpansionBookTabBase currentModule;
		for (int i = currentStartIteration; i < m_Tabs.Count(); ++i)
		{
			m_Tabs[i].HideButton();
		}
	}
	
	// ------------------------------------------------------------
	// Expansion OpenTab
	// ------------------------------------------------------------
	void OpenTab( ref ExpansionBookTabBase openedTab )
	{
		m_CurrentOpenTab = openedTab;
		HideCurrentPage( m_CurrentPage );
		LastOpenedTab = openedTab;
		
		//m_book_TAB_BOOKMARKS.Show( false );
		m_Book_Button_Left_Spacer.Show( false );
		m_Book_Button_Right_Spacer.Show( false );
		//m_book_BackButtonClass.SetBackButtonImage( true );
		
		m_book_PageNextButton.Show( false );
		m_book_PageBackButton.Show( false );
		m_book_CurrentPageText_PageLeft.Show( false );
		m_book_CurrentPageText_PageRight.Show( false );
	}
	
	// ------------------------------------------------------------
	// Expansion CloseTab
	// ------------------------------------------------------------
	void CloseTab()
	{
		if ( m_CurrentOpenTab )
		{
			m_CurrentOpenTab.HideLayout();
			//m_book_TAB_BOOKMARKS.Show( true );
			m_Book_Button_Left_Spacer.Show( true );
			m_Book_Button_Right_Spacer.Show( true );
			ShowCurrentPage( m_CurrentPage );
			
			UpdatePageButton();
			m_book_CurrentPageText_PageLeft.Show( true );
			m_book_CurrentPageText_PageRight.Show( true );

			m_CurrentOpenTab = NULL;
			LastOpenedTab = NULL;
			
			PlaySwitchPageSound();
		}
	}
	
	// ------------------------------------------------------------
	// Expansion CloseBook
	// ------------------------------------------------------------
	void CloseBook()
	{
		GetGame().GetUIManager().CloseAll();
		
		PlayCloseBookSound();
	}
	
	// ------------------------------------------------------------
	// Expansion PlayOpenBookSound
	// ------------------------------------------------------------
	void PlayOpenBookSound()
	{
		if (GetGame().IsClient() || !GetGame().IsMultiplayer())
		{	
			if ( m_Player )
			{
				m_Sound = SEffectManager.PlaySound("Expansion_OpenBook_SoundSet", m_Player.GetPosition());
				m_Sound.SetSoundAutodestroy( true );
			}
		}
	}
	
	// ------------------------------------------------------------
	// Expansion PlaySwitchPageSound
	// ------------------------------------------------------------
	void PlaySwitchPageSound()
	{
		if (GetGame().IsClient() || !GetGame().IsMultiplayer())
		{	
			if ( m_Player )
			{
				m_Sound = SEffectManager.PlaySound("Expansion_SwitchPage_SoundSet", m_Player.GetPosition());
				m_Sound.SetSoundAutodestroy( true );
			}
		}
	}
		
	// ------------------------------------------------------------
	// Expansion PlayCloseBookSound
	// ------------------------------------------------------------
	void PlayCloseBookSound()
	{
		if (GetGame().IsClient() || !GetGame().IsMultiplayer())
		{	
			if ( m_Player )
			{
				m_Sound = SEffectManager.PlaySound("Expansion_CloseBook_SoundSet", m_Player.GetPosition());
				m_Sound.SetSoundAutodestroy( true );
			}
		}
	}
	
	// ------------------------------------------------------------
	// Override OnMouseEnter
	// ------------------------------------------------------------	
	override bool OnMouseEnter( Widget w, int x, int y )
	{
		if( IsFocusable( w ) )
		{
			ColorHighlight( w, x, y );
			return true;
		}
		return false;
	}
	
	// ------------------------------------------------------------
	// Override OnMouseLeave
	// ------------------------------------------------------------	
	override bool OnMouseLeave( Widget w, Widget enterW, int x, int y )
	{
		if( IsFocusable( w ) )
		{
			ColorNormal( w, enterW, x, y );
			return true;
		}
		return false;
	}
	
	// ------------------------------------------------------------
	// Override OnFocus
	// ------------------------------------------------------------
	override bool OnFocus( Widget w, int x, int y )
	{
		if( IsFocusable( w ) )
		{
			ColorHighlight( w, x, y );
			return true;
		}
		return false;
	}
	
	// ------------------------------------------------------------
	// Override OnFocusLost
	// ------------------------------------------------------------
	override bool OnFocusLost( Widget w, int x, int y )
	{
		if( IsFocusable( w ) )
		{
			ColorNormal( w, null, x, y );
			return true;
		}
		return false;
	}
	
	// ------------------------------------------------------------
	// Expansion IsFocusable
	// ------------------------------------------------------------
	bool IsFocusable( Widget w )
	{
		if( w )
		{
			return ( w == m_back_Button );
		}
		return false;
	}
	
	//Coloring functions (Until WidgetStyles are useful)
	// ------------------------------------------------------------
	// ColorHighlight
	// ------------------------------------------------------------
	void ColorHighlight( Widget w, int x, int y )
	{
		int COLOR_HIGHLIGHT = ARGB( 255, 255, 0, 0 );
		
		SetFocus( w );
		if( w.IsInherited( ButtonWidget ) )
		{
			ButtonWidget button = ButtonWidget.Cast( w );
			button.SetTextColor( COLOR_HIGHLIGHT );
			button.SetAlpha( 0.9 );
		}
		
		TextWidget text		= TextWidget.Cast(w.FindWidget( w.GetName() + "_text" ) );
		TextWidget text2	= TextWidget.Cast(w.FindWidget( w.GetName() + "_text_1" ) );
		TextWidget label		= TextWidget.Cast(w.FindWidget( w.GetName() + "_label" ) );
		ImageWidget image	= ImageWidget.Cast( w.FindWidget( w.GetName() + "_image" ) );
		
		if( text )
		{
			text.SetColor( COLOR_HIGHLIGHT );
		}
		
		if( text2 )
		{
			text2.SetColor( COLOR_HIGHLIGHT );
		}
		
		if( label )
		{
			label.SetColor( COLOR_HIGHLIGHT );
		}
		
		if( image )
		{
			image.SetColor( COLOR_HIGHLIGHT );
		}
	}
	
	// ------------------------------------------------------------
	// ColorNormal
	// ------------------------------------------------------------
	void ColorNormal( Widget w, Widget enterW, int x, int y )
	{
		int COLOR_NORMAL = ARGB( 255, 0, 0, 0 );
		
		if( w.IsInherited( ButtonWidget ) )
		{
			ButtonWidget button = ButtonWidget.Cast( w );
			button.SetTextColor( COLOR_NORMAL );
			button.SetAlpha( 1 );
		}
		
		TextWidget text		= TextWidget.Cast(w.FindWidget( w.GetName() + "_text" ) );
		TextWidget text2	= TextWidget.Cast(w.FindWidget( w.GetName() + "_text_1" ) );
		TextWidget label	= TextWidget.Cast(w.FindWidget( w.GetName() + "_label" ) );
		ImageWidget image	= ImageWidget.Cast( w.FindWidget( w.GetName() + "_image" ) );
		
		if( text )
		{
			text.SetColor( COLOR_NORMAL );
		}
		
		if( text2 )
		{
			text2.SetColor( COLOR_NORMAL );
		}
		
		if( label )
		{
			label.SetColor( COLOR_NORMAL );
		}
		
		if( image )
		{
			image.SetColor( COLOR_NORMAL );
		}
	}
	
	// ------------------------------------------------------------
	// ExpansionBook GetCurrentTab
	// ------------------------------------------------------------
	ExpansionBookTabBase GetCurrentTab()
	{
		return m_CurrentOpenTab;
	}
	
	// ------------------------------------------------------------
	// ExpansionBook HideMenu
	// ------------------------------------------------------------
	void HideMenu()
	{
		m_back_Button_Panel.Show( false );
		m_book_Panel_Bookmarks_left.Show( false );
		m_book_Panel_Bookmarks_right.Show( false );
		m_Book_Pages_Background.Show( false );
		m_Book_Cover_Background.Show( false );
	}
	
	// ------------------------------------------------------------
	// ExpansionBook ShowMenu
	// ------------------------------------------------------------
	void ShowMenu()
	{
		m_back_Button_Panel.Show( true );
		m_book_Panel_Bookmarks_left.Show( true );
		m_book_Panel_Bookmarks_right.Show( true );
		m_Book_Pages_Background.Show( true );
		m_Book_Cover_Background.Show( true );
	}
	
	// ------------------------------------------------------------
	// ExpansionBook Show
	// ------------------------------------------------------------
	void Show()
	{
		layoutRoot.Show( true );
	}
	
	// ------------------------------------------------------------
	// ExpansionBook Hide
	// ------------------------------------------------------------
	void Hide()
	{
		layoutRoot.Show( false );
	}
	
	// ------------------------------------------------------------
	// ExpansionBook SetHasFocus
	// ------------------------------------------------------------
	void SetHasFocus(bool state)
	{
		#ifdef EXPANSIONEXLOGPRINT
		EXLogPrint( "ExpansionBook::SetHasFocus - Set state: " + state.ToString() );
		#endif
		
		m_HasFocus = state;
		
		#ifdef EXPANSIONEXLOGPRINT
		EXLogPrint( "ExpansionBook::SetHasFocus - m_HasFocus: " + m_HasFocus.ToString() );
		#endif
	}
}