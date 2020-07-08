/**
 * ExpansionMapMenuMarkerWindow.c
 *
 * DayZ Expansion Mod
 * www.dayzexpansion.com
 * © 2020 DayZ Expansion Mod Team
 *
 * This work is licensed under the Creative Commons Attribution-NonCommercial-NoDerivatives 4.0 International License. 
 * To view a copy of this license, visit http://creativecommons.org/licenses/by-nc-nd/4.0/.
 *
*/

class ExpansionMapMenuMarkerWindow extends ScriptedWidgetEventHandler
{
	protected Widget m_Root;
	protected Widget m_MarkerCreationWindow;
	protected TextWidget m_MarkerCreationWindowLable;
	protected GridSpacerWidget m_MarkerEntrysGrid;
	protected SliderWidget m_MarkerColorRedSlider;
	protected SliderWidget m_MarkerColorGreenSlider;
	protected SliderWidget m_MarkerColorBlueSlider;
	protected SliderWidget m_MarkerAlphaSlider;
	protected TextWidget m_OptionsHeaderText;
	protected Widget m_PartyOptionsPanel;
	protected CheckBoxWidget m_PartyMarkerCheckBox;
	protected Widget m_3DOptionsPanel;
	protected CheckBoxWidget m_3DMarkerCheckBox;
	protected EditBoxWidget m_MarkerNameEditBox;
	protected ImageWidget m_MarkerIconPreview;
	protected ButtonWidget m_CancelButton;
	protected TextWidget m_CancelButtonLable;
	protected ButtonWidget m_CreateButton;
	protected TextWidget m_CreateButtonLable;
	protected ButtonWidget m_DeleteButton;
	protected ButtonWidget m_TooltipButton;
	protected ImageWidget m_TooltipButtonIcon;

	protected Widget m_ArrowColorPanel;
	protected SliderWidget m_ArrowColorRedSlider;
	protected SliderWidget m_ArrowColorGreenSlider;
	protected SliderWidget m_ArrowColorBlueSlider;
	protected SliderWidget m_ArrowAlphaSlider;
	protected ButtonWidget m_ArrowCancelButton;
	protected ImageWidget m_ArrowCancelButtonIcon;
	protected ButtonWidget m_ArrowConfirmButton;
	protected ImageWidget m_ArrowConfirmButtonIcon;
	
	protected Widget m_MapStatsWindow;
	protected TextWidget m_MapStatsCursorPositionValue;
	
	protected ref ExpansionMapMenu m_MapMenu;
	protected MapWidget m_MapWidget;
	protected ref ExpansionMapMenuMarkerEntry m_CurrentSelectedMarker;

	protected ref array<ref ExpansionMapMenuMarkerEntry> m_MarkerIconsEntrys;
	protected ref array<ref ExpansionMapMarker> m_MarkerList;
	
	protected int m_MarkerColorRed = 255;
	protected int m_MarkerColorGreen = 255;
	protected int m_MarkerColorBlue = 255;
	protected int m_ArrowColorRed = 255;
	protected int m_ArrowColorGreen = 255;
	protected int m_ArrowColorBlue = 255;
	protected int m_ArrowColorAlpha = 255;
	protected int m_MarkerAlpha = 255;
	
	protected ref ExpansionMapMarkerModule m_MarkerModule;
	protected ref ExpansionPartyModule m_PartyModule;
	
	protected PlayerBase m_Player;
	autoptr array< ref ExpansionMarkerIcon > m_MarkersIconList;
	protected ref ExpansionMapMarker m_CurrentEditingMarker;
	protected ref ExpansionMapMenuMarker m_CurrentEditingMapMarker;
	protected bool m_IsEditingMarker = false;
	protected ref ExpansionUITooltip m_Tooltip;
	protected int m_MarkerIconIndex;
	
	// ------------------------------------------------------------
	// Expansion ExpansionMapMenuMarkerWindow Constructor
	// ------------------------------------------------------------
	void ExpansionMapMenuMarkerWindow(Widget parent, ExpansionMapMenu menu, MapWidget mapwidget)
	{
		m_Root = GetGame().GetWorkspace().CreateWidgets("DayZExpansion/GUI/layouts/map/expansion_marker_window.layout", parent);
		
		// Marker creation window
		m_MarkerCreationWindow = Widget.Cast(m_Root.FindAnyWidget("marker_window_content"));
		m_MarkerCreationWindowLable = TextWidget.Cast(m_Root.FindAnyWidget("marker_window_header_text"));
		m_MarkerEntrysGrid = GridSpacerWidget.Cast(m_Root.FindAnyWidget("marker_icons_content"));
		m_MarkerNameEditBox	= EditBoxWidget.Cast( m_Root.FindAnyWidget( "marker_name_setting_text" ) );
		
		m_MarkerColorRedSlider = SliderWidget.Cast( m_Root.FindAnyWidget( "marker_color_red_setting_slider" ) );
		m_MarkerColorGreenSlider = SliderWidget.Cast( m_Root.FindAnyWidget( "marker_color_green_setting_slider" ) );
		m_MarkerColorBlueSlider = SliderWidget.Cast( m_Root.FindAnyWidget( "marker_color_blue_setting_slider" ) );
		m_MarkerAlphaSlider	= SliderWidget.Cast( m_Root.FindAnyWidget( "marker_alpha_setting_slider" ) );

		m_OptionsHeaderText = TextWidget.Cast( m_Root.FindAnyWidget( "marker_options_header_text" ) );
		m_PartyOptionsPanel = Widget.Cast( m_Root.FindAnyWidget( "marker_party_option" ) );
		m_PartyMarkerCheckBox = CheckBoxWidget.Cast( m_Root.FindAnyWidget( "marker_party_option_checkbox" ) );
		m_3DOptionsPanel = Widget.Cast( m_Root.FindAnyWidget( "marker_3D_option" ) );
		m_3DMarkerCheckBox = CheckBoxWidget.Cast( m_Root.FindAnyWidget( "marker_3D_option_checkbox" ) );

		m_MarkerIconPreview	= ImageWidget.Cast( m_Root.FindAnyWidget( "marker_icon_image" ) );
		m_MarkerIconPreview.Show(false);

		m_CancelButton = ButtonWidget.Cast( m_Root.FindAnyWidget( "cancel_button" ) );
		m_CancelButtonLable = TextWidget.Cast( m_Root.FindAnyWidget( "cancel_lable" ) );
		m_CreateButton = ButtonWidget.Cast( m_Root.FindAnyWidget( "create_button" ) );
		m_CreateButtonLable = TextWidget.Cast( m_Root.FindAnyWidget( "create_lable" ) );
		m_DeleteButton = ButtonWidget.Cast( m_Root.FindAnyWidget( "delete_button" ) );
		
		m_TooltipButton = ButtonWidget.Cast( m_Root.FindAnyWidget("marker_info_button") );
		m_TooltipButtonIcon = ImageWidget.Cast( m_Root.FindAnyWidget("marker_info_icon") );

		m_ArrowColorPanel = Widget.Cast( m_Root.FindAnyWidget( "arrow_edit_content" ) );
		m_ArrowColorRedSlider = SliderWidget.Cast( m_Root.FindAnyWidget( "arrow_color_red_setting_slider" ) );
		m_ArrowColorGreenSlider = SliderWidget.Cast( m_Root.FindAnyWidget( "arrow_color_green_setting_slider" ) );
		m_ArrowColorBlueSlider = SliderWidget.Cast( m_Root.FindAnyWidget( "arrow_color_blue_setting_slider" ) );
		m_ArrowAlphaSlider = SliderWidget.Cast( m_Root.FindAnyWidget( "arrow_color_alpha_setting_slider" ) );
		m_ArrowCancelButton = ButtonWidget.Cast( m_Root.FindAnyWidget("arrow_cancel") );
		m_ArrowCancelButtonIcon = ImageWidget.Cast( m_Root.FindAnyWidget("arrow_cancel_icon") );
		m_ArrowConfirmButton = ButtonWidget.Cast( m_Root.FindAnyWidget("arrow_confirm") );
		m_ArrowConfirmButtonIcon = ImageWidget.Cast( m_Root.FindAnyWidget("arrow_confirm_icon") );
		
		m_MapStatsWindow = Widget.Cast( m_Root.FindAnyWidget("marker_window_stats_content") );
		m_MapStatsCursorPositionValue = TextWidget.Cast( m_Root.FindAnyWidget("PositionValue") );
		
		m_MapMenu = menu;
		m_MapWidget = mapwidget;
		
		m_Root.SetHandler(this);
		
		Init();
	}
	
