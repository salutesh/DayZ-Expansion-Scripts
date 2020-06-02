class ExpansionCOTPartyMenu: JMFormBase
{
	private ExpansionPartySettings m_Settings;
	private ExpansionCOTMapModule m_Module;

	void ExpansionCOTPartyMenu()
	{
		ExpansionSettings.SI_Party.Insert( this.OnSettingsUpdated );
	}

	void ~ExpansionCOTPartyMenu()
	{
		ExpansionSettings.SI_Party.Remove( this.OnSettingsUpdated );
	}

	protected override bool SetModule( ref JMRenderableModuleBase mdl )
	{
		m_Settings = GetExpansionSettings().GetParty();
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
		ref ExpansionPartySettings settings = GetExpansionSettings().GetParty();

		if ( !settings ) return;
	}

	override void OnHide()
	{
		super.OnHide();
	}

	void Click_Set( UIEvent eid, ref UIActionButton action )
	{
		ref ExpansionPartySettings settings = GetExpansionSettings().GetParty();

		if ( !settings ) return;

		GetRPCManager().SendRPC( "DayZExpansion", "ApplyPartySettings", new Param1< ref ExpansionPartySettings >( settings ), false );
	}
}