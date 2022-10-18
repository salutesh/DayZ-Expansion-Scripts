/**
 * ExpansionQuestObjectiveTravelEvent.c
 *
 * DayZ Expansion Mod
 * www.dayzexpansion.com
 * © 2022 DayZ Expansion Mod Team
 *
 * This work is licensed under the Creative Commons Attribution-NonCommercial-NoDerivatives 4.0 International License.
 * To view a copy of this license, visit http://creativecommons.org/licenses/by-nc-nd/4.0/.
 *
*/

class ExpansionQuestObjectiveTravelEvent: ExpansionQuestObjectiveEventBase
{
	protected float m_UpdateQueueTimer = 0;
	protected const float UPDATE_TICK_TIME = 2.0;

	override bool OnStart()
	{
	#ifdef EXPANSIONTRACE
		auto trace = CF_Trace_0(ExpansionTracing.QUESTS, this, "OnStart");
	#endif

		if (!super.OnStart())
			return false;

		TravelEventStart();

		return true;
	}

	override bool OnContinue()
	{
	#ifdef EXPANSIONTRACE
		auto trace = CF_Trace_0(ExpansionTracing.QUESTS, this, "OnContinue");
	#endif

		if (!super.OnContinue())
			return false;

		TravelEventStart();

		return true;
	}

#ifdef EXPANSIONMODNAVIGATION
	override void OnRecreateClientMarkers()
	{
		super.OnRecreateClientMarkers();

		if (GetQuest().GetQuestState() == ExpansionQuestState.STARTED)
		{
			vector position = GetObjectiveConfig().GetPosition();
			string markerName = GetObjectiveConfig().GetMarkerName();

			if (markerName != string.Empty || position != vector.Zero)
				GetQuest().CreateClientMarker(position, markerName);
		}
	}
#endif

	override void OnUpdate(float timeslice)
	{
		super.OnUpdate(timeslice);

		m_UpdateQueueTimer += timeslice;
		if (m_UpdateQueueTimer >= UPDATE_TICK_TIME)
		{
			vector position = GetObjectiveConfig().GetPosition();
			float maxDistance = GetObjectiveConfig().GetMaxDistance();
			float currentDistance;
			array<vector> groupMemberPos = new array<vector>;

			if (!GetQuest().IsGroupQuest() && GetQuest() && GetQuest().GetPlayer())
			{
				vector playerPos = GetQuest().GetPlayer().GetPosition();
				currentDistance = vector.Distance(playerPos, position);
			}
		#ifdef EXPANSIONMODGROUPS
			else if (GetQuest().IsGroupQuest() && GetQuest() && GetQuest().GetGroup())
			{
				//! Set the position of the group member that has the shortest distance to the target location
				//! as our current position if the quest is a group quest.

				ExpansionPartyData group = GetQuest().GetGroup();
				if (!group)
					return;

				array<ref ExpansionPartyPlayerData> groupPlayers = group.GetPlayers();
				foreach (ExpansionPartyPlayerData playerGroupData: groupPlayers)
				{
					PlayerBase groupPlayer = PlayerBase.GetPlayerByUID(playerGroupData.GetID());
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
					float dist = vector.Distance(pos, position);
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

				currentDistance = vector.Distance(groupMemberPos[posIndex], position);
			}
		#endif

			position[1] = GetGame().SurfaceY(position[0], position[2]);

			if (position != vector.Zero && currentDistance <= maxDistance && !IsCompleted())
			{
			#ifdef EXPANSIONMODQUESTSOBJECTIVEDEBUG
				Print(ToString() + "::OnUpdate - Complete!");
			#endif
				SetCompleted(true);
				OnComplete();
			}

			m_UpdateQueueTimer = 0.0;
		}
	}

	protected void TravelEventStart()
	{
	#ifdef EXPANSIONTRACE
		auto trace = CF_Trace_0(ExpansionTracing.QUESTS, this, "TravelEventStart");
	#endif

		vector position = GetObjectiveConfig().GetPosition();
		string markerName = GetObjectiveConfig().GetMarkerName();

	#ifdef EXPANSIONMODNAVIGATION
		if (markerName != string.Empty || position != vector.Zero)
			GetQuest().CreateClientMarker(position, markerName);
	#endif
	}

	vector GetPosition()
	{
		return GetObjectiveConfig().GetPosition();
	}

	override int GetObjectiveType()
	{
		return ExpansionQuestObjectiveType.TRAVEL;
	}
};