	// ------------------------------------------------------------
	// Expansion ExpansionMapMenuMarkerWindow Deconstructor
	// ------------------------------------------------------------
	void ~ExpansionMapMenuMarkerWindow()
	{
		delete m_Root;
	}
	
	// ------------------------------------------------------------
	// Expansion ExpansionMapMenuMarkerWindow Init
	// ------------------------------------------------------------
	void Init()
	{
		//! Get Player Base
		m_Player = PlayerBase.Cast( GetGame().GetPlayer() );
		
		//! Get Marker Module
		m_MarkerModule = ExpansionMapMarkerModule.Cast(GetModuleManager().GetModule(ExpansionMapMarkerModule));
		
		//! Get Party Module
		m_PartyModule = ExpansionPartyModule.Cast(GetModuleManager().GetModule(ExpansionPartyModule));
		
		//! Map Tooltip
		m_Tooltip = new ExpansionUITooltip("#STR_EXPANSION_MAP_TOOLTIP_TEXT", "#STR_EXPANSION_MAP_TOOLTIP_TITLE"); 
		
		//! Map Posititon Arrow Color
		LoadArrowColor();
		
		//! Map Marker Window - Marker Icons List
		m_MarkerIconsEntrys = new array<ref ExpansionMapMenuMarkerEntry>;
		m_MarkerList = new array<ref ExpansionMapMarker>;
		m_MarkersIconList = new array< ref ExpansionMarkerIcon >;
		JsonFileLoader< ref array< ref ExpansionMarkerIcon > >.JsonLoadFile( "DayZExpansion/Scripts/Data/Markers.json", m_MarkersIconList );
		FillMarkerList(m_MarkersIconList);
	}
	
	// ------------------------------------------------------------
	// Expansion FillMarkerList
	// ! Fills the marker list with entrys for each marker in m_MarkersIconList
	// ------------------------------------------------------------
	void FillMarkerList(ref array<ref ExpansionMarkerIcon> icons)
	{
		GridSpacerWidget markerGrid = GridSpacerWidget.Cast(m_Root.FindAnyWidget("marker_icons_content"));
		if (markerGrid && icons)
		{
			for (int i = 0; i < icons.Count(); ++i)
			{
				ExpansionMarkerIcon currentIcon = icons.Get(i);
				if (currentIcon)
				{
					ExpansionMapMenuMarkerEntry markerEntry = new ExpansionMapMenuMarkerEntry(m_MarkerEntrysGrid, currentIcon, this);
					m_MarkerIconsEntrys.Insert(markerEntry);

					//Set by default the first marker entry
					/*if (i == 0)
						SetMarkerSelection( markerEntry );*/
				}
			}
		}
	}
	
	// ------------------------------------------------------------
	// Expansion GetMarkerWindowRoot
	// ------------------------------------------------------------
	Widget GetMarkerWindowRoot()
	{
		return m_Root;
	}

	// ------------------------------------------------------------
	// Expansion ShowMarkerWindow
	// ------------------------------------------------------------
	void ShowMarkerWindow(bool state)
	{
		m_MarkerCreationWindow.Show(state);
	}
	
	// ------------------------------------------------------------
	// Expansion GetMarkerWindowState
	// ------------------------------------------------------------
	bool GetMarkerWindowState()
	{
		if (m_MarkerCreationWindow.IsVisible())
			return true;
		
		return false;
	}

	// ------------------------------------------------------------
	// Expansion ShowArrowColorPanel
	// ------------------------------------------------------------
	void ShowArrowColorPanel(bool state)
	{
		m_ArrowColorPanel.Show(state);
	}
	
	// ------------------------------------------------------------
	// Expansion GetArrowColorPanelState
	// ------------------------------------------------------------
	bool GetArrowColorPanelState()
	{
		if (m_ArrowColorPanel.IsVisible())
			return true;
		
		return false;
	}
	
	// ------------------------------------------------------------
	// Expansion ShowMapStatWindow
	// ------------------------------------------------------------
	void ShowMapStatWindow(bool state)
	{
		m_MapStatsWindow.Show(state);
	}
	
	// ------------------------------------------------------------
	// Expansion GetMapStatWindowState
	// ------------------------------------------------------------
	bool GetMapStatWindowState()
	{
		if (m_MapStatsWindow.IsVisible())
			return true;
		
		return false;
	}

