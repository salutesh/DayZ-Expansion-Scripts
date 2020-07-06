/**
 * ExpansionBookTabBase.c
 *
 * DayZ Expansion Mod
 * www.dayzexpansion.com
 * © 2020 DayZ Expansion Mod Team
 *
 * This work is licensed under the Creative Commons Attribution-NonCommercial-NoDerivatives 4.0 International License. 
 * To view a copy of this license, visit http://creativecommons.org/licenses/by-nc-nd/4.0/.
 *
*/

class ExpansionBookTabBase extends ScriptedWidgetEventHandler
{
	protected int 										COLOR_EXPANSION_BOOKMARK_ICON = ARGB( 255, 0, 0, 0 );	
	
	protected ExpansionBook								m_ParentMenu;
	protected Widget									m_ParentRoot;
	protected ref GridSpacerWidget 						m_ParentBookmark;
	
	//Button for bookmark
	protected string 									m_ButtonBackground;
	protected string 									m_BookmarkBackground;
	protected string 									m_IconButtonPath;
	protected string 									m_TextButton;
	
	protected Widget									m_BookmarkButtonMenu;
	protected Widget 									m_ButtonMenu;
	protected ButtonWidget 								m_ButtonBookmark;
	protected ButtonWidget								m_ButtonElement;
	protected ImageWidget								m_BookmarkIcon;
	protected int										m_ButtonColor;
	//End of button for bookmark
	
	//Layout things
	protected string									m_LayoutPath;
	protected Widget									m_RootLayout;
	
	// Sound
	protected PlayerBase								m_Player;
	protected EffectSound 								m_Sound;
	
	protected bool										m_IsTabOpen;
						
	// ------------------------------------------------------------
	// ExpansionBookTabBase Constructor
	// ------------------------------------------------------------	
	void ExpansionBookTabBase()
	{
		m_ButtonBackground = "set:book_assets image:menu_element_" + Math.RandomInt(1, 7);
		m_BookmarkBackground = "set:book_assets image:bookmark_element_" + Math.RandomInt(1, 4);
		
		if (GetGame() && (GetGame().IsClient() || !GetGame().IsMultiplayer())) 
		{
			m_Player = PlayerBase.Cast( GetGame().GetPlayer() );
		}
	
		m_IsTabOpen = false;
	}
	
	// ------------------------------------------------------------
	// ExpansionBookTabBase Destructor
	// ------------------------------------------------------------	
	void ~ExpansionBookTabBase()
	{
		
	}
	
	// ------------------------------------------------------------
	// Expansion SetButtonIconPath
	// ------------------------------------------------------------	
	void SetButtonIconPath( string path )
	{
		m_IconButtonPath = path;
	}
	
	// ------------------------------------------------------------
	// Expansion GetButtonIconPath
	// ------------------------------------------------------------		
	string GetButtonIconPath()
	{
		return m_IconButtonPath;
	}
	
	// ------------------------------------------------------------
	// Expansion SetButtonText
	// ------------------------------------------------------------		
	void SetButtonText( string text )
	{
		m_TextButton = text;
	}
	
	// ------------------------------------------------------------
	// Expansion GetButtonText
	// ------------------------------------------------------------		
	string GetButtonText()
	{
		return m_TextButton;
	}
	
	// ------------------------------------------------------------
	// Expansion SetButtonColor
	// ------------------------------------------------------------		
	void SetButtonColor( int color )
	{
		m_ButtonColor = color;
	}
	
	// ------------------------------------------------------------
	// Expansion SetLayoutPath
	// ------------------------------------------------------------		
	void SetLayoutPath( string path )
	{
		m_LayoutPath = path;
	}
	
	// ------------------------------------------------------------
	// Expansion CanBeShow
	// ------------------------------------------------------------		
	bool CanBeShow()
	{
		return true;
	}
	
