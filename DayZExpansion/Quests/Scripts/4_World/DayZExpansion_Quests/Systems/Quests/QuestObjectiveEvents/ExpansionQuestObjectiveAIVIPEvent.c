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
	protected float m_UpdateQueueTimer = 0;
	protected const float UPDATE_TICK_TIME = 2.0;

	protected eAIBase m_VIP;
	protected eAIGroup m_Group;

	//! Event called when the player starts and continues the quest
	override bool OnEventStart(bool continues = false)
	{		
		CreateVIP();

	#ifdef EXPANSIONMODNAVIGATION
		OnRecreateClientMarkers();
	#endif
		
		return true;
	}
	
#ifdef EXPANSIONMODNAVIGATION
	override void OnRecreateClientMarkers()
	{
		vector markerPosition = GetObjectiveConfig().GetPosition();
		string markerName = GetObjectiveConfig().GetMarkerName();
		if (markerName != string.Empty || markerPosition != vector.Zero)
			GetQuest().CreateClientMarker(markerPosition, markerName);
	}
#endif

	override bool OnComplete()
	{
	#ifdef EXPANSIONTRACE
		auto trace = CF_Trace_0(ExpansionTracing.QUESTS, this, "OnComplete");
	#endif
		
		if (!super.OnComplete())
			return false;	

		EmoteManager npcEmoteManager = m_VIP.GetEmoteManager();
		if (!npcEmoteManager.IsEmotePlaying())
		{
			npcEmoteManager.PlayEmote(EmoteConstants.ID_EMOTE_GREETING);
			GetGame().GetCallQueue(CALL_CATEGORY_SYSTEM).CallLater(npcEmoteManager.ServerRequestEmoteCancel, 2000);
		}

		m_Group.SetLeader(m_VIP);
		m_Group.AddWaypoint(GetObjectiveConfig().GetPosition());

		GetGame().GetCallQueue(CALL_CATEGORY_SYSTEM).CallLater(GetGame().ObjectDelete, 10000, false, m_VIP);

		return true;
	}

	//! Event called when the quest gets cleaned up (server shutdown/player disconnect).
	override bool OnCleanup()
	{
	#ifdef EXPANSIONTRACE
		auto trace = CF_Trace_0(ExpansionTracing.QUESTS, this, "OnCleanup");
	#endif
		
		if (!super.OnCleanup())
			return false;	

		m_VIP.SetPosition("0 0 0");  //! Make sure to move AI out of the way, otherwise invisible collision box will be left behind when deleting
		GetGame().ObjectDelete(m_VIP);

		return true;
	}

	//! Event called when the quest gets manualy canceled by the player.
	override bool OnCancel()
	{
	#ifdef EXPANSIONTRACE
		auto trace = CF_Trace_0(ExpansionTracing.QUESTS, this, "OnCancel");
	#endif
		
		if (!super.OnCancel())
			return false;	

		GetGame().ObjectDelete(m_VIP);

		return true;
	}

	void OnEntityKilled(EntityAI victim, EntityAI killer, Man killerPlayer = NULL)
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

	protected void CreateVIP()
	{
	#ifdef EXPANSIONTRACE
		auto trace = CF_Trace_0(ExpansionTracing.QUESTS, this, "CreateVIP");
	#endif

		ExpansionQuestObjectiveAIVIP vip = GetObjectiveConfig().GetAIVIP();
		if (!vip)
			return;

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