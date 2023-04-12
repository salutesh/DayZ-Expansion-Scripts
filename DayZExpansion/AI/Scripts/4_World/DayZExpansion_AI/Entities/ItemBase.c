modded class ItemBase
{
	private autoptr eAITargetInformation m_TargetInformation;

	bool m_Expansion_IsOwnerPlayer;

	void ItemBase()
	{
#ifdef EAI_TRACE
		auto trace = CF_Trace_0(this, "ItemBase");
#endif

		m_TargetInformation = CreateTargetInformation();
	}

	protected eAITargetInformation CreateTargetInformation()
	{
#ifdef EAI_TRACE
		auto trace = CF_Trace_0(this, "CreateTargetInformation");
#endif

		return new eAIItemTargetInformation(this);
	}

	eAITargetInformation GetTargetInformation()
	{
#ifdef EAI_TRACE
		auto trace = CF_Trace_0(this, "GetTargetInformation");
#endif

		return m_TargetInformation;
	}

	override bool EEOnDamageCalculated(TotalDamageResult damageResult, int damageType, EntityAI source, int component, string dmgZone, string ammo, vector modelPos, float speedCoef)
	{
		//! Ignore hit from bullet we create in WeaponBase::EEFired for bullet impact effect to prevent NULL pointers
		//! due to source not being set when trying to determine damage animation
		if (damageType == DT_FIRE_ARM && !source)
			return false;

		return super.EEOnDamageCalculated(damageResult, damageType, source, component, dmgZone, ammo, modelPos, speedCoef);
	}

	override void EEKilled(Object killer)
	{
		m_TargetInformation.OnDeath();

		super.EEKilled(killer);
	}

	override void EEHitBy(TotalDamageResult damageResult, int damageType, EntityAI source, int component, string dmgZone, string ammo, vector modelPos, float speedCoef)
	{
		m_TargetInformation.OnHit();

		super.EEHitBy(damageResult, damageType, source, component, dmgZone, ammo, modelPos, speedCoef);
	}

	override void EEInventoryIn(Man newParentMan, EntityAI diz, EntityAI newParent)
	{
		super.EEInventoryIn(newParentMan, diz, newParent);

		eAIBase ai;
		if (!Class.CastTo(ai, newParentMan))
		{
			m_Expansion_IsOwnerPlayer = true;
			return;
		}

		EXTrace.Print(EXTrace.AI, this, "::EEInventoryIn - " + newParentMan);

		ai.eAI_AddItem(this);
	}

	override void EEInventoryOut(Man oldParentMan, EntityAI diz, EntityAI newParent)
	{
		super.EEInventoryOut(oldParentMan, diz, newParent);

		eAIBase ai;
		if (!Class.CastTo(ai, oldParentMan))
			return;

		EXTrace.Print(EXTrace.AI, this, "::EEInventoryOut - " + oldParentMan);

		ai.eAI_RemoveItem(this);
	}

	override float GetProtectionLevel(int type, bool consider_filter = false, int system = 0)
	{
		Man player = GetHierarchyRootPlayer();
		if (!player || !player.IsInherited(eAIBase))
			return super.GetProtectionLevel(type, consider_filter, system);

		//! @note duplicated vanilla checks for AI but ignore quantity

		if (IsDamageDestroyed())
		{
			return 0;
		}
		
		if( GetInventory().GetAttachmentSlotsCount() != 0 )//is it an item with attachable filter ?
		{
			ItemBase filter = ItemBase.Cast(FindAttachmentBySlotName("GasMaskFilter"));
			if (filter )
			{
				return filter.GetProtectionLevel(type, false, system);//it's a valid filter, return the protection
			}
			else return 0;//otherwise return 0 when no filter attached
		}

		string subclass_path, entryName;

		switch (type)
		{
			case DEF_BIOLOGICAL:
				entryName = "biological";
				break;
			case DEF_CHEMICAL:
				entryName = "chemical";
				break;	
			default:
				entryName = "biological";
				break;
		}
		
		subclass_path = "CfgVehicles " + GetType() + " Protection ";
		
		return GetGame().ConfigGetFloat(subclass_path + entryName);
	}

	bool Expansion_TryTurningOn()
	{
		if ( HasEnergyManager() )
		{
			if ( GetCompEM().CanWork() )
			{
				GetCompEM().SwitchOn();
				return true;
			}
		}
		return false;
	}

	bool Expansion_TryTurningOff()
	{
		if ( HasEnergyManager() )
		{
			if ( GetCompEM().IsSwitchedOn() )
			{
				GetCompEM().SwitchOff();
				return true;
			}
		}
		return false;
	}

	bool Expansion_TryTurningOnAnyLightsOrNVG(out float nightVisibility, PlayerBase player, bool skipNonNVG = false, bool skipNVG = false)
	{
		if ( !skipNVG && !player.IsUnconscious() && !player.IsRestrained() )
		{
			NVGoggles goggles = NVGoggles.Cast(FindAttachmentBySlotName("NVG"));
			if (goggles)
			{
				goggles.RotateGoggles(false);
				nightVisibility = 0.35;
				EXTrace.Print(EXTrace.AI, player, "switched on " + goggles.ToString());
				return true;
			}
		}
		
		ActionManagerBase actionManager = player.GetActionManager();
		ActionTarget atrg = new ActionTarget(this, null, -1, vector.Zero, -1);

		if ( !skipNonNVG )
		{
			if ( actionManager.GetAction(ActionTurnOnHelmetFlashlight).Can(player, atrg, this) )
			{
				ItemBase itemChild = ItemBase.Cast(FindAttachmentBySlotName("helmetFlashlight"));
				if ( itemChild.Expansion_TryTurningOn() )
				{
					nightVisibility = 0.15;
					EXTrace.Print(EXTrace.AI, player, "switched on " + itemChild.ToString());
					return true;
				}
			}

			if ( IsInherited(Headtorch_ColorBase) && actionManager.GetAction(ActionTurnOnHeadtorch).Can(player, atrg, this) )
			{
				if ( Expansion_TryTurningOn() )
				{
					nightVisibility = 0.15;
					EXTrace.Print(EXTrace.AI, player, "switched on head torch");
					return true;
				}
			}
			else if ( actionManager.GetAction(ActionTurnOnWhileInHands).Can(player, atrg, this) )
			{
				if ( Expansion_TryTurningOn() )
				{
					nightVisibility = 0.15;
					EXTrace.Print(EXTrace.AI, player, "switched on " + ToString());
					return true;
				}
			}
		}

		return false;
	}

	bool Expansion_TryTurningOffAnyLightsOrNVG(PlayerBase player, bool skipNVG = false)
	{
		if ( !skipNVG && !player.IsUnconscious() && !player.IsRestrained() )
		{
			NVGoggles goggles = NVGoggles.Cast(FindAttachmentBySlotName("NVG"));
			if (goggles)
			{
				goggles.RotateGoggles(true);
				EXTrace.Print(EXTrace.AI, player, "switched off " + goggles.ToString());
				return true;
			}
		}

		ActionManagerBase actionManager = player.GetActionManager();
		ActionTarget atrg = new ActionTarget(this, null, -1, vector.Zero, -1);

		if ( actionManager.GetAction(ActionTurnOffHelmetFlashlight).Can(player, atrg, this) )
		{
			ItemBase itemChild = ItemBase.Cast(FindAttachmentBySlotName("helmetFlashlight"));
			if ( itemChild.Expansion_TryTurningOff() )
			{
				EXTrace.Print(EXTrace.AI, player, "switched off " + itemChild.ToString());
				return true;
			}
		}

		if ( IsInherited(Headtorch_ColorBase) && actionManager.GetAction(ActionTurnOffHeadtorch).Can(player, atrg, this) )
		{
			if ( Expansion_TryTurningOff() )
			{
				EXTrace.Print(EXTrace.AI, player, "switched off head torch");
				return true;
			}
		}
		else if ( actionManager.GetAction(ActionTurnOffWhileInHands).Can(player, atrg, this) )
		{
			if ( Expansion_TryTurningOff() )
			{
				EXTrace.Print(EXTrace.AI, player, "switched off " + ToString());
				return true;
			}
		}

		return false;
	}
};