	// ------------------------------------------------------------
	// Expansion CreateButton
	// ------------------------------------------------------------	
	void CreateButton( ref GridSpacerWidget parentBookmark, ref GridSpacerWidget parentButton )
	{
		if ( !parentBookmark || !parentButton )
		{
			return;
		}
		
		//Bookmark
		if ( !m_BookmarkButtonMenu || ( m_ParentBookmark != parentBookmark ) )
		{
			if ( m_ParentBookmark != parentBookmark && m_BookmarkButtonMenu )
			{
				m_BookmarkButtonMenu.Unlink();
			}
			
			m_BookmarkButtonMenu = GetGame().GetWorkspace().CreateWidgets( "DayZExpansion/GUI/layouts/book/expansion_book_bookmark.layout", parentBookmark );
			m_BookmarkButtonMenu.SetHandler( this );
			
			m_ButtonBookmark = ButtonWidget.Cast( m_BookmarkButtonMenu.FindAnyWidget( "bookmark_button" ) );
			m_BookmarkIcon = ImageWidget.Cast( m_BookmarkButtonMenu.FindAnyWidget( "bookmark_icon" ) );
			m_BookmarkIcon.LoadImageFile(0, GetButtonIconPath() );
			
			ImageWidget m_BookmarkBackGroundWidget = ImageWidget.Cast( m_BookmarkButtonMenu.FindAnyWidget( "bookmark_background" ) );
			m_BookmarkBackGroundWidget.LoadImageFile(0, m_BookmarkBackground);
			
			m_ParentBookmark = parentBookmark;
		}
		
		//ButtonMenu
		m_ButtonMenu = GetGame().GetWorkspace().CreateWidgets( "DayZExpansion/GUI/layouts/book/expansion_book_menu_element.layout", parentButton );
		m_ButtonMenu.SetHandler( this );
		
		TextWidget ButtonMenuLabel = TextWidget.Cast( m_ButtonMenu.FindAnyWidget( "book_menu_label" ) );
		ButtonMenuLabel.SetText( GetButtonText() );
		
		ImageWidget m_ButtonIcon = ImageWidget.Cast( m_ButtonMenu.FindAnyWidget( "book_menu_icon" ) );
		m_ButtonIcon.LoadImageFile(0, GetButtonIconPath() );
		
		m_ButtonElement = ButtonWidget.Cast( m_ButtonMenu.FindAnyWidget( "book_menu_button" ) );
		
		ImageWidget m_ButtonBackGroundWidget = ImageWidget.Cast( m_ButtonMenu.FindAnyWidget( "book_menu_background" ) );
		m_ButtonBackGroundWidget.LoadImageFile(0, m_ButtonBackground);
		
		//Update
		parentBookmark.Update();
		parentButton.Update();
	}
	
	// ------------------------------------------------------------
	// Expansion OpenTab
	// ------------------------------------------------------------		
	void OpenTab()
	{
		if ( m_ParentMenu )
			m_ParentMenu.CloseTab();
		
		CreateLayout();
		
		PlaySwitchPageSound();
		
		m_IsTabOpen = true;
	}
	
	// ------------------------------------------------------------
	// Expansion CreateLayout
	// ------------------------------------------------------------	
	private void CreateLayout()
	{
		if ( m_LayoutPath && m_LayoutPath != "" && m_ParentMenu )
		{
			m_ParentMenu.OpenTab( this );
			m_RootLayout = GetGame().GetWorkspace().CreateWidgets( m_LayoutPath, m_ParentRoot );
			m_RootLayout.SetHandler( this );
			OnCreatedLayout();
		}
	}
	
	// ------------------------------------------------------------
	// Expansion OnCreatedLayout
	// ------------------------------------------------------------		
	//Layout root var is m_RootLayout
	protected void OnCreatedLayout();
	
	// ------------------------------------------------------------
	// Expansion OnHidedLayout
	// ------------------------------------------------------------	
	//Called when tab is hided (could be when gps close or just click back button)
	protected void OnHidedLayout();
	
	// ------------------------------------------------------------
	// Expansion InitModule
	// ------------------------------------------------------------		
	void InitModule( ExpansionBook menu, Widget parentRoot )
	{
		m_ParentMenu = menu;
		m_ParentRoot = parentRoot;
	}
	
	// ------------------------------------------------------------
	// Expansion HideButton
	// ------------------------------------------------------------		
	void HideButton( bool deleteBookmark = false )
	{
		if ( m_ButtonMenu )
		{
			m_ButtonMenu.Unlink();
		}
		
		if ( deleteBookmark && m_BookmarkButtonMenu )
		{
			m_BookmarkButtonMenu.Unlink();
		}
	}
	
	// ------------------------------------------------------------
	// Expansion HideLayout
	// ------------------------------------------------------------		
	void HideLayout()
	{
		if ( m_RootLayout )
		{
			m_RootLayout.Unlink();
			m_RootLayout = null;
			OnHidedLayout();
		}
	}
	
