/**
 * ExpansionQuestObjectiveAIEscortEvent.c
 *
 * DayZ Expansion Mod
 * www.dayzexpansion.com
 * © 2023 DayZ Expansion Mod Team
 *
 * This work is licensed under the Creative Commons Attribution-NonCommercial-NoDerivatives 4.0 International License.
 * To view a copy of this license, visit http://creativecommons.org/licenses/by-nc-nd/4.0/.
 *
*/

#ifdef EXPANSIONMODAI
class ExpansionQuestObjectiveAIEscortEvent: ExpansionQuestObjectiveEventBase
{
	protected eAIBase m_VIP;
	protected bool m_DestinationReached;
	protected ref ExpansionQuestObjectiveAIEscortConfig m_AIEscortConfig;
	protected vector m_ObjectivePos;
	protected vector m_LastVIPPos;
	protected ExpansionEscortObjectiveSphereTrigger m_Trigger;

	//! Event called when the player starts the quest.
	override bool OnEventStart()
	{
	#ifdef EXPANSIONMODQUESTSOBJECTIVEDEBUG
		auto trace = EXTrace.Start(EXTrace.QUESTS, this);
	#endif

		if (!super.OnEventStart())
			return false;

		if (!Class.CastTo(m_AIEscortConfig, m_ObjectiveConfig))
			return false;

		m_ObjectivePos = m_AIEscortConfig.GetPosition();

		CreateVIP();

		if (m_AIEscortConfig.GetMarkerName() != string.Empty)
			CreateMarkers();

		if (!m_Trigger)
			CreateTrigger(m_ObjectivePos);

		return true;
	}

	//! Event called when the player starts the quest.
	override bool OnContinue()
	{
	#ifdef EXPANSIONMODQUESTSOBJECTIVEDEBUG
		auto trace = EXTrace.Start(EXTrace.QUESTS, this);
	#endif

		if (!super.OnContinue())
			return false;

		if (!Class.CastTo(m_AIEscortConfig, m_ObjectiveConfig))
			return false;

		m_ObjectivePos = m_AIEscortConfig.GetPosition();
		if (m_ObjectivePos == vector.Zero)
			return false;

		//! Only create the VIP and trigger when not already completed!
		if (!IsCompleted())
		{
			CreateVIP();

			if (!m_Trigger)
				CreateTrigger(m_ObjectivePos);

			if (m_AIEscortConfig.GetMarkerName() != string.Empty)
				CreateMarkers();
		}
		else
		{
			m_DestinationReached = true;
		}

		m_Quest.QuestCompletionCheck(true);

		return true;
	}

	override bool OnComplete()
	{
	#ifdef EXPANSIONMODQUESTSOBJECTIVEDEBUG
		auto trace = EXTrace.Start(EXTrace.QUESTS, this);
	#endif

		if (!super.OnComplete())
			return false;

		if (m_VIP)
		{
			EmoteManager npcEmoteManager = m_VIP.GetEmoteManager();
			if (!npcEmoteManager.IsEmotePlaying())
			{
				npcEmoteManager.PlayEmote(EmoteConstants.ID_EMOTE_GREETING);
				g_Game.GetCallQueue(CALL_CATEGORY_SYSTEM).CallLater(npcEmoteManager.ServerRequestEmoteCancel, 2000);
			}

			eAIFaction faction = new eAIFactionInvincibleObservers();

			auto group = eAIGroup.CreateGroup(faction);
			m_VIP.SetGroup(group);
			group.SetWaypointBehaviour(eAIWaypointBehavior.ONCE);
			group.AddWaypoint(m_ObjectivePos);

			g_Game.GetCallQueue(CALL_CATEGORY_SYSTEM).CallLater(DeleteVIP, 10000);

			auto player = m_Quest.GetPlayer();
			if (!player.Expansion_GetFormerGroup())
				player.SetGroup(null);
		}

		ObjectivePrint("End and return TRUE.");

		return true;
	}

	//! Event called when the quest gets cleaned up (server shutdown/player disconnect).
	override bool OnCleanup()
	{
	#ifdef EXPANSIONMODQUESTSOBJECTIVEDEBUG
		auto trace = EXTrace.Start(EXTrace.QUESTS, this);
	#endif

		if (!super.OnCleanup())
			return false;

		if (m_Trigger)
			g_Game.ObjectDelete(m_Trigger);

		if (m_VIP)
		{
			DeleteVIP();

			auto player = m_Quest.GetPlayer();
			if (!player.Expansion_GetFormerGroup())
				player.SetGroup(null);
		}
		
		ObjectivePrint("End and return TRUE.");

		return true;
	}

	override void CreateMarkers()
	{
		vector markerPosition = m_ObjectivePos;
		string markerName = m_AIEscortConfig.GetMarkerName();
		CreateObjectiveMarker(markerPosition, markerName);
	}

	override void OnEntityKilled(EntityAI victim, EntityAI killer, Man killerPlayer = null, map<Man, ref ExpansionEntityHitInfo> hitMap = null)
	{
	#ifdef EXPANSIONMODQUESTSOBJECTIVEDEBUG
		auto trace = EXTrace.Start(EXTrace.QUESTS, this);
	#endif

		eAIBase victimAI = eAIBase.Cast(victim);
		if (victimAI && victim == m_VIP)
		{
			m_Quest.SendNotification(StringLocaliser("VIP Killed"), new StringLocaliser("The VIP got killed. Objective failed..."), ExpansionIcons.GetPath("Error"), COLOR_EXPANSION_NOTIFICATION_ERROR);
			m_Quest.CancelQuest();
		}
	}

