// TODO: sync to the client automatically within DayZPlayerImplement
//  only data that needs to be known is just the id, members and faction
class eAIGroup
{
	private static autoptr array<ref eAIGroup> s_AllGroups = new array<ref eAIGroup>();
	private static int s_UpdateIndex;

	private static int s_IDCounter = 0;

	private autoptr array<eAITargetInformation> m_Targets = {};
	private int m_ID;

	//! Refer to eAIGroup::GetTargetInformation
	private autoptr eAIGroupTargetInformation m_TargetInformation;

	// Ordered array of group members. 0 is the leader.
	private autoptr array<DayZPlayerImplement> m_Members = {};
	private autoptr array<DayZPlayerImplement> m_Members_Deceased = {};
	private int m_MemberCount = 1;

	// What formation the group should keep
	private autoptr eAIFormation m_Form;

	// Group identity
	private ref eAIFaction m_Faction;
	private string m_Name;

	private autoptr array<vector> m_Waypoints;
	private eAIWaypointBehavior m_WaypointBehaviour = eAIWaypointBehavior.ALTERNATE;
	int m_CurrentWaypointIndex;
	bool m_BackTracking;

	private eAIGroupFormationState m_FormationState = eAIGroupFormationState.IN;

	bool m_UpdateSearchPosition;

