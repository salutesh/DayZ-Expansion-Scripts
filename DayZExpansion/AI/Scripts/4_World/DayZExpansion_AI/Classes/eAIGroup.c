// TODO: sync to the client automatically within DayZPlayerImplement
//  only data that needs to be known is just the id, members and faction
class eAIGroup
{
	private static autoptr array<ref eAIGroup> s_AllGroups = new array<ref eAIGroup>();
	private static int s_UpdateIndex;

	private static int s_IDCounter = 0;

	private autoptr array<eAITargetInformation> m_Targets;
	private int m_ID;

	//! Refer to eAIGroup::GetTargetInformation
	private autoptr eAIGroupTargetInformation m_TargetInformation;

	// Ordered array of group members. 0 is the leader.
	private autoptr array<DayZPlayerImplement> m_Members;

	// What formation the group should keep
	private autoptr eAIFormation m_Form;

	// Group identity
	private ref eAIFaction m_Faction;

	private autoptr array<vector> m_Waypoints;
	private eAIWaypointBehavior m_WaypointBehaviour = eAIWaypointBehavior.ALTERNATE;
	int m_CurrentWaypointIndex;
	bool m_BackTracking;

	private eAIGroupFormationState m_FormationState = eAIGroupFormationState.IN;

	bool m_UpdateSearchPosition;

	// return the group owned by leader, otherwise create a new one.
	static eAIGroup GetGroupByLeader(DayZPlayerImplement leader, bool createIfNoneExists = true, eAIFaction faction = null)
	{
#ifdef EAI_TRACE
		auto trace = CF_Trace_2("eAIGroup", "eAIGroup::GetGroupByLeader").Add(leader).Add(createIfNoneExists).Add(faction);
#endif

		for (int i = 0; i < s_AllGroups.Count(); i++)
			if (s_AllGroups[i].GetLeader() == leader)
				return s_AllGroups[i];

		if (!createIfNoneExists)
			return null;

		eAIGroup group = CreateGroup(faction);
		leader.SetGroup(group);
		return group;
	}

	static eAIGroup GetGroupByID(int id, bool createIfNoneExists = false)
	{
#ifdef EAI_TRACE
		auto trace = CF_Trace_2("eAIGroup", "eAIGroup::GetGroupByID").Add(id).Add(createIfNoneExists);
#endif

		for (int i = 0; i < s_AllGroups.Count(); i++)
			if (s_AllGroups[i].GetID() == id)
				return s_AllGroups[i];

		if (!createIfNoneExists)
			return null;

		eAIGroup group = new eAIGroup();
		group.m_ID = id;
		return group;
	}

	static eAIGroup CreateGroup(eAIFaction faction = null)
	{
#ifdef EAI_TRACE
		auto trace = CF_Trace_0("eAIGroup", "eAIGroup::CreateGroup");
#endif

		eAIGroup group = new eAIGroup(faction);

		s_IDCounter++;
		group.m_ID = s_IDCounter;

		return group;
	}

	static void DeleteGroup(eAIGroup group)
	{
#ifdef EAI_TRACE
		auto trace = CF_Trace_0("eAIGroup", "eAIGroup::DeleteGroup");
#endif

		int index = s_AllGroups.Find(group);
		s_AllGroups.Remove(index);
		delete group;
	}

	private void eAIGroup(eAIFaction faction = null)
	{
#ifdef EAI_TRACE
		auto trace = CF_Trace_0(this, "eAIGroup");
#endif

		m_TargetInformation = new eAIGroupTargetInformation(this);
		m_Targets = new array<eAITargetInformation>();

		m_Members = new array<DayZPlayerImplement>();

		m_Form = new eAIFormationVee(this);

		if (faction)
			m_Faction = faction;
		else
			m_Faction = new eAIFactionRaiders();

		m_Waypoints = new array<vector>();

		s_AllGroups.Insert(this);
	}

	/*private*/ void ~eAIGroup()
	{
#ifdef EAI_TRACE
		auto trace = CF_Trace_0(this, "~eAIGroup");
#endif

		if (!GetGame())
			return;

		int idx = s_AllGroups.Find(this);
		if (idx != -1)
			s_AllGroups.RemoveOrdered(idx);
	}

	void Delete()
	{
#ifdef EAI_TRACE
		auto trace = CF_Trace_0(this, "Delete");
#endif

		GetGame().GetCallQueue(CALL_CATEGORY_SYSTEM).Call(DeleteGroup, this);
	}

	void AddWaypoint(vector pos)
	{
#ifdef EAI_TRACE
		auto trace = CF_Trace_0(this, "AddWaypoint");
#endif

		m_Waypoints.Insert(pos);
	}

	array<vector> GetWaypoints()
	{
#ifdef EAI_TRACE
		auto trace = CF_Trace_0(this, "GetWaypoints");
#endif

		return m_Waypoints;
	}

	void ClearWaypoints()
	{
#ifdef EAI_TRACE
		auto trace = CF_Trace_0(this, "ClearWaypoints");
#endif

		m_Waypoints.Clear();
	}

