class eAIState_TakeItemToInventory: eAIState
{
	ItemBase m_Item;
	float m_Time;
	bool m_LoweringWeapon;

	override void OnEntry(string Event, ExpansionState From)
	{
		#ifdef DIAG
		EXTrace.Print(EXTrace.AI, unit, "TakeItemToInventory " + m_Item.ToString());
		#endif
		m_Time = 0;
		m_LoweringWeapon = false;
		if (unit.GetEmoteManager().IsEmotePlaying())
			unit.GetEmoteManager().ServerRequestEmoteCancel();
	}

	override int OnUpdate(float DeltaTime, int SimulationPrecision)
	{
		if (unit.IsUnconscious()) return EXIT;
		
		if (unit.GetEmoteManager().IsEmotePlaying() || unit.GetActionManager().GetRunningAction() || unit.GetWeaponManager().IsRunning() || !unit.GetCommand_MoveAI())
		{
			m_Time += DeltaTime;
			if (m_Time > 10)  //! Looks like something went terribly wrong
			{
				EXPrint(unit.ToString() + " TakeItemToInventory - timeout");
				unit.eAI_Unbug("take to inventory");
				m_Time = 0;
				return EXIT;
			}
			
			return CONTINUE;
		}
		
		//! Taking items while raised breaks hands! Wait until lowered
		if (unit.IsRaised())
		{
			m_LoweringWeapon = true;
			unit.RaiseWeapon(false);
			m_Time = 0;
		}
		
		if (m_LoweringWeapon)
		{
			if (m_Time < 0.5)
			{
				m_Time += DeltaTime;
				return CONTINUE;
			}
			
			m_Time = 0;
			m_LoweringWeapon = false;
		}
		
		if (m_Item && !m_Item.Expansion_GetRootPlayerAliveExcluding(null))
		{
			if (unit.eAI_GetThreatOverride(m_Item))
				return EXIT;
			
			if (!unit.eAI_TakeItemToInventory(m_Item))
			{
				unit.eAI_Unbug("take to inventory");
				return EXIT;
			}
		}
		
		if (m_Time < 0.5)
		{
			m_Time += DeltaTime;
			return CONTINUE;
		}
		
		if (unit.GetActionManager().GetRunningAction())
			return CONTINUE;
		
		return EXIT;
	}

	int Guard()
	{
		if (unit.IsFighting()) return eAITransition.FAIL;
		
		if (unit.IsRestrained()) return eAITransition.FAIL;
		if (unit.IsUnconscious()) return eAITransition.FAIL;
		if (unit.IsSwimming()) return eAITransition.FAIL;
		
		if (unit.eAI_IsChangingStance())
			return eAITransition.FAIL;
		
		if (unit.IsRaised()) return eAITransition.FAIL;
		
		if (unit.GetWeaponManager().IsRunning()) return eAITransition.FAIL;
		if (unit.GetActionManager().GetRunningAction()) return eAITransition.FAIL;
		
		eAITarget target = unit.GetTarget();
		ItemBase targetItem;
		
		if (!target || !Class.CastTo(targetItem, target.GetEntity()) || targetItem.Expansion_GetRootPlayerAliveExcluding(null) || targetItem.IsSetForDeletion() || unit.eAI_GetThreatOverride(targetItem))
			return eAITransition.FAIL;
		
		bool isClothing;
		if (targetItem.IsClothing())
			isClothing = true;
		
		if (targetItem.Expansion_IsLiveExplosive() || (!isClothing && !targetItem.CanPutInCargo(unit)))
			return eAITransition.FAIL;
		
		if (target.GetDistanceSq(true) > 4.0 || unit.eAI_IsItemObstructed(targetItem))
			return eAITransition.FAIL;
		
		if (target.GetThreat() <= 0.1)
			return eAITransition.FAIL;
		
		if (targetItem.IsWeapon())
		{
			//! PREPARE SWAP FROM CURRENT HAND ITEM TO GUN IN INV OR ON GROUND
			//! If target is gun and we have melee or bandage in hand (but shouldn't bandage right now), prepare swap
			ItemBase hands = unit.GetItemInHands();
			if (hands && (hands.Expansion_IsMeleeWeapon() || (hands.Expansion_CanBeUsedToBandage() && !unit.eAI_ShouldBandage())))
			{
				//! Only drop hand item if destroyed or doesn't fit in inventory
				if (hands.IsDamageDestroyed())
				{
					unit.eAI_DropItem(hands, false);
				}
				else if (!unit.eAI_TakeItemToInventory(hands))
				{
					unit.eAI_ThreatOverride(hands, true);  //! Make sure we do not attempt to take it again after  dropping
					unit.eAI_DropItem(hands, false);
				}
			}
			
			return eAITransition.FAIL;  //! Picking up guns is handled by TakeItemToHands state
		}
		else if (targetItem.Expansion_IsMeleeWeapon())
		{
			//! Picking up melee weapons is handled by TakeItemToHands state
			return eAITransition.FAIL;
		}

		if (targetItem.IsCorpse())
		{
			unit.eAI_ThreatOverride(targetItem, true);  //! Only set threat override after we are close
			return eAITransition.FAIL;
		}
		
		if (!isClothing && !unit.eAI_FindFreeInventoryLocationFor(targetItem))
		{
			unit.eAI_ThreatOverride(targetItem, true);
			return eAITransition.FAIL;
		}
		
		unit.eAI_ThreatOverride(targetItem, false);
		
		m_Item = targetItem;
		
		return eAITransition.SUCCESS;
	}
}
