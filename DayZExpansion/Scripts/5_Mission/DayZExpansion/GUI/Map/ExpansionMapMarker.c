/**
 * ExpansionMapMarker.c
 *
 * DayZ Expansion Mod
 * www.dayzexpansion.com
 * © 2021 DayZ Expansion Mod Team
 *
 * This work is licensed under the Creative Commons Attribution-NonCommercial-NoDerivatives 4.0 International License. 
 * To view a copy of this license, visit http://creativecommons.org/licenses/by-nc-nd/4.0/.
 *
*/

class ExpansionMapMarker : ExpansionMapWidgetBase
{
	private ref ExpansionMarkerData m_Data;

	protected Widget m_Frame;

	protected TextWidget m_Name;
	protected EditBoxWidget m_EditName;
	protected string m_MarkerName;

	protected ImageWidget m_Icon;
	protected ButtonWidget m_EditButton;
	protected Widget m_DragWidget;

	protected Widget m_EditPanel;

	protected GridSpacerWidget m_IconEntries;
	protected ref array< ref ExpansionMapMarkerIconItem > m_IconTypesArray;
	protected ExpansionIcon m_CurrentIcon;

	protected SliderWidget m_ColorRGB_Red;
	protected SliderWidget m_ColorRGB_Green;
	protected SliderWidget m_ColorRGB_Blue;

	protected ButtonWidget m_LeftButton;
	protected ButtonWidget m_RightButton;

	protected Widget m_State3DContainer;
	protected Widget m_StatePartyContainer;

	protected CheckBoxWidget m_State3DCheckbox;
	protected CheckBoxWidget m_StatePartyCheckbox;
	protected CheckBoxWidget m_StateDragLockCheckbox;

	private int m_PrimaryColor;
	private int m_HoverColor;

	private bool m_MouseHover;
	private bool m_Creating;
	private bool m_DragLocked;

	protected ExpansionMapMenu m_MapMenu;
	private ExpansionMarkerModule m_MarkerModule;
	
	protected ref ExpansionUITooltip m_PositionToolTip;
	
	// ------------------------------------------------------------
	// ExpansionMapMarker Constructor
	// ------------------------------------------------------------
	void ExpansionMapMarker( Widget parent, MapWidget mapWidget, bool autoInit = true )
	{
		#ifdef EXPANSION_MAP_MENU_DEBUG
		EXLogPrint("ExpansionMapMarker::ExpansionMapMarker - Start");
		#endif
		
		m_IconTypesArray = new array< ref ExpansionMapMarkerIconItem >;

		Class.CastTo( m_MarkerModule, GetModuleManager().GetModule( ExpansionMarkerModule ) );
		
		#ifdef EXPANSION_MAP_MENU_DEBUG
		EXLogPrint("ExpansionMapMarker::ExpansionMapMarker - End");
		#endif
	}
	
	// ------------------------------------------------------------
	// ExpansionMapMarker Destructor
	// ------------------------------------------------------------
	void ~ExpansionMapMarker()
	{
		#ifdef EXPANSION_MAP_MENU_DEBUG
		EXLogPrint("ExpansionMapMarker::~ExpansionMapMarker - Start");
		#endif
		
		delete m_IconTypesArray;
		
		delete m_PositionToolTip;
		
		
		#ifdef EXPANSION_MAP_MENU_DEBUG
		EXLogPrint("ExpansionMapMarker::~ExpansionMapMarker - End");
		#endif
	}
	
