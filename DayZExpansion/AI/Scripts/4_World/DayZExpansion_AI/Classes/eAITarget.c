class eAITarget
{
	eAIGroup group;
	int groupID;
	int found_at_time;
	int max_time;
	autoptr set<eAIBase> ai_list;
	eAITargetInformation info;

	void eAITarget(eAIGroup _group, int _found_at_time, int _max_time, eAITargetInformation _info)
	{
		group = _group;
		groupID = group.GetID();
		found_at_time = _found_at_time;
		if (_max_time != -1)
			max_time = _max_time;
		else
			max_time = 120000;

		ai_list = new set<eAIBase>();
		info = _info;
	}

#ifdef DIAG_DEVELOPER
	void ~eAITarget()
	{
		if (!GetGame())
			return;

		if (info)
			EXTrace.Print(EXTrace.AI, this, "~eAITarget() " + info.GetDebugName());
		else
			EXTrace.Print(EXTrace.AI, this, "~eAITarget()");
	}
#endif

	/**
	 * @brief Debugging information about the target
	 *
	 * @return string
	 */
	override string GetDebugName()
	{
		string str = ToString();

		str += ", ";
		str += "group=" + group.GetDebugName();

		str += ", ";
		str += "info=" + info.GetDebugName();

		return str;
	}

	bool IsActive()
	{
		return info.IsActive();
	}

	bool AddAI(eAIBase ai)
	{
		if (ai_list.Find(ai) != -1)
			return false;

#ifdef EXTRACE_DIAG
		auto trace = EXTrace.Start(EXTrace.AI, this, "" + ai);
#endif

		ai_list.Insert(ai);
		return true;
	}

	void Update(int _max_time = -1)
	{
		found_at_time = GetGame().GetTime();
		if (_max_time != -1)
			max_time = _max_time;
	}

	bool RemoveAI(eAIBase ai)
	{
		int idx = ai_list.Find(ai);

#ifdef EXTRACE_DIAG
		auto trace = EXTrace.Start(EXTrace.AI, this, "" + ai, "" + idx);
#endif

		if (idx == -1)
			return false;

		ai_list.Remove(idx);

		ai.OnRemoveTarget(this);

		if (CountAI() == 0)
		{
			if (group)
				group.OnTargetRemoved(info);

			info.m_Groups.Remove(groupID);
		}

		return true;
	}

	int FindAI(eAIBase ai)
	{
		return ai_list.Find(ai);
	}

	int CountAI()
	{
		return ai_list.Count();
	}

	bool HasInfo()
	{
		return info != null;
	}

	bool IsEntity()
	{
		return info.IsEntity();
	}

	EntityAI GetEntity()
	{
		return info.GetEntity();
	}

	bool IsExplosive()
	{
		return info.IsExplosive();
	}

	bool IsMechanicalTrap()
	{
		return info.IsMechanicalTrap();
	}

	vector GetPosition(eAIBase ai = null, bool actual = false)
	{
		return info.GetPosition(ai, actual);
	}

	vector GetAimOffset(eAIBase ai = null)
	{
		return info.GetAimOffset(ai);
	}

	float GetThreat(eAIBase ai = null, out eAITargetInformationState state = null)
	{
		return info.GetThreat(ai, state);
	}

	bool CanPutInCargo(EntityAI parent)
	{
		return info.CanPutInCargo(parent);
	}

	bool CanMeleeIfClose(eAIBase ai)
	{
		bool canYeet;
		if (ai.GetGroup().GetFaction().GetMeleeYeetForce() >= 1.0)
			canYeet = true;

		EntityAI entity = GetEntity();
		if (!entity || (entity.IsItemBase() && (!IsMechanicalTrap() || IsExplosive())) || (entity.IsTransport() && !canYeet))
			return false;

		DayZPlayerImplement player;
		if (!Class.CastTo(player, entity))
			return true;

		IEntity parent = player.Expansion_GetParent();
		Car car;
		//! Allow hitting vehicle passengers as long as engine isn't on or speed is below RegisterTransportHit tolerance 2 km/h = 0.555555 m/s
		if (parent && (GetVelocity(parent).LengthSq() > 0.308641 || (Class.CastTo(car, parent) && car.EngineIsOn())))
			return false;

		return true;
	}

	bool IsMeleeViable(eAIBase ai)
	{
		if (!CanMeleeIfClose(ai))
			return false;

		float distSq = GetDistanceSq(ai, true);

		if (distSq > 6.0025)
			return false;

		//if (distSq > 3.0625 && !ai.CanConsumeStamina(EStaminaConsumers.MELEE_HEAVY))
			//return false;

		EntityAI entityInHands = ai.GetHumanInventory().GetEntityInHands();

		Weapon_Base weapon;
		bool hasAmmo;
		if (Class.CastTo(weapon, entityInHands))
		{
			//! FIXME: Pistols fuck up the hand animation state (left hand stays on pistol after melee)
			if (weapon.IsKindOf("Pistol_Base"))
				return false;

			if (weapon.Expansion_GetMagazineAmmoCount() > 0)
			{
				hasAmmo = true;

				if (distSq > 1.0)
					return false;
			}

			if (weapon && !ai.CanConsumeStamina(EStaminaConsumers.MELEE_HEAVY))
				return false;
		}

		//! We don't punch the bear or the zombie if we have a firearm with ammo - unless it's explosive ammo
		if (GetEntity().IsInherited(Animal_UrsusArctos) || GetEntity().IsInherited(ZombieBase))
		{
			if (hasAmmo && !weapon.ShootsExplosiveAmmo())
				return false;
		}
		else
		{
			PlayerBase player;
			if (Class.CastTo(player, GetEntity()) && player.IsUnconscious())
				return false;
		}

		return true;
	}

	bool ShouldRemove(eAIBase ai = null)
	{
		return !info.IsActive() || (found_at_time + max_time <= GetGame().GetTime() && info.ShouldRemove(ai));
	}

	bool ShouldAvoid(eAIBase ai = null, float distance = 0.0)
	{
		return info.ShouldAvoid(ai, distance);
	}

	float GetMinDistance(eAIBase ai = null, float distance = 0.0)
	{
		return info.GetMinDistance(ai, distance);
	}

	float GetMinDistanceSq(eAIBase ai = null, float distance = 0.0)
	{
		return info.GetMinDistanceSq(ai, distance);
	}

	vector GetDirection(eAIBase ai, bool actual = false)
	{
		return info.GetDirection(ai, actual);
	}

	float GetDistance(eAIBase ai, bool actual = false)
	{
		return info.GetDistance(ai, actual);
	}

	float GetDistanceSq(eAIBase ai, bool actual = false)
	{
		return info.GetDistanceSq(ai, actual);
	}
};