	// ------------------------------------------------------------
	// Expansion SetMarkerSelection
	// ------------------------------------------------------------
	void SetMarkerSelection(ref ExpansionMapMenuMarkerEntry data)
	{
		m_CurrentSelectedMarker = data;

		UpdateMarkerPreview();
		
		if (m_MapMenu.GetTempMarker() && m_CurrentSelectedMarker.GetMarkerData().Path != "")
		{
			m_MapMenu.GetTempMarker().ChangeIcon(m_CurrentSelectedMarker.GetMarkerData().Path);
		}
		
		if (m_CurrentEditingMapMarker && m_CurrentSelectedMarker.GetMarkerData().Path != "")
		{
			m_CurrentEditingMapMarker.ChangeIcon(m_CurrentSelectedMarker.GetMarkerData().Path);
		}
	}
	
	// ------------------------------------------------------------
	// Expansion GetSelecterMarker
	// ------------------------------------------------------------
	ref ExpansionMapMenuMarkerEntry GetSelecterMarker()
	{
		return m_CurrentSelectedMarker;
	}
	
	// ------------------------------------------------------------
	// Expansion UpdateMarkerPreview
	// ------------------------------------------------------------
	void UpdateMarkerPreview()
	{
		if (m_CurrentSelectedMarker && m_CurrentSelectedMarker.GetMarkerData().Path != "")
		{
			m_MarkerIconPreview.LoadImageFile(0, GetSelecterMarker().GetMarkerData().Path);
			m_MarkerIconPreview.Show(true);
			
			UpdatePreviewColor();
		}
	}
	
	// ------------------------------------------------------------
	// Expansion UpdatePreviewColor
	// Update color of preview icon of marker
	// ------------------------------------------------------------	
	void UpdatePreviewColor()
	{
		m_MarkerIconPreview.SetColor(ARGB(m_MarkerAlpha, m_MarkerColorRed, m_MarkerColorGreen, m_MarkerColorBlue));
		
		if (m_MapMenu.GetTempMarker())
		{
			ExpansionMapMenuMarker marker = m_MapMenu.GetTempMarker();
			marker.ChangeColor(ARGB(m_MarkerAlpha, m_MarkerColorRed, m_MarkerColorGreen, m_MarkerColorBlue));
		}
		
		if (m_CurrentEditingMapMarker)
		{
			m_CurrentEditingMapMarker.ChangeColor(ARGB(m_MarkerAlpha, m_MarkerColorRed, m_MarkerColorGreen, m_MarkerColorBlue));
		}
	}
	
	// ------------------------------------------------------------
	// Expansion UpdateTempMarkerName
	// ------------------------------------------------------------
	void UpdateMarkerNames()
	{
		if (m_MapMenu.GetTempMarker())
		{
			ExpansionMapMenuMarker marker = m_MapMenu.GetTempMarker();
			marker.ChangeName( m_MarkerNameEditBox.GetText() );
		}
		
		if (m_CurrentEditingMapMarker)
		{
			m_CurrentEditingMapMarker.ChangeName( m_MarkerNameEditBox.GetText() );
		}
	}
	
	// ------------------------------------------------------------
	// Expansion ShowPartyMarkerOption
	// ------------------------------------------------------------	
	void ShowPartyMarkerOption(bool state)
	{
		m_PartyOptionsPanel.Show(state);
	}
	
	// ------------------------------------------------------------
	// Expansion Show3DMarkerOption
	// ------------------------------------------------------------	
	void Show3DMarkerOption(bool state)
	{
		m_3DOptionsPanel.Show(state);
	}
	
	// ------------------------------------------------------------
	// Expansion UpdateVisiblityOptionsHeader
	// ------------------------------------------------------------	
	void UpdateVisiblityOptionsHeader()
	{
		m_OptionsHeaderText.Show( m_PartyOptionsPanel.IsVisible() || m_3DOptionsPanel.IsVisible() );
	}
	
