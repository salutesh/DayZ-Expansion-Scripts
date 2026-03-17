modded class ItemBase
{
	private ref eAIItemTargetInformation m_TargetInformation;
	ref eAIDamageHandler m_eAI_DamageHandler;

	bool m_Expansion_IsOwnerPlayer;
	float m_eAI_LastRecordedImpactTime;

	void ItemBase()
	{
		m_TargetInformation = CreateTargetInformation();

		if (g_Game.IsServer())
			m_eAI_DamageHandler = new eAIDamageHandler(this, m_TargetInformation);
	}

	protected eAIItemTargetInformation CreateTargetInformation()
	{
		return new eAIItemTargetInformation(this);
	}

	eAIItemTargetInformation GetTargetInformation()
	{
#ifdef EAI_TRACE
		auto trace = CF_Trace_0(this, "GetTargetInformation");
#endif

		return m_TargetInformation;
	}

	override void EEKilled(Object killer)
	{
		m_TargetInformation.OnDeath(killer);

		super.EEKilled(killer);

		eAIBase ai;
		if (Class.CastTo(ai, GetHierarchyRootPlayer()))
		{
			EXTrace.Print(EXTrace.AI, this, "::EEKilled - " + ai);
			ai.eAI_OnItemDestroyed(this);
		}
	}

	override bool EEOnDamageCalculated(TotalDamageResult damageResult, int damageType, EntityAI source, int component, string dmgZone, string ammo, vector modelPos, float speedCoef)
	{
		if (!super.EEOnDamageCalculated(damageResult, damageType, source, component, dmgZone, ammo, modelPos, speedCoef))
			return false;

		if (!m_eAI_DamageHandler.OnDamageCalculated(damageResult, damageType, source, component, dmgZone, ammo, modelPos, speedCoef))
			return false;

		return true;
	}

	override void EEHitBy(TotalDamageResult damageResult, int damageType, EntityAI source, int component, string dmgZone, string ammo, vector modelPos, float speedCoef)
	{
	#ifdef DIAG_DEVELOPER
		EXTrace.PrintHit(EXTrace.AI, this, "EEHitBy[" + m_eAI_DamageHandler.m_HitCounter + "]", damageResult, damageType, source, component, dmgZone, ammo, modelPos, speedCoef);

	#ifdef EXPANSION_AI_DMGDEBUG_CHATTY
		ExpansionStatic.MessageNearPlayers(GetPosition(), 100.0, "[" + ExpansionStatic.FormatFloat(g_Game.GetTickTime(), 3, false) + "] hit " + ToString() + " " + dmgZone);
	#endif
	#endif

		m_TargetInformation.OnHit(damageResult, damageType, source, component, dmgZone, ammo, modelPos, speedCoef);

		super.EEHitBy(damageResult, damageType, source, component, dmgZone, ammo, modelPos, speedCoef);
	}

	override void EEHealthLevelChanged(int oldLevel, int newLevel, string zone)
	{
		m_TargetInformation.OnHealthLevelChanged(oldLevel, newLevel, zone);

		super.EEHealthLevelChanged(oldLevel, newLevel, zone);

		if (!g_Game.IsServer())
			return;

		eAIBase ai;
		if (Class.CastTo(ai, GetHierarchyRootPlayer()) && GetInventory().IsAttachment() && m_Initialized)
			ai.eAI_UpdateProtectionLevels();
	}

	override void EEItemAttached(EntityAI item, string slot_name)
	{
		super.EEItemAttached(item, slot_name);

		if (!g_Game.IsServer())
			return;

		eAIBase ai;
		if (Class.CastTo(ai, GetHierarchyParent()))
			ai.eAI_UpdateProtectionLevels(this, 1);
	}

	override void EEItemDetached(EntityAI item, string slot_name)
	{
		super.EEItemDetached(item, slot_name);

		if (!g_Game.IsServer())
			return;

		eAIBase ai;
		if (Class.CastTo(ai, GetHierarchyParent()))
			ai.eAI_UpdateProtectionLevels(this, -1);
	}

	override void EEItemLocationChanged(notnull InventoryLocation oldLoc, notnull InventoryLocation newLoc)
	{
		super.EEItemLocationChanged(oldLoc, newLoc);

		EntityAI oldParent = oldLoc.GetParent();
		EntityAI newParent = newLoc.GetParent();

		Man oldOwner;
		Man newOwner;

		if (oldParent)
			oldOwner = oldParent.GetHierarchyRootPlayer();

		if (newParent)
			newOwner = newParent.GetHierarchyRootPlayer();

		if (oldLoc.GetType() == InventoryLocationType.CARGO && oldOwner != newOwner)
			eAI_OnCargoExit(oldOwner, oldParent);

		if (newLoc.GetType() == InventoryLocationType.CARGO && oldOwner != newOwner)
			eAI_OnCargoEnter(newOwner, newParent);
	}

	void eAI_OnCargoEnter(Man owner, EntityAI parent)
	{
		if (!g_Game.IsServer())
			return;

		eAIBase ai;
		if (Class.CastTo(ai, owner))
			ai.eAI_OnCargoEnter(parent, this);
	}

	void eAI_OnCargoExit(Man owner, EntityAI parent)
	{
		if (!g_Game.IsServer())
			return;

		eAIBase ai;
		if (Class.CastTo(ai, owner))
			ai.eAI_OnCargoExit(parent, this);
	}

	override void OnInventoryEnter(Man player)
	{
		super.OnInventoryEnter(player);

		m_TargetInformation.RemoveFromAll();

		eAIBase ai;
		if (!Class.CastTo(ai, player))
		{
			m_Expansion_IsOwnerPlayer = true;
			return;
		}

		if (!g_Game.IsServer())
			return;

		EXTrace.Print(EXTrace.AI, this, "::OnInventoryEnter - " + player);

		ai.eAI_OnInventoryEnter(this);
	}

	override void OnInventoryExit(Man player)
	{
		super.OnInventoryExit(player);

		if (!g_Game.IsServer())
			return;

		eAIBase ai;
		if (!Class.CastTo(ai, player))
			return;

		EXTrace.Print(EXTrace.AI, this, "::OnInventoryExit - " + player);

		ai.eAI_OnInventoryExit(this);
	}

	override void OnWasAttached(EntityAI parent, int slot_id)
	{
		super.OnWasAttached(parent, slot_id);

		if (!g_Game.IsServer())
			return;

		eAIBase ai;
		if (!Class.CastTo(ai, parent.GetHierarchyRootPlayer()))
			return;

		//! If mag was attached to item that's in AI possession, remove from tracked list
		Magazine mag;
		if (Class.CastTo(mag, this))
		{
			EXTrace.Print(EXTrace.AI, this, "::OnWasAttached - " + ai + " " + parent + " " + InventorySlots.GetSlotName(slot_id));
			ai.eAI_OnMagAttached(parent, slot_id, mag);
		}
	}

	override void OnWasDetached(EntityAI parent, int slot_id)
	{
		super.OnWasDetached(parent, slot_id);

		if (!g_Game.IsServer())
			return;

		eAIBase ai;
		if (!Class.CastTo(ai, parent.GetHierarchyRootPlayer()))
			return;

		//! If mag was detached from item that's in AI possession, add to tracked list
		Magazine mag;
		if (Class.CastTo(mag, this))
		{
			EXTrace.Print(EXTrace.AI, this, "::OnWasDetached - " + ai + " " + parent + " " + InventorySlots.GetSlotName(slot_id));
			ai.eAI_OnMagDetached(parent, slot_id, mag);
		}
	}

	override void EOnContact(IEntity other, Contact extra)
	{
		if (m_CanPlayImpactSound && g_Game.IsServer())
		{
			float time = g_Game.GetTickTime();
			if (time - m_eAI_LastRecordedImpactTime > 1.0)
			{
				//! Because impacts may happen rapidly, we only update this once every second
				m_eAI_LastRecordedImpactTime = time;

			/*
				//! This WOULD work if all items had NoiseImpact...
				string path;

				if (IsWeapon())
					path = CFG_WEAPONSPATH;
				else if (IsMagazine())
					path = CFG_MAGAZINESPATH;
				else
					path = CFG_VEHICLESPATH;
	
				eAINoiseSystem.AddNoise(this, path + " " + GetType() + " NoiseImpact", m_ImpactSpeed, eAINoiseType.SOUND);
			*/

				eAINoiseSystem.AddNoise(this, CFG_VEHICLESPATH + " Bone NoiseImpact", 0.1, eAINoiseType.SOUND);
			}
		}
	}

	override bool AreChildrenAccessible()
	{
		eAIBase ai;
		if (Class.CastTo(ai, GetHierarchyRootPlayer()))
			return true;

		return super.AreChildrenAccessible();
	}

	override void OnEnergyConsumed()
	{
		//! Refill energy while in AI inventory, essentially disabling energy consumption
		if (eAIBase.Cast(GetHierarchyRoot()))
			GetCompEM().SetEnergy0To1(1.0);
		else
			super.OnEnergyConsumed();
	}

	override void OnSwitchOn()
	{
		super.OnSwitchOn();

		eAI_OnSwitchOn();
	}

	override void OnSwitchOff()
	{
		super.OnSwitchOff();

		eAI_OnSwitchOff();
	}

	void eAI_OnSwitchOn()
	{
		eAIBase ai = eAI_GetHierarchyRootAI();
		if (ai && eAI_GetNightVisibility() > 0)
			ai.eAI_AddActiveVisibilityEnhancer(this);
	}

	void eAI_OnSwitchOff()
	{
		eAIBase ai = eAI_GetHierarchyRootAI();
		if (ai)
			ai.eAI_RemoveActiveVisibilityEnhancer(this);
	}

	bool Expansion_TryTurningOn()
	{
		if ( HasEnergyManager() )
		{
			if ( GetCompEM().CanSwitchOn() && GetCompEM().CanWork() )
			{
				GetCompEM().SwitchOn();
				EXTrace.Print(EXTrace.AI, GetHierarchyRootPlayer(), "switched on " + ToString());
				return true;
			}
		}
		return false;
	}

	bool Expansion_TryTurningOff()
	{
		if ( HasEnergyManager() )
		{
			if ( GetCompEM().CanSwitchOff() )
			{
				GetCompEM().SwitchOff();
				EXTrace.Print(EXTrace.AI, GetHierarchyRootPlayer(), "switched off " + ToString());
				return true;
			}
		}
		return false;
	}

	bool Expansion_TryTurningOnAnyLightsOrNVG(inout float nightVisibility, PlayerBase player, bool skipNonNVG = false, bool skipNVG = false)
	{
		EXError.Error(this, "DEPRECATED, use Expansion_TryTurningOnAnyLightOrNVG(bool skipNonNVG, ...)");
		if (Expansion_TryTurningOnAnyLightOrNVG(skipNonNVG, skipNVG))
			return true;
		return false;
	}

	ItemBase Expansion_TryTurningOnAnyLightOrNVG(bool skipNonNVG = false, bool skipNVG = false)
	{
		ItemBase item = Expansion_GetAnyLightOrNVG(skipNonNVG, skipNVG);

		if (item && item.Expansion_TryTurningOn())
			return item;

		return null;
	}

	ItemBase Expansion_GetAnyLightOrNVG(bool skipNonNVG = false, bool skipNVG = false)
	{
		if (!skipNVG)
		{
			NVGoggles goggles = NVGoggles.Cast(FindAttachmentBySlotName("NVG"));
			if (goggles)
				return goggles;
		}

		if (!skipNonNVG)
		{
			ItemBase candidate;

			if (!Class.CastTo(candidate, FindAttachmentBySlotName("helmetFlashlight")))
				candidate = this;

			TTypenameArray lights = {Flashlight, Headtorch_ColorBase, TLRLight, FlammableBase, UniversalLight};
			if (ExpansionStatic.IsAnyOf(candidate, lights))
				return candidate;
		}

		return null;
	}

	bool Expansion_TryTurningOffAnyLightsOrNVG(PlayerBase player, bool skipNVG = false, bool force = false)
	{
		EXError.Error(this, "DEPRECATED, use Expansion_TryTurningOffAnyLightOrNVG(bool skipNVG)");
		if (Expansion_TryTurningOffAnyLightOrNVG(skipNVG))
			return true;
		return false;
	}

	ItemBase Expansion_TryTurningOffAnyLightOrNVG(bool skipNVG = false)
	{
		ItemBase item = Expansion_GetAnyLightOrNVG(false, skipNVG);

		if (item && item.Expansion_TryTurningOff())
			return item;

		return null;
	}

	float eAI_GetNightVisibility(bool checkWorking = true)
	{
		//! Have to deal with NV optics here since ItemOptics is not moddable...
		ItemOptics optic;
		if (Class.CastTo(optic, this) && optic.GetCurrentNVType() != NVTypes.NONE)
		{
			if (!checkWorking || !HasEnergyManager() || GetCompEM().CanWork())
				return Math.Min(optic.GetZeroingDistanceZoomMax() * 0.001, 1.0);
		}

		return 0.0;
	}

	bool Expansion_CanRepair(EntityAI entity)
	{
		int repairKitType = ConfigGetInt("repairKitType");

		if (repairKitType != 0)
		{
			TIntArray repairableWithKits = {};
			entity.ConfigGetIntArray("repairableWithKits", repairableWithKits);

			return repairableWithKits.Find(repairKitType) != -1;
		}

		return false;
	}

	Man Expansion_GetRootPlayerAliveExcluding(Man player)
	{
		Man owner = GetHierarchyRootPlayer();
		if (owner && owner != player && owner.IsAlive())
			return owner;

		return null;
	}

	float Expansion_GetHealthDamage()
	{
		return 0;
	}

	bool eAI_IsSilent()
	{
		return true;
	}

	eAIBase eAI_GetHierarchyRootAI()
	{
		return eAIBase.Cast(GetHierarchyRootPlayer());
	}

	//! @brief returns cargo size (including attachments)
	int eAI_GetCargoSize()
	{
		int cargoSize;

		CargoBase cargo = GetInventory().GetCargo();
		if (cargo)
			cargoSize += cargo.GetWidth() * cargo.GetHeight();

		for (int i = 0; i < GetInventory().AttachmentCount(); ++i)
		{
			EntityAI att = GetInventory().GetAttachmentFromIndex(i);
			cargo = att.GetInventory().GetCargo();
			if (cargo)
				cargoSize += cargo.GetWidth() * cargo.GetHeight();
		}

		return cargoSize;
	}
};