	void SetWaypointBehaviour(eAIWaypointBehavior bhv)
	{
#ifdef EAI_TRACE
		auto trace = CF_Trace_0(this, "SetWaypointBehaviour");
#endif

		switch (bhv)
		{
			case eAIWaypointBehavior.HALT_OR_LOOP:
				if (Math.RandomIntInclusive(0, 1))
					bhv = eAIWaypointBehavior.HALT;
				else
					bhv = eAIWaypointBehavior.LOOP;
				break;
			case eAIWaypointBehavior.HALT_OR_ALTERNATE:
				if (Math.RandomIntInclusive(0, 1))
					bhv = eAIWaypointBehavior.HALT;
				else
					bhv = eAIWaypointBehavior.ALTERNATE;
				break;
		}

		m_WaypointBehaviour = bhv;
	}

	eAIWaypointBehavior GetWaypointBehaviour()
	{
#ifdef EAI_TRACE
		auto trace = CF_Trace_0(this, "GetWaypointBehaviour");
#endif

		return m_WaypointBehaviour;
	}

	void SetFormationState(eAIGroupFormationState state)
	{
#ifdef EAI_TRACE
		auto trace = CF_Trace_0(this, "SetFormationState");
#endif

		m_FormationState = state;
	}

	eAIGroupFormationState GetFormationState()
	{
#ifdef EAI_TRACE
		auto trace = CF_Trace_0(this, "GetFormationState");
#endif

		return m_FormationState;
	}

	void SetFaction(eAIFaction f)
	{
#ifdef EAI_TRACE
		auto trace = CF_Trace_0(this, "SetFaction");
#endif

		m_Faction = f;
	}

	eAIFaction GetFaction()
	{
#ifdef EAI_TRACE
		auto trace = CF_Trace_0(this, "GetFaction");
#endif

		return m_Faction;
	}

	/**
	 * @brief The unique ID for this group
	 *
	 * @return int
	 */
	int GetID()
	{
		return m_ID;
	}

	/**
	 * @brief Internal event fired when this group needs to know that is now targetting something
	 *
	 * @param target The target being added
	 */
	void OnTargetAdded(eAITargetInformation target)
	{
#ifdef EAI_TRACE
		auto trace = CF_Trace_1(this, "OnTargetAdded").Add(target);
#endif

		m_Targets.Insert(target);
	}

	/**
	 * @brief Internal event fired when this group needs to know that is no longer targetting something
	 *
	 * @param target The target being removed
	 */
	void OnTargetRemoved(eAITargetInformation target)
	{
#ifdef EAI_TRACE
		auto trace = CF_Trace_1(this, "OnTargetRemoved").Add(target);
#endif

		m_Targets.RemoveItem(target);
	}

	/**
	 * @brief Processes all the targets so they can be removed when the time has been exceeded
	 */
	void ProcessTargets()
	{
#ifdef EAI_TRACE
		auto trace = CF_Trace_0(this, "ProcessTargets");
#endif

		for (int i = m_Targets.Count() - 1; i >= 0; i--)
			m_Targets[i].Process(m_ID);
	}

	/**
	 * @brief This target is both used by the owned AI's and enemy groups.
	 * The owned AI's will use this to get the position they should move to
	 * The enemy AI's will use this similar to a normal entity if they are
	 * targetting the group as a whole and not a singular AI. If they are
	 * targetting a singular AI then they would use GetTargetInformation.
	 *
	 * @return the target
	 */
	eAITargetInformation GetTargetInformation()
	{
#ifdef EAI_TRACE
		auto trace = CF_Trace_0(this, "GetTargetInformation");
#endif

		return m_TargetInformation;
	}

	void Update(float pDt)
	{
#ifdef EAI_TRACE
		auto trace = CF_Trace_0(this, "Update");
#endif

		ProcessTargets();

		m_Form.Update(pDt);
	}

	static void UpdateAll(float pDt, int groupsPerTick = 10)
	{
#ifdef EAI_TRACE
		auto trace = CF_Trace_0("eAIGroup", "UpdateAll");
#endif

		// don't process if we aren't the server
		if (!GetGame().IsServer())
			return;

		int count = s_AllGroups.Count();

		if (!count)
			return;

		if (groupsPerTick > count)
			groupsPerTick = count;

		while (groupsPerTick > 0)
		{
			if (s_UpdateIndex >= count)
				s_UpdateIndex = 0;

			s_AllGroups[s_UpdateIndex++].Update(pDt);

			groupsPerTick--;
		} 
	}

	int GetMemberIndex(eAIBase ai)
	{
#ifdef EAI_TRACE
		auto trace = CF_Trace_1(this, "GetMemberIndex").Add(ai);
#endif

		int pos = 0;
		vector position = "0 0 0";

		for (int i = 0; i < m_Members.Count(); i++)
		{
			// ignore members who have died so their position can be taken over
			if (!m_Members[i] || !m_Members[i].IsAlive())
				continue;

			if (m_Members[i] == ai)
				return pos;

			pos++;
		}

		return -1;
	}