	// ------------------------------------------------------------
	// Expansion CreateMarker
	// Events when marker creation button is clicked
	// ------------------------------------------------------------	
	void CreateMarker()
	{
		int minNameLength = 3;
		int maxNameLength = 32;
		TStringArray allowedCharacters = {"A","B","C","D","E","F","G","H","I","J","K","L","M","N","O","P","Q","R","S","T","U","V","W","X","Y","Z","a","b","c","d","e","f","g","h","i","j","k","l","m","n","o","p","q","r","s","t","u","v","w","x","y","z","0","1","2","3","4","5","6","7","8","9","_"," ","-","?","!","&"};

		string markerName 	= m_MarkerNameEditBox.GetText();
		
		if (m_CurrentSelectedMarker)
		{
			m_MarkerIconIndex = m_CurrentSelectedMarker.GetMarkerIconIndex();
		}
		else
		{
			m_MarkerIconIndex = -1;
		}
		
		vector markerPos 	= m_MapMenu.GetCurrentMarkerPosition();
		int markerColor 	= ARGB(m_MarkerAlpha, m_MarkerColorRed, m_MarkerColorGreen, m_MarkerColorBlue);
		bool isPartyMarker  = m_PartyMarkerCheckBox.IsChecked();
		bool is3DMarker  	= m_3DMarkerCheckBox.IsChecked();

		string markerName2;
		string currentMarkerName;
		ExpansionMapMarker currentMarker;

		//! Check marker name lenght (Min:3 / Max: 32 characters)
		if (markerName.Length() < minNameLength || markerName.Length() > maxNameLength) 
		{
			GetNotificationSystem().CreateNotification( new StringLocaliser( "STR_EXPANSION_MARKER_SYSTEM_TITLE" ), new StringLocaliser( "STR_EXPANSION_MARKER_SYSTEM_NAME_LENGHT" ), EXPANSION_NOTIFICATION_ICON_ERROR, COLOR_EXPANSION_NOTIFICATION_ERROR, 5 );
			return;
		}

		//! Check if they are arny illigal characters in the marker name
		for ( int i = 0; i < markerName.Length(); i++ )
		{
			if (allowedCharacters.Find(markerName.Get(i)) == -1)
			{
				GetNotificationSystem().CreateNotification( new StringLocaliser( "STR_EXPANSION_MARKER_SYSTEM_TITLE" ), new StringLocaliser( "STR_EXPANSION_MARKER_SYSTEM_ILLEGAL_CHARACTER" ), EXPANSION_NOTIFICATION_ICON_ERROR, COLOR_EXPANSION_NOTIFICATION_ERROR, 5 );
				return;
			}
		}

		//! Check if a marker icon is selected
		if ( m_MarkerIconIndex <= 0 )
		{
			GetNotificationSystem().CreateNotification( new StringLocaliser( "STR_EXPANSION_MARKER_SYSTEM_TITLE" ), new StringLocaliser( "STR_EXPANSION_MARKER_NO_MARKER_SELECTED" ), EXPANSION_NOTIFICATION_ICON_ERROR, COLOR_EXPANSION_NOTIFICATION_ERROR, 5 );
			
			return;
		}

		if (!markerPos)
		{
			GetNotificationSystem().CreateNotification( new StringLocaliser( "STR_EXPANSION_MARKER_SYSTEM_TITLE" ), new StringLocaliser( "STR_EXPANSION_MARKER_NO_POSITION_SELECTED" ), EXPANSION_NOTIFICATION_ICON_ERROR, COLOR_EXPANSION_NOTIFICATION_ERROR, 5 );
			
			return;
		}

		//Check if created marker is a a party marker
		if ( isPartyMarker && m_PartyModule.HasParty() )
		{
			//Check if marker name exist in party markers
			markerName2 = markerName;
			markerName2.ToLower();

			ref array<ref ExpansionMapMarker> markers = m_PartyModule.GetParty().GetAllMarkers();

			for (int j = 0; j < markers.Count(); ++j)
			{
				currentMarker = markers.Get(j);
				currentMarkerName = currentMarker.GetMarkerText();
				currentMarkerName.ToLower();

				if (markerName2 == currentMarkerName)
				{
					GetNotificationSystem().CreateNotification( new StringLocaliser( "STR_EXPANSION_MARKER_SYSTEM_TITLE" ), new StringLocaliser( "STR_EXPANSION_MARKER_SYSTEM_MARKER_NAME_ALREADY_EXIST" ), EXPANSION_NOTIFICATION_ICON_ERROR, COLOR_EXPANSION_NOTIFICATION_ERROR, 5 );
					return;
				}
			}

			// Add party marker
			m_MapMenu.AddPartyMarker(markerName, m_MarkerIconIndex, markerPos, markerColor, is3DMarker);
			
			CancelMarkerEdit();
			
			// The player already have a notification for party marker being created in ExpansionPartyModule.c line 1206
			// GetNotificationSystem().CreateNotification( new StringLocaliser( "STR_EXPANSION_MARKER_SYSTEM_TITLE" ), new StringLocaliser( "STR_EXPANSION_MARKER_SYSTEM_ADDED_MARKER" ), EXPANSION_NOTIFICATION_ICON_CHECK, COLOR_EXPANSION_NOTIFICATION_SUCCSESS, 5 );
		}
		else
		{
			//Check if marker name exist in personnal markers
			markerName2 = markerName;
			markerName2.ToLower();

			for (int k = 0; k < m_MarkerModule.Count(); ++k)
			{
				currentMarker = m_MarkerModule.GetMarker(k);
				currentMarkerName = currentMarker.GetMarkerText();
				currentMarkerName.ToLower();

				if (markerName2 == currentMarkerName)
				{
					GetNotificationSystem().CreateNotification( new StringLocaliser( "STR_EXPANSION_MARKER_SYSTEM_TITLE" ), new StringLocaliser( "STR_EXPANSION_MARKER_SYSTEM_MARKER_NAME_ALREADY_EXIST", markerName ), EXPANSION_NOTIFICATION_ICON_ERROR, COLOR_EXPANSION_NOTIFICATION_ERROR, 5 );
					return;
				}
			}

			// Add personal marker
			m_MapMenu.AddPersonalMarker(markerName, m_MarkerIconIndex, markerPos, markerColor, is3DMarker);
			
			CancelMarkerEdit();
			
			GetNotificationSystem().CreateNotification( new StringLocaliser( "STR_EXPANSION_MARKER_SYSTEM_TITLE" ), new StringLocaliser( "STR_EXPANSION_MARKER_SYSTEM_ADDED_MARKER", markerName ), EXPANSION_NOTIFICATION_ICON_CHECK, COLOR_EXPANSION_NOTIFICATION_SUCCSESS, 5 );
		}
		
		if (m_MapMenu.GetTempMarker())
		{
			m_MapMenu.GetTempMarker().DeleteMarker();
		}
	}
	
	// ------------------------------------------------------------
	// Expansion CancelMarkerEdit
	// Events when marker edit is canceled
	// ------------------------------------------------------------	
	void CancelMarkerEdit()
	{
		m_MarkerCreationWindowLable.SetText("#STR_EXPANSION_MAP_MARKER_CREATION_TITLE");
		m_CreateButtonLable.SetText("#STR_EXPANSION_MAP_MARKER_CREATE_BUTTON_LABLE");
		m_CancelButtonLable.SetText("#STR_EXPANSION_MAP_MARKER_CANCEL_BUTTON_LABLE");
		m_DeleteButton.Show(false);
		
		m_MarkerIconPreview.Show(false);
		m_MarkerNameEditBox.SetText("");
		
		m_MarkerAlphaSlider.SetCurrent(255);
		m_MarkerColorRedSlider.SetCurrent(255);
		m_MarkerColorGreenSlider.SetCurrent(255);
		m_MarkerColorBlueSlider.SetCurrent(255);

		m_MarkerAlpha = m_MarkerAlphaSlider.GetCurrent();
		m_MarkerColorRed = m_MarkerColorRedSlider.GetCurrent();
		m_MarkerColorGreen = m_MarkerColorGreenSlider.GetCurrent();
		m_MarkerColorBlue = m_MarkerColorBlueSlider.GetCurrent();

		m_CurrentEditingMapMarker = NULL;
		m_CurrentEditingMarker = NULL;
		m_IsEditingMarker = false;
		
		if (m_MapMenu)
		{
			m_MapMenu.SetEditingMarker(false);
			m_MapMenu.LoadMarkers();
			
			if (m_MapMenu.GetTempMarker())
			{
				m_MapMenu.RemoveTempMarker();
			}
		}
		
		ToggleMarkerWindow();
	}

