/**
 * ExpansionQuestObjectiveEventBase.c
 *
 * DayZ Expansion Mod
 * www.dayzexpansion.com
 * © 2022 DayZ Expansion Mod Team
 *
 * This work is licensed under the Creative Commons Attribution-NonCommercial-NoDerivatives 4.0 International License.
 * To view a copy of this license, visit http://creativecommons.org/licenses/by-nc-nd/4.0/.
 *
*/

class ExpansionQuestObjectiveEventBase
{
	protected int m_Index = -1;
	protected ExpansionQuest m_Quest;
	protected bool m_Completed = false;
	protected bool m_Initialized = false;
	protected bool m_Active = false;
	protected ref ExpansionQuestObjectiveConfig m_ObjectiveConfig;
	protected int m_TimeLimit = -1;
	protected float m_TimeLimitTimer = 0;
	protected const float UPDATE_TIME_LIMIT_TICK_TIME = 1.0;
	protected float m_TimeLimitSyncTimer = 0;
	protected const float UPDATE_TIME_SYNC_TIME = 10.0;

	protected ref ExpansionQuestsPlayerInventory m_PlayerEntityInventory;
	protected ref array<EntityAI> m_PlayerItems;
#ifdef EXPANSIONMODGROUPS
	protected ref ExpansionQuestsGroupInventory m_GroupEntityInventory;
	protected ref array<EntityAI> m_GroupItems;
#endif

	void ExpansionQuestObjectiveEventBase(ExpansionQuest quest)
	{
		m_Quest = quest;
	}

	void SetIndex(int index)
	{
		m_Index = index;
	}

	int GetIndex()
	{
		return m_Index;
	}

	ExpansionQuest GetQuest()
	{
		return m_Quest;
	}

	bool IsInitialized()
	{
		return m_Initialized;
	}

	bool IsActive()
	{
		return m_Active;
	}

	void SetIsActive(bool state)
	{
		m_Active = state;
	}

	bool IsCompleted()
	{
		return m_Completed;
	}

	void SetCompleted(bool state)
	{
		m_Completed = state;
	}

	void SetInitialized(bool state)
	{
		m_Initialized = state;
	}

	void SetObjectiveConfig(ExpansionQuestObjectiveConfig config)
	{
		m_ObjectiveConfig = config;
	}

	void SetTimeLimit(int time)
	{
		m_TimeLimit = time;
	}

	int GetTimeLimit()
	{
		return m_TimeLimit;
	}

	void OnTimeLimitReached()
	{
		CancelQuest();
	}

	void CancelQuest()
	{
		if (!GetQuest().GetPlayer().GetIdentity())
			return;

		GetQuest().GetQuestModule().CancelQuestServer(GetQuest().GetQuestConfig().GetID(), GetQuest().GetPlayer().GetIdentity());
	}

	//! Event called when the player starts the quest
	bool OnStart()
	{
		SetInitialized(true);
		SetIsActive(true);

		if (!OnEventStart())
			return false;

		return true;
	}

	bool OnEventStart(bool continues = false)
	{
		return true;
	}

	//! Event called when objective is completed
	bool OnComplete()
	{
		GetQuest().CompletionCheck();

		return true;
	}

	bool OnIncomplete()
	{
		GetQuest().CompletionCheck();
	#ifdef EXPANSIONMODNAVIGATION
		OnRecreateClientMarkers();
	#endif

		return true;
	}

	//! Event called when quest is completed and turned-in
	bool OnTurnIn()
	{
		SetInitialized(false);
		SetIsActive(false);

		return true;
	}

	bool OnCancel()
	{
		SetInitialized(false);
		SetIsActive(false);

		return true;
	}

	//! Event called when the player continues the quest after a server restart/reconnect
	bool OnContinue()
	{
		SetInitialized(true);
		SetIsActive(true);

		if (!OnEventStart(true))
			return false;

		return true;
	}