	// ------------------------------------------------------------
	// ExpansionMapMarker OnInit
	// ------------------------------------------------------------
	protected override void OnInit( Widget layoutRoot )
	{
		#ifdef EXPANSION_MAP_MENU_DEBUG
		EXLogPrint("ExpansionMapMarker::OnInit - Start");
		#endif
				
		Class.CastTo( m_Frame, layoutRoot.FindAnyWidget( "marker_frame" ) );
		Class.CastTo( m_Name, layoutRoot.FindAnyWidget( "marker_name" ) );
		Class.CastTo( m_EditName, layoutRoot.FindAnyWidget( "marker_edit_name" ) );
		Class.CastTo( m_Icon, layoutRoot.FindAnyWidget( "marker_icon" ) );
		Class.CastTo( m_EditButton, layoutRoot.FindAnyWidget( "marker_button" ) );
		Class.CastTo( m_DragWidget, layoutRoot.FindAnyWidget( "marker_icon_panel" ) );

		if ( CanEdit() )
		{
			m_EditPanel = GetGame().GetWorkspace().CreateWidgets( GetEditLayoutPath(), layoutRoot.FindAnyWidget( "marker_frame" ) );
			m_EditPanel.Show( false );
		
			OnEditInit( m_EditPanel );
		}

		m_PrimaryColor = ARGB( 150, 255, 255, 255 );
		m_HoverColor = ARGB( 255, 255, 255, 255 );
		
		m_Frame.SetColor( ARGB( 0, 0, 0, 0 ) );
		
		#ifdef EXPANSION_MAP_MENU_DEBUG
		EXLogPrint("ExpansionMapMarker::OnInit - End");
		#endif
	}
	
	// ------------------------------------------------------------
	// ExpansionMapMarker OnEditInit
	// ------------------------------------------------------------
	protected void OnEditInit( Widget layoutRoot )
	{
		#ifdef EXPANSION_MAP_MENU_DEBUG
		EXLogPrint("ExpansionMapMarker::OnEditInit - Start");
		#endif
		
		Class.CastTo( m_IconEntries, layoutRoot.FindAnyWidget( "marker_icons_content" ) );
		Class.CastTo( m_LeftButton, layoutRoot.FindAnyWidget( "marker_button_left" ) );
		Class.CastTo( m_RightButton, layoutRoot.FindAnyWidget( "marker_button_right" ) );

		Class.CastTo( m_ColorRGB_Red, layoutRoot.FindAnyWidget( "marker_color_picker_rgb_red_slider" ) );
		Class.CastTo( m_ColorRGB_Green, layoutRoot.FindAnyWidget( "marker_color_picker_rgb_green_slider" ) );
		Class.CastTo( m_ColorRGB_Blue, layoutRoot.FindAnyWidget( "marker_color_picker_rgb_blue_slider" ) );

		Class.CastTo( m_State3DContainer, layoutRoot.FindAnyWidget( "marker_state_3d_container" ) );
		Class.CastTo( m_StatePartyContainer, layoutRoot.FindAnyWidget( "marker_state_party_container" ) );

		Class.CastTo( m_State3DCheckbox, layoutRoot.FindAnyWidget( "marker_state_3d_checkbox" ) );
		Class.CastTo( m_StatePartyCheckbox, layoutRoot.FindAnyWidget( "marker_state_party_checkbox" ) );
		Class.CastTo( m_StateDragLockCheckbox, layoutRoot.FindAnyWidget( "marker_state_lock_checkbox" ) );

		for (int i = 0; i < ExpansionIcons.Count(); ++i)
		{
			m_IconTypesArray.Insert( new ExpansionMapMarkerIconItem( m_IconEntries, ExpansionIcons.Get( i ), this ) );
		}
		
		#ifdef EXPANSION_MAP_MENU_DEBUG
		EXLogPrint("ExpansionMapMarker::OnEditInit - End");
		#endif
	}
	
	// ------------------------------------------------------------
	// ExpansionMapMarker SetMapMenu
	// ------------------------------------------------------------
	void SetMapMenu( ExpansionMapMenu menu )
	{
		#ifdef EXPANSION_MAP_MENU_DEBUG
		EXLogPrint("ExpansionMapMarker::SetMapMenu - Start");
		#endif
		
		m_MapMenu = menu;
		
		#ifdef EXPANSION_MAP_MENU_DEBUG
		EXLogPrint("ExpansionMapMarker::SetMapMenu - End");
		#endif
	}
	
	// ------------------------------------------------------------
	// ExpansionMapMarker GetEditLayoutPath
	// ------------------------------------------------------------
	string GetEditLayoutPath()
	{
		return "DayZExpansion/GUI/layouts/map/expansion_map_marker_edit.layout";
	}
	