	// ------------------------------------------------------------
	// Expansion OnUpdate
	// ------------------------------------------------------------
	//Called each frame. (Called only if tab opened)
	void OnUpdate( float timeslice );
	
	// ------------------------------------------------------------
	// Expansion OnRefresh
	// ------------------------------------------------------------
	//Called when refresh of book tab is needed. (Called only if tab opened)
	void OnRefresh();
	
	// ------------------------------------------------------------
	// Expansion OnResizeScreen
	// ------------------------------------------------------------	
	void OnResizeScreen()
	{
		if ( m_RootLayout )
		{
			//Use getSize and setSize here
		}
	}
	
	// ------------------------------------------------------------
	// Expansion CloseTab
	// ------------------------------------------------------------	
	void CloseTab()
	{
		if ( m_RootLayout )
		{
			m_ParentMenu.CloseTab();
			m_IsTabOpen = false;
		}
	}
	
	// ------------------------------------------------------------
	// Expansion SetColorButton
	// ------------------------------------------------------------		
	protected void SetColorButton()
	{
		if( m_BookmarkIcon )
		{
			m_BookmarkIcon.SetColor( m_ButtonColor );
		}
	}
		
	// ------------------------------------------------------------
	// Expansion SetNormalColorButton
	// ------------------------------------------------------------
	protected void SetNormalColorButton()
	{
		SetFocus( null );
				
		if( m_BookmarkIcon )
		{
			m_BookmarkIcon.SetColor( COLOR_EXPANSION_BOOKMARK_ICON );
		}
	}
		
	// ------------------------------------------------------------
	// Override OnFocus
	// ------------------------------------------------------------	
	override bool OnFocus( Widget w, int x, int y )
	{
		super.OnFocus(w, x, y);
		
		if( w == m_ButtonBookmark ) 
		{
			SetColorButton();
			return true;
		}
		
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
	// Override OnMouseEnter
	// ------------------------------------------------------------	
	override bool OnMouseEnter( Widget w, int x, int y )
	{
		super.OnMouseEnter(w, x, y);
		
		if( w == m_ButtonBookmark ) 
		{
			SetColorButton();
			return true;
		}
		
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
		super.OnMouseLeave(w, enterW, x, y);
		
		if( w == m_ButtonBookmark ) 
		{
			SetNormalColorButton();
			return true;
		}
		
		if( IsFocusable( w ) )
		{
			ColorNormal( w, enterW, x, y );
			return true;
		}
		
		return false;
	}
	
	// ------------------------------------------------------------
	// Override OnClick
	// ------------------------------------------------------------	
	override bool OnClick( Widget w, int x, int y, int button )
	{
		super.OnClick(w, x, y, button);
		
		if ( ( m_ButtonBookmark && w == m_ButtonBookmark ) || ( m_ButtonElement && w == m_ButtonElement ) )
		{
			if ( m_ParentMenu && m_ParentRoot )
			{
				OpenTab();
			}
			return true;
		}
		
		return false;
	}
	
	// ------------------------------------------------------------
	// Override OnMouseButtonDown
	// ------------------------------------------------------------	
	override bool OnMouseButtonDown( Widget w, int x, int y, int button )
	{
		return super.OnMouseButtonDown(w, x, y, button);
	}
	
	// ------------------------------------------------------------
	// Override OnMouseButtonUp
	// ------------------------------------------------------------		
	override bool OnMouseButtonUp( Widget w, int x, int y, int button )
	{
		return super.OnMouseButtonUp(w, x, y, button);
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
	// Expansion IsFocusable
	// ------------------------------------------------------------
	bool IsFocusable( Widget w )
	{
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
		TextWidget label	= TextWidget.Cast(w.FindWidget( w.GetName() + "_label" ) );
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
	// IsOpen
	// ------------------------------------------------------------
	bool IsOpen()
	{
		return m_IsTabOpen;
	}
	
	// ------------------------------------------------------------
	// IsServerInfoTab
	// ------------------------------------------------------------
	bool IsServerInfoTab()
	{
		return false;
	}
	
	// ------------------------------------------------------------
	// OnTabOpen
	// ------------------------------------------------------------
	void OnTabOpen();
	
	// ------------------------------------------------------------
	// OnTabClose
	// ------------------------------------------------------------
	void OnTabClose();
}