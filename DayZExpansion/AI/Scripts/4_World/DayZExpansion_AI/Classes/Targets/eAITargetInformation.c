/**
 * @class eAITargetInformation
 *
 * @brief Is an abstract class
 */
class eAITargetInformation
{
	// in most circumstances an entity should only be in 1 group
	ref map<int, int> m_Groups = new map<int, int>;  //! Contains all groups actively tracking this target
	ref map<eAIBase, ref eAITarget> m_Targets = new map<eAIBase, ref eAITarget>;  //! Contains all AI that know about this target (not necessarily actively tracking it)

	float m_MinDistance;

	void ~eAITargetInformation()
	{
		if (!g_Game)
			return;

#ifdef EXTRACE_DIAG
		auto trace = EXTrace.Start(EXTrace.AI, this);
#endif

		RemoveFromAll();
	}
	
	void DeferredInit();

	override string GetDebugName()
	{
		return ToString();
	}

	/**
	 * @brief Get the entity that this target belongs to, if it does so
	 *
	 * @return EntityAI
	 */
	EntityAI GetEntity()
	{
#ifdef EAI_TRACE
		auto trace = CF_Trace_0(this, "GetEntity");
#endif

		return null;
	}

	IEntity GetParent()
	{
		return null;
	}

	string GetEntityDebugName()
	{
		return "";
	}

	float GetLifetime()
	{
		return 0.0;
	}

	bool CanPutInCargo(EntityAI parent)
	{
		return false;
	}

	bool IsCreature()
	{
		return false;
	}

	bool IsDoor()
	{
		return false;
	}

	bool IsEntity()
	{
		return false;
	}

	bool IsExplosive()
	{
		return false;
	}

	bool IsGroup()
	{
		return false;
	}

	bool IsHazard()
	{
		return false;
	}

	bool IsInanimate()
	{
		return false;
	}

	bool IsItem()
	{
		return false;
	}

	bool IsMechanicalTrap()
	{
		return false;
	}

	bool IsNoise()
	{
		return false;
	}

	bool IsPlayer()
	{
		return false;
	}

	bool IsVehicle()
	{
		return false;
	}

	bool IsZombie()
	{
		return false;
	}

	/**
	 * @brief Abstract function. If the target is active or not.
	 *
	 * @return bool
	 */
	bool IsActive()
	{
#ifdef EAI_TRACE
		auto trace = CF_Trace_0(this, "IsActive");
#endif

		return true;
	}

	bool IsUnconscious()
	{
		return false;
	}

	bool IsCrawling()
	{
		return false;
	}

	//! Players only
	bool IsRaised()
	{
		return false;
	}

	//! Creatures/Players
	bool IsFighting()
	{
		return false;
	}

	bool IsAcuteDanger(eAIBase ai = null)
	{
		return false;
	}

	bool IsLit()
	{
		return false;
	}

	float GetAttackCooldown()
	{
		return 0;
	}

	/**
	 * @brief Abstract function. Get the position for the AI within the target. Each AI could have their own position for the target.
	 *
	 * @param ai null default, gets the position for the AI if specified, otherwise returns a default value
	 * @return EntityAI
	 */
	vector GetPosition(eAIBase ai = null, bool actual = false, eAITargetInformationState state = null)
	{
#ifdef EAI_TRACE
		auto trace = CF_Trace_1(this, "GetPosition").Add(ai);
#endif

		return "0 0 0";
	}

	/**
	 * @brief Abstract function. Get the aim offset for the AI within the target. Each AI could have their own offset for the target.
	 *
	 * @param ai null default, gets the aim offset for the AI if specified, otherwise returns a default value
	 * @return EntityAI
	 */
	vector GetAimOffset(eAIBase ai = null)
	{
#ifdef EAI_TRACE
		auto trace = CF_Trace_1(this, "GetAimOffset").Add(ai);
#endif

		return "0 0 0";
	}

	/**
	 * @brief Abstract function. Get the threat level for the AI within the target. Each AI could have their own threat level for the target.
	 *
	 * @param ai null default, if given includes the AI in threat calculation
	 * @return int
	 */
	float GetThreat(eAIBase ai = null, eAITargetInformationState state = null)
	{
#ifdef EAI_TRACE
		auto trace = CF_Trace_1(this, "GetThreat").Add(ai);
#endif

		if (!ai)
			return CalculateThreat();

		bool created;

		if (!state)
			state = GetTargetForAIEx(ai, true, created);

		if (!created)
			state.UpdateThreat();

		return state.m_ThreatLevel;
	}

	float GetCachedThreat(eAIBase ai, bool ignoreLOS = false)
	{
		eAITargetInformationState state = GetTargetForAI(ai);

		if (state)
			return state.GetCachedThreat(ignoreLOS);

		return 0.0;
	}

	float CalculateThreat(eAIBase ai = null, eAITargetInformationState state = null)
	{
		return 0.0;
	}

	bool ShouldRemove(eAIBase ai = null)
	{
#ifdef EAI_TRACE
		auto trace = CF_Trace_1(this, "ShouldRemove").Add(ai);
#endif

		return 0;
	}

