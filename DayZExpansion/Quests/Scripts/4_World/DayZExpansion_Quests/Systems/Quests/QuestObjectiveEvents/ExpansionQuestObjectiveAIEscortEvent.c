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
	protected eAIGroup m_Group;
	protected ExpansionEscortObjectiveSphereTrigger m_ObjectiveTrigger;
	protected bool m_DestinationReached;
	protected ref ExpansionQuestObjectiveAIEscortConfig m_Config;
	protected vector m_ObjectivePos;

	//! Event called when the player starts the quest.
	override bool OnEventStart()
	{
		auto trace = EXTrace.Start(EXTrace.QUESTS, this);

		if (!super.OnEventStart())
			return false;

		if (!Class.CastTo(m_Config, m_ObjectiveConfig))
			return false;

		m_ObjectivePos = m_Config.GetPosition();

		CreateVIP();

	#ifdef EXPANSIONMODNAVIGATION
		if (m_Config.GetMarkerName() != string.Empty)
			CreateMarkers();
	#endif

		if (!m_ObjectiveTrigger)
			CreateTrigger(m_ObjectivePos);

		return true;
	}

	//! Event called when the player starts the quest.
	override bool OnContinue()
	{
		auto trace = EXTrace.Start(EXTrace.QUESTS, this);

		if (!super.OnContinue())
			return false;

		if (!Class.CastTo(m_Config, m_ObjectiveConfig))
			return false;

		m_ObjectivePos = m_Config.GetPosition();

		//! Only create the VIP and trigger when not already completed!
		if (!IsCompleted())
		{
			CreateVIP();

			if (!m_ObjectiveTrigger)
				CreateTrigger(m_ObjectivePos);

		#ifdef EXPANSIONMODNAVIGATION
			if (m_Config.GetMarkerName() != string.Empty)
				CreateMarkers();
		#endif
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
		auto trace = EXTrace.Start(EXTrace.QUESTS, this);

		if (!super.OnComplete())
			return false;

		EmoteManager npcEmoteManager = m_VIP.GetEmoteManager();
		if (!npcEmoteManager.IsEmotePlaying())
		{
			npcEmoteManager.PlayEmote(EmoteConstants.ID_EMOTE_GREETING);
			GetGame().GetCallQueue(CALL_CATEGORY_SYSTEM).CallLater(npcEmoteManager.ServerRequestEmoteCancel, 2000);
		}

		m_Group.SetLeader(m_VIP);
		m_Group.AddWaypoint(m_ObjectivePos);

		GetGame().GetCallQueue(CALL_CATEGORY_SYSTEM).CallLater(DeleteVIP, 10000);

		auto player = m_Quest.GetPlayer();
		player.SetGroup(player.Expansion_GetFormerGroup());

		ObjectivePrint("End and return TRUE.");

		return true;
	}

	//! Event called when the quest gets cleaned up (server shutdown/player disconnect).
	override bool OnCleanup()
	{
		auto trace = EXTrace.Start(EXTrace.QUESTS, this);

		if (!super.OnCleanup())
			return false;

		DeleteVIP();

		if (m_ObjectiveTrigger)
			GetGame().ObjectDelete(m_ObjectiveTrigger);

		auto player = m_Quest.GetPlayer();
		player.SetGroup(player.Expansion_GetFormerGroup());

		ObjectivePrint("End and return TRUE.");

		return true;
	}

#ifdef EXPANSIONMODNAVIGATION
	override void CreateMarkers()
	{
		if (!m_Config)
			return;

		vector markerPosition = m_ObjectivePos;
		string markerName = m_Config.GetMarkerName();
		CreateObjectiveMarker(markerPosition, markerName);
	}
#endif

	override void OnEntityKilled(EntityAI victim, EntityAI killer, Man killerPlayer = NULL)
	{
		auto trace = EXTrace.Start(EXTrace.QUESTS, this);

		if (killer == victim || killer == null)
			return;

		eAIBase victimAI = eAIBase.Cast(victim);
		if (victimAI && victim == m_VIP)
		{
			m_Quest.SendNotification(StringLocaliser("VIP Killed"), new StringLocaliser("The VIP got killed. Objective failed.."), ExpansionIcons.GetPath("Error"), COLOR_EXPANSION_NOTIFICATION_ERROR);
			m_Quest.CancelQuest();
		}
	}

	protected void DeleteVIP()
	{
		auto trace = EXTrace.Start(EXTrace.QUESTS, this);

		if (!m_VIP)
			return;

		//! Make sure to move AI out of the way, otherwise invisible collision box will be left behind when deleting
		m_VIP.SetPosition("0 0 0");
		GetGame().ObjectDelete(m_VIP);
	}

	protected void CreateVIP()
	{
		auto trace = EXTrace.Start(EXTrace.QUESTS, this);

		ExpansionQuestObjectiveAIVIP vip = m_Config.GetAIVIP();
		if (!vip)
			return;

		m_Group = eAIGroup.GetGroupByLeader(m_Quest.GetPlayer(), true, null, false);
		m_VIP = SpawnAI_VIP(m_Quest.GetPlayer(), vip.GetNPCLoadoutFile(), vip.GetNPCClassName());
		if (!m_VIP)
			return;

		m_VIP.eAI_SetPassive();
		m_Group.SetWaypointBehaviour(eAIWaypointBehavior.ALTERNATE);
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

		eAIBase ai;
		if (!Class.CastTo(ai, GetGame().CreateObject(className, owner.GetPosition())))
			return null;

		ai.SetGroup(eAIGroup.GetGroupByLeader(owner));
		ai.Expansion_SetCanBeLooted(m_Config.CanLootAI());
		ai.Expansion_SetQuestVIP(true);
		ExpansionHumanLoadout.Apply(ai, loadout, false);

		return ai;
	}

	protected void CreateTrigger(vector pos)
	{
		auto trace = EXTrace.Start(EXTrace.QUESTS, this);

		Class.CastTo(m_ObjectiveTrigger, GetGame().CreateObjectEx("ExpansionEscortObjectiveSphereTrigger", pos, ECE_NONE));
		m_ObjectiveTrigger.SetPosition(pos);
		m_ObjectiveTrigger.SetObjectiveData(Math.Round(m_Config.GetMaxDistance()), this);
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
		auto trace = EXTrace.Start(EXTrace.QUESTS, this);
		ObjectivePrint("m_DestinationReached: " + m_DestinationReached);
		return m_DestinationReached;
	}

	//! Used by the trigger
	void SetReachedLocation(bool state)
	{
		auto trace = EXTrace.Start(EXTrace.QUESTS, this);

		ObjectivePrint("State: " + state);
		m_DestinationReached = state;
		m_Quest.QuestCompletionCheck(true);
	}

	void OnDissmissAIGroup()
	{
		m_Quest.SendNotification(StringLocaliser("VIP Group Dismissed"), new StringLocaliser("The group with the VIP got dismissed. Objective failed.."), ExpansionIcons.GetPath("Error"), COLOR_EXPANSION_NOTIFICATION_ERROR);
		m_Quest.CancelQuest();
	}

	override int GetObjectiveType()
	{
		return ExpansionQuestObjectiveType.AIESCORT;
	}
};
#endif