	// ------------------------------------------------------------
	// ExpansionMapMarker CanEditName
	// ------------------------------------------------------------	
	bool CanEditName()
	{
		return true;
	}
	
	// ------------------------------------------------------------
	// ExpansionMapMarker CanEdit
	// ------------------------------------------------------------	
	bool CanEdit()
	{
		return true;
	}
	
	// ------------------------------------------------------------
	// ExpansionMapMarker IsEditting
	// ------------------------------------------------------------
	bool IsEditting()
	{
		return m_EditPanel && m_EditPanel.IsVisible();
	}
	
	// ------------------------------------------------------------
	// ExpansionMapMarker SetCreation
	// ------------------------------------------------------------
	void SetCreation( bool creating )
	{
		#ifdef EXPANSION_MAP_MENU_DEBUG
		EXLogPrint("ExpansionMapMarker::SetCreation - Start");
		#endif
		
		m_Creating = creating;
		
		#ifdef EXPANSION_MAP_MENU_DEBUG
		EXLogPrint("ExpansionMapMarker::SetCreation - End");
		#endif
	}
	
	// ------------------------------------------------------------
	// ExpansionMapMarker IsCreating
	// ------------------------------------------------------------
	bool IsCreating()
	{
		return m_Creating;
	}
	
	// ------------------------------------------------------------
	// ExpansionMapMarker IsRGBSlider
	// ------------------------------------------------------------
	bool IsRGBSlider( Widget w )
	{
		return w == m_ColorRGB_Red || w == m_ColorRGB_Green || w == m_ColorRGB_Blue;
	}
	
	// ------------------------------------------------------------
	// ExpansionMapMarker IsEditButton
	// ------------------------------------------------------------
	override bool IsEditButton( Widget w )
	{
		return w == m_EditButton;
	}
	
	// ------------------------------------------------------------
	// ExpansionMapMarker IsDragWidget
	// ------------------------------------------------------------
	override bool IsDragWidget( Widget w )
	{
		return w == m_EditButton;
	}
	
	// ------------------------------------------------------------
	// ExpansionMapMarker SetMarkerData
	// ------------------------------------------------------------
	void SetMarkerData(  ExpansionMarkerData data )
	{
		#ifdef EXPANSION_MAP_MENU_DEBUG
		EXLogPrint("ExpansionMapMarker::SetMarkerData - Start");
		#endif
		
		m_Data = data;

		if ( m_Data )
		{
			m_Data.SetHandler( this );
			if ( HasBeenInitialized() )
				SetFromMarkerData();
		}
		
		#ifdef EXPANSION_MAP_MENU_DEBUG
		EXLogPrint("ExpansionMapMarker::SetMarkerData - End");
		#endif
	}
	
	// ------------------------------------------------------------
	// ExpansionMapMarker SetFromMarkerData
	// ------------------------------------------------------------
	void SetFromMarkerData()
	{
		#ifdef EXPANSION_MAP_MENU_DEBUG
		EXLogPrint("ExpansionMapMarker::SetFromMarkerData - Start");
		#endif
		
		if ( m_Data && !IsEditting() )
		{
			if ( !IsDragging() )
				SetPosition( m_Data.GetPosition() );
			else
				m_Data.SetPosition( GetPosition() );
			SetName( m_Data.GetName() );
			SetIcon( m_Data.GetIcon() );
			SetPrimaryColor( m_Data.GetColor() );
			GetLayoutRoot().Show( !ShouldHide() );

			if ( CanEdit() )
			{
				m_State3DCheckbox.SetChecked( m_Data.Is3D() );
				m_StateDragLockCheckbox.SetChecked(m_Data.GetLockState());
			}
		}
		
		#ifdef EXPANSION_MAP_MENU_DEBUG
		EXLogPrint("ExpansionMapMarker::SetFromMarkerData - End");
		#endif
	}
	