	vector GetFormationPosition(eAIBase ai)
	{
#ifdef EAI_TRACE
		auto trace = CF_Trace_1(this, "GetFormationPosition").Add(ai);
#endif

		int pos = GetMemberIndex(ai);
		vector position = "0 0 0";
		if (pos != -1)
			position = m_Form.GetPosition(pos);
			
		return m_Form.ToWorld(position);
	}

	void SetLeader(DayZPlayerImplement leader)
	{
#ifdef EAI_TRACE
		auto trace = CF_Trace_1(this, "SetLeader").Add(leader);
#endif

		if (!IsMember(leader))
			AddMember(leader);

		DayZPlayerImplement temp = m_Members[0];
		if (temp == leader)
			return;

		m_Members[0] = leader;
		m_Members[0].SetGroupMemberIndex(0);

		for (int i = 1; i < Count(); i++)
		{
			if (m_Members[i] && m_Members[i] == leader)
			{
				m_Members[i] = temp;
				m_Members[i].SetGroupMemberIndex(i);
				return;
			}
		}
	}

	DayZPlayerImplement GetLeader()
	{
#ifdef EAI_TRACE
		auto trace = CF_Trace_0(this, "GetLeader");
#endif

		return m_Members[0];
	}

	eAIFormation GetFormation()
	{
#ifdef EAI_TRACE
		auto trace = CF_Trace_0(this, "GetFormation");
#endif

		return m_Form;
	}

	void SetFormation(eAIFormation f)
	{
#ifdef EAI_TRACE
		auto trace = CF_Trace_0(this, "SetFormation");
#endif

		f.SetGroup(this);
		m_Form = f;
	}

	bool IsMember(DayZPlayerImplement member)
	{
#ifdef EAI_TRACE
		auto trace = CF_Trace_1(this, "IsMember").Add(member);
#endif

		return m_Members.Find(member) != -1;
	}

	int AddMember(DayZPlayerImplement member)
	{
#ifdef EAI_TRACE
		auto trace = CF_Trace_1(this, "AddMember").Add(member);
#endif

		return m_Members.Insert(member);
	}

	void Client_SetMemberIndex(DayZPlayerImplement member, int index)
	{
#ifdef EAI_TRACE
		auto trace = CF_Trace_2(this, "Client_SetMemberIndex").Add(member).Add(index);
#endif

		if (index >= m_Members.Count())
		{
			m_Members.Resize(index + 1);
		}

		m_Members[index] = member;

		int removeFrom = m_Members.Count();

		for (int i = m_Members.Count() - 1; i > index; i--)
		{
			if (m_Members[i] != null)
				break;
			removeFrom = i;
		}

		m_Members.Resize(removeFrom);
	}

	bool RemoveMember(DayZPlayerImplement member, bool autoDelete = true)
	{
#ifdef EAI_TRACE
		auto trace = CF_Trace_2(this, "RemoveMember").Add(member).Add(autoDelete);
#endif

		return RemoveMember(m_Members.Find(member), autoDelete);
	}

	bool RemoveMember(int i, bool autoDelete = true)
	{
#ifdef EAI_TRACE
		auto trace = CF_Trace_2(this, "RemoveMember").Add(i).Add(autoDelete);
#endif

		if (i < 0 || i >= m_Members.Count())
			return false;

		m_Members.RemoveOrdered(i);

		if (autoDelete && m_Members.Count() == 0)
		{
			Delete();
		}

		return true;
	}

	DayZPlayerImplement GetMember(int i)
	{
#ifdef EAI_TRACE
		auto trace = CF_Trace_1(this, "GetMember").Add(i);
#endif

		return m_Members[i];
	}

	int GetIndex(DayZPlayerImplement player)
	{
#ifdef EAI_TRACE
		auto trace = CF_Trace_1(this, "GetIndex").Add(player);
#endif

		return m_Members.Find(player);
	}

	int Count()
	{
#ifdef EAI_TRACE
		auto trace = CF_Trace_0(this, "Count");
#endif

		return m_Members.Count();
	}

	//! Clears ALL AI from the server
	static void Admin_ClearAllAI()
	{
#ifdef EAI_TRACE
		auto trace = CF_Trace_0("eAIGroup", "Admin_ClearAllAI");
#endif

		foreach (eAIGroup group : s_AllGroups)
		{
			group.ClearAI();
		}
	}

	void ClearAI(bool autodelete = true)
	{
		eAIBase ai;
		for (int i = Count() - 1; i > -1; i--)
		{
			if (!Class.CastTo(ai, GetMember(i)) || ai.IsInherited(eAINPCBase))
			{
				continue;
			}

			RemoveMember(i, autodelete);
			auto hands = ai.GetHumanInventory().GetEntityInHands();
			//! Prevent AI from dropping item in hands on death
			if (hands)
				GetGame().ObjectDelete(hands);
			ai.SetAllowDamage(true);
			//! Kill AI to remove collision
			ai.SetHealth(0);
			//! Delete body after delay (don't remove it too early after death or invisible collision will still be there, five seconds seems to work well)
			GetGame().GetCallQueue(CALL_CATEGORY_SYSTEM).CallLater(GetGame().ObjectDelete, 5000, false, ai);
		}
	}
};
