class ExpansionCOTSafeZoneMenu: JMFormBase
{
	private ExpansionSafeZoneSettings m_Settings;
	private ExpansionCOTSafeZoneModule m_Module;

	void ExpansionCOTSafeZoneMenu()
	{
		ExpansionSettings.SI_SafeZone.Insert( OnSettingsUpdated );
	}

	void ~ExpansionCOTSafeZoneMenu()
	{
		ExpansionSettings.SI_SafeZone.Remove( OnSettingsUpdated );
	}

	protected override bool SetModule( ref JMRenderableModuleBase mdl )
	{
		m_Settings = GetExpansionSettings().GetSafeZone();
		return m_Settings && Class.CastTo( m_Module, mdl );
	}

	override void OnInit()
	{
		ref Widget action = UIActionManager.CreateGridSpacer( layoutRoot.FindAnyWidget( "actions_wrapper" ), 1, 1 );

		UIActionManager.CreateButton( action, "Apply Settings", this, "Click_Set" );
	}

	override void OnShow()
	{
		super.OnShow();

		OnSettingsUpdated();
	}

	void OnSettingsUpdated()
	{
	}

	override void OnHide()
	{
		super.OnHide();
	}

	void Click_Set( UIEvent eid, ref UIActionButton action )
	{
	}
}