	// ------------------------------------------------------------
	// ExpansionMapMarker GetMarkerData
	// ------------------------------------------------------------
	ExpansionMarkerData GetMarkerData()
	{
		return m_Data;
	}
	
	// ------------------------------------------------------------
	// ExpansionMapMarker SetName
	// ------------------------------------------------------------
	void SetName( string name )
	{
		#ifdef EXPANSION_MAP_MENU_DEBUG
		EXLogPrint("ExpansionMapMarker::SetName - Start");
		#endif
		
		m_MarkerName = name;
		m_Name.SetText( name );
		m_EditName.SetText( name );
		
		#ifdef EXPANSION_MAP_MENU_DEBUG
		EXLogPrint("ExpansionMapMarker::SetName - End");
		#endif
	}
	
	// ------------------------------------------------------------
	// ExpansionMapMarker SetName
	// ------------------------------------------------------------
	void SetName( StringLocaliser name )
	{
		#ifdef EXPANSION_MAP_MENU_DEBUG
		EXLogPrint("ExpansionMapMarker::SetName - Start");
		#endif
		
		m_MarkerName = name.GetText();
		m_Name.SetText( name.GetText() );
		m_EditName.SetText( name.GetText() );
		
		#ifdef EXPANSION_MAP_MENU_DEBUG
		EXLogPrint("ExpansionMapMarker::SetName - End");
		#endif
	}
	
	// ------------------------------------------------------------
	// ExpansionMapMarker GetName
	// ------------------------------------------------------------
	string GetName()
	{
		return m_MarkerName;
	}
	
	// ------------------------------------------------------------
	// ExpansionMapMarker SetIcon
	// ------------------------------------------------------------
	void SetIcon(  ExpansionIcon icon )
	{
		#ifdef EXPANSION_MAP_MENU_DEBUG
		EXLogPrint("ExpansionMapMarker::SetIcon - Start");
		#endif
		
		m_CurrentIcon = icon;
		if ( m_CurrentIcon )
		{
			m_Icon.LoadImageFile( 0, m_CurrentIcon.Path );

			for ( int i = 0; i < m_IconTypesArray.Count(); ++i )
			{
				m_IconTypesArray[i].UpdateSelection( m_CurrentIcon );
			}
		}
		
		#ifdef EXPANSION_MAP_MENU_DEBUG
		EXLogPrint("ExpansionMapMarker::SetIcon - End");
		#endif
	}
	
	// ------------------------------------------------------------
	// ExpansionMapMarker SetIcon
	// ------------------------------------------------------------
	void SetIcon( string icon )
	{
		SetIcon( ExpansionIcons.Get( icon ) );
	}
	
	// ------------------------------------------------------------
	// ExpansionMapMarker GetIconWidget
	// ------------------------------------------------------------
	ImageWidget GetIconWidget()
	{
		return m_Icon;
	}
	
	// ------------------------------------------------------------
	// ExpansionMapMarker GetDragWidget
	// ------------------------------------------------------------	
	Widget GetDragWidget()
	{
		return m_DragWidget;
	}
	
	// ------------------------------------------------------------
	// ExpansionMapMarker SetPrimaryColor
	// ------------------------------------------------------------
	void SetPrimaryColor( int color )
	{
		#ifdef EXPANSION_MAP_MENU_DEBUG
		EXLogPrint("ExpansionMapMarker::SetPrimaryColor - Start");
		#endif
		
		m_PrimaryColor = color;

		if ( m_ColorRGB_Red && m_ColorRGB_Green && m_ColorRGB_Blue )
		{
			int a = ( color >> 24 ) & 0xFF;
			int r = ( color >> 16 ) & 0xFF;
			int g = ( color >> 8 ) & 0xFF;
			int b = ( color ) & 0xFF;

			m_ColorRGB_Red.SetCurrent( r );
			m_ColorRGB_Green.SetCurrent( g );
			m_ColorRGB_Blue.SetCurrent( b );
		}
		
		if ( !m_MouseHover )
		{
			m_Name.SetColor( m_PrimaryColor );
			m_Icon.SetColor( m_PrimaryColor );
		}

		for ( int i = 0; i < m_IconTypesArray.Count(); ++i )
		{
			m_IconTypesArray[i].SetColor( m_PrimaryColor );
		}
		
		#ifdef EXPANSION_MAP_MENU_DEBUG
		EXLogPrint("ExpansionMapMarker::SetPrimaryColor - End");
		#endif
	}
	
