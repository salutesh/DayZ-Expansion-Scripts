/**
 * @class eAITargetInformation
 *
 * @brief Is an abstract class
 */
class eAITargetInformation
{
	// in most circumstances an entity should only be in 1 group
	private autoptr map<int, ref eAITarget> m_Groups;

	void eAITargetInformation()
	{
#ifdef EAI_TRACE
		auto trace = CF_Trace_0(this, "eAITargetInformation");
#endif

		m_Groups = new map<int, ref eAITarget>();
	}

	void ~eAITargetInformation()
	{
#ifdef EAI_TRACE
		auto trace = CF_Trace_0(this, "~eAITargetInformation");
#endif

		foreach (int id, eAITarget info : m_Groups)
		{
			Remove(id);
		}
	}

	/**
	 * @brief Debugging information about the target
	 *
	 * @return string
	 */
	override string GetDebugName()
	{
		string str = super.GetDebugName();
		
		return str;
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

	/**
	 * @brief Abstract function. Get the position for the AI within the target. Each AI could have their own position for the target.
	 *
	 * @param ai null default, gets the position for the AI if specified, otherwise returns a default value
	 * @return EntityAI
	 */
	vector GetPosition(eAIBase ai = null)
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
	 * @param ai null default, gets the position for the AI if specified, otherwise returns a default value
	 * @return int
	 */
	float GetThreat(eAIBase ai = null)
	{
#ifdef EAI_TRACE
		auto trace = CF_Trace_1(this, "GetThreat").Add(ai);
#endif

		return 0;
	}

	bool ShouldRemove(eAIBase ai = null)
	{
#ifdef EAI_TRACE
		auto trace = CF_Trace_1(this, "ShouldRemove").Add(ai);
#endif

		return 0;
	}

	/**
	 * @brief Abstract function. Get the distance to the target.
	 *
	 * @param ai gets the position for the AI if specified
	 * @return float
	 */
	float GetDistance(eAIBase ai)
	{
#ifdef EAI_TRACE
		auto trace = CF_Trace_1(this, "GetDistance").Add(ai);
#endif

		return 0;
	}

	float GetDistanceSq(eAIBase ai)
	{
#ifdef EAI_TRACE
		auto trace = CF_Trace_1(this, "GetDistanceSq").Add(ai);
#endif

		return 0;
	}

	/**
	 * @brief Processes the group
	 *
	 * @param group_id group id of an eAIGroup
	 */
	void Process(int group_id)
	{
#ifdef EAI_TRACE
		auto trace = CF_Trace_1(this, "Process").Add(group_id);
#endif

		eAITarget target;
		if (!m_Groups.Find(group_id, target))
		{
			Error("eAITargetInformation::Process called when target is not in group specified | group_id=" + group_id);
			return;
		}

		if (!IsActive() || target.found_at_time + target.max_time <= GetGame().GetTime())
		{
			Remove(group_id);
		}
	}

	/**
	 * @brief Refreshes the target time for the group
	 *
	 * @param group the eAIGroup
	 * @param max_time the new maximum time it should be targetting for, if int.MIN then not updated
	 */
	void Update(eAIGroup group, int max_time = -2147483647 /*int.MIN*/)
	{
#ifdef EAI_TRACE
		auto trace = CF_Trace_2(this, "Update").Add(group).Add(max_time);
#endif

		eAITarget target;
		if (!m_Groups.Find(group.GetID(), target))
			return;

		target.found_at_time = GetGame().GetTime();
		if (max_time != -2147483647 /*int.MIN*/)
			target.max_time = max_time;
	}

	/**
	 * @brief Inserts the group into the target
	 *
	 * @param group_id group id of an eAIGroup
	 */
	eAITarget Insert(notnull eAIGroup group, int max_time = -2147483647 /*-int.MIN*/)
	{
#ifdef EAI_TRACE
		auto trace = CF_Trace_2(this, "Insert").Add(group).Add(max_time);
#endif

		eAITarget target;
		target = new eAITarget(group, GetGame().GetTime(), max_time, this);
		if (!m_Groups.Insert(group.GetID(), target))
			return null;

		group.OnTargetAdded(this);

		return target;
	}

	/**
	 * @brief Tells the target that the AI is targeting it, inserting the group
	 *
	 * @param ai eAIBase object
	 * @param max_time time the eAIBase will be targetting this target for
	 */
	eAITarget Insert(eAIBase ai, int max_time = -2147483647 /*int.MIN*/)
	{
#ifdef EAI_TRACE
		auto trace = CF_Trace_2(this, "Insert").Add(ai).Add(max_time);
#endif

		eAITarget target;
		target = Insert(ai.GetGroup(), max_time);
		if (!target)
			return null;
		if (target.AddAI(ai))
			ai.OnAddTarget(target);
		return target;
	}

	/**
	 * @brief Tells the target that the AI is targetting it. If the group didn't know about the target, insert the group
	 *
	 * @param ai eAIBase object
	 * @param max_time time the eAIBase will be targetting this target for, if int.MIN then the time of the group isn't updated
	 */
	eAITarget AddAI(eAIBase ai, int max_time = -2147483647 /*int.MIN*/)
	{
#ifdef EAI_TRACE
		auto trace = CF_Trace_2(this, "AddAI").Add(ai).Add(max_time);
#endif

		eAITarget target;
		int group_id = ai.GetGroup().GetID();
		if (!m_Groups.Find(group_id, target))
			return Insert(ai, max_time);

		if (max_time != -2147483647 /*int.MIN*/)
			target.max_time = max_time;

		if (target.AddAI(ai))
			ai.OnAddTarget(target);

		return target;
	}

	/**
	 * @brief Tells the target that the AI is no longer targetting it
	 *
	 * @param ai eAIBase object
	 */
	bool RemoveAI(eAIBase ai)
	{
#ifdef EAI_TRACE
		auto trace = CF_Trace_1(this, "RemoveAI").Add(ai);
#endif

		eAITarget target;
		int group_id = ai.GetGroup().GetID();
		if (!m_Groups.Find(group_id, target))
			return false;

		if (!target.RemoveAI(ai))
			return false;

		ai.OnRemoveTarget(target);

		if (target.CountAI() == 0)
		{
			target.group.OnTargetRemoved(this);

			m_Groups.Remove(group_id);
		}

		return true;
	}

	/**
	 * @brief Tells the target that the group and it's AI is no longer targetting it
	 *
	 * @param group_id the ID of the eAIGroup
	 */
	void Remove(int group_id)
	{
#ifdef EAI_TRACE
		auto trace = CF_Trace_1(this, "Remove").Add(group_id);
#endif

		eAITarget target;
		if (!m_Groups.Find(group_id, target))
			return;

		if (target.group && target.ai_list)
		{
			target.group.OnTargetRemoved(this);

			foreach (eAIBase ai : target.ai_list)
			{
				if (!ai)
					continue;
				
				ai.OnRemoveTarget(target);
			}
		}

		m_Groups.Remove(group_id);
		delete target;
	}

	/**
	 * @brief Tells the target that the group and it's AI is no longer targetting it
	 *
	 * @param group the eAIGroup
	 */
	void Remove(eAIGroup group)
	{
#ifdef EAI_TRACE
		auto trace = CF_Trace_1(this, "Remove").Add(group);
#endif

		int group_id = group.GetID();

		Remove(group_id);
	}

	void RemoveFromAll()
	{
#ifdef EAI_TRACE
		auto trace = CF_Trace_0(this, "RemoveFromAll");
#endif

		TIntArray ids = m_Groups.GetKeyArray();
		foreach (int id : ids)
			Remove(id);
	}

	/**
	 * @brief Checks to see if any group/ai is currently targetting this target
	 *
	 * @return bool true if being targetted, false otherwise
	 */
	bool IsTargetted()
	{
#ifdef EAI_TRACE
		auto trace = CF_Trace_0(this, "IsTargetted");
#endif

		return m_Groups.Count() > 0;
	}

	/**
	 * @brief Checks to see if the group specified is currently targetting this target
	 *
	 * @param group_id the group id of the eAIGroup
	 * @return bool true if being targetted, false otherwise
	 */
	bool IsTargetted(int group_id)
	{
#ifdef EAI_TRACE
		auto trace = CF_Trace_1(this, "IsTargetted").Add(group_id);
#endif

		return m_Groups.Contains(group_id);
	}

	/**
	 * @brief Checks to see if the group specified is currently targetting this target
	 *
	 * @param group the eAIGroup
	 * @return bool true if being targetted, false otherwise
	 */
	bool IsTargetted(notnull eAIGroup group)
	{
#ifdef EAI_TRACE
		auto trace = CF_Trace_1(this, "IsTargetted").Add(group);
#endif

		return m_Groups.Contains(group.GetID());
	}

	/**
	 * @brief Checks to see if the group specified is currently targetting this target
	 *
	 * @param group_id the group id of the eAIGroup
	 * @return bool true if being targetted, false otherwise
	 */
	bool IsTargetted(int group_id, out int num_ai)
	{
#ifdef EAI_TRACE
		auto trace = CF_Trace_1(this, "IsTargetted").Add(group_id);
#endif

		eAITarget target;
		if (!m_Groups.Find(group_id, target))
			return false;

		num_ai = target.ai_list.Count();

		return true;
	}

	/**
	 * @brief Checks to see if the group specified is currently targetting this target
	 *
	 * @param group the eAIGroup
	 * @return bool true if being targetted, false otherwise
	 */
	bool IsTargetted(notnull eAIGroup group, out int num_ai)
	{
#ifdef EAI_TRACE
		auto trace = CF_Trace_1(this, "IsTargetted").Add(group);
#endif

		eAITarget target;
		if (!m_Groups.Find(group.GetID(), target))
			return false;

		num_ai = target.ai_list.Count();

		return true;
	}

	/**
	 * @brief Checks to see if the AI specified is currently targetting this target
	 *
	 * @param group the eAIGroup
	 * @return bool true if being targetted, false otherwise
	 */
	bool IsTargettedBy(eAIBase ai)
	{
#ifdef EAI_TRACE
		auto trace = CF_Trace_1(this, "IsTargettedBy").Add(ai);
#endif

		eAITarget target;
		if (!m_Groups.Find(ai.GetGroup().GetID(), target))
			return false;

		return target.FindAI(ai) != -1;
	}

	//! entity specific implementations for abstracted call in eAIEntityTargetInformation
	void OnDeath()
	{
		RemoveFromAll();
	}

	void OnHit()
	{
	}

	// this is really bad but unfortunately we can't reasonably mod the Object class
	// and for some circumstances using templates just won't work.
	//
	// wherever possible, please use
	//	'eAIEntity<DayZPlayerImplement>.GetTargetInformation(GetGame().GetPlayer())`
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
