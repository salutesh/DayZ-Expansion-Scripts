/**
 * ExpansionQuestObjectiveAIVIPEvent.c
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
class ExpansionQuestObjectiveAIVIPEvent: ExpansionQuestObjectiveEventBase
{
	private float m_UpdateQueueTimer = 0;
	private const float UPDATE_TICK_TIME = 2.0;

	private eAIBase m_VIP;
	private eAIGroup m_Group;

	//! Event called when the player starts the quest
	override void OnStart()
	{
		ObjectivePrint(ToString() + "::OnStart - Start");

		CreateVIP();

		super.OnStart();

	#ifdef EXPANSIONMODNAVIGATION
		vector markerPosition = GetObjectiveConfig().GetPosition();
		string markerName = GetObjectiveConfig().GetMarkerName();
		GetQuest().CreateClientMarker(markerPosition, markerName);
	#endif

		ObjectivePrint(ToString() + "::OnStart - End");
	}

	//! Event called when the player continues the quest after a server restart/reconnect
	override void OnContinue()
	{
		ObjectivePrint(ToString() + "::OnContinue - Start");

		CreateVIP();

	#ifdef EXPANSIONMODNAVIGATION
		vector markerPosition = GetObjectiveConfig().GetPosition();
		string markerName = GetObjectiveConfig().GetMarkerName();
		GetQuest().CreateClientMarker(markerPosition, markerName);
	#endif

		super.OnContinue();

		ObjectivePrint(ToString() + "::OnContinue - End");
	}

	override void OnComplete()
	{
		EmoteManager npcEmoteManager = m_VIP.GetEmoteManager();
		if (!npcEmoteManager.IsEmotePlaying())
		{
			npcEmoteManager.PlayEmote(EmoteConstants.ID_EMOTE_GREETING);
			GetGame().GetCallQueue(CALL_CATEGORY_SYSTEM).CallLater(npcEmoteManager.ServerRequestEmoteCancel, 2000);
		}

		m_Group.SetLeader(m_VIP);
		m_Group.AddWaypoint(GetObjectiveConfig().GetPosition());

		GetGame().GetCallQueue(CALL_CATEGORY_SYSTEM).CallLater(GetGame().ObjectDelete, 10000, false, m_VIP);

		super.OnComplete();
	}

	//! Event called when the quest gets cleaned up (server shutdown/player disconnect).
	override void OnCleanup()
	{
		ObjectivePrint(ToString() + "::OnCleanup - Start");

		GetGame().ObjectDelete(m_VIP);

		super.OnCleanup();

		ObjectivePrint(ToString() + "::OnCleanup - End");
	}

	//! Event called when the quest gets manualy canceled by the player.
	override void OnCancel()
	{
		ObjectivePrint(ToString() + "::OnCancel - Start");

		GetGame().ObjectDelete(m_VIP);

		super.OnCancel();

		ObjectivePrint(ToString() + "::OnCancel - End");
	}

	void OnEntityKilled(EntityAI victim, EntityAI killer)
	{
	#ifdef EXPANSIONTRACE
		auto trace = CF_Trace_0(ExpansionTracing.QUESTS, this, "OnEntityKilled");
	#endif

		eAIBase victimAI = eAIBase.Cast(victim);
		if (victimAI && victim == m_VIP)
		{
			GetQuest().GetQuestModule().CancelQuestServer(GetQuest().GetQuestConfig().GetID(), GetQuest().GetPlayer().GetIdentity());
		}
	}

	private void CreateVIP()
	{
		ExpansionQuestObjectiveAIVIP vip = GetObjectiveConfig().GetAIVIP();
		if (!vip)
			return;

		//vector pos = GetQuest().GetPlayer().GetPosition();
		m_Group = eAIGroup.GetGroupByLeader(GetQuest().GetPlayer());
		if (!m_Group)
			return;

		m_VIP = SpawnAI_VIP(GetQuest().GetPlayer(), vip.GetNPCLoadoutFile());
		if (!m_VIP)
			return;

		m_VIP.eAI_SetPassive();

		#ifdef EXPANSIONMODAI
		m_Group.SetWaypointBehaviour(eAIWaypointBehavior.ALTERNATE);
		#else
		GetGame().GetCallQueue(CALL_CATEGORY_SYSTEM).CallLater(m_VIP.RequestTransition, 10000, false, "Rejoin");
		m_VIP.SetAI(m_Group);
		#endif
	}

	eAIBase SpawnAI_VIP(PlayerBase owner, string loadout = "HumanLoadout.json")
	{
		#ifdef EAI_TRACE
		auto trace = CF_Trace_0(this, "SpawnAI_VIP");
		#endif

		eAIBase ai;
		if (!Class.CastTo(ai, GetGame().CreateObject(GetRandomAI(), owner.GetPosition()))) return null;

		ai.SetGroup(eAIGroup.GetGroupByLeader(owner));

		ExpansionHumanLoadout.Apply(ai, loadout, false);

		return ai;
	}

	override void OnUpdate(float timeslice)
	{
		super.OnUpdate(timeslice);

		//! TODO: Check if OnUpdate is still running after Quest got auto-canceled when timeout reached
		if (!m_VIP)
			return;

		m_UpdateQueueTimer += timeslice;
		if (m_UpdateQueueTimer >= UPDATE_TICK_TIME)
		{
			vector position = GetObjectiveConfig().GetPosition();
			float maxDistance = GetObjectiveConfig().GetMaxDistance();

			//! Set the position of the VIP AI NPC for the distance to the target location as our current position
			vector vipPos = m_VIP.GetPosition();
			float currentDistance = vector.Distance(vipPos, position);

			position[1] = GetGame().SurfaceY(position[0], position[2]);

			if (maxDistance < 10)
				maxDistance = 10.0;

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
		return ExpansionQuestObjectiveType.AIVIP;
	}
};
#endif