	// ------------------------------------------------------------
	// Expansion EditMarker
	// Events when marker edit button is clicked
	// ------------------------------------------------------------	
	void EditMarker(string name, bool partyMarker, ExpansionMapMenuMarker mapMarker)
	{
		if (m_IsEditingMarker)
			return;	
		
		ExpansionMapMarker currentMarker;
		ExpansionMapMarker editingMarker;
		string currentMarkerName;
		string oldMarkerName = name;
		oldMarkerName.ToLower();
		
		m_CurrentEditingMapMarker = mapMarker;

		if (m_MapMenu.GetTempMarker())
		{
			m_MapMenu.RemoveTempMarker();
		}
		
		// Marker is personal marker
		if (!partyMarker)
		{
			for (int i = 0; i < m_MarkerModule.Count(); ++i)
			{
				currentMarker = m_MarkerModule.GetMarker(i);
				currentMarkerName = currentMarker.GetMarkerText();
				currentMarkerName.ToLower();

				if (oldMarkerName == currentMarkerName)
				{
					editingMarker = currentMarker;
				}
			}
		}
		else
		{
			if (m_PartyModule.HasParty() && m_PartyModule.GetParty().GetAllMarkers())
			{
				if (m_PartyModule.GetParty().GetAllMarkers().Count() >= 0)
				{	
					// Check if edited marker is party marker
					array<ref ExpansionMapMarker> markers = m_PartyModule.GetParty().GetAllMarkers();
					for (int j = 0; j < markers.Count(); ++j)
					{
						currentMarker = markers.Get(j);
						currentMarkerName = currentMarker.GetMarkerText();
						currentMarkerName.ToLower();
	
						if (oldMarkerName == currentMarkerName)
						{
							editingMarker = currentMarker;
						}
					}
				}
			}
		}

		if (editingMarker)
		{
			string markerNameOld = editingMarker.GetMarkerText();
			int markerIconIndexOld = editingMarker.GetIconIndex();
			string markerIconPathOld = ExpansionMarkerIcons.GetMarkerPath(markerIconIndexOld);
			int markerColorOld = editingMarker.GetMarkerColor();
			bool isPartyMarker = editingMarker.IsPartyMarker();
			bool is3DMarker = editingMarker.Is3DMarker();
			int alphaOld;
			int redOld;
			int greenOld;
			int blueOld;

			if (!GetMarkerWindowState())
				ToggleMarkerWindow();

			m_MarkerCreationWindowLable.SetText("#STR_EXPANSION_MAP_MARKER_EDIT_TITLE");
			m_CreateButtonLable.SetText("#STR_EXPANSION_MAP_MARKER_EDIT_BUTTON_LABLE");
			m_CancelButtonLable.SetText("#STR_EXPANSION_MAP_MARKER_CANCEL_EDIT_BUTTON_LABLE");
			m_DeleteButton.Show( true );
			
			GetARGB(markerColorOld, alphaOld, redOld, greenOld, blueOld);
			m_MarkerAlpha = alphaOld;
			m_MarkerColorRed = redOld;
			m_MarkerColorGreen = greenOld;
			m_MarkerColorBlue = blueOld;

			m_MarkerAlphaSlider.SetCurrent(m_MarkerAlpha);
			m_MarkerColorRedSlider.SetCurrent(m_MarkerColorRed);
			m_MarkerColorGreenSlider.SetCurrent(m_MarkerColorGreen);
			m_MarkerColorBlueSlider.SetCurrent(m_MarkerColorBlue);
			
			//! Set marker list entry selection dependig on old marker icon
			for (int k = 0; k < m_MarkerIconsEntrys.Count(); ++k)
			{
				ExpansionMapMenuMarkerEntry entry = m_MarkerIconsEntrys.Get(k);
				int markerEntryInconIndex = entry.GetMarkerIconIndex();

				if (markerIconIndexOld == markerEntryInconIndex)
				{
					SetMarkerSelection( entry );
				}
			}
			
			m_MarkerIconPreview.SetColor( ARGB(m_MarkerAlpha, m_MarkerColorRed, m_MarkerColorGreen, m_MarkerColorBlue) );
			m_MarkerNameEditBox.SetText(markerNameOld);

			if (isPartyMarker)
				m_PartyMarkerCheckBox.SetChecked(true);
			else
				m_PartyMarkerCheckBox.SetChecked(false);

			if (is3DMarker)
				m_3DMarkerCheckBox.SetChecked(true);
			else
				m_3DMarkerCheckBox.SetChecked(false);

			m_CurrentEditingMarker = editingMarker;
			m_MapMenu.SetEditingMarker(true);
			
			m_IsEditingMarker = true;
			GetNotificationSystem().CreateNotification( new StringLocaliser( "STR_EXPANSION_MARKER_SYSTEM_TITLE" ), new StringLocaliser( "STR_EXPANSION_MARKER_SYSTEM_EDITING_MARKER" ), EXPANSION_NOTIFICATION_ICON_MARKER, COLOR_EXPANSION_NOTIFICATION_INFO, 5 );
		}
	}

