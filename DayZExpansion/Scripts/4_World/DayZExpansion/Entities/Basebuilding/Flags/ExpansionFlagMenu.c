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

class ExpansionFlagMenu extends UIScriptedMenu
{
	protected Widget m_FlagWindow;
	protected TextWidget m_FlagWindowLable;
	protected GridSpacerWidget m_FlagEntrysGrid;
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
	
	protected ExpansionFlagBase m_CurrentFlag;
	protected ExpansionTerritoryFlag m_CurrentTerritoryFlag;
	
	protected ref ExpansionTerritoryModule m_TerritoryModule;
	protected autoptr ref array<ref ExpansionFlagTexture> m_FlagTextures;
	protected ref array<ref ExpansionFlagMenuTextureEntry> m_TextureEntrys;
	protected int m_CurrentSelectedTexture;
	
	// ------------------------------------------------------------
	// Expansion ExpansionFlagMenu Constructor
	// ------------------------------------------------------------	
	void ExpansionFlagMenu()
	{
		m_TerritoryModule = ExpansionTerritoryModule.Cast( GetModuleManager().GetModule( ExpansionTerritoryModule ) );
		m_FlagTextures = new array<ref ExpansionFlagTexture>;
		m_TextureEntrys = new array<ref ExpansionFlagMenuTextureEntry>;
		
		LoadFlagTextures();
		
		m_CurrentSelectedTexture = -1;
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
		m_FlagEntrysGrid = GridSpacerWidget.Cast( layoutRoot.FindAnyWidget("flag_textures_content") );
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
		
		SetTerritoryInfo();
		
		return layoutRoot;
	}
	
	// ------------------------------------------------------------
	// Expansion LoadFlagTextures
	// Contains id and name information for each flag texture
	// ------------------------------------------------------------
	void LoadFlagTextures()
	{
		AddFlagTexture(1, "Expansion");
		AddFlagTexture(2, "White");
		AddFlagTexture(3, "Australia");
		AddFlagTexture(4, "Canada");
		AddFlagTexture(5, "Chernarus");
		AddFlagTexture(6, "France");
		AddFlagTexture(7, "Germany");
		AddFlagTexture(8, "Latvia");
		AddFlagTexture(9, "Luxembourg");
		AddFlagTexture(10, "Mexico");
		AddFlagTexture(11, "Netherlands");
		AddFlagTexture(12, "New Zealand");
		AddFlagTexture(13, "Norway");
		AddFlagTexture(14, "Poland");
		AddFlagTexture(15, "Russia");
		AddFlagTexture(16, "UK");
		AddFlagTexture(17, "USA");
	}
	
	// ------------------------------------------------------------
	// Expansion AddFlagTexture
	// ------------------------------------------------------------
	void AddFlagTexture( int id, string name )
	{
		ExpansionFlagTexture flagTexture = new ExpansionFlagTexture( id, name );
		
		m_FlagTextures.Insert( flagTexture );
	}
	
	// ------------------------------------------------------------
	// Expansion LoadTextureList
	// ------------------------------------------------------------
	void LoadTextureList()
	{
		if (m_FlagTextures.Count() == 0)
			return;
		
		if (m_TextureEntrys.Count() > 0)
			m_TextureEntrys.Clear();
		
		for ( int i = 0; i < m_FlagTextures.Count(); i++ )
   		{
			ExpansionFlagTexture texture = ExpansionFlagTexture.Cast( m_FlagTextures.Get(i) );
			ExpansionFlagMenuTextureEntry texture_entry = new ExpansionFlagMenuTextureEntry(m_FlagEntrysGrid, texture);
			
			m_TextureEntrys.Insert( texture_entry );
		}
	}
	
	// ------------------------------------------------------------
	// Expansion SetCurrentTexture
	// ------------------------------------------------------------
	void SetCurrentTexture(ExpansionFlagTexture texture)
	{
		string path = GetFlagTexturePath( texture.ID );
		
		m_FlagPreview.LoadImageFile( 0, path );
		m_FlagPreview.Show( true );
		
		m_CurrentSelectedTexture = texture.ID;
	}
	
	// ------------------------------------------------------------
	// Expansion ChangeFlag
	// ------------------------------------------------------------
	void ChangeFlag()
	{
		if ( m_CurrentSelectedTexture != -1 )
		{
			if ( m_TerritoryModule && m_CurrentFlag && !m_CurrentFlag.IsTerritoryFlag() )
			{
				m_TerritoryModule.ChangeFlagTexture( m_CurrentSelectedTexture, m_CurrentFlag );
			} 
			else if ( m_TerritoryModule && !m_CurrentFlag && m_CurrentTerritoryFlag && m_CurrentTerritoryFlag.IsTerritoryFlag() )
			{
				m_TerritoryModule.ChangeFlagTexture( m_CurrentSelectedTexture, m_CurrentTerritoryFlag );
			}

			Close();
		}
	}
	
	// ------------------------------------------------------------
	// Expansion ConfirmTerritoryCreation
	// ------------------------------------------------------------	
	void ConfirmTerritoryCreation()
	{
		string territoryName = m_TerritoryNameEditbox.GetText();
		
		//! Territory name cant be empty
		if ( territoryName == "" )
		{
			return;
		}
		
		#ifdef EXPANSIONEXLOGPRINT
		EXLogPrint("ExpansionFlagMenu::ConfirmTerritoryCreation territoryName : " + territoryName);
		#endif
		
		//! Call request of territory creation
		if ( m_CurrentFlag && !m_CurrentFlag.IsTerritoryFlag() )
		{
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
		LoadTextureList();
		
		PlayerBase player = PlayerBase.Cast( GetGame().GetPlayer() );
		
		m_CurrentFlag = player.FindNearestFlag();
		m_CurrentTerritoryFlag = m_TerritoryModule.FindNearestTerritoryFlag(player);
		
		if ( m_CurrentFlag && !m_CurrentFlag.IsTerritoryFlag() )
		{
			if ( player.IsInTerritory() )
			{
				m_FlagCreateButton.Show( false );
			}
			else
			{
				m_FlagCreateButton.Show( true );
			}
			
			m_FlagWindowLable.SetText( "Flag" );
		} 
		else if ( !m_CurrentFlag && m_CurrentTerritoryFlag && m_CurrentTerritoryFlag.IsTerritoryFlag() )
		{
			m_FlagCreateButton.Show( false );
			m_FlagWindowLable.SetText( "Territory Flag" );
		}
	}
	
	// ------------------------------------------------------------
	// Expansion SetTerritoryInfo
	// ------------------------------------------------------------	
	void SetTerritoryInfo()
	{
		PlayerBase clientPlayer = PlayerBase.Cast( GetGame().GetPlayer() );
		
		m_TerritoryOwnerName.SetText( clientPlayer.GetIdentityName() );
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
		if (input.LocalPress("UAUIBack", false) || input.LocalHold("UAExpansionMapToggle", false))
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
			m_TerritoryWindow.Show( true );
						
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
			m_TerritoryWindow.Show( false );
			m_FlagWindow.Show( true );
			
			return true;
		}
		
		return false;
	}
}