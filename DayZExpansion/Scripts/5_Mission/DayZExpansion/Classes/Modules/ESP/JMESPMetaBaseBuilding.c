modded class JMESPMetaBaseBuilding
{
	ExpansionESPModificationModule exp_Module;

	UIActionText m_Action_Code;

	UIActionButton m_Action_RequestCode;
	UIActionButton m_Action_RemoveCode;

	override void Create( JMESPModule mod )
	{
		Class.CastTo( exp_Module, GetModuleManager().GetModule( ExpansionESPModificationModule ) );

		super.Create( mod );
	}

	override void CreateActions( Widget parent )
	{
		super.CreateActions( parent );

		UIActionManager.CreatePanel( parent, 0xFF000000, 1 );

		m_Action_Code = UIActionManager.CreateText( parent, "Codelock: ", "UNKNOWN CODE" );
		m_Action_RequestCode = UIActionManager.CreateButton( parent, "Refresh Code", this, "Action_RequestCode" );
		m_Action_RemoveCode = UIActionManager.CreateButton( parent, "Remove Code", this, "Action_RemoveCode" );
	}

	void SetCode( string code )
	{
		m_Action_Code.SetText( code );
	}

	void Action_RequestCode( UIEvent eid, ref UIActionBase action )
	{
		if ( eid != UIEvent.CLICK )
			return;

		exp_Module.RequestCode( target );
	}

	void Action_RemoveCode( UIEvent eid, ref UIActionBase action )
	{
		if ( eid != UIEvent.CLICK )
			return;

		exp_Module.RemoveCode( target );
	}
};