/**
 * ExpansionFlagMenu.c
 *
 * DayZ Expansion Mod
 * www.dayzexpansion.com
 * © 2024 DayZ Expansion Mod Team
 *
 * This work is licensed under the Creative Commons Attribution-NonCommercial-NoDerivatives 4.0 International License.
 * To view a copy of this license, visit http://creativecommons.org/licenses/by-nc-nd/4.0/.
 *
*/

/**@class		ExpansionFlagMenu
 * @brief		
 **/

class ExpansionFlagMenu: ExpansionUIScriptedMenu
{
	protected Widget m_FlagWindow;
	protected TextWidget m_FlagWindowLable;
	protected WrapSpacerWidget m_FlagEntrysGrid;
	protected ImageWidget m_FlagPreview;
	protected ButtonWidget m_FlagConfirmButton;
	protected ButtonWidget m_FlagCreateButton;
	protected ButtonWidget m_FlagCancelButton;
	
	protected Widget m_TerritoryWindow;
	protected TextWidget m_TerritoryOwnerName;
	protected TextWidget m_TerritoryPosition;
	protected EditBoxWidget m_TerritoryNameEditbox;
	protected ButtonWidget m_TerritoryConfirmButton;
	protected ButtonWidget m_TerritoryCancelButton;
	
	protected Widget m_TerritoryDialogWindow;
	protected ButtonWidget m_TerritoryDialogConfirmButton;
	protected ButtonWidget m_TerritoryDialogCancelButton;
	
	protected Widget m_FlagFilterPanel;
	protected EditBoxWidget m_FlagFilterBox;
	protected ButtonWidget m_FlagFilterButton;
	protected ImageWidget m_FlagFilterButtonIcon;
	
	protected TerritoryFlag m_CurrentFlag;
	
	protected ref ExpansionTerritoryModule m_TerritoryModule;
	protected ref array<ref ExpansionFlagMenuTextureEntry> m_TextureEntrys;
	protected string m_CurrentSelectedTexture;

	void ExpansionFlagMenu()
	{
		m_TerritoryModule = ExpansionTerritoryModule.Cast( CF_ModuleCoreManager.Get( ExpansionTerritoryModule ) );
		m_TextureEntrys = new array<ref ExpansionFlagMenuTextureEntry>;
		
		m_CurrentSelectedTexture = "";
	}

	override Widget Init()
	{
		layoutRoot = GetGame().GetWorkspace().CreateWidgets("DayZExpansion/BaseBuilding/GUI/layouts/flag/expansion_flag_menu.layout");
		
		//! Flag Window
		m_FlagWindow = Widget.Cast( layoutRoot.FindAnyWidget( "flag_window_content" ) );
		m_FlagWindowLable = TextWidget.Cast( layoutRoot.FindAnyWidget("flag_window_header_text") );
		m_FlagEntrysGrid = WrapSpacerWidget.Cast( layoutRoot.FindAnyWidget("flag_textures_content") );
		m_FlagPreview = ImageWidget.Cast( layoutRoot.FindAnyWidget( "flag_icon_image" ) );
		m_FlagPreview.Show( false );
		m_FlagConfirmButton = ButtonWidget.Cast( layoutRoot.FindAnyWidget( "confirm" ) );
		m_FlagCreateButton = ButtonWidget.Cast( layoutRoot.FindAnyWidget( "create" ) );
		m_FlagCancelButton = ButtonWidget.Cast( layoutRoot.FindAnyWidget( "cancel" ) );
		
		//! Territory Window
		m_TerritoryWindow = Widget.Cast( layoutRoot.FindAnyWidget( "territory_window_content" ) );
		m_TerritoryOwnerName = TextWidget.Cast( layoutRoot.FindAnyWidget("territory_owner_name") );
		m_TerritoryPosition = TextWidget.Cast( layoutRoot.FindAnyWidget("territory_pos") );
		m_TerritoryNameEditbox = EditBoxWidget.Cast( layoutRoot.FindAnyWidget("territory_name_editbox") );
		m_TerritoryConfirmButton = ButtonWidget.Cast( layoutRoot.FindAnyWidget( "create_territory" ) );
		m_TerritoryCancelButton = ButtonWidget.Cast( layoutRoot.FindAnyWidget( "cancel_territory" ) );
		
		//! Territory Dialog Window
		m_TerritoryDialogWindow = Widget.Cast( layoutRoot.FindAnyWidget( "territory_dialog" ) );
		m_TerritoryDialogConfirmButton = ButtonWidget.Cast( layoutRoot.FindAnyWidget( "territory_dialog_ok_button" ) );
		m_TerritoryDialogCancelButton = ButtonWidget.Cast( layoutRoot.FindAnyWidget( "territory_dialog_cancel_button" ) );
		
		//! Flag filter
		m_FlagFilterPanel = layoutRoot.FindAnyWidget("flag_filter_panel");
		m_FlagFilterBox = EditBoxWidget.Cast( layoutRoot.FindAnyWidget("flag_filter_box") );
		m_FlagFilterButton = ButtonWidget.Cast( layoutRoot.FindAnyWidget("flag_filter_clear") );
		m_FlagFilterButtonIcon = ImageWidget.Cast( layoutRoot.FindAnyWidget("flag_filter_clear_icon") ); 
		
		if ( GetExpansionSettings().GetBaseBuilding().FlagMenuMode == ExpansionFlagMenuMode.NoFlagChoice )
		{
			m_FlagWindow.Show( false );
			m_FlagFilterPanel.Show( false );
			m_TerritoryWindow.Show( true );
		}

		SetTerritoryInfo();
		
		return layoutRoot;
	}

