class eAITarget
{
	eAIGroup group;
	int found_at_time;
	int max_time;
	autoptr set<eAIBase> ai_list;
	eAITargetInformation info;

	void eAITarget(eAIGroup _group, int _found_at_time, int _max_time, eAITargetInformation _info)
	{
		group = _group;
		found_at_time = _found_at_time;
		if (_max_time != -2147483647 /*int.MIN*/)
			max_time = _max_time;
		else
			max_time = 10000;

		ai_list = new set<eAIBase>();
		info = _info;
	}

	bool AddAI(eAIBase ai)
	{
		if (ai_list.Find(ai) != -1)
			return false;

		ai_list.Insert(ai);
		return true;
	}

	bool RemoveAI(eAIBase ai)
	{
		int idx = ai_list.Find(ai);
		if (idx == -1)
			return false;

		ai_list.Remove(idx);

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

	vector GetPosition(eAIBase ai = null)
	{
		return info.GetPosition(ai);
	}

	vector GetAimOffset(eAIBase ai = null)
	{
		return info.GetAimOffset(ai);
	}

	float GetThreat(eAIBase ai = null)
	{
		return info.GetThreat(ai);
	}

	bool IsMeleeViable(eAIBase ai)
	{
		float dist = GetDistance(ai);
		if (dist > 2.0)
			return false;

		if (dist > 1.0 && !ai.CanConsumeStamina(EStaminaConsumers.MELEE_HEAVY))
			return false;

		//! We don't punch the bear if we have a firearm with ammo
		if (GetEntity().IsInherited(Animal_UrsusArctos))
		{
			Weapon weapon = Weapon.Cast(ai.GetItemInHands());
			if (!weapon)
				return true;
			int mi = weapon.GetCurrentMuzzle();
			if (!weapon.IsChamberEmpty(mi))
				return false;
			Magazine mag = weapon.GetMagazine(mi);
			if (mag && mag.GetAmmoCount() > 0)
				return false;
		}
		else
		{
			auto player = PlayerBase.Cast(GetEntity());
			if (player && player.IsUnconscious())
				return false;
		}

		return true;
	}

	float ShouldRemove(eAIBase ai = null)
	{
		return info.ShouldRemove(ai);
	}

	float GetDistance(eAIBase ai)
	{
		return info.GetDistance(ai);
	}

	float GetDistanceSq(eAIBase ai)
	{
		return info.GetDistanceSq(ai);
	}
};