	//! Event called when the quest gets cleaned up
	bool OnCleanup()
	{
		SetInitialized(false);

		return true;
	}

	//! Event called when the quest markers should get recreated
#ifdef EXPANSIONMODNAVIGATION
	void OnRecreateClientMarkers();
#endif

	//! Event called for group quests only when a group member joins/rejoins the quest group
#ifdef EXPANSIONMODGROUPS
	void OnGroupMemberJoined(string playerUID)
	{
	#ifdef EXPANSIONMODNAVIGATION
		OnRecreateClientMarkers();
	#endif
	}

	//! Event called for group quests only when a group member leaves the quest group
	void OnGroupMemberLeave(string playerUID);
#endif

	protected bool DestinationCheck()
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
				return false;

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
		if (position != vector.Zero && currentDistance <= maxDistance)
			return true;

		return false;
	}

	void OnUpdate(float timeslice)
	{
		if (!IsInitialized() || !IsActive() || !GetQuest() || !GetQuest().GetPlayer() || !GetObjectiveConfig())
			return;

		if (m_TimeLimit > -1)
		{
			m_TimeLimitTimer += timeslice;
			if (m_TimeLimitTimer >= UPDATE_TIME_LIMIT_TICK_TIME)
			{
				m_TimeLimit -= UPDATE_TIME_LIMIT_TICK_TIME;
				if (m_TimeLimit <= 0)
				{
					OnTimeLimitReached();
					m_TimeLimit = -1;
				}

				m_TimeLimitTimer = 0;
			}

			m_TimeLimitSyncTimer +=timeslice;
			if (m_TimeLimit > 0 && m_TimeLimitSyncTimer >= UPDATE_TIME_SYNC_TIME)
			{
				GetQuest().UpdateQuest();
				m_TimeLimitSyncTimer = 0;
			}
		}
	}

	ExpansionQuestObjectiveConfig GetObjectiveConfig()
	{
		return m_ObjectiveConfig;
	}

	bool HasDynamicState()
	{
		return false;
	}
	
	protected void EnumeratePlayerInventory(PlayerBase player)
	{
		if (!player || !player.IsAlive() || !player.GetInventory())
			return;

		m_PlayerEntityInventory = new ExpansionQuestsPlayerInventory(player);
	}

#ifdef EXPANSIONMODGROUPS
	protected void EnumerateGroupInventory(ExpansionPartyData group)
	{
		if (!group)
			return;

		m_GroupEntityInventory = new ExpansionQuestsGroupInventory(group);
	}
#endif


	void QuestDebug()
	{
		ObjectivePrint("------------------------------------------------------------");
		ObjectivePrint(ToString() + "::ObjectiveDebug - Objective index: " + m_Index);
		ObjectivePrint(ToString() + "::ObjectiveDebug - Objective quest: " + m_Quest);
		ObjectivePrint(ToString() + "::ObjectiveDebug - Objective completed: " + m_Completed);
		ObjectivePrint(ToString() + "::ObjectiveDebug - Objective Initialized: " + m_Initialized);
		ObjectivePrint(ToString() + "::ObjectiveDebug - Objective active: " + m_Active);
		ObjectivePrint(ToString() + "::ObjectiveDebug - Objective config: " + m_ObjectiveConfig);
		ObjectivePrint(ToString() + "::ObjectiveDebug - Objective time limit: " + m_TimeLimit);
		ObjectivePrint(ToString() + "::ObjectiveDebug - Objective time remaining: " + GetGame().GetCallQueue(CALL_CATEGORY_SYSTEM).GetRemainingTime(OnTimeLimitReached));
		ObjectivePrint("------------------------------------------------------------");
	}

	void ObjectivePrint(string text)
	{
	#ifdef EXPANSIONMODQUESTSOBJECTIVEDEBUG
		Print(text);
	#endif
	}

	int GetObjectiveType()
	{
		return ExpansionQuestObjectiveType.NONE;
	}
};