	bool ShouldAvoid(eAIBase ai = null, float distance = 0.0)
	{
		return false;
	}

	float GetMinDistance(eAIBase ai = null, float distance = 0.0)
	{
		return m_MinDistance;
	}

	float GetMinDistanceSq(eAIBase ai = null, float distance = 0.0)
	{
		float minDist = GetMinDistance(ai, distance);
		return minDist * minDist;
	}

	vector GetDirection(eAIBase ai, bool actual = false, eAITargetInformationState state = null)
	{
		return vector.Zero;
	}

	/**
	 * @brief Abstract function. Get the distance to the target.
	 *
	 * @param ai
	 * @return float
	 */
	float GetDistance(eAIBase ai, bool actual = false, eAITargetInformationState state = null)
	{
#ifdef EAI_TRACE
		auto trace = CF_Trace_1(this, "GetDistance").Add(ai);
#endif

		return 0;
	}

	float GetDistanceSq(eAIBase ai, bool actual = false, eAITargetInformationState state = null)
	{
#ifdef EAI_TRACE
		auto trace = CF_Trace_1(this, "GetDistanceSq").Add(ai);
#endif

		return 0;
	}

	/**
	 * @brief Tells the target that the AI knows about it (not actively tracking)
	 *
	 * @param ai eAIBase object
	 * @param initialUpdate whether to update threat level and position on target creation
	 */
	eAITarget InsertAI(eAIBase ai, bool initialUpdate = true)
	{
#ifdef EXTRACE_DIAG
		auto trace = EXTrace.Start(EXTrace.AI, this, "" + ai, "" + initialUpdate);
#endif

		eAITarget target = new eAITarget(ai, this, initialUpdate);

		m_Targets.Insert(ai, target);

		ai.m_eAI_TargetInformationStates.Insert(this, target);

		return target;
	}

	/**
	 * @brief Tells the target that the AI is tracking it and returns a target object for the AI (creates if it doesn't exist)
	 *
	 * @param ai eAIBase object
	 * @param maxTime time the eAIBase will be tracking this target for, if -1 then the time isn't updated
	 * @param initialUpdate whether to update threat level and position on target creation
	 */
	eAITarget AddAI(eAIBase ai, int maxTime = -1, bool initialUpdate = true, out bool created = false)
	{
		eAITarget target;

		if (!m_Targets.Find(ai, target))
		{
			target = InsertAI(ai, initialUpdate);
			created = true;
		}

		if (!target.m_IsTracked)
			target.Track(maxTime);
		else if (maxTime != -1)
			target.m_MaxTime = maxTime;

		return target;
	}

	/**
	 * @brief return target state for AI if it exists
	 */
	eAITarget GetTargetForAI(eAIBase ai)
	{
		return m_Targets[ai];
	}

	/**
	 * @brief return target state for AI (create if it doesn't exist)
	 */
	eAITarget GetTargetForAIEx(eAIBase ai, bool initialUpdate = true, out bool created = false)
	{
		eAITarget target;

		if (!m_Targets.Find(ai, target))
		{
			target = InsertAI(ai, initialUpdate);
			created = true;
		}

		return target;
	}

	/**
	 * @brief Add/update target for all AI friendly to player
	 * 
	 * @param player
	 * @param update If true (default) and AI is already targeting the target, update foundAtTime and maxTime
	 * @param threat Initial threat level if non-zero
	 */
	void AddFriendlyAI(DayZPlayerImplement player, int maxTime = -1, bool update = true, float threat = 0.0)
	{
#ifdef EXTRACE_DIAG
		auto trace = EXTrace.Start(EXTrace.AI, this, "" + player, "" + maxTime, "" + update, "" + threat);
#endif

		eAIBase ai;
		if (Class.CastTo(ai, player))
			GetTargetForAIEx(ai, false);

		eAIGroup group = player.GetGroup();
		eAIFaction faction = group.GetFaction();
		eAIGroup otherGroup;

		foreach (eAIBase other, eAITarget target: m_Targets)
		{
		#ifdef DIAG_DEVELOPER
			EXTrace.Print(EXTrace.AI, this, "" + other);
		#endif

			if (!other)
				continue;

			if (other != player)
			{
				otherGroup = other.GetGroup();

				if (otherGroup != group && !otherGroup.GetFaction().IsFriendly(faction))
					continue;
			}

			if (!target.m_IsTracked)
				target.Track(maxTime);
			else if (update)
				target.Update(maxTime);

			if (threat > target.m_ThreatLevelActive)
			{
				if (!target.m_LOS)
				{
					target.SetInitial(threat, player.GetPosition());  //! We deliberately don't use attacker position but victim position
					target.m_SearchOnLOSLost = true;
				}
				else
				{
					target.SetThreat(threat);
				}
			}
		}
	}