	protected void DeleteVIP()
	{
	#ifdef EXPANSIONMODQUESTSOBJECTIVEDEBUG
		auto trace = EXTrace.Start(EXTrace.QUESTS, this);
	#endif

		if (!m_VIP)
			return;
		
		g_Game.GetCallQueue(CALL_CATEGORY_SYSTEM).Remove(this.UpdateVIPPosition);
		//! Make sure to move AI out of the way, otherwise invisible collision box will be left behind when deleting
		m_VIP.SetPosition("0 0 0");
		g_Game.ObjectDelete(m_VIP);
	}

	protected void CreateVIP()
	{
	#ifdef EXPANSIONMODQUESTSOBJECTIVEDEBUG
		auto trace = EXTrace.Start(EXTrace.QUESTS, this);
	#endif

		if (!m_AIEscortConfig)
			return;

		m_VIP = SpawnAI_VIP(m_Quest.GetPlayer(), m_AIEscortConfig.GetLoadout(), m_AIEscortConfig.GetNPCClassName());
		if (!m_VIP)
			return;

		m_VIP.eAI_SetPassive();
		if (!m_VIP.m_Expansion_NetsyncData)
			m_VIP.m_Expansion_NetsyncData = new ExpansionNetsyncData(m_VIP);
	
		m_VIP.m_Expansion_NetsyncData.Set(0, m_AIEscortConfig.GetNPCName());
		m_VIP.m_Expansion_NetsyncData.Set(1, "set:expansion_iconset image:icon_profile");
		
		g_Game.GetCallQueue(CALL_CATEGORY_SYSTEM).CallLater(this.UpdateVIPPosition, 10 * 1000, true);
	}

	protected eAIBase SpawnAI_VIP(PlayerBase owner, string loadout = "HumanLoadout", string className = "")
	{
		TStringArray validAITypes = {"eAIBase"};

		if (className != string.Empty)
		{
			if (!ExpansionStatic.IsAnyOf(className, validAITypes))
				className = GetRandomAI();
		}
		else
		{
			className = GetRandomAI();
		}
		
		vector spawnPos = owner.GetPosition();
		if (m_LastVIPPos != vector.Zero)
			spawnPos = m_LastVIPPos;
		
		eAIBase ai;
		if (!Class.CastTo(ai, g_Game.CreateObject(className, spawnPos)))
			return null;

		eAIGroup group = owner.GetGroup();

		//! If player has no group, create new group with player as leader
		if (!group)
		{
			group = eAIGroup.GetGroupByLeader(owner);
		}
		else
		{
			owner.Expansion_SetFormerGroup(group);
			group.ClearWaypoints();
		}

		//! Add AI to player group
		ai.SetGroup(group);
		
		ai.Expansion_SetCanBeLooted(m_AIEscortConfig.CanBeLooted());
		ai.Expansion_SetQuestVIP(true);
		ExpansionHumanLoadout.Apply(ai, loadout, false);

		return ai;
	}

	protected void CreateTrigger(vector pos)
	{
	#ifdef EXPANSIONMODQUESTSOBJECTIVEDEBUG
		auto trace = EXTrace.Start(EXTrace.QUESTS, this);
	#endif

		m_Trigger = ExpansionEscortObjectiveSphereTrigger.Cast(g_Game.CreateObjectEx("ExpansionEscortObjectiveSphereTrigger", pos, ECE_LOCAL));
		if (!m_Trigger)
			return;

		m_Trigger.SetPosition(pos);
		m_Trigger.SetTriggerRadius(m_AIEscortConfig.GetMaxDistance());
		m_Trigger.SetObjectiveEvent(this);
	}

	vector GetPosition()
	{
		return m_ObjectivePos;
	}

	eAIBase GetAIVIP()
	{
		return m_VIP;
	}

	override bool CanComplete()
	{
	#ifdef EXPANSIONMODQUESTSOBJECTIVEDEBUG
		auto trace = EXTrace.Start(EXTrace.QUESTS, this);
	#endif
		ObjectivePrint("m_DestinationReached: " + m_DestinationReached);
		return m_DestinationReached;
	}

	//! Used by the trigger
	void SetReachedLocation(bool state)
	{
	#ifdef EXPANSIONMODQUESTSOBJECTIVEDEBUG
		auto trace = EXTrace.Start(EXTrace.QUESTS, this);
	#endif

		ObjectivePrint("State: " + state);
		m_DestinationReached = state;
		m_Quest.QuestCompletionCheck(true);
	}

	void OnDissmissAIGroup()
	{
		m_Quest.SendNotification(StringLocaliser("VIP Group Dismissed"), new StringLocaliser("The group with the VIP got dismissed. Objective failed..."), ExpansionIcons.GetPath("Error"), COLOR_EXPANSION_NOTIFICATION_ERROR);
		m_Quest.CancelQuest();
	}
	
	vector GetVIPPosition()
	{
		if (m_VIP)
			return m_VIP.GetPosition();
		
		return vector.Zero;
	}
	
	void SetLastVIPPosition(vector pos)
	{
		m_LastVIPPos = pos;
	}
	
	void UpdateVIPPosition()
	{
		m_LastVIPPos = GetVIPPosition();
		if (m_TimeLimit == -1 && m_Quest)
			m_Quest.UpdateQuest(false);
	}

	override int GetObjectiveType()
	{
		return ExpansionQuestObjectiveType.AIESCORT;
	}
};
#endif