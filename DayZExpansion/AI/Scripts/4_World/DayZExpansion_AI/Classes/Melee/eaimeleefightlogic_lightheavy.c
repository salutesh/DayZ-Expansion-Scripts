class eAIMeleeFightLogic_LightHeavy: DayZPlayerMeleeFightLogic_LightHeavy
{
	eAIBase m_AI;
	eAIMeleeCombat m_eAI_MeleeCombat;
	bool m_eAI_Melee;
	bool m_eAI_IsInCombo;
	int m_eAI_BlockEndTime;

	override void Init(DayZPlayerImplement player)
	{
		super.Init(player);

		m_AI = eAIBase.Cast(player);
		m_eAI_MeleeCombat = m_AI.m_eMeleeCombat;
	}

	override protected EMeleeHitType GetAttackTypeFromInputs(HumanInputController pInputs)
	{
		if (m_AI.CanConsumeStamina(EStaminaConsumers.MELEE_HEAVY))
		{
			return EMeleeHitType.HEAVY;
		}

		return EMeleeHitType.LIGHT;
	}

	//! Always treat mechanical traps as non-alignable
	override protected float GetAttackTypeByDistanceToTarget(EntityAI pTarget, EMeleeTargetType pTargetType = EMeleeTargetType.ALIGNABLE)
	{
		ItemBase item;
		if (Class.CastTo(item, pTarget) && item.Expansion_IsMechanicalTrap())
			return 1.0;

		return super.GetAttackTypeByDistanceToTarget(pTarget, pTargetType);
	}

	override bool HandleFightLogic(int pCurrentCommandID, HumanInputController pInputs, EntityAI pEntityInHands, HumanMovementState pMovementState, out bool pContinueAttack)
	{
		HumanCommandMove hcm		= m_AI.GetCommand_Move();

		if (!m_AI.m_eAI_IsFightingFSM)
		{
			if (m_IsInBlock && hcm)
			{
				hcm.SetMeleeBlock(false);
				SetBlock(false);
			}

			return false;
		}

		eAITarget target = m_AI.GetTarget();
		if (!target)
			return false;

		InventoryItem itemInHands	= InventoryItem.Cast(pEntityInHands);
		bool isFireWeapon			= itemInHands && itemInHands.IsWeapon();
		int stance					= m_AI.eAI_GetStance();

		//! Check if we need to damage
		HandleHitEvent(pCurrentCommandID, pInputs, itemInHands, pMovementState, pContinueAttack);

		//! evade and blocking logic - WIP
		//! (bare hand or with melee weapon only)
		if (!isFireWeapon && hcm)
		{
			bool block;

			//! evades in raised erc stance while moving
			if (stance == DayZPlayerConstants.STANCEIDX_ERECT && m_AI.IsRaised())
			{
				int roll;

				//! 50% chance to evade if attack cooldown is bigger than evade cooldown
				if (!m_IsEvading && target.GetAttackCooldown() > EVADE_COOLDOWN && Math.RandomInt(0, 2))
				{
					roll = Math.RandomIntInclusive(-1, 1);  //! -1 = roll right, 0 = don't evade, 1 = roll left
				}

				//! If already blocking, keep blocking until enemy attack ends or timeout reached
				if (m_IsInBlock)
				{
					if (target.IsFighting() && g_Game.GetTime() < m_eAI_BlockEndTime)
					{
						block = true;
					}
				#ifdef DIAG_DEVELOPER
					else
					{
						EXTrace.Print(EXTrace.AI, m_AI, "HandleFightLogic - end blocking attack (cooldown " + target.GetAttackCooldown() + " " + target.IsFighting() +  ")");
					}
				#endif
				}
				//! Enemy starts attacking, start blocking if not a player (blocking against player only when moving backwards)
				else if (!target.IsPlayer() && target.GetAttackCooldown() > 0)
				{
				#ifdef DIAG_DEVELOPER
					EXTrace.Print(EXTrace.AI, m_AI, "HandleFightLogic - start blocking attack (cooldown " + target.GetAttackCooldown() + " " + target.IsFighting() +  ")");
				#endif
					m_eAI_BlockEndTime = g_Game.GetTime() + Math.RandomInt(1500, 3000);
					block = true;
				}

				if (roll != 0 && m_AI.CanConsumeStamina(EStaminaConsumers.MELEE_EVADE))
				{
					float angle;
					if (roll == 1)
					{
						angle = -90;	// left
					}
					else
					{
						angle = 90;		// right
					}

					// start melee evade
					m_IsEvading = true;
					SetCooldown(EVADE_COOLDOWN, EFightLogicCooldownCategory.EVADE);
					hcm.StartMeleeEvadeA(angle);
					m_AI.DepleteStamina(EStaminaModifiers.MELEE_EVADE);
					
					//Inflict shock when sidestepping with broken legs
					if (m_AI.GetBrokenLegs() == eBrokenLegs.BROKEN_LEGS)
					{
						m_AI.m_ShockHandler.SetShock(PlayerConstants.BROKEN_LEGS_LIGHT_MELEE_SHOCK);
						m_AI.m_ShockHandler.CheckValue(true);
					}

				#ifdef DIAG_DEVELOPER
					EXTrace.Print(EXTrace.AI, m_AI, "HandleFightLogic - evade " + angle);
				#endif
				}
			}

		#ifdef DIAG_DEVELOPER
			bool isInBlock = m_IsInBlock;
		#endif

			//! blocks in raised erc/pro stance
			if ((stance == DayZPlayerConstants.STANCEIDX_ERECT || stance == DayZPlayerConstants.STANCEIDX_PRONE) && m_AI.IsRaised())
			{
				float angle2 = m_AI.m_eAI_CommandMove.GetCurrentMovementAngle();

				//! Block if moving backwards or if blocking attack
				if (angle2 < -130.0 || angle2 > 130 || block)
				{
					hcm.SetMeleeBlock(true);
					SetBlock(true);
				}
				else
				{
					hcm.SetMeleeBlock(false);
					SetBlock(false);
				}
			}
			else
			{
				hcm.SetMeleeBlock(false);
				SetBlock(false);
			}

		#ifdef DIAG_DEVELOPER
			if (m_IsInBlock != isInBlock)
				EXTrace.Print(EXTrace.AI, m_AI, "HandleFightLogic - block " + m_IsInBlock);
		#endif
		}

		//! do not perform attacks when blocking
		if (m_IsInBlock || m_IsEvading)
		{
			m_eAI_Melee = false;
			return false;
		}

		bool startMeleeSuccess;

		//! Start or continue a melee attack
		if (m_eAI_Melee || (pContinueAttack && isFireWeapon))
		{
			m_eAI_Melee = false;

			//! Currently not performing any attacks, so here we start the initial
			if (hcm)
			{
				m_eAI_IsInCombo = false;

				//! melee with firearm
				if (isFireWeapon)
				{
					startMeleeSuccess = HandleInitialFirearmMelee(pCurrentCommandID, pInputs, itemInHands, pMovementState, pContinueAttack);
				}
				else
				{
					//! first attack in raised erc (light or heavy if modifier is used). Also added support for finishers
					if (stance == DayZPlayerConstants.STANCEIDX_ERECT && m_AI.IsRaised())
					{
						startMeleeSuccess = HandleInitialMeleeErc(pCurrentCommandID, pInputs, itemInHands, pMovementState, pContinueAttack);
					}
					//! kick from raised pne
					else if (stance == DayZPlayerConstants.STANCEIDX_PRONE && m_AI.IsRaised())
					{
						startMeleeSuccess = HandleProneKick(pCurrentCommandID, pInputs, itemInHands, pMovementState, pContinueAttack);
					}
					//! sprint attack in erc stance
					else if (stance == DayZPlayerConstants.STANCEIDX_ERECT && m_AI.IsSprintFull())
					{
						startMeleeSuccess = HandleSprintAttack(pCurrentCommandID, pInputs, itemInHands, pMovementState, pContinueAttack);
					}
				}

				if (startMeleeSuccess)
					m_AI.eAI_SkipMelee("starting melee succeeded", target, false);
				else
					m_AI.eAI_SkipMelee("starting melee failed", target, true);
			}
			//! combo hits - when we are already in Melee command
			else if (pCurrentCommandID == DayZPlayerConstants.COMMANDID_MELEE2)
			{
				m_IsInComboRange = m_AI.GetCommand_Melee2().IsInComboRange();
				//! Only attempt combo if target is still ahead
				if (m_IsInComboRange && m_eAI_MeleeCombat.eAI_IsAimingAhead())
				{
					startMeleeSuccess = HandleComboHit(pCurrentCommandID, pInputs, itemInHands, pMovementState, pContinueAttack);
				}
			}
		}

		if (startMeleeSuccess)
			eAI_UpdateAttackCooldown();

		return startMeleeSuccess;
	}
	
	//! NOTE: Only singular (or first) hits, combo attacks are handled in combo
	override protected bool HandleInitialFirearmMelee(int pCurrentCommandID, HumanInputController pInputs, InventoryItem itemInHands, HumanMovementState pMovementState, out bool pContinueAttack)
	{	
		//! don't allow bash to interfere with actions like chambering or ejecting bullets
		Weapon_Base weapon = Weapon_Base.Cast(itemInHands);
		if (weapon.IsWaitingForActionFinish())
			return false;
		
		//! perform firearm melee from raised erect or continue with attack after character stand up from crouch
		if (m_AI.m_eAI_CommandMove.GetCurrentMovementSpeed() <= 2.05 && ((m_AI.eAI_GetStance() == DayZPlayerConstants.STANCEIDX_ERECT && m_AI.IsRaised()) || pContinueAttack) && !m_AI.IsFighting())
		{
			m_HitType = GetAttackTypeByWeaponAttachments(itemInHands);
			if ( m_HitType == EMeleeHitType.NONE )
			{
				pContinueAttack = false;
				return false; //! exit if there is no suitable attack
			}
			
			m_MeleeCombat.Update(itemInHands, m_HitType);

			EntityAI target;
			EMeleeTargetType targetType;
			GetTargetData(target, targetType);
			float attackByDistance = GetAttackTypeByDistanceToTarget(target, targetType);

			m_AI.StartCommand_Melee2(target, m_HitType == EMeleeHitType.WPN_STAB, attackByDistance, m_MeleeCombat.GetHitPos());			
			m_AI.DepleteStamina(EStaminaModifiers.MELEE_HEAVY);
			DisableControls();

			pContinueAttack = false; // reset continueAttack flag

			return true;
		}

		return false;
	}

	override protected bool HandleComboHit(int pCurrentCommandID, HumanInputController pInputs, InventoryItem itemInHands, HumanMovementState pMovementState, out bool pContinueAttack)
	{
		if (super.HandleComboHit(pCurrentCommandID, pInputs, itemInHands, pMovementState, pContinueAttack))
		{
			m_AI.m_eAI_MeleeTime = g_Game.GetTime();
			m_eAI_IsInCombo = true;
			return true;
		}

		return false;
	}

	override protected void EvaluateHit(InventoryItem weapon)
	{
		if (m_MeleeCombat.GetTargetEntity())
		{
			super.EvaluateHit(weapon);
		}
		else
		{
			eAITarget target = m_AI.GetTarget();
			if (target)
				m_AI.eAI_SkipMelee("missed melee target", target, true);
		}
	}

	void eAI_EndBlock()
	{
	#ifdef DIAG_DEVELOPER
		EXTrace.Print(EXTrace.AI, m_AI, "HandleFightLogic - end blocking attack after hit");
	#endif
		HumanCommandMove hcm = m_AI.GetCommand_Move();
		if (hcm)
			hcm.SetMeleeBlock(false);
		SetBlock(false);
	}
};
