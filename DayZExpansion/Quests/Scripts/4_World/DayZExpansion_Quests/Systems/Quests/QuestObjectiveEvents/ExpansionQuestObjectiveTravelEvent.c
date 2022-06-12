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
	private float m_UpdateQueueTimer = 0;
	private const float UPDATE_TICK_TIME = 2.0;
	
	override void OnStart()
	{
		vector position = GetObjectiveConfig().GetPosition();
		string markerName = GetObjectiveConfig().GetMarkerName();

	#ifdef EXPANSIONMODNAVIGATION
		GetQuest().CreateClientMarker(position, markerName);
	#endif

		super.OnStart();
	}

	override void OnContinue()
	{
		vector position = GetObjectiveConfig().GetPosition();
		string markerName = GetObjectiveConfig().GetMarkerName();

	#ifdef EXPANSIONMODNAVIGATION
		GetQuest().CreateClientMarker(position, markerName);
	#endif

		super.OnContinue();
	}

#ifdef EXPANSIONMODNAVIGATION
	override void OnRecreateClientMarkers()
	{
		super.OnRecreateClientMarkers();

		if (GetQuest().GetQuestState() == ExpansionQuestState.STARTED)
		{
			vector position = GetObjectiveConfig().GetPosition();
			string markerName = GetObjectiveConfig().GetMarkerName();

			GetQuest().CreateClientMarker(position, markerName);
		}
	}
#endif

	override void OnUpdate(float timeslice)
	{
		if (!GetObjectiveConfig() || !GetQuest() || !GetQuest().GetPlayer() || !IsInitialized())
			return;
		
		m_UpdateQueueTimer += timeslice;
		if (m_UpdateQueueTimer >= UPDATE_TICK_TIME)
		{
			vector position = GetObjectiveConfig().GetPosition();
			float maxDistance = GetObjectiveConfig().GetMaxDistance();
			float currentDistance;
	
			//! Set the position of the group member that has the shortest distance to the target location
			//! as our current position if the quest is a group quest.
			array<vector> groupMemberPos = new array<vector>;
			if (GetQuest().IsGroupQuest())
			{
				ExpansionPartyData group = GetQuest().GetGroup();
				if (!group)
					return;
	
				for (int i = 0; i < group.GetPlayers().Count(); i++)
				{
					ExpansionPartyPlayerData playerGroupData = group.GetPlayers()[i];
					if (!playerGroupData)
						continue;
	
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
			else
			{
				vector playerPos = GetQuest().GetPlayer().GetPosition();
				currentDistance = vector.Distance(playerPos, position);
			}
	
			position[1] = GetGame().SurfaceY(position[0], position[2]);
	
			if (position != vector.Zero && currentDistance <= maxDistance)
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

	vector GetPosition()
	{
		return GetObjectiveConfig().GetPosition();
	}
	
	override int GetObjectiveType()
	{
		return ExpansionQuestObjectiveType.TRAVEL;
	}
};