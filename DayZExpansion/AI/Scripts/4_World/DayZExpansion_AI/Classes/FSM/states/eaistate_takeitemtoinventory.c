class eAIState_TakeItemToInventory: eAIState_TakeItem_Base
{
	override int OnUpdateEx(float DeltaTime, int SimulationPrecision)
	{
		if (m_Item && !m_Item.Expansion_GetRootPlayerAliveExcluding(null))
		{
			if (unit.eAI_GetThreatOverride(m_Item))
				return EXIT;
			
			if (!unit.eAI_TakeItemToInventory(m_Item))
			{
				unit.eAI_Unbug(m_Name + "(" + m_Item + ") - failure");
				return EXIT;
			}
		}
		
		return CONTINUE;
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