	// ------------------------------------------------------------
	// Expansion CreateEditedMarker
	// Events when creating edited marker
	// ------------------------------------------------------------	
	void CreateEditedMarker(ExpansionMapMarker entry)
	{
		int minNameLength = 3;
		int maxNameLength = 15;
		TStringArray allowedCharacters = {"A","B","C","D","E","F","G","H","I","J","K","L","M","N","O","P","Q","R","S","T","U","V","W","X","Y","Z","a","b","c","d","e","f","g","h","i","j","k","l","m","n","o","p","q","r","s","t","u","v","w","x","y","z","0","1","2","3","4","5","6","7","8","9","_"," "};

		string markerName = m_MarkerNameEditBox.GetText();
		m_MarkerIconIndex = GetSelecterMarker().GetMarkerIconIndex();

		if ( m_MarkerIconIndex >= ExpansionMarkerIcons.Count() )
		{
			m_MarkerIconIndex = 0;
		}
		else if (m_MarkerIconIndex < 0)
		{
			m_MarkerIconIndex = 0;
		}

		vector markerPos 	= m_MapMenu.GetCurrentMarkerPosition();
		if (!markerPos)
			markerPos = entry.GetMarkerPosition();
		
		int markerColor 	= ARGB(m_MarkerAlpha, m_MarkerColorRed, m_MarkerColorGreen, m_MarkerColorBlue);
		bool isPartyMarker  = m_PartyMarkerCheckBox.IsChecked();
		bool is3DMarker  	= m_3DMarkerCheckBox.IsChecked();

		string markerNameNew;
		string currentMarkerName;
		ExpansionMapMarker currentMarker;
		string markerNameEntry = entry.GetMarkerText();
		string markerNameOld = entry.GetMarkerText();
		markerNameOld.ToLower();

		//! Check marker name lenght (Min:3 / Max: 15 characters)
		if (markerName.Length() < minNameLength || markerName.Length() > maxNameLength) 
		{
			GetNotificationSystem().CreateNotification( new StringLocaliser( "STR_EXPANSION_MARKER_SYSTEM_TITLE" ), new StringLocaliser( "STR_EXPANSION_MARKER_SYSTEM_NAME_LENGHT" ), EXPANSION_NOTIFICATION_ICON_ERROR, COLOR_EXPANSION_NOTIFICATION_ERROR, 5 );
			
			return;
		}

		//! Check if they are arny illigal characters in the marker name
		for ( int i = 0; i < markerName.Length(); i++ )
		{
			if (allowedCharacters.Find(markerName.Get(i)) == -1)
			{
				GetNotificationSystem().CreateNotification( new StringLocaliser( "STR_EXPANSION_MARKER_SYSTEM_TITLE" ), new StringLocaliser( "STR_EXPANSION_MARKER_SYSTEM_ILLEGAL_CHARACTER" ), EXPANSION_NOTIFICATION_ICON_ERROR, COLOR_EXPANSION_NOTIFICATION_ERROR, 5 );
				
				return;
			}
		}

		//! Check if a marker icon is selected
		if (!m_MarkerIconIndex || m_MarkerIconIndex < 0)
		{
			GetNotificationSystem().CreateNotification( new StringLocaliser( "STR_EXPANSION_MARKER_SYSTEM_TITLE" ), new StringLocaliser( "STR_EXPANSION_MARKER_NO_MARKER_SELECTED" ), EXPANSION_NOTIFICATION_ICON_ERROR, COLOR_EXPANSION_NOTIFICATION_ERROR, 5 );
			
			return;
		}

		if (!markerPos)
		{
			GetNotificationSystem().CreateNotification( new StringLocaliser( "STR_EXPANSION_MARKER_SYSTEM_TITLE" ), new StringLocaliser( "STR_EXPANSION_MARKER_NO_POSITION_SELECTED" ), EXPANSION_NOTIFICATION_ICON_ERROR, COLOR_EXPANSION_NOTIFICATION_ERROR, 5 );
			
			return;
		}
		
		//Check if marker was a party marker before and is now a personal marker and same for personal markers
		if (!isPartyMarker && entry.IsPartyMarker())
		{
			if (m_MapMenu)
			{
				//Delete party marker entry because it will be a personal marker now
				m_MapMenu.RemovePartyMarker(markerNameEntry);
			}
		}
		else if (isPartyMarker && !entry.IsPartyMarker())
		{
			if (m_MapMenu)
			{
				//Delete personal marker entry because it will be a party marker now
				m_MapMenu.RemovePersonalMarker(markerNameEntry);
			}
		}

		//Check if created marker is a party marker
		if (isPartyMarker && m_PartyModule.HasParty())
		{
			//Check if marker name exist in party markers
			markerNameNew = markerName;
			markerNameNew.ToLower();

			array<ref ExpansionMapMarker> markers = m_PartyModule.GetParty().GetAllMarkers();
			for (int j = 0; j < markers.Count(); ++j)
			{
				currentMarker = markers.Get(j);
				currentMarkerName = currentMarker.GetMarkerText();
				currentMarkerName.ToLower();

				if (markerNameNew == currentMarkerName && currentMarkerName != markerNameOld)
				{
					GetNotificationSystem().CreateNotification( new StringLocaliser( "STR_EXPANSION_MARKER_SYSTEM_TITLE" ), new StringLocaliser( "STR_EXPANSION_MARKER_SYSTEM_MARKER_NAME_ALREADY_EXIST" ), EXPANSION_NOTIFICATION_ICON_ERROR, COLOR_EXPANSION_NOTIFICATION_ERROR, 5 );
					return;
				}
			}
			m_MapMenu.RemovePartyMarker(markerNameEntry);
			m_MapMenu.AddPartyMarker(markerName, m_MarkerIconIndex, markerPos, markerColor, is3DMarker);
			
			CancelMarkerEdit();
			
			GetNotificationSystem().CreateNotification( new StringLocaliser( "STR_EXPANSION_MARKER_SYSTEM_TITLE" ), new StringLocaliser( "STR_EXPANSION_MARKER_EDITED_MARKER" ), EXPANSION_NOTIFICATION_ICON_MARKER, COLOR_EXPANSION_NOTIFICATION_INFO, 5 );
		}
		else if (!isPartyMarker)
		{
			//Check if marker name exist in personnal markers
			markerNameNew = markerName;
			markerNameNew.ToLower();

			for (int k = 0; k < m_MarkerModule.Count(); ++k)
			{
				currentMarker = m_MarkerModule.GetMarker(k);
				currentMarkerName = currentMarker.GetMarkerText();
				currentMarkerName.ToLower();

				if (markerNameNew == currentMarkerName && currentMarkerName != markerNameOld)
				{
					GetNotificationSystem().CreateNotification( new StringLocaliser( "STR_EXPANSION_MARKER_SYSTEM_TITLE" ), new StringLocaliser( "STR_EXPANSION_MARKER_SYSTEM_MARKER_NAME_ALREADY_EXIST", markerName ), EXPANSION_NOTIFICATION_ICON_ERROR, COLOR_EXPANSION_NOTIFICATION_ERROR, 5 );
					return;
				}
			}

			// Edit personal marker
			m_MapMenu.RemovePersonalMarker(markerNameEntry);
			m_MapMenu.AddPersonalMarker(markerName, m_MarkerIconIndex, markerPos, markerColor, is3DMarker);
			
			CancelMarkerEdit();
			
			GetNotificationSystem().CreateNotification( new StringLocaliser( "STR_EXPANSION_MARKER_SYSTEM_TITLE" ), new StringLocaliser( "STR_EXPANSION_MARKER_EDITED_MARKER" ), EXPANSION_NOTIFICATION_ICON_MARKER, COLOR_EXPANSION_NOTIFICATION_INFO, 5 );
		}
	}
	
	// ------------------------------------------------------------
	// Expansion GetCurrentMapMarker
	// ------------------------------------------------------------	
	ExpansionMapMenuMarker GetCurrentMapMarker()
	{
		if (m_CurrentEditingMapMarker == NULL || !m_CurrentEditingMapMarker)
			return NULL;
		
		return m_CurrentEditingMapMarker;
	}

	// ------------------------------------------------------------
	// Expansion ToggleMarkerWindow
	// ------------------------------------------------------------	
	void ToggleMarkerWindow()
	{
		if (GetMarkerWindowState())
			ShowMarkerWindow(false);
		else
			ShowMarkerWindow(true);
		
		if (GetMapStatWindowState())
			ShowMapStatWindow(false);
		else
			ShowMapStatWindow(true);
	}