	void LoadTextureList()
	{
		array<ref ExpansionFlagTexture> flagTextures = GetExpansionFlagTextures().GetAll();
		if (flagTextures.Count() == 0)
			return;
		
		if (m_TextureEntrys.Count() > 0)
			m_TextureEntrys.Clear();
		
		for ( int i = 0; i < flagTextures.Count(); i++ )
   		{
			ExpansionFlagTexture texture = ExpansionFlagTexture.Cast( flagTextures.Get(i) );
			if (!texture)
				continue;
			
			ExpansionFlagMenuTextureEntry texture_entry = new ExpansionFlagMenuTextureEntry(m_FlagEntrysGrid, texture);
			
			m_TextureEntrys.Insert( texture_entry );
		}
	}

	void SetCurrentTexture( ExpansionFlagTexture texture)
	{
		if (!texture)
			return;
		
		m_FlagPreview.LoadImageFile( 0, texture.Path );
		m_FlagPreview.Show( true );
		
		m_CurrentSelectedTexture = texture.Path;
	}

	void ChangeFlag()
	{
		if ( m_CurrentSelectedTexture != "" )
		{
			if ( m_TerritoryModule && m_CurrentFlag )
			{
				m_TerritoryModule.ChangeFlagTexture( m_CurrentSelectedTexture, m_CurrentFlag );
			}

			Close();
		}
	}

	bool FindChar( TStringArray key, string word )
	{
		// Each letters of the word
		for ( int i = 0; i < word.Length(); ++i )
		{
			// Each forbbidden letters
			for ( int u = 0; u < key.Count(); ++u )
			{
				if ( word[i] == key[u] )
					return true;
			}
		}

		return false;
	}

	void ConfirmTerritoryCreation()
	{
#ifdef EXPANSIONTRACE
		auto trace = CF_Trace_0(ExpansionTracing.TERRITORY, this, "ConfirmTerritoryCreation");
#endif

		string territoryName = m_TerritoryNameEditbox.GetText();
		TStringArray forrbidenChars = new TStringArray;
		forrbidenChars.Insert( "!" );
		forrbidenChars.Insert( "<" );
		forrbidenChars.Insert( ">" );
		forrbidenChars.Insert( "/" );
		
		//! Territory name can't be empty or have a illegal characters
		if ( territoryName == "" || FindChar(forrbidenChars,territoryName) )
		{
			PlayerBase player = PlayerBase.Cast( GetGame().GetPlayer() );
			ExpansionNotification("STR_EXPANSION_TERRITORY_TITLE", new StringLocaliser("STR_EXPANSION_TERRITORY_NAMEINVALID", territoryName)).Error(player.GetIdentity());
			return;
		}
		
		CF_Log.Debug("ExpansionFlagMenu::ConfirmTerritoryCreation territoryName : " + territoryName);
		
		//! Call request of territory creation
		if ( m_CurrentFlag && !m_CurrentFlag.HasExpansionTerritoryInformation() )
		{
			ChangeFlag();
			m_TerritoryModule.CreateTerritory( territoryName, m_CurrentFlag );
		}
	}

	override void OnShow()
	{
		super.OnShow();

		SetFocus(layoutRoot);
		GetGame().GetMission().PlayerControlDisable(INPUT_EXCLUDE_ALL);
		
		PPEffects.SetBlurMenu(0.5);
		
		//! Load and create flag texture list entrys
		if ( GetExpansionSettings().GetBaseBuilding().FlagMenuMode == ExpansionFlagMenuMode.Enabled )
		{
			LoadTextureList();
		}
		
		PlayerBase player = PlayerBase.Cast( GetGame().GetPlayer() );
		
		m_CurrentFlag = player.FindNearestFlag();
		if (!m_CurrentFlag)
			return;
		
		if ( !m_CurrentFlag.HasExpansionTerritoryInformation() )
		{
			if ( player.IsInTerritory() || !GetExpansionSettings().GetTerritory().EnableTerritories )
			{
				m_FlagCreateButton.Show( false );
			}
			else
			{
				m_FlagCreateButton.Show( true );
			}
			
			m_FlagWindowLable.SetText( "#STR_EXPANSION_FLAG_TITLE" );
		} 
		else
		{
			m_FlagCreateButton.Show( false );
			m_FlagWindowLable.SetText( "#STR_EXPANSION_FLAG_TERRITORY_FLAG" );
		}
	}

