class eAITarget: eAITargetInformationState
{
	eAIGroup m_Group;
	int m_GroupID;
	int m_FoundAtTime;
	int m_MaxTime;

	bool m_IsTracked;

#ifdef DIAG_DEVELOPER
	void ~eAITarget()
	{
		if (!g_Game)
			return;

		if (m_Info)
			EXTrace.Print(EXTrace.AI, this, "~eAITarget() " + m_Info.GetDebugName());
		else
			EXTrace.Print(EXTrace.AI, this, "~eAITarget()");
	}
#endif

	/**
	 * @brief Start tracking this target
	 * 
	 * @param maxTime time to track this target for
	 */
	void Track(int maxTime = -1)
	{
		if (m_IsTracked)
		{
			EXError.Warn(this, "Already tracked");
			return;
		}

		m_IsTracked = true;

		m_FoundAtTime = g_Game.GetTime();

		if (maxTime != -1)
			m_MaxTime = maxTime;
		else
			m_MaxTime = 120000;

		m_Group = m_AI.GetGroup();
		m_GroupID = m_Group.GetID();

		int count;

		if (!m_Info.m_Groups.Find(m_GroupID, count))
			m_Group.OnTargetTrackingStart(m_Info);

		++count;

		if (count > m_Group.Count())
			EXError.Error(this, string.Format("Number of AI tracking %1 (%2) can't be above number of AI in group ID %3 (%4)", GetDebugName(), count, m_GroupID, m_Group.Count()));
		else
			m_Info.m_Groups[m_GroupID] = count;

		m_AI.eAI_AddTarget(this);
	}

	int GetRemainingTime()
	{
		return m_MaxTime - (g_Game.GetTime() - m_FoundAtTime);
	}

	/**
	 * @brief Debugging information about the target
	 *
	 * @return string
	 */
	override string GetDebugName()
	{
		string str = ToString();

		str += ", ";
		str += "ai=" + m_AI.GetDebugName();

		str += ", ";
		str += "info=" + m_Info.GetDebugName();

		return str;
	}

	bool IsActive()
	{
		return m_Info.IsActive();
	}

	//! Players only
	bool IsUnconscious()
	{
		return m_Info.IsUnconscious();
	}

	//! Players only
	bool IsRaised()
	{
		return m_Info.IsRaised();
	}

	//! Creatures/Players
	bool IsFighting()
	{
		return m_Info.IsFighting();
	}

	bool IsAcuteDanger()
	{
		return m_Info.IsAcuteDanger(m_AI);
	}

	bool IsLit()
	{
		return m_Info.IsLit();
	}

	float GetAttackCooldown()
	{
		return m_Info.GetAttackCooldown();
	}

	void Update(int maxTime = -1)
	{
		UpdateFoundAtTime();

		if (maxTime != -1)
			m_MaxTime = maxTime;
	}

	void Remove()
	{
		m_Info.RemoveAI(m_AI);
	}

	void UpdateFoundAtTime()
	{
		m_FoundAtTime = g_Game.GetTime();
	}

	bool HasInfo()
	{
		return m_Info != null;
	}

	bool IsCreature()
	{
		return m_Info.IsCreature();
	}

	bool IsDoor()
	{
		return m_Info.IsDoor();
	}

	bool IsEntity()
	{
		return m_Info.IsEntity();
	}

	bool IsExplosive()
	{
		return m_Info.IsExplosive();
	}

	bool IsGroup()
	{
		return m_Info.IsGroup();
	}

	bool IsHazard()
	{
		return m_Info.IsHazard();
	}

	bool IsInanimate()
	{
		return m_Info.IsInanimate();
	}

	bool IsItem()
	{
		return m_Info.IsItem();
	}

	bool IsMechanicalTrap()
	{
		return m_Info.IsMechanicalTrap();
	}

	bool IsNoise()
	{
		return m_Info.IsNoise();
	}

	bool IsPlayer()
	{
		return m_Info.IsPlayer();
	}

	bool IsVehicle()
	{
		return m_Info.IsVehicle();
	}

	bool IsZombie()
	{
		return m_Info.IsZombie();
	}

	EntityAI GetEntity()
	{
		return m_Info.GetEntity();
	}

	IEntity GetParent()
	{
		return m_Info.GetParent();
	}

	float GetLifetime()
	{
		return m_Info.GetLifetime();
	}

	vector GetPosition(bool actual = false)
	{
		return m_Info.GetPosition(m_AI, actual, this);
	}

	vector GetAimOffset()
	{
		return m_Info.GetAimOffset(m_AI);
	}

	float GetThreat()
	{
		return m_Info.GetThreat(m_AI, this);
	}

	bool CanPutInCargo(EntityAI parent)
	{
		return m_Info.CanPutInCargo(parent);
	}

	bool CanMeleeIfClose()
	{
		bool canYeet;
		if (m_AI.GetGroup().GetFaction().GetMeleeYeetForce() >= 1.0)
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

	bool IsMeleeViable()
	{
		if (!CanMeleeIfClose())
			return false;

		float distSq = GetDistanceSq(true);

		float range = m_AI.m_eMeleeCombat.eAI_GetRange();
		if (distSq > range * range)
			return false;

		if (IsUnconscious())
			return false;

		EntityAI entityInHands = m_AI.GetHumanInventory().GetEntityInHands();

		Weapon_Base weapon;
		if (Class.CastTo(weapon, entityInHands))
		{
			if (!m_AI.CanConsumeStamina(EStaminaConsumers.MELEE_HEAVY))
				return false;

			//! If we have a chambered firearm that doesn't shoot explosive ammo...
			if (weapon.Expansion_IsChambered() && !weapon.ShootsExplosiveAmmo())
			{
				EntityAI tgtEntity = GetEntity();

				//! ...don't melee if fighting a bear
				if (tgtEntity.IsInherited(Animal_UrsusArctos))
					return false;

				//! ...if avg dmg per shot is above or equal target health or weapon has mag w/ ammo...
				if (weapon.Expansion_GetAvgDmgPerShot() >= tgtEntity.GetHealth() || weapon.Expansion_GetMagazineAmmoCount() > 0)
				{
					//! ...don't melee if fighting multiple zombies/wolves or last melee was less than three seconds ago
					if (m_AI.m_eAI_AcuteDangerTargetCount > 1 || g_Game.GetTime() - m_AI.m_eAI_MeleeTime < 3000)
						return false;
				}
			}
		}
		else if (entityInHands && !entityInHands.IsMeleeWeapon())
		{
			return false;
		}

		return true;
	}

	bool ShouldRemove()
	{
		return !m_Info.IsActive() || (m_FoundAtTime + m_MaxTime <= g_Game.GetTime() && m_Info.ShouldRemove(m_AI));
	}

	bool ShouldAvoid(float distance = 0.0)
	{
		return m_Info.ShouldAvoid(m_AI, distance);
	}

	float GetMinDistance(float distance = 0.0)
	{
		return m_Info.GetMinDistance(m_AI, distance);
	}

	float GetMinDistanceSq(float distance = 0.0)
	{
		return m_Info.GetMinDistanceSq(m_AI, distance);
	}

	vector GetDirection(bool actual = false)
	{
		return m_Info.GetDirection(m_AI, actual, this);
	}

	float GetDistance(bool actual = false)
	{
		return m_Info.GetDistance(m_AI, actual, this);
	}

	float GetDistanceSq(bool actual = false)
	{
		return m_Info.GetDistanceSq(m_AI, actual, this);
	}
};