	/**
	 * @brief Tells the target that the AI is no longer tracking it
	 *
	 * @param ai eAIBase object
	 */
	bool RemoveAI(eAIBase ai)
	{
#ifdef EXTRACE_DIAG
		auto trace = EXTrace.Start(EXTrace.AI, this, "" + ai);
#endif

		eAITarget target;
		if (!m_Targets.Find(ai, target))
			return false;

		OnRemove(ai, target);
		m_Targets.Remove(ai);

		return true;
	}

	void OnRemove(eAIBase ai, eAITarget target)
	{
		if (ai)
			ai.eAI_RemoveTarget(target);

		int groupID = target.m_GroupID;

		eAIGroup group = target.m_Group;
		if (!group)
		{
			m_Groups.Remove(target.m_GroupID);
			return;
		}

		int count;

		if (m_Groups.Find(groupID, count))
		{
			--count;

			if (count == 0)
			{
				group.OnTargetTrackingEnd(this);
				m_Groups.Remove(groupID);
			}
			else if (count < 0)
			{
				EXError.Error(this, string.Format("Number of AI tracking %1 can't be below zero", GetDebugName()));
			}
			else
			{
				m_Groups[groupID] = count;
			}
		}
	}

	void RemoveFromAll()
	{
#ifdef EXTRACE_DIAG
		auto trace = EXTrace.Start(EXTrace.AI, this, GetEntityDebugName(), "" + m_Targets.Count());
#endif

		foreach (eAIBase ai, eAITarget target: m_Targets)
		{
			if (target.m_IsTracked)
				OnRemove(ai, target);
			else if (ai)
				ai.eAI_RemoveTargetInfoState(this);
		}

		m_Groups.Clear();
		m_Targets.Clear();
	}

	/**
	 * @brief Checks to see if any group/ai is currently tracking this target
	 *
	 * @return bool true if being tracked, false otherwise
	 */
	bool IsTracked()
	{
		return m_Groups.Count() > 0;
	}

	/**
	 * @brief Checks to see if the group specified is currently tracking this target
	 *
	 * @param group_id the group id of the eAIGroup
	 * @return bool true if being tracked, false otherwise
	 */
	bool IsTrackedBy(int group_id)
	{
		return m_Groups.Contains(group_id);
	}

	/**
	 * @brief Checks to see if the group specified is currently tracking this target
	 *
	 * @param group the eAIGroup
	 * @return bool true if being tracked, false otherwise
	 */
	bool IsTrackedBy(notnull eAIGroup group)
	{
		return m_Groups.Contains(group.GetID());
	}

	/**
	 * @brief Checks to see if the group specified is currently tracking this target
	 *
	 * @param group_id the group id of the eAIGroup
	 * @param [out] num_ai number of AI tracking this target
	 * @return bool true if being tracked, false otherwise
	 */
	bool IsTrackedBy(int group_id, out int num_ai)
	{
		if (!m_Groups.Find(group_id, num_ai))
			return false;

		return true;
	}

	/**
	 * @brief Checks to see if the group specified is currently tracking this target
	 *
	 * @param group the eAIGroup
	 * @return bool true if being tracked, false otherwise
	 */
	bool IsTrackedBy(notnull eAIGroup group, out int num_ai)
	{
		if (!m_Groups.Find(group.GetID(), num_ai))
			return false;

		return true;
	}

	/**
	 * @brief Checks to see if the AI specified is currently tracking this target
	 *
	 * @param group the eAIGroup
	 * @return bool true if being tracked, false otherwise
	 */
	bool IsTrackedBy(eAIBase ai)
	{
		eAITarget target;
		if (!m_Targets.Find(ai, target))
			return false;

		return target.m_IsTracked;
	}

	//! entity specific implementations for abstracted call in eAIEntityTargetInformation
	void OnDeath(Object killer)
	{
#ifdef EXTRACE_DIAG
		auto trace = EXTrace.Start(EXTrace.AI, this);
#endif

		RemoveFromAll();
	}

	void OnHit(TotalDamageResult damageResult, int damageType, EntityAI source, int component, string dmgZone, string ammo, vector modelPos, float speedCoef)
	{
	}

	void OnHealthLevelChanged(int oldLevel, int newLevel, string zone)
	{
	}

	// this is really bad but unfortunately we can't reasonably mod the Object class
	// and for some circumstances using templates just won't work.
	//
	// wherever possible, please use
	//	'eAIEntity<DayZPlayerImplement>.GetTargetInformation(g_Game.GetPlayer())`
	static eAITargetInformation GetTargetInformation(Object entity)
	{
		DayZPlayerImplement player;
		if (Class.CastTo(player, entity))
			return player.GetTargetInformation();

		ZombieBase zombie;
		if (Class.CastTo(zombie, entity))
			return zombie.GetTargetInformation();

		AnimalBase animal;
		if (Class.CastTo(animal, entity))
			return animal.GetTargetInformation();

		ItemBase item;
		if (Class.CastTo(item, entity))
			return item.GetTargetInformation();

		CarScript car;
		if (Class.CastTo(car, entity))
			return car.GetTargetInformation();

		return null;
	}
};
