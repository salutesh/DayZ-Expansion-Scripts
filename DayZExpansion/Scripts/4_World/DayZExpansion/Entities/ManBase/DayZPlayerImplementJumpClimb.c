modded class DayZPlayerImplementJumpClimb
{
	private ref SHumanCommandClimbResult m_ClimbRes;

	override void JumpOrClimb()
	{
		SHumanCommandClimbSettings hcls = m_Player.GetDayZPlayerType().CommandClimbSettingsW();
		
		if ( m_Player.m_MovementState.m_iMovement != DayZPlayerConstants.MOVEMENTIDX_IDLE )
			hcls.m_fFwMaxDistance = 2.5;
		else
			hcls.m_fFwMaxDistance = 1.2;
		
		if ( m_ClimbRes == NULL )
			m_ClimbRes = new SHumanCommandClimbResult();
		
		HumanCommandClimb.DoClimbTest( m_Player, m_ClimbRes, 0 );
		if ( m_ClimbRes.m_bIsClimb || m_ClimbRes.m_bIsClimbOver )
		{
			int climbType = GetClimbType( m_ClimbRes.m_fClimbHeight );

			if ( !m_Player.CanClimb( climbType, m_ClimbRes ) )
				return;

			if ( Climb( m_ClimbRes ) )
			{
				if ( climbType == 1 )
					m_Player.DepleteStamina( EStaminaModifiers.VAULT );
				else if ( climbType == 2 )
					m_Player.DepleteStamina( EStaminaModifiers.CLIMB );

				return;
			}
		}
		
		if ( !m_Player.CanJump() )
			return;
		
		Jump();
		m_Player.DepleteStamina( EStaminaModifiers.JUMP );
	}

	private override bool Climb( SHumanCommandClimbResult pClimbRes )
	{
		int climbType = GetClimbType( pClimbRes.m_fClimbHeight );	
		if ( climbType != -1 )
		{
			m_Player.OnClimbStart( pClimbRes, climbType );
			m_Player.StopHandEvent();
		}

		return climbType != -1;
	}

	private override void Jump()
	{
		#ifdef EXPANSIONEXPRINT
		EXPrint("DayZPlayerImplementJumpClimb::Jump Start");
		#endif
		
		m_bIsJumpInProgress = true;
		m_Player.SetFallYDiff( m_Player.GetPosition()[1] );

		m_Player.OnJumpStart();
		m_Player.StartCommand_ExpansionFall( 2.6 ); 
		m_Player.StopHandEvent();
		
		#ifdef EXPANSIONEXPRINT
		EXPrint("DayZPlayerImplementJumpClimb::Jump End");
		#endif
	}
}