	// ------------------------------------------------------------
	// ExpansionMapMarker GetPrimaryColor
	// ------------------------------------------------------------
	int GetPrimaryColor()
	{
		return m_PrimaryColor;
	}
	
	// ------------------------------------------------------------
	// ExpansionMapMarker SetHoverColour
	// ------------------------------------------------------------
	void SetHoverColour( int color )
	{
		#ifdef EXPANSION_MAP_MENU_DEBUG
		EXLogPrint("ExpansionMapMarker::SetHoverColour - Start");
		#endif
		
		m_HoverColor = color;
		
		#ifdef EXPANSION_MAP_MENU_DEBUG
		EXLogPrint("ExpansionMapMarker::SetHoverColour - End");
		#endif
	}
	
	// ------------------------------------------------------------
	// ExpansionMapMarker GetHoverColor
	// ------------------------------------------------------------
	int GetHoverColor()
	{
		return m_HoverColor;
	}
	
	// ------------------------------------------------------------
	// ExpansionMapMarker IsMouseHovering
	// ------------------------------------------------------------
	bool IsMouseHovering()
	{
		return m_MouseHover;
	}

	private bool ShouldHide()
	{
		if ( !m_Data.IsMapVisible() )
			return true;

		if ( !m_MarkerModule.IsMapVisible( m_Data.GetType() ) )
			return true;

		return false;
	}
	
	// ------------------------------------------------------------
	// ExpansionMapMarker Update
	// ------------------------------------------------------------
	override void Update( float pDt )
	{
		#ifdef EXPANSION_MAP_MENU_UPDATE_DEBUG
		EXLogPrint("ExpansionMapMarker::Update - Start");
		#endif
		
		super.Update( pDt );
		
		
		if ( m_Data )
		{
			if ( ShouldHide() )
			{
				GetLayoutRoot().Show( false );

				if ( IsEditting() )
					CloseEditPanel();
			} else 
			{
				GetLayoutRoot().Show( true );

				if ( !IsDragging() )
					SetPosition( m_Data.GetPosition() );
			}
			

			if (m_MouseHover && KeyState(KeyCode.KC_LSHIFT)) {
				
				vector pos = GetPosition();				
				m_PositionToolTip = new ExpansionUITooltip(string.Format("<p>X: %1 Y: %2 Z: %3</p>", pos[0], pos[1], pos[2]), "<p>" + GetName() + "<p>");
				m_PositionToolTip.SetTextPos("center");
				int x, y;
				GetMousePos(x, y);
				m_PositionToolTip.SetPos(x, y);
				m_PositionToolTip.ShowTooltip();

			} else {
				delete m_PositionToolTip;
			}
			
		} else
		{
			GetLayoutRoot().Show( IsCreating() );
		}
		
		#ifdef EXPANSION_MAP_MENU_UPDATE_DEBUG
		EXLogPrint("ExpansionMapMarker::Update - End");
		#endif
	}
	
	// ------------------------------------------------------------
	// ExpansionMapMarker SetPosition
	// ------------------------------------------------------------
	override void SetPosition( vector position, bool performDropEvent = false )
	{
		#ifdef EXPANSION_MAP_MENU_UPDATE_DEBUG
		EXLogPrint("ExpansionMapMarker::SetPosition - Start");
		#endif
		
		position[1] = GetGame().SurfaceY( position[0], position[2] ) + 1.0;

		super.SetPosition( position, performDropEvent );
		
		#ifdef EXPANSION_MAP_MENU_UPDATE_DEBUG
		EXLogPrint("ExpansionMapMarker::SetPosition - End");
		#endif
	}
	
	bool IsDragLocked()
	{
		return m_DragLocked && !ExpansionStatic.Key_SHIFT();
	}

