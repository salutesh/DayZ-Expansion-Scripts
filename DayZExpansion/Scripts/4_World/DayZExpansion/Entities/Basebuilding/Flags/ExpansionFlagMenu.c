/**
 * ExpansionFlagBase.c
 *
 * DayZ Expansion Mod
 * www.dayzexpansion.com
 * © 2020 DayZ Expansion Mod Team
 *
 * This work is licensed under the Creative Commons Attribution-NonCommercial-NoDerivatives 4.0 International License.
 * To view a copy of this license, visit http://creativecommons.org/licenses/by-nc-nd/4.0/.
 *
*/

/**@class		ExpansionFlagMenu
 * @brief		
 **/

class ExpansionFlagMenu extends ExpansionUIScriptedMenu
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
	
	protected TerritoryFlag m_CurrentFlag;
	
	protected ref ExpansionTerritoryModule m_TerritoryModule;
	protected ref array<ref ExpansionFlagMenuTextureEntry> m_TextureEntrys;
	protected string m_CurrentSelectedTexture;
	
	// ------------------------------------------------------------
	// Expansion ExpansionFlagMenu Constructor
	// ------------------------------------------------------------	
	void ExpansionFlagMenu()
	{
		m_TerritoryModule = ExpansionTerritoryModule.Cast( GetModuleManager().GetModule( ExpansionTerritoryModule ) );
		m_TextureEntrys = new array<ref ExpansionFlagMenuTextureEntry>;
		
		m_CurrentSelectedTexture = "";
	}

	// ------------------------------------------------------------
	// Expansion Override Init
	// ------------------------------------------------------------
	override Widget Init()
	{
		layoutRoot = GetGame().GetWorkspace().CreateWidgets("DayZExpansion/GUI/layouts/flag/expansion_flag_menu.layout");
		
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
		
		if ( GetExpansionSettings().GetBaseBuilding().EnableFlagMenu == FlagMenuMode.NoFlagChoice )
		{
			m_FlagWindow.Show( false );
			m_TerritoryWindow.Show( true );
		}

		SetTerritoryInfo();
		
		return layoutRoot;
	}
	
	// ------------------------------------------------------------
	// Expansion LoadTextureList
	// ------------------------------------------------------------
	void LoadTextureList()
	{
		array<ref ExpansionFlagTexture> flagTextures = GetExpansionStatic().GetFlagTextures();
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
	
	// ------------------------------------------------------------
	// Expansion SetCurrentTexture
	// ------------------------------------------------------------
	void SetCurrentTexture( ExpansionFlagTexture texture)
	{
		if (!texture)
			return;
		
		m_FlagPreview.LoadImageFile( 0, texture.Path );
		m_FlagPreview.Show( true );
		
		m_CurrentSelectedTexture = texture.Path;
	}
	
	// ------------------------------------------------------------
	// Expansion ChangeFlag
	// ------------------------------------------------------------
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
	
	// ------------------------------------------------------------
	// Expansion FindChar
	// ------------------------------------------------------------	
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
	
	// ------------------------------------------------------------
	// Expansion ConfirmTerritoryCreation
	// ------------------------------------------------------------	
	void ConfirmTerritoryCreation()
	{
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
		
		#ifdef EXPANSIONEXLOGPRINT
		EXLogPrint("ExpansionFlagMenu::ConfirmTerritoryCreation territoryName : " + territoryName);
		#endif
		
		//! Call request of territory creation
		if ( m_CurrentFlag && !m_CurrentFlag.HasExpansionTerritoryInformation() )
		{
			ChangeFlag();
			m_TerritoryModule.CreateTerritory( territoryName, m_CurrentFlag );
		}
	}
	
	// ------------------------------------------------------------
	// Expansion Override OnShow
	// ------------------------------------------------------------	
	override void OnShow()
	{
		super.OnShow();

		SetFocus(layoutRoot);
		GetGame().GetMission().PlayerControlDisable(INPUT_EXCLUDE_ALL);
		
		PPEffects.SetBlurMenu(0.5);
		
		//! Load and create flag texture list entrys
		if ( GetExpansionSettings().GetBaseBuilding().EnableFlagMenu == FlagMenuMode.Enabled )
		{
			LoadTextureList();
		}
		
		PlayerBase player = PlayerBase.Cast( GetGame().GetPlayer() );
		
		m_CurrentFlag = player.FindNearestFlag();
		if (!m_CurrentFlag)
			return;
		
		if ( !m_CurrentFlag.HasExpansionTerritoryInformation() )
		{
			if ( player.IsInTerritory() )
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
	
	// ------------------------------------------------------------
	// Expansion SetTerritoryInfo
	// ------------------------------------------------------------	
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
	
	// ------------------------------------------------------------
	// Override OnHide
	// ------------------------------------------------------------
	override void OnHide()
	{
		super.OnHide();

		GetGame().GetMission().PlayerControlEnable(true);
		
		PPEffects.SetBlurMenu( 0.0 );
	}
	
	// ------------------------------------------------------------
	// Expansion Override Update
	// ------------------------------------------------------------	
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
	
	// ------------------------------------------------------------
	// Expansion Override OnClick
	// ------------------------------------------------------------	
	override bool OnClick( Widget w, int x, int y, int button )
	{
		//! Flag Window Buttons
		if( w == m_FlagConfirmButton )
		{
			ChangeFlag();		
			return true;
		}
		
		if( w == m_FlagCancelButton )
		{
			Close();			
			return true;
		}
		
		if( w == m_FlagCreateButton )
		{
			m_FlagWindow.Show( false );
			m_TerritoryWindow.Show( true );
						
			return true;
		}
		
		//! Territory Dialog Buttons
		if( w == m_TerritoryDialogConfirmButton )
		{
			ConfirmTerritoryCreation();
			Close();
						
			return true;
		}
		
		if( w == m_TerritoryDialogCancelButton )
		{
			m_TerritoryDialogWindow.Show( false );
			
			if ( GetExpansionSettings().GetBaseBuilding().EnableFlagMenu == FlagMenuMode.NoFlagChoice )
			{
				m_TerritoryWindow.Show( true );
			} else {
				m_TerritoryWindow.Show( true );
			}
						
			return true;
		}
		
		//! Territory Window Buttons
		if( w == m_TerritoryConfirmButton )
		{
			m_TerritoryWindow.Show( false );
			m_TerritoryDialogWindow.Show( true );
			
			return true;
		}
		
		if( w == m_TerritoryCancelButton )
		{
			if ( GetExpansionSettings().GetBaseBuilding().EnableFlagMenu == FlagMenuMode.NoFlagChoice )
			{
				Close();
			} else {
				m_TerritoryWindow.Show( false );
				m_FlagWindow.Show( true );
			}
			
			return true;
		}
		
		return false;
	}
}