/**
 * DayZPlayerImplementJumpClimb.c
 *
 * DayZ Expansion Mod
 * www.dayzexpansion.com
 * © 2022 DayZ Expansion Mod Team
 *
 * This work is licensed under the Creative Commons Attribution-NonCommercial-NoDerivatives 4.0 International License. 
 * To view a copy of this license, visit http://creativecommons.org/licenses/by-nc-nd/4.0/.
 *
*/

modded class DayZPlayerImplementJumpClimb
{
	bool Expansion_Climb()
	{
#ifdef EXPANSIONTRACE
		auto trace = CF_Trace_1(EXTrace.PLAYER, this).Add(m_Player);
#endif

		if (m_Player.m_ExClimbResult.m_bIsClimb || m_Player.m_ExClimbResult.m_bIsClimbOver)
		{
			int climbType = GetClimbType(m_Player.m_ExClimbResult.m_fClimbHeight);

			if (climbType != -1 && m_Player.CanClimb(climbType, m_Player.m_ExClimbResult))
			{
			#ifndef DAYZ_1_19
				//! 1.20+
				m_Player.Expansion_OnClimbStart(climbType);
			#endif
				m_Player.StartCommand_Climb(m_Player.m_ExClimbResult, climbType);
				m_Player.StopHandEvent();

				if (climbType == 1)
					m_Player.DepleteStamina(EStaminaModifiers.VAULT);
				else if (climbType == 2)
					m_Player.DepleteStamina(EStaminaModifiers.CLIMB);

				return true;
			}
		}

		return false;
	}

#ifndef DAYZ_1_19
	//! 1.20+
	override void JumpOrClimb()
	{
#ifdef DIAG
		auto trace = CF_Trace_1(EXTrace.PLAYER, this).Add(m_Player);
#endif

		SHumanCommandClimbSettings hcls = m_Player.GetDayZPlayerType().CommandClimbSettingsW();

		if (m_Player.m_MovementState.m_iMovement != DayZPlayerConstants.MOVEMENTIDX_IDLE)
			hcls.m_fFwMaxDistance = 2.5;
		else
			hcls.m_fFwMaxDistance = 1.2;

		HumanCommandClimb.DoClimbTest(m_Player, m_Player.m_ExClimbResult, 0);
		if (m_Player.m_ExClimbResult.m_bIsClimb || m_Player.m_ExClimbResult.m_bIsClimbOver)
		{
			int climbType = GetClimbType(m_Player.m_ExClimbResult.m_fClimbHeight);

			if (m_Player.CallExpansionClimbCode())
			{
				if (climbType != -1 && m_Player.CanClimb(climbType, m_Player.m_ExClimbResult))
				{
					m_Player.Expansion_OnClimbStart(climbType);
					m_Player.StopHandEvent();

					if (climbType == 1)
						m_Player.DepleteStamina(EStaminaModifiers.VAULT);
					else if (climbType == 2)
						m_Player.DepleteStamina(EStaminaModifiers.CLIMB);
				}

				return;
			}
		}

		super.JumpOrClimb();
	}
#endif
};
