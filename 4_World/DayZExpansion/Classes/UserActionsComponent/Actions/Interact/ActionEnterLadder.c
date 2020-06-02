modded class ActionEnterLadder
{
	override void Start( ActionData action_data )
	{
		super.Start( action_data );
		
		if ( ExpansionAttachmentHelper.CanAttachTo( action_data.m_Player, action_data.m_Target.GetObject() ) )
		{
			action_data.m_Player.AttachTo( action_data.m_Target.GetObject(), true );
		}
	}
}