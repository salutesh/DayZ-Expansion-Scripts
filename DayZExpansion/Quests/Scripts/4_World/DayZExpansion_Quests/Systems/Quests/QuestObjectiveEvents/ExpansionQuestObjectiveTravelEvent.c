/**
 * ExpansionQuestObjectiveTravelEvent.c
 *
 * DayZ Expansion Mod
 * www.dayzexpansion.com
 * © 2023 DayZ Expansion Mod Team
 *
 * This work is licensed under the Creative Commons Attribution-NonCommercial-NoDerivatives 4.0 International License.
 * To view a copy of this license, visit http://creativecommons.org/licenses/by-nc-nd/4.0/.
 *
*/

class ExpansionQuestObjectiveTravelEvent: ExpansionQuestObjectiveEventBase
{
	protected bool m_DestinationReached;
	protected vector m_Position;
	protected int m_PointSearchCount;
	protected ref ExpansionQuestObjectiveTravelConfig m_TravelConfig;

	override bool OnEventStart()
	{
	#ifdef EXPANSIONMODQUESTSOBJECTIVEDEBUG
		auto trace = EXTrace.Start(EXTrace.QUESTS, this);
	#endif

		if (!super.OnEventStart())
			return false;

		if (!Class.CastTo(m_TravelConfig, m_ObjectiveConfig))
			return false;

		Init();

		return true;
	}

	protected void Init()
	{
	#ifdef EXPANSIONMODQUESTSOBJECTIVEDEBUG
		auto trace = EXTrace.Start(EXTrace.QUESTS, this);
	#endif

		//! Set objective position.
		m_Position = m_TravelConfig.GetPosition();
		if (m_Position == vector.Zero)
		{
			if (m_Quest.GetPlayer())
			{
				vector playerPos = m_Quest.GetPlayer().GetPosition();
				m_Position = GetRandomPointInCircle(playerPos, m_TravelConfig.GetMaxDistance());
				if (m_Position == vector.Zero)
					m_Position = playerPos;
				else
					m_Position[1] = g_Game.SurfaceY(m_Position[0], m_Position[2]);
			}
		}

		if (CanCreateMarkers())
			CreateMarkers();

		//! Create objective trigger.
		if (!ExpansionQuestModule.GetModuleInstance().QuestTriggerExists(m_Quest.GetQuestConfig().GetID(), GetObjectiveType(), m_ObjectiveConfig.GetID()))
			CreateTrigger(m_Position);

		g_Game.GetCallQueue(CALL_CATEGORY_SYSTEM).CallLater(DestinationCheck, 500);
	}

	protected void DestinationCheck()
	{
	#ifdef EXPANSIONMODQUESTSOBJECTIVEDEBUG
		auto trace = EXTrace.Start(EXTrace.QUESTS, this);
	#endif

		float currentDistance;
		if (!GetQuest().GetQuestConfig().IsGroupQuest())
		{
			vector playerPos = GetQuest().GetPlayer().GetPosition();
			currentDistance = vector.Distance(playerPos, m_Position);
		}
		else
		{
			//! Set the position of the group member that has the shortest distance to the target location
			//! as our current position if the quest is a group quest.
			array<vector> groupMemberPos = new array<vector>;
			set<string> memberUIDs = m_Quest.GetPlayerUIDs();
			foreach (string memberUID: memberUIDs)
			{
				PlayerBase groupPlayer = PlayerBase.GetPlayerByUID(memberUID);
				if (!groupPlayer)
					continue;

				groupMemberPos.Insert(groupPlayer.GetPosition());
			}

			float smallestDistance;
			int posIndex;
			bool firstSet = false;
			for (int p = 0; p < groupMemberPos.Count(); p++)
			{
				vector pos = groupMemberPos[p];
				float dist = vector.Distance(pos, m_Position);
				if (!firstSet)
				{
					smallestDistance = dist;
					posIndex = p;
					firstSet = true;
				}
				else if (firstSet && dist < smallestDistance)
				{
					smallestDistance = dist;
					posIndex = p;
				}
			}

			currentDistance = vector.Distance(groupMemberPos[posIndex], m_Position);
		}

		float maxDistance = m_TravelConfig.GetMaxDistance();
		if (currentDistance <= maxDistance)
		{
			ObjectivePrint("End and return TRUE");
			SetReachedLocation(true);
			return;
		}

		ObjectivePrint("End and return FALSE");
		SetReachedLocation(false);
	}

