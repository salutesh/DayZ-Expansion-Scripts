/**
 * DayZPlayerImplementJumpClimb.c
 *
 * DayZ Expansion Mod
 * www.dayzexpansion.com
 * © 2021 DayZ Expansion Mod Team
 *
 * This work is licensed under the Creative Commons Attribution-NonCommercial-NoDerivatives 4.0 International License. 
 * To view a copy of this license, visit http://creativecommons.org/licenses/by-nc-nd/4.0/.
 *
*/

modded class DayZPlayerImplementJumpClimb
{
	override void JumpOrClimb()
	{
		SHumanCommandClimbSettings hcls = m_Player.GetDayZPlayerType().CommandClimbSettingsW();
		
		if ( m_Player.m_MovementState.m_iMovement != DayZPlayerConstants.MOVEMENTIDX_IDLE )
			hcls.m_fFwMaxDistance = 2.5;
		else
			hcls.m_fFwMaxDistance = 1.2;
		
		HumanCommandClimb.DoClimbTest( m_Player, m_Player.m_ExClimbResult, 0 );
		if ( m_Player.m_ExClimbResult.m_bIsClimb || m_Player.m_ExClimbResult.m_bIsClimbOver )
		{
			if ( m_Player.CallExpansionClimbCode() )
			{
				int climbType = GetClimbType( m_Player.m_ExClimbResult.m_fClimbHeight );	

				if ( climbType != -1 && m_Player.CanClimb( climbType, m_Player.m_ExClimbResult ) )
				{
					m_Player.OnClimbStart( climbType );
					m_Player.StopHandEvent();

					if ( climbType == 1 )
						m_Player.DepleteStamina( EStaminaModifiers.VAULT );
					else if ( climbType == 2 )
						m_Player.DepleteStamina( EStaminaModifiers.CLIMB );
				}

				return;
			}
		}

		super.JumpOrClimb();
	}

	private override void Jump()
	{
		m_bIsJumpInProgress = true;
		m_Player.SetFallYDiff( m_Player.GetPosition()[1] );

		m_Player.OnJumpStart();
		m_Player.StartCommand_ExpansionFall( 2.6 ); 
		m_Player.StopHandEvent();
	}
};