	// ------------------------------------------------------------
	// Expansion DeleteAllChildrens
	// ------------------------------------------------------------
	void DeleteAllChildrens( Widget parent )
	{
		Widget child = parent.GetChildren();
		
		while ( child )
		{
			child.Unlink();
			child = parent.GetSibling();
		}
	}
	
	// ------------------------------------------------------------
	// Expansion GetMapMenu
	// ------------------------------------------------------------
	ref ExpansionMapMenu GetMapMenu()
	{
		return m_MapMenu;
	}

	// ------------------------------------------------------------
	// Expansion SetEditingMarker
	// ------------------------------------------------------------	
	void SetEditingMarker(bool state)
	{
		m_IsEditingMarker = state;

		if (!state)
			CancelMarkerEdit();
	}

	// ------------------------------------------------------------
	// Expansion UpdateArrowColor
	// Update color of player position arrow
	// ------------------------------------------------------------	
	void UpdateArrowColor()
	{
		if (m_ArrowColorAlpha && m_ArrowColorRed && m_ArrowColorGreen && m_ArrowColorBlue)
		{
			if ( m_MapMenu && m_MapMenu.GetPlayerPositionArrow() )
			{
				ExpansionMapMenuPositionArrow arrow = ExpansionMapMenuPositionArrow.Cast(m_MapMenu.GetPlayerPositionArrow());
				arrow.SetArrowColor( m_ArrowColorAlpha, m_ArrowColorRed, m_ArrowColorGreen, m_ArrowColorBlue );
			
				if ( GetExpansionSettings() && GetExpansionSettings().GetGeneral().EnableHUDGPS )
				{
					MissionGameplay gameplay = MissionGameplay.Cast( GetGame().GetMission() );
					if ( gameplay )
					{
						ExpansionIngameHud hud = ExpansionIngameHud.Cast( gameplay.GetExpansionHud() );
						if ( hud )
							hud.GetGPSArrow().SetColor( ARGB( m_ArrowColorAlpha, m_ArrowColorRed, m_ArrowColorGreen, m_ArrowColorBlue ) );
					}
				}
			}
		}
	}
	
	// ------------------------------------------------------------
	// Expansion SaveArrowColor
	// Saves color of player position arrow in player profile
	// ------------------------------------------------------------
	void SaveArrowColor()
	{
		int color = ARGBtoInt(m_ArrowColorAlpha, m_ArrowColorRed, m_ArrowColorGreen, m_ArrowColorBlue);
		
		GetGame().SetProfileString( "ExpansionProfileArrowColor", color.ToString() );
		GetGame().SaveProfile();

		GetNotificationSystem().CreateNotification( new StringLocaliser( "STR_EXPANSION_MARKER_SYSTEM_TITLE" ), new StringLocaliser( "STR_EXPANSION_MARKER_SAVED_PLAYER_POSITION_COLOR" ), EXPANSION_NOTIFICATION_ICON_CHECK, COLOR_EXPANSION_NOTIFICATION_SUCCSESS, 5 );
	}
	
	// ------------------------------------------------------------
	// Expansion LoadArrowColor
	// Loads saved color of player position arrow from player profile
	// ------------------------------------------------------------
	void LoadArrowColor()
	{
		string color_string;
		if ( GetGame().GetProfileString( "ExpansionProfileArrowColor", color_string ) )
		{		
			int color = color_string.ToInt();
			int a;
			int r;
			int g;
			int b;
			
			IntToARGB(color, a, r, g, b);

			m_ArrowColorAlpha = a;
			m_ArrowColorRed = r;
			m_ArrowColorGreen = g;
			m_ArrowColorBlue = b;
		}
		else
		{
			m_ArrowColorAlpha = 255;
			m_ArrowColorRed = 233;
			m_ArrowColorGreen = 30;
			m_ArrowColorBlue = 99;
		}

		m_ArrowColorRedSlider.SetCurrent(m_ArrowColorRed);
		m_ArrowColorGreenSlider.SetCurrent(m_ArrowColorGreen);
		m_ArrowColorBlueSlider.SetCurrent(m_ArrowColorBlue);
		m_ArrowAlphaSlider.SetCurrent(m_ArrowColorAlpha);
		
		UpdateArrowColor();
	}
		
	//============================================
	// Expansion RefreshPositionValue
	//============================================
	void RefreshPositionValue()
	{
		int mouse_x;
		int mouse_y;
		vector positionCursor;
		int x;
		int z;
		
		GetGame().GetMousePos( mouse_x, mouse_y );
		positionCursor = m_MapWidget.ScreenToMap(Vector( mouse_x, mouse_y, 0 ));
		x = Math.Round(positionCursor[0]);
		z = Math.Round(positionCursor[2]);
		
		m_MapStatsCursorPositionValue.SetText( x.ToString() + " : " + z.ToString() );
	}

	// ------------------------------------------------------------
	// Override OnMouseEnter
	// ------------------------------------------------------------	
	override bool OnMouseEnter( Widget w, int x, int y )
	{
		if (w == m_TooltipButton)
		{
			#ifdef EXPANSIONEXLOGPRINT
			EXLogPrint("ExpansionMapMenu::OnMouseLeave - Mouse on m_TooltipButton");
			#endif

			m_Tooltip.ShowTooltip();
			m_TooltipButtonIcon.SetColor( ARGB(255, 255, 255, 255) );

			return true;
		}

		if (w == m_ArrowConfirmButton)
		{
			m_ArrowConfirmButtonIcon.SetColor( ARGB(255, 33, 46, 60) );

			return true;
		}

		if (w == m_ArrowCancelButton)
		{
			m_ArrowCancelButtonIcon.SetColor( ARGB(255, 33, 46, 60) );

			return true;
		}
		
		return false;
	}
	
	// ------------------------------------------------------------
	// Override OnMouseLeave
	// ------------------------------------------------------------	
	override bool OnMouseLeave( Widget w, Widget enterW, int x, int y )
	{
		if (w == m_TooltipButton)
		{
			#ifdef EXPANSIONEXLOGPRINT
			EXLogPrint("ExpansionMapMenu::OnMouseLeave - Mouse left m_TooltipButton");
			#endif
			
			m_Tooltip.HideTooltip();
			m_TooltipButtonIcon.SetColor( ARGB(255, 243, 156, 18) );

			return true;
		}

		if (w == m_ArrowConfirmButton)
		{
			m_ArrowConfirmButtonIcon.SetColor( ARGB(255, 255, 255, 255) );

			return true;
		}

		if (w == m_ArrowCancelButton)
		{
			m_ArrowCancelButtonIcon.SetColor( ARGB(255, 255, 255, 255) );

			return true;
		}
		
		return false;
	}
	
