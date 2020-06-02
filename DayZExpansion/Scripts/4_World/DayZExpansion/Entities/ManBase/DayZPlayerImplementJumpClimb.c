modded class DayZPlayerImplementJumpClimb
{	
	private override bool Climb( SHumanCommandClimbResult pClimbRes )
	{
		if ( !super.Climb( pClimbRes ) )
			return false;

		m_Player.OnClimbStart( pClimbRes );

		return true;
	}

	private override void Jump()
	{
		#ifdef EXPANSIONEXPRINT
		EXPrint("DayZPlayerImplementJumpClimb::Jump Start");
		#endif
		
		m_bIsJumpInProgress = true;
		m_Player.SetFallYDiff( m_Player.GetPosition()[1] );

		m_Player.OnJumpStart();
		m_Player.StartCommand_ExpansionFall( 3.5 );
		
		#ifdef EXPANSIONEXPRINT
		EXPrint("DayZPlayerImplementJumpClimb::Jump End");
		#endif
	}
}