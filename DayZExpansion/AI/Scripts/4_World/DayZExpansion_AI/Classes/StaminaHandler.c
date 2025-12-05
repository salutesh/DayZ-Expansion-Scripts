/**
 * StaminaHandler.c
 * 
 * Partly based on Enfusion AI Project Copyright 2021 William Bowers
 *
 * DayZ Expansion Mod
 * www.dayzexpansion.com
 * © 2023 DayZ Expansion Mod Team
 *
 * This work is licensed under the Creative Commons Attribution-NonCommercial-NoDerivatives 4.0 International License. 
 * To view a copy of this license, visit http://creativecommons.org/licenses/by-nc-nd/4.0/.
 *
*/

modded class StaminaHandler
{
	override void Update(float deltaT, int pCurrentCommandID)
	{
		if (!eAI_HasUnlimitedStamina())
			super.Update(deltaT, pCurrentCommandID);
	}

	bool eAI_HasUnlimitedStamina()
	{
		if (m_Player && m_Player.IsAI() && !m_Player.IsDamageDestroyed())
		{
			eAIGroup group = m_Player.GetGroup();

			if (!group)
			{
				CF.FormatError("%1 group is NULL", m_Player.ToString());
				return false;
			}

			if (group.GetFaction().HasUnlimitedStamina())
				return true;
		}

		return false;
	}

	override void DepleteStaminaEx(EStaminaModifiers modifier, float dT = -1, float coef = 1.0)
	{
		if (!eAI_HasUnlimitedStamina())
			super.DepleteStaminaEx(modifier, dT, coef);
	}

	override protected void StaminaProcessor_Move(HumanMovementState pHumanMovementState)
	{
		if (!m_Player.IsAI())
		{
			super.StaminaProcessor_Move(pHumanMovementState);
			return;
		}

		switch (pHumanMovementState.m_iMovement)
		{
			case DayZPlayerConstants.MOVEMENTIDX_SPRINT:
				//! Uncomment below to make AI deplete stamina from sprinting
			/*
				if (pHumanMovementState.m_iStanceIdx == DayZPlayerConstants.STANCEIDX_ERECT)
				{
					m_StaminaDelta = -GameConstants.STAMINA_DRAIN_STANDING_SPRINT_PER_SEC * CfgGameplayHandler.GetSprintStaminaModifierErc();
					SetCooldown(GameConstants.STAMINA_REGEN_COOLDOWN_DEPLETION);
					break;
				}
				else if ( pHumanMovementState.m_iStanceIdx == DayZPlayerConstants.STANCEIDX_CROUCH)
				{
					m_StaminaDelta = -GameConstants.STAMINA_DRAIN_CROUCHED_SPRINT_PER_SEC * CfgGameplayHandler.GetSprintStaminaModifierCro();
					SetCooldown(GameConstants.STAMINA_REGEN_COOLDOWN_DEPLETION);
					break;
				}
			*/

				if (!m_IsInCooldown || pHumanMovementState.m_iStanceIdx == DayZPlayerConstants.STANCEIDX_PRONE)
					m_StaminaDelta = GameConstants.STAMINA_GAIN_JOG_PER_SEC;
				break;
				
			case DayZPlayerConstants.MOVEMENTIDX_RUN:
				//! Uncomment below to make AI deplete stamina from jogging in water
			/*
				if (m_Player.GetCurrentWaterLevel() >= m_HumanMoveSettings.m_fWaterLevelSpeedRectrictionHigh)
				{
					m_StaminaDelta = -GameConstants.STAMINA_DRAIN_STANDING_SPRINT_PER_SEC * CfgGameplayHandler.GetSprintStaminaModifierErc();
					break;
				}
			*/
	
				if (!m_IsInCooldown)
					m_StaminaDelta = (GameConstants.STAMINA_GAIN_JOG_PER_SEC + CalcStaminaGainBonus());
				break;
				
			case DayZPlayerConstants.MOVEMENTIDX_WALK:
				if (!m_IsInCooldown)
					m_StaminaDelta = (GameConstants.STAMINA_GAIN_WALK_PER_SEC + CalcStaminaGainBonus());
				break;
				
			case DayZPlayerConstants.MOVEMENTIDX_IDLE:
				if (m_Player.IsRolling())
				{
					m_StaminaDelta = GameConstants.STAMINA_GAIN_ROLL_PER_SEC;
					break;
				}

				if (!m_IsInCooldown)
					m_StaminaDelta = (GameConstants.STAMINA_GAIN_IDLE_PER_SEC + CalcStaminaGainBonus());
				break;
				
			default:
				if (!m_IsInCooldown)
					m_StaminaDelta = GameConstants.STAMINA_GAIN_IDLE_PER_SEC;
				break;
		}
	}

	override protected void StaminaProcessor_Ladder(HumanMovementState pHumanMovementState)
	{
		if (!m_Player.IsAI())
		{
			super.StaminaProcessor_Ladder(pHumanMovementState);
			return;
		}

		switch (pHumanMovementState.m_iMovement)
		{
			case 2: //climb up (fast)
				//! Uncomment below to make AI deplete stamina from climbing ladder fast
			/*
				m_StaminaDelta = -GameConstants.STAMINA_DRAIN_LADDER_FAST_PER_SEC * CfgGameplayHandler.GetSprintLadderStaminaModifier();
				SetCooldown(GameConstants.STAMINA_REGEN_COOLDOWN_DEPLETION);
				break;
			*/
				
			case 1: //climb up (slow)
				if (!m_IsInCooldown)
					m_StaminaDelta = (GameConstants.STAMINA_GAIN_LADDER_PER_SEC + CalcStaminaGainBonus());
				break;
				
			default:
				if (!m_IsInCooldown)
					m_StaminaDelta = GameConstants.STAMINA_GAIN_IDLE_PER_SEC + CalcStaminaGainBonus();
				break;
		}
	}

	override protected void StaminaProcessor_Swimming(HumanMovementState pHumanMovementState)
	{
		if (!m_Player.IsAI())
		{
			super.StaminaProcessor_Swimming(pHumanMovementState);
			return;
		}

		switch (pHumanMovementState.m_iMovement)
		{
			case 3: //swim fast
				//! Uncomment below to make AI deplete stamina from swimming fast
			/*
				m_StaminaDelta = -GameConstants.STAMINA_DRAIN_SWIM_FAST_PER_SEC * CfgGameplayHandler.GetSprintSwimmingStaminaModifier();
				SetCooldown(GameConstants.STAMINA_REGEN_COOLDOWN_DEPLETION);
				break;
			*/
				
			case 2: //swim slow
				if (!m_IsInCooldown)
					m_StaminaDelta = (GameConstants.STAMINA_GAIN_SWIM_PER_SEC + CalcStaminaGainBonus());
				break;
				
			default:
				if (!m_IsInCooldown)
					m_StaminaDelta = GameConstants.STAMINA_GAIN_IDLE_PER_SEC + CalcStaminaGainBonus();
				break;
		}
	}
}