	// ------------------------------------------------------------
	// Expansion ClearChages
	// ------------------------------------------------------------	
	void ClearChages()
	{
		m_MarkerNameEditBox.SetText("");
		
		m_MarkerAlphaSlider.SetCurrent(255);
		m_MarkerColorRedSlider.SetCurrent(255);
		m_MarkerColorGreenSlider.SetCurrent(255);
		m_MarkerColorBlueSlider.SetCurrent(255);
		
		m_MarkerAlpha = m_MarkerAlphaSlider.GetCurrent();
		m_MarkerColorRed = m_MarkerColorRedSlider.GetCurrent();
		m_MarkerColorGreen = m_MarkerColorGreenSlider.GetCurrent();
		m_MarkerColorBlue = m_MarkerColorBlueSlider.GetCurrent();
		
		m_PartyMarkerCheckBox.SetChecked(false);
		m_3DMarkerCheckBox.SetChecked(false);
		
		m_MarkerIconPreview.LoadImageFile(0, "");
		m_MarkerIconPreview.SetColor(ARGB(255,255,255,255));
		m_MarkerIconPreview.Show(false);
	}
	
	// ------------------------------------------------------------
	// Expansion DeleteEditingMarker
	// ------------------------------------------------------------	
	void DeleteEditingMarker(ExpansionMapMarker marker)
	{
		string removeMarkerName;
		ExpansionMapMarker currentMarker;
		ExpansionMapMarker removeMarker;
		
		//! Check if marker is personal marker
		for (int i = 0; i < m_MarkerModule.Count(); ++i)
		{
			currentMarker = m_MarkerModule.GetMarker(i);

			if ( currentMarker.GetMarkerText() == marker.GetMarkerText() )
			{
				removeMarker = currentMarker;
			}
		}

		//! Delete personal marker
		if (removeMarker)
		{
			removeMarkerName = removeMarker.GetMarkerText();
			m_MapMenu.RemovePersonalMarker(removeMarkerName);
		}

		// Check if marker is party marker
		if (m_PartyModule.HasParty())
		{
			ref array<ref ExpansionMapMarker> markers = m_PartyModule.GetParty().GetAllMarkers();
			if (markers.Count() > 0)
			{
				for (int j = 0; j < markers.Count(); ++j)
				{
					currentMarker = markers.Get(j);
	
					if ( currentMarker.GetMarkerText() == marker.GetMarkerText() )
					{
						removeMarker = currentMarker;
					}
				}
			}
			
			//! Delete party marker
			if (removeMarker && m_PartyModule.HasParty())
			{
				if (removeMarker.IsPartyMarker())
				{
					ExpansionPartyModule module;
					if (Class.CastTo( module, GetModuleManager().GetModule(ExpansionPartyModule)))
					{
						removeMarkerName = removeMarker.GetMarkerText();
						m_MapMenu.RemovePartyMarker(removeMarkerName);
					}
				}
			}
		}
		
		CancelMarkerEdit();
	}
	
	// ------------------------------------------------------------
	// Expansion Override OnClick
	// ------------------------------------------------------------	
	override bool OnClick(Widget w, int x, int y, int button)
	{
		if (w == m_CancelButton)
		{
			if (!m_IsEditingMarker)
			{
				ClearChages();
				ToggleMarkerWindow();
				
				if (m_MapMenu.GetTempMarker())
					m_MapMenu.RemoveTempMarker();
			}
			else
			{
				CancelMarkerEdit();
			}

			return true;
		}

		if (w == m_CreateButton)
		{
			if (!m_IsEditingMarker)
			{
				CreateMarker();
			} 
			else if (m_IsEditingMarker && m_CurrentEditingMarker)
			{
				CreateEditedMarker(m_CurrentEditingMarker);
			}

			return true;
		}
		
		if (w == m_DeleteButton)
		{
			if (m_IsEditingMarker && m_CurrentEditingMarker)
			{
				DeleteEditingMarker(m_CurrentEditingMarker);
			}
		}

		if (w == m_ArrowConfirmButton)
		{
			SaveArrowColor();
			ShowArrowColorPanel(false);
			
			return true;
		}

		if (w == m_ArrowCancelButton)
		{
			ShowArrowColorPanel(false);
			
			return true;
		}

		return false;
	}
	
	// ------------------------------------------------------------
	// Expansion OnChange Update
	// ------------------------------------------------------------	
	override bool OnChange(Widget w, int x, int y, bool finished)
	{
		if ( w == m_MarkerColorRedSlider )
		{
			m_MarkerColorRed = m_MarkerColorRedSlider.GetCurrent();
			UpdatePreviewColor();

			return true;
		}
		
		if ( w == m_MarkerColorGreenSlider )
		{
			m_MarkerColorGreen = m_MarkerColorGreenSlider.GetCurrent();
			UpdatePreviewColor();

			return true;
		}
		
		if ( w == m_MarkerColorBlueSlider )
		{
			m_MarkerColorBlue = m_MarkerColorBlueSlider.GetCurrent();
			UpdatePreviewColor();

			return true;
		}
		
		if ( w == m_MarkerAlphaSlider )
		{
			m_MarkerAlpha = m_MarkerAlphaSlider.GetCurrent();
			UpdatePreviewColor();
			
			return true;
		}

		if ( w == m_ArrowColorRedSlider )
		{
			m_ArrowColorRed = m_ArrowColorRedSlider.GetCurrent();
			UpdateArrowColor();
			
			return true;
		}
		
		if ( w == m_ArrowColorGreenSlider )
		{
			m_ArrowColorGreen = m_ArrowColorGreenSlider.GetCurrent();
			UpdateArrowColor();
			
			return true;
		}
		
		if ( w == m_ArrowColorBlueSlider )
		{
			m_ArrowColorBlue = m_ArrowColorBlueSlider.GetCurrent();
			UpdateArrowColor();
			
			return true;
		}
		
		if ( w == m_ArrowAlphaSlider )
		{
			m_ArrowColorAlpha = m_ArrowAlphaSlider.GetCurrent();
			UpdateArrowColor();
			
			return true;
		}
		
		if ( w == m_MarkerNameEditBox)
		{
			UpdateMarkerNames();
			return true;
		}
		
		return super.OnChange( w, x, y, finished );
	}
}