	override bool OnDrag( Widget w, int x, int y )
	{
		if ( IsDragLocked() )
			return false;

		return super.OnDrag( w, x, y );
	}

	// ------------------------------------------------------------
	// ExpansionMapMarker OnDrop
	// ------------------------------------------------------------
	override void OnDrop()
	{
		#ifdef EXPANSION_MAP_MENU_DEBUG
		EXLogPrint("ExpansionMapMarker::OnDrop - Start");
		#endif
		
		super.OnDrop();

		if ( IsDragLocked() && !IsDragging() )
			return;

		if ( IsCreating() )
			return;

		if ( !m_Data )
			return;


		int type = m_Data.GetType();
		switch ( type )
		{
		case ExpansionMapMarkerType.PERSONAL:
			m_Data.SetPosition( GetPosition() );
			break;
		case ExpansionMapMarkerType.PARTY:
			ExpansionPartyModule party;
			if ( Class.CastTo( party, GetModuleManager().GetModule( ExpansionPartyModule ) ) )
				party.UpdatePositionMarker( m_Data.GetUID(), GetPosition() );
			break;
		default:
			Error( "Marker type " + typename.EnumToString( ExpansionMapMarkerType, type ) + " not supported." );
			return;
		}
		
		#ifdef EXPANSION_MAP_MENU_DEBUG
		EXLogPrint("ExpansionMapMarker::OnDrop - End");
		#endif
	}
	
	// ------------------------------------------------------------
	// ExpansionMapMarker ShowEditPanel
	// ------------------------------------------------------------
	void ShowEditPanel()
	{
		#ifdef EXPANSION_MAP_MENU_DEBUG
		EXLogPrint("ExpansionMapMarker::ShowEditPanel - Start");
		#endif
		
		if ( !CanEdit() || !m_MapMenu )
			return;

		if ( !m_Data )
		{
			SetName( "New Marker" );
		}

		if ( !IsCreating() && !m_MapMenu.SetSelectedMarker( m_Data ) )
			return;

		SetFromMarkerData();

		m_Frame.SetColor( ARGB( 200, 50, 50, 50 ) );

		m_EditPanel.Show( true );

		if ( CanEditName() )
		{
			m_EditName.Show( true );
			m_EditName.SetText("");
			m_Name.Show( false );
		}

		if ( IsCreating() )
		{
			bool canCreateParty = false;
			ExpansionPartyModule party;
			if ( Class.CastTo( party, GetModuleManager().GetModule( ExpansionPartyModule ) ) )
				canCreateParty = party.HasParty() && GetExpansionSettings().GetParty().CanCreatePartyMarkers && GetExpansionSettings().GetParty().EnableParties;
			
			m_StatePartyContainer.Show( canCreateParty );
			
			m_State3DContainer.Show( GetExpansionSettings().GetMap().CanCreate3DMarker );
			m_State3DCheckbox.Show( GetExpansionSettings().GetMap().CanCreate3DMarker );
			m_State3DCheckbox.SetChecked( false );
			m_StateDragLockCheckbox.SetChecked(GetExpansionClientSettings().DefaultMarkerLockState);

			m_LeftButton.SetText( "#STR_EXPANSION_MAP_MARKER_CANCEL_BUTTON_LABLE" );
			m_RightButton.SetText( "#STR_EXPANSION_MAP_MARKER_CREATE_BUTTON_LABLE" );
		} else
		{
			m_StatePartyContainer.Show( false );

			if ( !m_Data.Is3D() )
			{
				m_State3DContainer.Show( GetExpansionSettings().GetMap().CanCreate3DMarker );
				m_State3DCheckbox.Show( GetExpansionSettings().GetMap().CanCreate3DMarker );
			}

			m_EditName.SetText(m_Data.GetName());
			m_LeftButton.SetText( "#STR_EXPANSION_MAP_MARKER_DELETE_SHORT_BUTTON_LABLE" );
			m_RightButton.SetText( "#STR_EXPANSION_MAP_MARKER_UPDATE_TITLE" );
		}
		
		ExpansionMapMenu mapmenu = ExpansionMapMenu.Cast( GetGame().GetUIManager().GetMenu() );
		if(mapmenu)
		{
			mapmenu.SetIsEditingMarker(true);			
		}
		
		#ifdef EXPANSION_MAP_MENU_DEBUG
		EXLogPrint("ExpansionMapMarker::ShowEditPanel - End");
		#endif
	}
	
