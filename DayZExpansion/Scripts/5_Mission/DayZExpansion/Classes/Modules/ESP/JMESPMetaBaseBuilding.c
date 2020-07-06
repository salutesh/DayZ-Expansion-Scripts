/*
modded class JMESPMetaBaseBuilding
{
	UIActionButton m_Action_RequestCode;
	UIActionButton m_Action_RemoveCode;

	override void CreateActions( Widget parent )
	{
		super.CreateActions( parent );

		UIActionManager.CreatePanel( parent, 0xFF000000, 1 );

		UIActionManager.CreateText( parent, "Codelock: ", "" );
		m_Action_RequestCode = UIActionManager.CreateButton( parent, "Request Code", this, "Action_RequestCode" );
		m_Action_RemoveCode = UIActionManager.CreateButton( parent, "Remove Code", this, "Action_RemoveCode" );
	}

	void Action_RequestCode( UIEvent eid, ref UIActionBase action )
	{
		if ( eid != UIEvent.CLICK )
			return;

		//module.RequestCode( target );
	}

	void Action_RemoveCode( UIEvent eid, ref UIActionBase action )
	{
		if ( eid != UIEvent.CLICK )
			return;

		//module.RemoveCode( target );
	}
};
*/