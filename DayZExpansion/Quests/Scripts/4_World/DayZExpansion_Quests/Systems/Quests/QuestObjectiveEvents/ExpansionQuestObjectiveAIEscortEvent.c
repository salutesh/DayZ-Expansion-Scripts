/**
 * ExpansionQuestObjectiveAIEscortEvent.c
 *
 * DayZ Expansion Mod
 * www.dayzexpansion.com
 * © 2022 DayZ Expansion Mod Team
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

	//! Event called when the player starts the quest.
	override bool OnEventStart()
	{
		ObjectivePrint(ToString() + "::OnEventStart - Start");

		if (!super.OnEventStart())
			return false;

		if (!Class.CastTo(m_Config, m_ObjectiveConfig))
			return false;
		
		CreateVIP();

	#ifdef EXPANSIONMODNAVIGATION
		CreateMarkers();
	#endif

		if (!m_ObjectiveTrigger)
			CreateTrigger(m_Config.GetPosition());

		ObjectivePrint(ToString() + "::OnEventStart - End and return TRUE.");

		return true;
	}

	//! Event called when the player starts the quest.
	override bool OnContinue()
	{
		ObjectivePrint(ToString() + "::OnContinue - Start");

		if (!super.OnContinue())
			return false;
		
		if (!Class.CastTo(m_Config, m_ObjectiveConfig))
			return false;
		
		//! Only create the VIP and trigger when not already completed!
		if (m_Quest.GetQuestState() == ExpansionQuestState.STARTED)
		{
			CreateVIP();
	
			if (!m_ObjectiveTrigger)
				CreateTrigger(m_Config.GetPosition());
			
		#ifdef EXPANSIONMODNAVIGATION
			CreateMarkers();
		#endif
		}

		m_Quest.QuestCompletionCheck();
		
		ObjectivePrint(ToString() + "::OnContinue - End and return TRUE.");

		return true;
	}

	override bool OnComplete()
	{
		ObjectivePrint(ToString() + "::OnComplete - Start");

		if (!super.OnComplete())
			return false;

		EmoteManager npcEmoteManager = m_VIP.GetEmoteManager();
		if (!npcEmoteManager.IsEmotePlaying())
		{
			npcEmoteManager.PlayEmote(EmoteConstants.ID_EMOTE_GREETING);
			GetGame().GetCallQueue(CALL_CATEGORY_SYSTEM).CallLater(npcEmoteManager.ServerRequestEmoteCancel, 2000);
		}

		m_Group.SetLeader(m_VIP);
		m_Group.AddWaypoint(m_Config.GetPosition());

		GetGame().GetCallQueue(CALL_CATEGORY_SYSTEM).CallLater(DeleteVIP, 10000);

		auto player = m_Quest.GetPlayer();
		player.SetGroup(player.Expansion_GetFormerGroup());

		ObjectivePrint(ToString() + "::OnComplete - End and return TRUE.");

		return true;
	}

	//! Event called when the quest gets cleaned up (server shutdown/player disconnect).
	override bool OnCleanup()
	{
		ObjectivePrint(ToString() + "::OnCleanup - Start");

		if (!super.OnCleanup())
			return false;

		DeleteVIP();

		if (m_ObjectiveTrigger)
			GetGame().ObjectDelete(m_ObjectiveTrigger);

		ObjectivePrint(ToString() + "::OnCleanup - End and return TRUE.");

		return true;
	}

	//! Event called when the quest gets canceled by the player.
	override bool OnCancel()
	{
		ObjectivePrint(ToString() + "::OnCancel - Start");

		if (!super.OnCancel())
			return false;

		m_VIP.SetPosition("0 0 0");  //! Make sure to move AI out of the way, otherwise invisible collision box will be left behind when deleting
		GetGame().ObjectDelete(m_VIP);

		if (m_ObjectiveTrigger)
			GetGame().ObjectDelete(m_ObjectiveTrigger);

		auto player = m_Quest.GetPlayer();
		player.SetGroup(player.Expansion_GetFormerGroup());

		ObjectivePrint(ToString() + "::OnCancel - End and return TRUE.");

		return true;
	}
	
#ifdef EXPANSIONMODNAVIGATION
	override void CreateMarkers()
	{
		if (!Class.CastTo(m_Config, m_ObjectiveConfig))
			return;

		vector markerPosition = m_Config.GetPosition();
		string markerName = m_Config.GetMarkerName();
		CreateObjectiveMarker(markerPosition, markerName);
	}
#endif

	void OnEntityKilled(EntityAI victim, EntityAI killer, Man killerPlayer = NULL)
	{
		ObjectivePrint(ToString() + "::OnEntityKilled - Start");

		eAIBase victimAI = eAIBase.Cast(victim);
		if (victimAI && victim == m_VIP)
		{
			m_Quest.SendNotification(StringLocaliser("VIP Killed"), new StringLocaliser("The VIP got killed. Objective failed.."), ExpansionIcons.GetPath("Error"), COLOR_EXPANSION_NOTIFICATION_ERROR);
			m_Quest.CancelQuest();
		}

		ObjectivePrint(ToString() + "::OnEntityKilled - End");
	}

	protected void DeleteVIP()
	{
		if (!m_VIP)
			return;

		//! Make sure to move AI out of the way, otherwise invisible collision box will be left behind when deleting
		m_VIP.SetPosition("0 0 0");
		GetGame().ObjectDelete(m_VIP);
	}

	protected void CreateVIP()
	{
		ObjectivePrint(ToString() + "::CreateVIP - Start");

		ExpansionQuestObjectiveAIVIP vip = m_Config.GetAIVIP();
		if (!vip)
			return;

		m_Group = eAIGroup.GetGroupByLeader(m_Quest.GetPlayer(), true, null, false);

		m_VIP = SpawnAI_VIP(m_Quest.GetPlayer(), vip.GetNPCLoadoutFile());
		if (!m_VIP)
			return;

		m_VIP.eAI_SetPassive();

	#ifdef EXPANSIONMODAI
		m_Group.SetWaypointBehaviour(eAIWaypointBehavior.ALTERNATE);
	#else
		GetGame().GetCallQueue(CALL_CATEGORY_SYSTEM).CallLater(m_VIP.RequestTransition, 10000, false, "Rejoin");
		m_VIP.SetAI(m_Group);
	#endif

		ObjectivePrint(ToString() + "::CreateVIP - End");
	}

	protected eAIBase SpawnAI_VIP(PlayerBase owner, string loadout = "HumanLoadout")
	{
		eAIBase ai;
		if (!Class.CastTo(ai, GetGame().CreateObject(GetRandomAI(), owner.GetPosition())))
			return null;

		ai.SetGroup(eAIGroup.GetGroupByLeader(owner));
		ExpansionHumanLoadout.Apply(ai, loadout, false);

		return ai;
	}

	protected void CreateTrigger(vector pos)
	{
		ObjectivePrint(ToString() + "::CreateTrigger - Start");

		Class.CastTo(m_ObjectiveTrigger, GetGame().CreateObjectEx("ExpansionEscortObjectiveSphereTrigger", pos, ECE_NONE));
		m_ObjectiveTrigger.SetPosition(pos);
		m_ObjectiveTrigger.SetObjectiveData(Math.Round(m_Config.GetMaxDistance()), this);

		ObjectivePrint(ToString() + "::CreateTrigger - End");
	}

	vector GetPosition()
	{
		return m_Config.GetPosition();
	}

	eAIBase GetAIVIP()
	{
		return m_VIP;
	}

	override bool CanComplete()
	{
		ObjectivePrint(ToString() + "::CanComplete - Start");
		ObjectivePrint(ToString() + "::CanComplete - m_DestinationReached: " + m_DestinationReached);

		bool conditionsResult = m_DestinationReached;
		if (!conditionsResult)
		{
			ObjectivePrint(ToString() + "::CanComplete - End and return: FALSE");
			return false;
		}

		ObjectivePrint(ToString() + "::CanComplete - End and return: TRUE");

		return super.CanComplete();
	}

	//! Used by the trigger
	void SetReachedLocation(bool state)
	{
		ObjectivePrint(ToString() + "::SetReachedLocation - Start");
		ObjectivePrint(ToString() + ":: SetReachedLocation - State: " + state);
		m_DestinationReached = state;
		m_Quest.QuestCompletionCheck();
		ObjectivePrint(ToString() + "::SetReachedLocation - End");
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