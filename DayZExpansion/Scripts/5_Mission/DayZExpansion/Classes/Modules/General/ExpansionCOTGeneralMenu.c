class ExpansionCOTGeneralMenu: JMFormBase
{
	private ExpansionGeneralSettings m_Settings;
	private ExpansionCOTGeneralModule m_Module;

	private UIActionButton m_Apply;
	private UIActionButton m_Refresh;

	private UIActionScroller m_ActionListScroller;
	private Widget m_ActionListContainer;

	private UIActionCheckbox m_PlayerLocationNotifier;
	private UIActionCheckbox m_ShowPlayerJoinServer;
	private UIActionSelectBox m_JoinMessageType;
	private UIActionCheckbox m_ShowPlayerLeftServer;
	private UIActionSelectBox m_LeftMessageType;
	//! private UIActionCheckbox m_EnableKillFeed;
	//! private UIActionSelectBox m_KillFeedMessageType;
	//! private UIActionCheckbox m_EnableHumanity;
	private UIActionCheckbox m_EnableGlobalChat;
	private UIActionCheckbox m_UseCustomMappingModule;
	private UIActionCheckbox m_InteriorBuilding;
	private UIActionSelectBox m_LightingConfig;
	private UIActionSelectBox m_EnableLamps;
	private UIActionCheckbox m_EnableGenerators;
	private UIActionCheckbox m_EnableLighthouses;
	private UIActionCheckbox m_EnableAutoRun;
	private UIActionCheckbox m_EnableHUDGPS;
	private UIActionCheckbox m_NeedGPSItemForKeyBinding;
	private UIActionCheckbox m_NeedMapItemForKeyBinding;
	private UIActionCheckbox m_EnableHUDNightvisionOverlay;
	private UIActionCheckbox m_EnablePlayerTags;
	private UIActionSlider m_PlayerTagViewRange;
	//! private UIActionCheckbox m_EnableHumanityOnPlayerTags;
	private UIActionCheckbox m_EnablePlayerList;
	private UIActionCheckbox m_UnlimitedStamina;
	private UIActionSelectBox m_VehicleSync;

	void ExpansionCOTGeneralMenu()
	{
	}

	void ~ExpansionCOTGeneralMenu()
	{
	}

	protected override bool SetModule( ref JMRenderableModuleBase mdl )
	{
		m_Settings = GetExpansionSettings().GetGeneral();
		return m_Settings && Class.CastTo( m_Module, mdl );
	}

	override void OnInit()
	{
		Widget admin_actions			= UIActionManager.CreateGridSpacer( layoutRoot.FindAnyWidget( "panel_bottom" ), 1, 2 );
		m_Refresh						= UIActionManager.CreateButton( admin_actions, "Refresh", this, "Click_Refresh" );
		m_Apply							= UIActionManager.CreateButton( admin_actions, "Apply", this, "Click_Apply" );

		m_ActionListScroller			= UIActionManager.CreateScroller( layoutRoot.FindAnyWidget( "panel_top" ) );
		m_ActionListScroller.SetFixedHeight( 300 - 30 );

		Widget actions					= m_ActionListScroller.GetContentWidget();

		m_PlayerLocationNotifier		= UIActionManager.CreateCheckbox( actions, "Player Location Notifier:" );

		UIActionManager.CreatePanel( actions, 0xAA000000, 1 );

		m_ShowPlayerJoinServer			= UIActionManager.CreateCheckbox( actions, "Player Join:" );
		m_JoinMessageType				= UIActionManager.CreateSelectionBox( actions, "  Type:", { "Chat", "Notification" } );

		UIActionManager.CreatePanel( actions, 0xAA000000, 1 );

		m_ShowPlayerLeftServer			= UIActionManager.CreateCheckbox( actions, "Player Left:" );
		m_LeftMessageType				= UIActionManager.CreateSelectionBox( actions, "  Type:", { "Chat", "Notification" } );

		UIActionManager.CreatePanel( actions, 0xAA000000, 1 );

		// m_EnableKillFeed				= UIActionManager.CreateCheckbox( actions, "Kill Feed:" );
		// m_KillFeedMessageType		= UIActionManager.CreateSelectionBox( actions, "  Type:", { "Chat", "Notification" } );

		// UIActionManager.CreatePanel( actions, 0xAA000000, 1 );

		// m_EnableHumanity				= UIActionManager.CreateCheckbox( actions, "Humanity:" );

		// UIActionManager.CreatePanel( actions, 0xAA000000, 1 );

		m_EnableGlobalChat				= UIActionManager.CreateCheckbox( actions, "Global Chat:" );

		UIActionManager.CreatePanel( actions, 0xAA000000, 1 );

		m_UseCustomMappingModule		= UIActionManager.CreateCheckbox( actions, "Custom Mapping:" );
		m_InteriorBuilding				= UIActionManager.CreateCheckbox( actions, "Interior Building:" );

		UIActionManager.CreatePanel( actions, 0xAA000000, 1 );

		m_LightingConfig					= UIActionManager.CreateSelectionBox( actions, "Lighting Config:", { "Default", "Dark", "Gloom" } );

		m_EnableLamps					= UIActionManager.CreateSelectionBox( actions, "Lamps:", { "Disable", "Generators", "Always On", "Always On Everywhere" } );

		UIActionManager.CreatePanel( actions, 0xAA000000, 1 );

		m_EnableGenerators				= UIActionManager.CreateCheckbox( actions, "Generators:" );

		UIActionManager.CreatePanel( actions, 0xAA000000, 1 );

		m_EnableLighthouses				= UIActionManager.CreateCheckbox( actions, "Lighthouses:" );

		UIActionManager.CreatePanel( actions, 0xAA000000, 1 );

		m_EnableAutoRun					= UIActionManager.CreateCheckbox( actions, "Autorun:" );

		UIActionManager.CreatePanel( actions, 0xAA000000, 1 );

		m_EnableHUDGPS					= UIActionManager.CreateCheckbox( actions, "Global Positioning System:" );

		UIActionManager.CreatePanel( actions, 0xAA000000, 1 );

		m_NeedGPSItemForKeyBinding 		= UIActionManager.CreateCheckbox( actions, "Require GPS Item:" );
		m_NeedMapItemForKeyBinding 		= UIActionManager.CreateCheckbox( actions, "Require Map Item:" );

		UIActionManager.CreatePanel( actions, 0xAA000000, 1 );

		m_EnableHUDNightvisionOverlay	= UIActionManager.CreateCheckbox( actions, "Nightvision Overlay:" );

		UIActionManager.CreatePanel( actions, 0xAA000000, 1 );

		m_EnablePlayerTags				= UIActionManager.CreateCheckbox( actions, "Player Tags:" );
		m_PlayerTagViewRange			= UIActionManager.CreateSlider( actions, "  View Range:", 0, 100 );
		
		// m_EnableHumanityOnPlayerTags	= UIActionManager.CreateCheckbox( actions, "  Humanity:" );

		UIActionManager.CreatePanel( actions, 0xAA000000, 1 );

		m_EnablePlayerList				= UIActionManager.CreateCheckbox( actions, "Player List:" );

		UIActionManager.CreatePanel( actions, 0xAA000000, 1 );

		m_UnlimitedStamina				= UIActionManager.CreateCheckbox( actions, "Unlimited Stamina:" );

		UIActionManager.CreatePanel( actions, 0xAA000000, 1 );
		
		m_VehicleSync					= UIActionManager.CreateSelectionBox( actions, "Vehicle Sync:", { "Server", "Predictive", "Client" } );

		m_ActionListScroller.UpdateScroller();
	}

	override void OnShow()
	{
		super.OnShow();

		Refresh();
	}

	override void OnHide()
	{
		super.OnHide();
	}

	override void OnClientPermissionsUpdated()
	{
		super.OnClientPermissionsUpdated();

		m_Apply.UpdatePermission( "Expansion.General.Apply" );
	}

	void Click_Refresh( UIEvent eid, ref UIActionBase action )
	{
		if ( eid != UIEvent.CLICK )
			return;
		
		Refresh();
	}

	void Refresh()
	{
		ExpansionGeneralSettings settings = GetExpansionSettings().GetGeneral();
		
		m_PlayerLocationNotifier.SetChecked( settings.PlayerLocationNotifier );
		m_ShowPlayerJoinServer.SetChecked( settings.ShowPlayerJoinServer );
		m_JoinMessageType.SetSelection( settings.JoinMessageType, false );
		m_ShowPlayerLeftServer.SetChecked( settings.ShowPlayerLeftServer );
		m_LeftMessageType.SetSelection( settings.LeftMessageType, false );
		// m_EnableKillFeed.SetChecked( settings.EnableKillFeed );
		// m_KillFeedMessageType.SetSelection( settings.KillFeedMessageType, false );
		// m_EnableHumanity.SetChecked( settings.EnableHumanity );
		m_EnableGlobalChat.SetChecked( settings.EnableGlobalChat );
		m_UseCustomMappingModule.SetChecked( settings.Mapping.UseCustomMappingModule );
		m_InteriorBuilding.SetChecked( settings.Mapping.InteriorBuilding );
		m_LightingConfig.SetSelection( settings.LightingConfig, false );
		m_EnableLamps.SetSelection( settings.EnableLamps, false );
		m_EnableGenerators.SetChecked( settings.EnableGenerators );
		m_EnableLighthouses.SetChecked( settings.EnableLighthouses );
		m_EnableAutoRun.SetChecked( settings.EnableAutoRun );
		m_EnableHUDGPS.SetChecked( settings.EnableHUDGPS );
		m_NeedGPSItemForKeyBinding.SetChecked( settings.NeedGPSItemForKeyBinding );
		m_NeedMapItemForKeyBinding.SetChecked( settings.NeedMapItemForKeyBinding );
		m_EnableHUDNightvisionOverlay.SetChecked( settings.EnableHUDNightvisionOverlay );
		m_EnablePlayerTags.SetChecked( settings.EnablePlayerTags );
		m_PlayerTagViewRange.SetCurrent( settings.PlayerTagViewRange );
		// m_EnableHumanityOnPlayerTags.SetChecked( settings.EnableHumanityOnPlayerTags );
		m_EnablePlayerList.SetChecked( settings.EnablePlayerList );
		m_UnlimitedStamina.SetChecked( settings.UnlimitedStamina );
		m_VehicleSync.SetSelection( settings.VehicleSync, false );
	}

	void Click_Apply( UIEvent eid, ref UIActionBase action )
	{
		if ( eid != UIEvent.CLICK )
			return;

		ExpansionGeneralSettings settings = new ExpansionGeneralSettings;
		settings.Copy( GetExpansionSettings().GetGeneral() );

		settings.PlayerLocationNotifier = m_PlayerLocationNotifier.IsChecked();
		settings.ShowPlayerJoinServer = m_ShowPlayerJoinServer.IsChecked();
		settings.JoinMessageType = m_JoinMessageType.GetSelection();
		settings.ShowPlayerLeftServer = m_ShowPlayerLeftServer.IsChecked();
		settings.LeftMessageType = m_LeftMessageType.GetSelection();
		// settings.EnableKillFeed = m_EnableKillFeed.IsChecked();
		// settings.KillFeedMessageType = m_KillFeedMessageType.GetSelection();
		// settings.EnableHumanity = m_EnableHumanity.IsChecked();
		settings.EnableGlobalChat = m_EnableGlobalChat.IsChecked();
		settings.Mapping.UseCustomMappingModule = m_UseCustomMappingModule.IsChecked();
		settings.Mapping.InteriorBuilding = m_InteriorBuilding.IsChecked();
		settings.LightingConfig = m_LightingConfig.GetSelection();
		settings.EnableLamps = m_EnableLamps.GetSelection();
		settings.EnableGenerators = m_EnableGenerators.IsChecked();
		settings.EnableLighthouses = m_EnableLighthouses.IsChecked();
		settings.EnableAutoRun = m_EnableAutoRun.IsChecked();
		settings.EnableHUDGPS = m_EnableHUDGPS.IsChecked();
		settings.NeedGPSItemForKeyBinding = m_NeedGPSItemForKeyBinding.IsChecked();
		settings.NeedMapItemForKeyBinding = m_NeedMapItemForKeyBinding.IsChecked();
		settings.EnableHUDNightvisionOverlay = m_EnableHUDNightvisionOverlay.IsChecked();
		settings.EnablePlayerTags = m_EnablePlayerTags.IsChecked();
		settings.PlayerTagViewRange = m_PlayerTagViewRange.GetCurrent();
		// settings.EnableHumanityOnPlayerTags = m_EnableHumanityOnPlayerTags.IsChecked();
		settings.EnablePlayerList = m_EnablePlayerList.IsChecked();
		settings.UnlimitedStamina = m_UnlimitedStamina.IsChecked();
		settings.VehicleSync = m_VehicleSync.GetSelection();

		m_Module.Update( settings );
	}
}