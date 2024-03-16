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
			max_time = 60000;

		ai_list = new set<eAIBase>();
		info = _info;
	}

#ifdef DIAG
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

	bool AddAI(eAIBase ai)
	{
		if (ai_list.Find(ai) != -1)
			return false;

#ifdef DIAG
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

#ifdef DIAG
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

	EntityAI GetEntity()
	{
		return info.GetEntity();
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

	bool IsMeleeViable(eAIBase ai)
	{
		EntityAI entity = GetEntity();
		if (!entity || entity.IsInherited(ItemBase))
			return false;

		EntityAI entityInHands = ai.GetHumanInventory().GetEntityInHands();

		float distSq = GetDistanceSq(ai, true);
		if (distSq > 3.24)
			return false;

		//! DayZPlayerMeleeFightLogic_LightHeavy.CLOSE_TARGET_DISTANCE = 1.5
		if (distSq > 2.25 && !ai.CanConsumeStamina(EStaminaConsumers.MELEE_HEAVY))
			return false;

		Weapon_Base weapon;
		bool hasAmmo;
		if (Class.CastTo(weapon, entityInHands) && weapon.Expansion_HasAmmo())
			hasAmmo = true;
		if (distSq > 1.0 && hasAmmo)
			return false;

		//! We don't punch the bear or the zombie if we have a firearm with ammo
		if (GetEntity().IsInherited(Animal_UrsusArctos) || GetEntity().IsInherited(ZombieBase))
		{
			if (hasAmmo)
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

	float GetMinDistance(eAIBase ai = null)
	{
		return info.GetMinDistance(ai);
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