	override bool OnContinue()
	{
	#ifdef EXPANSIONMODQUESTSOBJECTIVEDEBUG
		auto trace = EXTrace.Start(EXTrace.QUESTS, this);
	#endif

		if (!super.OnContinue())
			return false;

		if (!Class.CastTo(m_TravelConfig, m_ObjectiveConfig))
			return false;

		if (IsCompleted())
		{
			if (!m_TravelConfig.TriggerOnExit())
			{
				m_DestinationReached = true;
				return true;
			}
		}

		Init();

		return true;
	}

	override bool OnCleanup()
	{
	#ifdef EXPANSIONMODQUESTSOBJECTIVEDEBUG
		auto trace = EXTrace.Start(EXTrace.QUESTS, this);
	#endif

		if (!Class.CastTo(m_TravelConfig, m_ObjectiveConfig))
			return false;

		int questID = m_Quest.GetQuestConfig().GetID();
		int objectiveType = GetObjectiveType();
		int objectiveID = m_TravelConfig.GetID();
		if (!ExpansionQuestModule.GetModuleInstance().IsOtherQuestInstanceActive(questID))
			ExpansionQuestModule.GetModuleInstance().RemoveObjectiveTrigger(questID, ExpansionObjectiveTriggerType.TRAVEL, objectiveType, objectiveID);

		if (!super.OnCleanup())
			return false;

		return true;
	}

	override bool CanComplete()
	{
	#ifdef EXPANSIONMODQUESTSOBJECTIVEDEBUG
		auto trace = EXTrace.Start(EXTrace.QUESTS, this);
	#endif

		return m_DestinationReached;
	}

	override void CreateMarkers()
	{
	#ifdef EXPANSIONMODQUESTSOBJECTIVEDEBUG
		auto trace = EXTrace.Start(EXTrace.QUESTS, this);
	#endif

		string markerName = m_TravelConfig.GetMarkerName();
		if (markerName != string.Empty)
			CreateObjectiveMarker(m_Position, markerName);
	}

	vector GetPosition()
	{
		return m_Position;
	}

	//! Used by the trigger
	void SetReachedLocation(bool state)
	{
	#ifdef EXPANSIONMODQUESTSOBJECTIVEDEBUG
		auto trace = EXTrace.Start(EXTrace.QUESTS, this);
	#endif
		ObjectivePrint("State: " + state);
		m_DestinationReached = state;

		if (state)
			RemoveObjectiveMarkers();

		m_Quest.QuestCompletionCheck(true);
	}

	void SetLocationPosition(vector pos)
	{
	#ifdef EXPANSIONMODQUESTSOBJECTIVEDEBUG
		auto trace = EXTrace.Start(EXTrace.QUESTS, this);
	#endif
		ObjectivePrint("Position: " + pos.ToString());
		m_Position = pos;
	}

	bool GetLocationState()
	{
		return m_DestinationReached;
	}

	protected void CreateTrigger(vector pos)
	{
		array<ExpansionObjectiveTriggerBase> triggers = new array<ExpansionObjectiveTriggerBase>;
		ExpansionTravelObjectiveSphereTrigger trigger = ExpansionTravelObjectiveSphereTrigger.Cast(g_Game.CreateObjectEx("ExpansionTravelObjectiveSphereTrigger", pos, ECE_LOCAL));
		if (!trigger)
			return;

		trigger.SetPosition(pos);
		trigger.SetObjectiveData(m_Quest.GetQuestConfig().GetID(), GetObjectiveType(), m_TravelConfig.GetID());
		triggers.Insert(trigger);

		ExpansionQuestModule.GetModuleInstance().SetQuestTriggers(m_Quest.GetQuestConfig().GetID(), triggers);
	}

	protected vector GetRandomPointInCircle(vector pos, float radius)
    {
		m_PointSearchCount++;
		if (m_PointSearchCount == 100)
		{
			m_PointSearchCount = 0;
			return vector.Zero;
		}
		else
		{
			ObjectivePrint("m_PointSearchCount: " + m_PointSearchCount);
		}

		vector position = ExpansionMath.GetRandomPointInCircle(pos, radius);
		//! @note g_Game.SurfaceIsPond is INCREDIBLY slow, DO NOT USE EVER! g_Game.IsSea is ok
        if (ExpansionStatic.SurfaceIsWater(position[0], position[2]))
            return GetRandomPointInCircle(pos, radius);

        array<Object> position_objects = {};
        array<CargoBase> position_cargos = {};
        g_Game.GetObjectsAtPosition(position, 10, position_objects, position_cargos);
        if (position_objects.Count() > 0)
            return GetRandomPointInCircle(pos, radius);

        return position;
    }

	override bool CanCreateMarkers()
	{
		if (m_TravelConfig.GetMarkerName() == string.Empty)
			return false;

		return !m_DestinationReached;
	}

	override int GetObjectiveType()
	{
		return ExpansionQuestObjectiveType.TRAVEL;
	}
};