	// ------------------------------------------------------------
	// ExpansionMapMarker CloseEditPanel
	// ------------------------------------------------------------
	void CloseEditPanel()
	{
		#ifdef EXPANSION_MAP_MENU_DEBUG
		EXLogPrint("ExpansionMapMarker::CloseEditPanel - Start");
		#endif
		
		m_Frame.SetColor( ARGB( 0, 0, 0, 0 ) );

		m_EditPanel.Show( false );

		m_EditName.Show( false );
		m_Name.Show( true );
		
		ExpansionMapMenu mapmenu = ExpansionMapMenu.Cast( GetGame().GetUIManager().GetMenu() );
		if(mapmenu)
		{
			mapmenu.SetIsEditingMarker(false);			
		}
		
		#ifdef EXPANSION_MAP_MENU_DEBUG
		EXLogPrint("ExpansionMapMarker::CloseEditPanel - End");
		#endif
	}
	
	// ------------------------------------------------------------
	// ExpansionMapMarker OnCancleButtonClick
	// ------------------------------------------------------------
	void OnCancleButtonClick()
	{
		#ifdef EXPANSION_MAP_MENU_DEBUG
		EXLogPrint("ExpansionMapMarker::OnCancleButtonClick - Start");
		#endif

		if (CanEdit())
		{
			CloseEditPanel();
	
			SetFromMarkerData();
	
			ExpansionMapMenu menu;
			if ( Class.CastTo( menu, GetGame().GetUIManager().GetMenu() ) )
			{
				if ( IsCreating() )
				{
					menu.DeletePreviewMarker();
				} else {
					menu.DeleteSelectedMarker();
				}
			}
		}
		
		#ifdef EXPANSION_MAP_MENU_DEBUG
		EXLogPrint("ExpansionMapMarker::OnCancleButtonClick - End");
		#endif
	}
	
	// ------------------------------------------------------------
	// ExpansionMapMarker OnCreateButtonClick
	// ------------------------------------------------------------
	void OnCreateButtonClick()
	{
		#ifdef EXPANSION_MAP_MENU_DEBUG
		EXLogPrint("ExpansionMapMarker::OnCreateButtonClick - Start");
		#endif
		
		if (CanEdit())
		{
			string name = m_MarkerName;
			
			if ( CanEditName() )
			{
				name = m_EditName.GetText();
			}
			
			CloseEditPanel();
	
			if ( IsCreating() )
			{
				if ( m_StatePartyCheckbox.IsChecked() )
				{
					m_Data = ExpansionMarkerData.Create( ExpansionMapMarkerType.PARTY );
				} else
				{
					m_Data = ExpansionMarkerData.Create( ExpansionMapMarkerType.PERSONAL );
				}
			}
	
			m_Data.SetName( name );
			m_Data.SetIcon( m_CurrentIcon );
			m_Data.SetColor( m_PrimaryColor );
			m_Data.SetPosition( GetPosition() );
			m_Data.Set3D( m_State3DCheckbox.IsChecked() );
			m_Data.SetLockState( m_StateDragLockCheckbox.IsChecked() );
	
			ExpansionMapMenu menu;
			if ( Class.CastTo( menu, GetGame().GetUIManager().GetMenu() ) )
			{
				if ( IsCreating() )
				{
					menu.CompleteCreationNewMarker();
				} else
				{
					menu.UpdateSelectedMarker();
				}
			}
	
			SetFromMarkerData();
	
			m_Creating = false;
		}
		
		#ifdef EXPANSION_MAP_MENU_DEBUG
		EXLogPrint("ExpansionMapMarker::OnCreateButtonClick - End");
		#endif
	}
		
