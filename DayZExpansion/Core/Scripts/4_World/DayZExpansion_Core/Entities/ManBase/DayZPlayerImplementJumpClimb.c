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
		auto trace = EXTrace.Start(EXTrace.PLAYER, this, "" + m_Player);

		SHumanCommandClimbSettings hcls = m_Player.GetDayZPlayerType().CommandClimbSettingsW();

		if (m_Player.m_MovementState.m_iMovement != DayZPlayerConstants.MOVEMENTIDX_IDLE)
			hcls.m_fFwMaxDistance = 2.5;
		else
			hcls.m_fFwMaxDistance = 1.2;

		if (m_Player.m_ExClimbResult.m_bIsClimb || m_Player.m_ExClimbResult.m_bIsClimbOver)
		{
			int climbType = GetClimbType(m_Player.m_ExClimbResult.m_fClimbHeight);

			if (climbType != -1 && m_Player.CanClimb(climbType, m_Player.m_ExClimbResult))
			{
				Expansion_DebugJumpOrClimb(trace, climbType);

				m_Player.OnClimbStart(climbType);
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

	override void JumpOrClimb()
	{
		auto trace = EXTrace.Start(EXTrace.PLAYER, this, "" + m_Player);

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
					Expansion_DebugJumpOrClimb(trace, climbType);

					m_Player.OnClimbStart(climbType);
					m_Player.StopHandEvent();

					if (climbType == 1)
						m_Player.DepleteStamina(EStaminaModifiers.VAULT);
					else if (climbType == 2)
						m_Player.DepleteStamina(EStaminaModifiers.CLIMB);
				}

				return;
			}
			else
			{
				if (climbType != -1 && m_Player.CanClimb(climbType, m_Player.m_ExClimbResult))
				{
					Expansion_DebugJumpOrClimb(trace, climbType);
				}
			}
		}

		super.JumpOrClimb();
	}

	void Expansion_DebugJumpOrClimb(EXTrace trace, int climbType)
	{
		if (!trace)
			return;

		EXPrint("isClimb " + m_Player.m_ExClimbResult.m_bIsClimb);
		EXPrint("isClimbOver " + m_Player.m_ExClimbResult.m_bIsClimbOver);
		EXPrint("finishWithFall " + m_Player.m_ExClimbResult.m_bFinishWithFall);
		EXPrint("hasParent " + m_Player.m_ExClimbResult.m_bHasParent);

		EXPrint("climbHeight " + m_Player.m_ExClimbResult.m_fClimbHeight);

		EXPrint("climbGrabPoint " + m_Player.m_ExClimbResult.m_ClimbGrabPoint);
		EXPrint("climbGrabPointNormal " + m_Player.m_ExClimbResult.m_ClimbGrabPointNormal);
		EXPrint("climbStandPoint " + m_Player.m_ExClimbResult.m_ClimbStandPoint);
		EXPrint("climbOverStandPoint " + m_Player.m_ExClimbResult.m_ClimbOverStandPoint);

		Object object;

		EXPrint("grabPointParent " + m_Player.m_ExClimbResult.m_GrabPointParent + " " + Expansion_GetIEntityPosition(m_Player.m_ExClimbResult.m_GrabPointParent));
		if (Class.CastTo(object, m_Player.m_ExClimbResult.m_GrabPointParent))
			EXPrint("grabPointParent type " + object.GetType());

		EXPrint("climbStandPointParent " + m_Player.m_ExClimbResult.m_ClimbStandPointParent + " " + Expansion_GetIEntityPosition(m_Player.m_ExClimbResult.m_ClimbStandPointParent));
		if (Class.CastTo(object, m_Player.m_ExClimbResult.m_ClimbStandPointParent))
			EXPrint("climbStandPointParent type " + object.GetType());

		EXPrint("climbOverStandPointParent " + m_Player.m_ExClimbResult.m_ClimbOverStandPointParent + " " + Expansion_GetIEntityPosition(m_Player.m_ExClimbResult.m_ClimbOverStandPointParent));
		if (Class.CastTo(object, m_Player.m_ExClimbResult.m_ClimbOverStandPointParent))
			EXPrint("climbOverStandPointParent type " + object.GetType());

		EXPrint("climbType " + climbType);
	}

	vector Expansion_GetIEntityPosition(IEntity entity)
	{
		vector mat[4];
		if (entity)
			entity.GetTransform(mat);
		return mat[3];
	}
};