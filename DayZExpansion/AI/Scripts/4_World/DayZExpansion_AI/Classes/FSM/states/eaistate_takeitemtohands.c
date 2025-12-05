class eAIState_TakeItemToHands: eAIState_TakeItem_Base
{
	int m_RequestSwapToMeleeForZombieTime;
	bool m_AllowDrop;

	override int OnUpdateEx(float DeltaTime, int SimulationPrecision)
	{
		ItemBase hands = unit.GetItemInHands();
		if (m_Item && hands != m_Item && !m_Item.Expansion_GetRootPlayerAliveExcluding(unit))
		{
			if (unit.eAI_GetThreatOverride(m_Item))
			{
				//! If we can't take the item, exit
				if (!unit.eAI_TakeItemToInventoryDropShoulderImpl(m_Item))
					return EXIT;
			}
			else if (hands)
			{
				//! Try to put current hand item in inventory (shoulder slot or cargo), drop if not possible
				if (!unit.eAI_TakeItemToInventoryDropShoulderImpl(hands, m_AllowDrop))
				{
					if (m_AllowDrop)
						unit.eAI_DropItem(hands, false, true, false);
					else
					{
						unit.eAI_ThreatOverride(m_Item, true);
						return EXIT;
					}
				}
				else if (unit.m_eAI_LastDroppedItem == m_Item)
					unit.eAI_ThreatOverride(m_Item, false);
				else
					unit.m_eAI_LastDroppedItem = null;
			}
			else if (!unit.eAI_TakeItemToHands(m_Item))
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
		
		ItemBase hands = unit.GetItemInHands();
		
		//! If ruined, drop
		if (hands && hands.IsDamageDestroyed())
		{
			unit.eAI_DropItem(hands, false);
			return EXIT;
		}
		
		//! First check if we want to switch to bandage
		if (unit.eAI_ShouldBandage())
		{
			if (!hands || !hands.Expansion_CanBeUsedToBandage())
			{
				//! Item in hand is either not bandage/rag or is ruined
				m_Item = unit.GetBandageToUse();
				if (m_Item)
				{
					m_AllowDrop = true;
					return eAITransition.SUCCESS;
				}
			}
			else
			{
				//! Item in hands and can be used to bandage
				return eAITransition.FAIL;
			}
		}
		
		eAITarget target = unit.GetTarget();
		EntityAI targetEntity;
		ItemBase targetItem;
		if (target)
		{
			targetEntity = target.GetEntity();
			Class.CastTo(targetItem, targetEntity);
		}
		
		bool preferExplosiveAmmo;
		bool preferMelee;
		bool hasMeleeInHands;
		bool preferTargetItem;
		
		if (hands)
		{
			if (hands.Expansion_IsMeleeWeapon())
				hasMeleeInHands = true;
			
			if (target)
			{
				if (target.IsMechanicalTrap() && target.IsActive() && !target.IsExplosive() && !target.CanPutInCargo(unit))
				{
					preferMelee = true;
				}
				else if (targetEntity && (targetEntity.IsCorpse() || (targetEntity.IsAnimal() && targetEntity.IsDamageDestroyed())))
				{
					preferMelee = true;
				}
				else if (target.IsZombie())
				{
					if ((hasMeleeInHands || (!hands.eAI_IsSilent() && g_Game.GetTime() - m_RequestSwapToMeleeForZombieTime > 60000)) && unit.m_eAI_SilentAttackViabilityTime > 2.0 && unit.eAI_HasLOS() && unit.GetHealth01() > 0.8)
					{
						preferMelee = true;
						if (!hasMeleeInHands)
							m_RequestSwapToMeleeForZombieTime = g_Game.GetTime();
					}
				}
			}
			
			if (preferMelee)
			{
				if (hasMeleeInHands)
					return eAITransition.FAIL;
			}
			else if (targetItem && unit.eAI_WeaponSelection(hands, targetItem))
			{
				preferTargetItem = true;
			}
			else if (hands.IsWeapon())
			{
				if (unit.eAI_ShouldPreferExplosiveAmmo())
				{
					if (hands.ShootsExplosiveAmmo())
						return eAITransition.FAIL;
					else
						preferExplosiveAmmo = true;
				}
			}
		}
		
		ItemBase item;  //! The item we may want to take to hands
		
		if (!preferMelee)
		{
			if (!preferTargetItem)
			{
				item = unit.eAI_GetWeaponToUse(true, preferExplosiveAmmo);
				if (item)
				{
					if (hands)
					{
						if (item == hands || item.Expansion_GetHierarchyRootItem() == hands || !unit.eAI_WeaponSelection(hands, item))
							return eAITransition.FAIL;
					}
					
					m_Item = item;
					m_AllowDrop = true;
					EXTrace.Print(EXTrace.AI, unit, "Taking " + item + " to hands because not prefering melee or target and weapon to use is not current hand item or better than " + hands);
					return eAITransition.SUCCESS;
				}
			}
			
			if (targetItem && ((targetItem.IsWeapon() && (!preferExplosiveAmmo || targetItem.ShootsExplosiveAmmo())) || (!preferExplosiveAmmo && targetItem.Expansion_IsMeleeWeapon() && (!hasMeleeInHands || hands.Expansion_CompareDPS(targetItem) < 0))) && !targetItem.Expansion_GetRootPlayerAliveExcluding(null) && !targetItem.IsSetForDeletion())
			{
				if (target.GetDistanceSq(true) <= 4.0 && target.GetThreat() > 0.1 && !unit.eAI_IsItemObstructed(targetItem))
				{
					m_Item = targetItem;
					m_AllowDrop = true;
					EXTrace.Print(EXTrace.AI, unit, "Taking target " + targetItem + " to hands because not prefering melee but prefering target over " + hands);
					return eAITransition.SUCCESS;
				}
			}
			
			if (!preferTargetItem && !preferExplosiveAmmo)
			{
				item = unit.eAI_GetWeaponToUse(true, false);
				
				//! If we have no non-explosive ammo weapon and target is not an item and distance is at least 30 m,
				//! use explosive ammo weapon if we have any
				if (!item && target && !targetItem && target.GetDistanceSq() > 30.0)
					item = unit.eAI_GetWeaponToUse(true, true);
			}
			
			if (item)
			{
				if (hands)
				{
					if (item == hands || item.Expansion_GetHierarchyRootItem() == hands || !unit.eAI_WeaponSelection(hands, item))
						return eAITransition.FAIL;
				}
				
				m_Item = item;
				m_AllowDrop = true;
				EXTrace.Print(EXTrace.AI, unit, "Taking " + item + " to hands because not prefering melee and weapon to use is not current hand item or better than " + hands);
				return eAITransition.SUCCESS;
			}
			
			if (hands)
				return eAITransition.FAIL;
		}
		
		if (hasMeleeInHands)
			return eAITransition.FAIL;
		
		item = unit.GetMeleeWeaponToUse();
		if (item)
		{
			if (hands)
			{
				if (item == hands || item.Expansion_GetHierarchyRootItem() == hands)
					return eAITransition.FAIL;
				
				if (preferMelee && unit.eAI_GetThreatOverride(item))
					return eAITransition.FAIL;
			}
			
			m_Item = item;
			m_AllowDrop = !preferMelee;
			return eAITransition.SUCCESS;
		}
		
		 return eAITransition.FAIL;
	}
}