	// ------------------------------------------------------------
	// ExpansionMapMarker OnEdit
	// ------------------------------------------------------------
	override void OnEdit()
	{
		#ifdef EXPANSION_MAP_MENU_DEBUG
		EXLogPrint("ExpansionMapMarker::OnEdit - Start");
		#endif
		
		ShowEditPanel();
		
		#ifdef EXPANSION_MAP_MENU_DEBUG
		EXLogPrint("ExpansionMapMarker::OnEdit - End");
		#endif
	}

	void FocusOnMarker( bool shouldEdit = false )
	{
		GetMapWidget().SetScale( 0.05 );
		GetMapWidget().SetMapPos( GetPosition() );

		if ( m_Data )
		{
			if ( m_Data.GetType() == ExpansionMapMarkerType.PARTY || m_Data.GetType() == ExpansionMapMarkerType.PERSONAL )
			{
				if ( shouldEdit )
				{
					ShowEditPanel();
				}
			}
		}
	}
		
	// ------------------------------------------------------------
	// ExpansionMapMarker OnClick
	// ------------------------------------------------------------
	override bool OnClick( Widget w, int x, int y, int button )
	{		
		if ( w != NULL )
		{
			if ( w == m_LeftButton )
			{
				OnCancleButtonClick();

				return true;
			}

			if ( w == m_RightButton )
			{
				OnCreateButtonClick();

				return true;
			}
		}

		return super.OnClick( w, x, y, button );
	}
		
	// ------------------------------------------------------------
	// ExpansionMapMarker OnChange
	// ------------------------------------------------------------	
	override bool OnChange( Widget w, int x, int y, bool finished )
	{
		if ( w != NULL && IsRGBSlider( w ) )
		{
			SetPrimaryColor( ARGB( 255, m_ColorRGB_Red.GetCurrent(), m_ColorRGB_Green.GetCurrent(), m_ColorRGB_Blue.GetCurrent() ) );

			return true;
		}
		
		if (w != NULL && w == m_StateDragLockCheckbox)
		{
			if (m_StateDragLockCheckbox.IsChecked())
			{
				m_DragLocked = true;
			}
			else
			{
				m_DragLocked = false;
			}
			
			return true;
		}

		return super.OnChange( w, x, y, finished );
	}
	
 	// ------------------------------------------------------------
	// ExpansionMapMarker OnMouseEnter
	// ------------------------------------------------------------   
	override bool OnMouseEnter( Widget w, int x, int y )
	{		
		if ( w != NULL && IsEditButton( w ) )
		{
			m_MouseHover = true;
			m_Name.SetColor( m_HoverColor );
			m_Icon.SetColor( m_HoverColor );
			return true;
		}

		return super.OnMouseEnter( w, x, y );
	}
	
 	// ------------------------------------------------------------
	// ExpansionMapMarker OnMouseLeave
	// ------------------------------------------------------------
	override bool OnMouseLeave( Widget w, Widget enterW, int x, int y )
	{		
		if ( w != NULL && IsEditButton( w ) )
		{
			m_MouseHover = false;
			m_Name.SetColor( m_PrimaryColor );
			m_Icon.SetColor( m_PrimaryColor );
			return true;
		}

		return super.OnMouseLeave( w, enterW, x, y );
	}
 	
	// ------------------------------------------------------------
	// ExpansionMapMarker SetIgnorePointer
	// ------------------------------------------------------------	
	void SetIgnorePointer(bool state)
	{
		if (state)
		{
			GetLayoutRoot().SetFlags(WidgetFlags.IGNOREPOINTER);
			m_DragWidget.SetFlags(WidgetFlags.IGNOREPOINTER);
			m_EditButton.SetFlags(WidgetFlags.IGNOREPOINTER);
		}
		else
		{
			GetLayoutRoot().ClearFlags(WidgetFlags.IGNOREPOINTER);
			m_DragWidget.ClearFlags(WidgetFlags.IGNOREPOINTER);
			m_EditButton.ClearFlags(WidgetFlags.IGNOREPOINTER);
		}
	}
};