	// return the group owned by leader, otherwise create a new one.
	static eAIGroup GetGroupByLeader(DayZPlayerImplement leader, bool createIfNoneExists = true, eAIFaction faction = null, bool autoDeleteFormerGroupIfEmpty = true)
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
		leader.SetGroup(group, autoDeleteFormerGroupIfEmpty);
		return group;
	}

	override string GetDebugName()
	{
		string str = ToString();
		
		str += ", ";
		str += "faction=" + m_Faction;

		return str;
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
#ifdef DIAG
		auto trace = EXTrace.Start(EXTrace.AI, eAIGroup, "" + group);
#endif

		int index = s_AllGroups.Find(group);
		if (index > -1)
		{
		#ifdef DIAG
			EXTrace.Print(EXTrace.AI, eAIGroup, "Removing group from s_AllGroups");
		#endif
			s_AllGroups.Remove(index);
		}
	}

	private void eAIGroup(eAIFaction faction = null)
	{
#ifdef EAI_TRACE
		auto trace = CF_Trace_0(this, "eAIGroup");
#endif

		m_TargetInformation = new eAIGroupTargetInformation(this);

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
		if (!GetGame())
			return;

	#ifdef DIAG
		EXTrace.Print(EXTrace.AI, this, "~eAIGroup");
	#endif

		DeleteGroup(this);
	}

	void Delete()
	{
#ifdef DIAG
		auto trace = EXTrace.Start(EXTrace.AI, this);
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

	vector GetCurrentWaypoint()
	{
		return m_Waypoints[m_CurrentWaypointIndex];
	}

	void ClearWaypoints()
	{
#ifdef EAI_TRACE
		auto trace = CF_Trace_0(this, "ClearWaypoints");
#endif

		m_Waypoints.Clear();
		m_CurrentWaypointIndex = 0;
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

	/**
	 * @brief Set waypoint behavior automatically based on proximity of 1st and last waypoint to one another
	 * 
	 * If 1st and last waypoints are close (within 1.5 m) to one another, waypoint behavior will be set to LOOP.
	 * If they are not close, it will be set to the passed in behavior (default ALTERNATE).
	 */
	void SetWaypointBehaviourAuto(eAIWaypointBehavior bhv = eAIWaypointBehavior.ALTERNATE)
	{
		int count = m_Waypoints.Count();

		if (count > 1 && vector.DistanceSq(m_Waypoints[0], m_Waypoints[count - 1]) <= 2.25)
			SetWaypointBehaviour(eAIWaypointBehavior.LOOP);
		else
			SetWaypointBehaviour(bhv);
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
#ifdef DIAG
		auto trace = CF_Trace_1(EXTrace.AI, this).Add(f);
#endif

		if (!f)
		{
			Error("Cannot set NULL faction");
			return;
		}

		int oldFactionTypeID = m_Faction.GetTypeID();
		int newFactionTypeID = f.GetTypeID();

		m_Faction = f;

		if (newFactionTypeID == oldFactionTypeID)
			return;

		foreach (auto member: m_Members)
		{
			if (member)
				member.eAI_SetFactionTypeID(newFactionTypeID);
		}
	}

	eAIFaction GetFaction()
	{
#ifdef EAI_TRACE
		auto trace = CF_Trace_0(this, "GetFaction");
#endif

		return m_Faction;
	}

	void SetName(string name)
	{
		m_Name = name;
	}

	string GetName()
	{
		return m_Name;
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
	 * @brief Add/update target for all group members
	 * 
	 * @param info Target information
	 * @param update If true (default) and member is already targeting the target, update found_at_time and max_time
	 * @param threat Initial threat level if non-zero
	 */
	void AddTarget(DayZPlayerImplement player, eAITargetInformation info, int max_time = -1, bool update = true, float threat = 0.0)
	{
#ifdef DIAG
		auto trace = EXTrace.Start(EXTrace.AI, this, info.GetDebugName(), "" + max_time, "" + update, "" + threat);
#endif

		eAIBase ai;
		eAITarget target;
		foreach (DayZPlayerImplement member: m_Members)
		{
			if (Class.CastTo(ai, member))
			{
				target = info.AddAI(ai, max_time);

				if (update)
				{
					target.Update(max_time);
					update = false;  //! Since targets are per-group, we only need to do this once per loop
				}

				if (threat > 0.0)
				{
					auto state = ai.eAI_GetTargetInformationState(info, false);
					if (threat > state.m_ThreatLevelActive)
						state.SetInitial(threat, player.GetPosition());  //! We deliberately don't use attacker position but victim position
				}
			}
		}
	}

	/**
	 * @brief Internal event fired when this group needs to know that is now targetting something
	 *
	 * @param target The target being added
	 */
	void OnTargetAdded(eAITargetInformation target)
	{
#ifdef DIAG
		EXTrace.Print(EXTrace.AI, this, "OnTargetAdded " + target.GetDebugName());
#endif

		m_Targets.Insert(target);
	}

	/**
	 * @brief Return true if at least one member in group is targeting target
	 */
	bool IsTargeting(eAITargetInformation target)
	{
		if (m_Targets.Find(target) > -1)
			return true;

		return false;
	}

	/**
	 * @brief Internal event fired when this group needs to know that is no longer targetting something
	 *
	 * @param target The target being removed
	 */
	void OnTargetRemoved(eAITargetInformation target)
	{
#ifdef DIAG
		EXTrace.Print(EXTrace.AI, this, "OnTargetRemoved " + target.GetDebugName());
#endif

		m_Targets.RemoveItem(target);
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
	eAIGroupTargetInformation GetTargetInformation()
	{
#ifdef EAI_TRACE
		auto trace = CF_Trace_0(this, "GetTargetInformation");
#endif

		return m_TargetInformation;
	}

	//! DEPRECATED - group will never contain NULL or members that aren't alive
	int GetMemberIndex(eAIBase ai)
	{
		EXError.WarnOnce(this, "GetMemberIndex() is deprecated. Please use GetIndex().");

		return GetIndex(ai);
	}

	vector GetFormationPosition(eAIBase ai)
	{
#ifdef EAI_TRACE
		auto trace = CF_Trace_1(this, "GetFormationPosition").Add(ai);
#endif

		float time = GetGame().GetTickTime();
		bool isInitialUpdate = ai.m_eAI_FormationPositionUpdateTime == 0.0;
		if (time - ai.m_eAI_FormationPositionUpdateTime > Math.RandomFloat(2.0, 4.0) || isInitialUpdate)
		{
			int index = GetIndex(ai);
			if (index != -1)
			{
				auto leader = GetFormationLeader();
				if (index > 0 && leader != GetLeader())
					index--;
				vector position = m_Form.GetPosition(index);
				vector direction = m_Form.GetDirection(index);
				
				bool isMoving;
				if (leader && leader.Expansion_GetMovementSpeed() > 0)
					isMoving = true;
				if (isMoving || isInitialUpdate)
				{
					for (int i = 0; i < 3; i++)
					{
						position[i] = position[i] + m_Form.GetLooseness();
						if (i == 2)
							position[i] = position[i] + isMoving;  //! Compensate for head start of leader
					}
				}
				
				ai.m_eAI_FormationPosition = position;
				ai.m_eAI_FormationDirection = direction;
				ai.m_eAI_FormationPositionUpdateTime = time;
			}
		}
			
		return m_Form.ToWorld(ai.m_eAI_FormationPosition);
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

	DayZPlayerImplement GetFormationLeader()
	{
		auto leader = GetLeader();

		if (leader && !leader.IsAI() && m_Members.Count() > 1 && m_Waypoints.Count() > 0)
			return m_Members[1];

		return leader;
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

		int index = m_Members.Insert(member);

		Send_SetInGroup();

		return index;
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
#ifdef DIAG
		auto trace = EXTrace.Start(EXTrace.AI, this, "" + member, "" + autoDelete);
#endif

		return RemoveMember(m_Members.Find(member), autoDelete);
	}

	bool RemoveMember(int i, bool autoDelete = true)
	{
#ifdef DIAG
		auto trace = EXTrace.Start(EXTrace.AI, this, "" + i, "" + autoDelete);
#endif

		if (i < 0 || i >= m_Members.Count())
			return false;

		auto member = m_Members[i];

		m_Members.RemoveOrdered(i);

		if (autoDelete && m_Members.Count() == 0)
		{
			Delete();
		}
		else
		{
			Send_SetInGroup();

			if (member && !member.IsAlive())
				m_Members_Deceased.Insert(member);
		}

		return true;
	}

	bool RemoveDeceased(DayZPlayerImplement member)
	{
#ifdef DIAG
		auto trace = EXTrace.Start(EXTrace.AI, this, "" + member);
#endif

		return RemoveDeceased(m_Members_Deceased.Find(member));
	}

	bool RemoveDeceased(int i)
	{
#ifdef DIAG
		auto trace = EXTrace.Start(EXTrace.AI, this, "" + i);
#endif

		if (i < 0 || i >= m_Members_Deceased.Count())
			return false;

		m_Members_Deceased.RemoveOrdered(i);

		return true;
	}

	/**
	 * @brief send whether leader is in group with more than one other member to client
	 * 
	 * @note This is necessary because on client, group can only contain the members in network bubble,
	 *       so the count does not reflect the value on server.
	 */
	void Send_SetInGroup()
	{
	#ifdef SERVER
	#ifdef DIAG
		auto trace = EXTrace.Start(EXTrace.AI, this);
	#endif

		if (m_Members.Count() <= 2 && m_Members.Count() != m_MemberCount)
		{
			m_MemberCount = m_Members.Count();

			auto leader = GetLeader();
			if (leader && leader.GetIdentity())
			{
				auto rpc = GetDayZGame().GetExpansionGame().GetRPCManager().CreateRPC("RPC_SetInGroup");
				if (m_MemberCount > 1)
					rpc.Write(true);
				else
					rpc.Write(false);
				rpc.Expansion_Send(true, leader.GetIdentity());
			}
		}
	#endif
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

	/**
	 * @brief Return random member excluding player. If group size is 1, return null.
	 * 
	 * @param player Player to exclude
	 */
	DayZPlayerImplement GetRandomMemberExcluding(DayZPlayerImplement player)
	{
		int count = Count();

		if (count == 1)
			return null;

		int index = Math.RandomInt(0, count);
		DayZPlayerImplement member = m_Members[index];

		if (member == player)
		{
			if (index + 1 == count)
				index--;
			else
				index++;

			member = m_Members[index];
		}

		return member;
	}

	int Count()
	{
#ifdef EAI_TRACE
		auto trace = CF_Trace_0(this, "Count");
#endif

		return m_Members.Count();
	}

	//! DEPRECATED - group will never contain NULL or members that aren't alive
	int GetAliveCount()
	{
		EXError.WarnOnce(this, "GetAliveCount() is deprecated. Please use Count().");

		return Count();
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

	void ClearAI(bool autodelete = true, bool deferDespawnUntilLoosingAggro = false)
	{
	#ifdef DIAG
		auto trace = EXTrace.Start(EXTrace.AI, this);
	#endif

		int i;
		eAIBase ai;

		for (i = Count() - 1; i > -1; i--)
		{
			if (!Class.CastTo(ai, GetMember(i)) || ai.IsInherited(eAINPCBase))
				continue;

			if (deferDespawnUntilLoosingAggro && ai.GetThreatToSelf() >= 0.4)
				ai.eAI_SetDespawnOnLoosingAggro(true);
			else
				ai.eAI_Despawn();
		}

		for (i = m_Members_Deceased.Count() - 1; i > -1; i--)
		{
			if (!Class.CastTo(ai, m_Members_Deceased[i]))
				continue;

			ai.eAI_Despawn();
		}
	}

	TStringArray DumpState(PlayerIdentity sender, bool includeMembers = true, string indent = string.Empty)
	{
		TStringArray report = {};

		report.Insert(indent + string.Format("Group ID %1 %2", m_ID, m_Name));

		if (indent)
			indent.Replace("-", " ");

		report.Insert(indent + string.Format("|- Faction %1", m_Faction.GetName()));

		string form = m_Form.ClassName().Substring(12, m_Form.ClassName().Length() - 12);
		report.Insert(indent + string.Format("|- Formation %1 %2", form, typename.EnumToString(eAIGroupFormationState, m_FormationState)));

		string lineSegment;
		if (includeMembers)
			lineSegment = "|";
		else
			lineSegment = "\\";

		report.Insert(indent + string.Format(lineSegment + "- Waypoints %1 %2", m_Waypoints.Count(), typename.EnumToString(eAIWaypointBehavior, m_WaypointBehaviour)));

		if (includeMembers)
		{
			eAIBase ai;
			string subIndent = "|  ";
			lineSegment = "|";
			foreach (int i, DayZPlayerImplement member: m_Members)
			{
				if (Class.CastTo(ai, member))
				{
					if (i == Count() - 1)
					{
						subIndent = ".  ";
						lineSegment = "\\";
					}

					report.Insert(indent + string.Format(lineSegment + "- AI group member %1", i));
					report.InsertAll(ai.eAI_DumpState(sender, false, indent + subIndent + lineSegment + "- "));
				}
			}
		}

		return report;
	}
};
