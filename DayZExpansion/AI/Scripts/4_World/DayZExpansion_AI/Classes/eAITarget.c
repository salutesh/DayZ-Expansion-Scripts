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
		if (_max_time != -1)
			max_time = _max_time;
		else
			max_time = 60000;

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

		ai.OnRemoveTarget(this);

		if (CountAI() == 0)
		{
			group.OnTargetRemoved(info);

			info.m_Groups.Remove(ai.GetGroup().GetID());
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

	float GetThreat(eAIBase ai = null)
	{
		return info.GetThreat(ai);
	}

	bool IsMeleeViable(eAIBase ai)
	{
		if (GetEntity().IsInherited(ItemBase))
			return false;

		EntityAI entityInHands = ai.GetHumanInventory().GetEntityInHands();

		//! Can't hit low targets if no weapon
		if (!entityInHands && info.GetAimOffset(ai)[1] < 1.0)
			return false;

		float distSq = GetDistanceSq(ai, true);
		if (distSq > 3.24)
			return false;

		//! DayZPlayerMeleeFightLogic_LightHeavy.CLOSE_TARGET_DISTANCE = 1.5
		if (distSq > 2.25 && !ai.CanConsumeStamina(EStaminaConsumers.MELEE_HEAVY))
			return false;

		Weapon_Base weapon;
		bool hasAmmo = Class.CastTo(weapon, entityInHands) && weapon.Expansion_HasAmmo();
		if (distSq > 1.0 && hasAmmo)
			return false;

		//! We don't punch the bear if we have a firearm with ammo
		if (GetEntity().IsInherited(Animal_UrsusArctos))
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

	float GetDistance(eAIBase ai, bool actual = false)
	{
		return info.GetDistance(ai, actual);
	}

	float GetDistanceSq(eAIBase ai, bool actual = false)
	{
		return info.GetDistanceSq(ai, actual);
	}
};