	void SetTerritoryInfo()
	{
		PlayerBase clientPlayer = PlayerBase.Cast( GetGame().GetPlayer() );
		
		if ( clientPlayer )
		{
			m_TerritoryOwnerName.SetText( clientPlayer.GetIdentityName() );
			m_CurrentFlag = clientPlayer.FindNearestFlag();
		}
		
		if ( m_CurrentFlag )
			m_TerritoryPosition.SetText( "X: " + Math.Round( m_CurrentFlag.GetPosition()[0] ) + " Y: " + Math.Round( m_CurrentFlag.GetPosition()[2] ) );
	}

	override void OnHide()
	{
		super.OnHide();

		GetGame().GetMission().PlayerControlEnable(true);
		
		PPEffects.SetBlurMenu( 0.0 );
	}

	override void Update( float timeslice )
	{
		Input input = GetGame().GetInput();
		if (input.LocalPress("UAUIBack", false))
		{			
			Close();
			return;
		}

		super.Update(timeslice);
	}

	override bool OnClick( Widget w, int x, int y, int button )
	{
		//! Flag Window Buttons
		if ( w == m_FlagConfirmButton )
		{
			ChangeFlag();		
			return true;
		}
		
		if ( w == m_FlagCancelButton )
		{
			Close();			
			return true;
		}
		
		if ( w == m_FlagCreateButton )
		{
			if (GetExpansionSettings().GetBaseBuilding().FlagMenuMode == ExpansionFlagMenuMode.Disabled || !GetExpansionSettings().GetTerritory().EnableTerritories)
				return false;
			
			m_FlagWindow.Show( false );
			m_FlagFilterPanel.Show( false );
			m_TerritoryWindow.Show( true );
						
			return true;
		}
		
		//! Territory Dialog Buttons
		if ( w == m_TerritoryDialogConfirmButton )
		{
			if (GetExpansionSettings().GetBaseBuilding().FlagMenuMode == ExpansionFlagMenuMode.Disabled || !GetExpansionSettings().GetTerritory().EnableTerritories)
				return false;
			
			ConfirmTerritoryCreation();
			Close();
						
			return true;
		}
		
		if ( w == m_TerritoryDialogCancelButton )
		{
			m_TerritoryDialogWindow.Show( false );
			
			m_TerritoryWindow.Show( true );
						
			return true;
		}
		
		//! Territory Window Buttons
		if ( w == m_TerritoryConfirmButton )
		{
			m_TerritoryWindow.Show( false );
			m_TerritoryDialogWindow.Show( true );
			
			return true;
		}
		
		if ( w == m_TerritoryCancelButton )
		{
			if ( GetExpansionSettings().GetBaseBuilding().FlagMenuMode == ExpansionFlagMenuMode.NoFlagChoice )
			{
				Close();
			} else {
				m_TerritoryWindow.Show( false );
				m_FlagWindow.Show( true );
				m_FlagFilterPanel.Show( true );
			}
			
			return true;
		}
		
		if ( w == m_FlagFilterButton )
		{
			m_FlagFilterBox.SetText("");
			UpdateFlags();
			return true;
		}
		
		return false;
	}
	
	override bool OnChange(Widget w, int x, int y, bool finished)
	{		
		if (w == m_FlagFilterBox)
		{
			UpdateFlags();
		}
		
		return false;
	}
	
	override bool OnMouseEnter(Widget w, int x, int y)
	{
		if (w != NULL)
		{
			if (w == m_FlagFilterButton)
			{
				m_FlagFilterButtonIcon.SetColor(ARGB(255, 68, 68, 68));
				return true;
			}
		}

		return false;
	}
	
	override bool OnMouseLeave(Widget w, Widget enterW, int x, int y)
	{
		if (w != NULL)
		{
			if (w == m_FlagFilterButton)
			{
				m_FlagFilterButtonIcon.SetColor(ARGB(255, 251, 252, 254));
				return true;
			}
		}

		return false;
	}
	
	protected void UpdateFlags()
	{
		string search = GetSearchText();
		search.ToLower();
		foreach (ExpansionFlagMenuTextureEntry flagEntry: m_TextureEntrys)
		{
			if (search != "")
			{
				string flagName = flagEntry.GetName();
				flagName.ToLower();
				if (!flagName.Contains(search))
				{
					flagEntry.Hide();
				}
				else
				{
					if (!flagEntry.IsVisible())
						flagEntry.Show();
				}	
			}
			else
			{
				if (!flagEntry.IsVisible())
					flagEntry.Show();
			}
		}
	}
	
	string GetSearchText()
	{
		return m_FlagFilterBox.GetText();
	}
}