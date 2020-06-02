class ExpansionCOTMapMenu: JMFormBase
{
	private ExpansionMapSettings m_Settings;
	private ExpansionCOTMapModule m_Module;

	void ExpansionCOTMapMenu()
	{
		ExpansionSettings.SI_Map.Insert( OnSettingsUpdated );
	}

	void ~ExpansionCOTMapMenu()
	{
		ExpansionSettings.SI_Map.Remove( OnSettingsUpdated );
	}

	protected override bool SetModule( ref JMRenderableModuleBase mdl )
	{
		m_Settings = GetExpansionSettings().GetMap();
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
}