modded class ItemBase
{
	private autoptr eAITargetInformation m_TargetInformation;

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

		return new eAIEntityTargetInformation(this);
	}

	eAITargetInformation GetTargetInformation()
	{
#ifdef EAI_TRACE
		auto trace = CF_Trace_0(this, "GetTargetInformation");
#endif

		return m_TargetInformation;
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
			return;

		if (IsWeapon())
		{
			ai.m_Weapons.Insert(this);
			return;
		}

		if (IsMeleeWeapon())
		{
			//! basically any item has isMeleeWeapon=1 in config, but we only want actual viable melee options
			//! (axes, knifes, sickles...)
			TStringArray inventorySlot();
			ConfigGetTextArray("inventorySlot", inventorySlot);
			if (inventorySlot.Find("Shoulder") == -1 && inventorySlot.Find("Melee") == -1 && inventorySlot.Find("Knife") == -1)
			{
				if (ConfigGetString("animClass") != "Knife")
				{
					TStringArray itemInfo();
					ConfigGetTextArray("itemInfo", itemInfo);
					if (itemInfo.Find("Knife") == -1)
					{
						string suicideAnim = ConfigGetString("suicideAnim");
						if (!suicideAnim || suicideAnim != "woodaxe")
							return;
					}
				}
			}

			ai.m_MeleeWeapons.Insert(this);
			return;
		}
	}

	override void EEInventoryOut(Man oldParentMan, EntityAI diz, EntityAI newParent)
	{
		super.EEInventoryOut(oldParentMan, diz, newParent);

		eAIBase ai;
		if (!Class.CastTo(ai, oldParentMan))
			return;

		if (IsWeapon())
		{
			ai.m_Weapons.RemoveItem(this);
			return;
		}

		if (IsMeleeWeapon())
		{
			ai.m_MeleeWeapons.RemoveItem(this);
			return;
		}
	}
};
