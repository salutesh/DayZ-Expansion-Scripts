/**
 * ExpansionQuestObjectiveAIPatrolEvent.c
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
class ExpansionQuestObjectiveAICampEvent: ExpansionQuestObjectiveAIEventBase
{
	protected ref ExpansionQuestObjectiveAICampConfig m_AICampConfig;

	override bool OnEventStart()
	{
	#ifdef EXPANSIONMODQUESTSOBJECTIVEDEBUG
		auto trace = EXTrace.Start(EXTrace.QUESTS, this);
	#endif

		if (!Class.CastTo(m_AICampConfig, m_ObjectiveConfig))
			return false;
		
		if (!super.OnEventStart())
			return false;

		return true;
	}

	override bool OnContinue()
	{
	#ifdef EXPANSIONMODQUESTSOBJECTIVEDEBUG
		auto trace = EXTrace.Start(EXTrace.QUESTS, this);
	#endif

		if (!Class.CastTo(m_AICampConfig, m_ObjectiveConfig))
			return false;
		
		if (!super.OnContinue())
			return false;

		return true;
	}

	override bool OnCleanup()
	{
	#ifdef EXPANSIONMODQUESTSOBJECTIVEDEBUG
		auto trace = EXTrace.Start(EXTrace.QUESTS, this);
	#endif

		if (!Class.CastTo(m_AICampConfig, m_ObjectiveConfig))
			return false;

		if (m_AICampConfig.GetInfectedDeletionRadius() > 0)
		{
			int questID = m_Quest.GetQuestConfig().GetID();
			int objectiveType = GetObjectiveType();
			int objectiveID = m_AICampConfig.GetID();
			if (!ExpansionQuestModule.GetModuleInstance().IsOtherQuestInstanceActive(questID))
				ExpansionQuestModule.GetModuleInstance().RemoveObjectiveTrigger(questID, ExpansionObjectiveTriggerType.AICAMP, objectiveType, objectiveID);
		}

		if (!super.OnCleanup())
			return false;
		
		return true;
	}

	override void OnEntityKilled(EntityAI victim, EntityAI killer, Man killerPlayer = null, map<Man, ref ExpansionEntityHitInfo> hitMap = null)
	{
	#ifdef EXPANSIONMODQUESTSOBJECTIVEDEBUG
		auto trace = EXTrace.Start(EXTrace.QUESTS, this);
	#endif

		if (!m_AICampConfig)
			return;

		//! If the ai camp need to be killed with a special weapon check incoming killer class type
		if (m_AICampConfig.GetAllowedWeapons().Count() > 0)
		{
			if (!ExpansionStatic.IsAnyOf(killer, m_AICampConfig.GetAllowedWeapons(), true))
			{
				ObjectivePrint("::OnEntityKilled - Entity got not killed with any allowed weapon! Skip..");
				return;
			}
		}
		
		//! Check if the killer player was in legit kill range.
		if (killerPlayer && !IsInRange(killerPlayer.GetPosition(), victim.GetPosition(), m_AICampConfig.GetMaxDistance(), m_AICampConfig.GetMinDistance()))
		{
			ObjectivePrint("::OnEntityKilled - Killer is out of legit kill range! Skip..");
			return;
		}
		
		//! If the target need to be hit at a certain zone we check the hit map for the last valid hit of the killer player on the victim entity.
		TStringArray allowedDamageZones = m_AICampConfig.GetAllowedDamageZones();
		if (allowedDamageZones.Count() > 0 && killerPlayer)
		{
			ObjectivePrint("::OnEntityKilled - Check if entity got hit at valid zone..");
			bool hasHitEntity = false;
			
			if (hitMap.Count() == 0)
				return;
			
			ExpansionEntityHitInfo hitInfo;
			if (!hitMap.Find(killerPlayer, hitInfo))
				return;
			
			string damageZone = hitInfo.GetZone();
			string killerUID;
			if (killerPlayer.GetIdentity())
				killerUID = killerPlayer.GetIdentity().GetId();
			ObjectivePrint("::OnEntityKilled - Last known hit zone for player with UID: " + killerUID + " | Zone: " + damageZone);
			int hitFound = allowedDamageZones.Find(damageZone);
			if (hitFound == -1)
			{
				ObjectivePrint("Entity killed was not hit on a valid damage zone! Skip..");
				return;
			}
		}

		super.OnEntityKilled(victim, killer, killerPlayer);
	}

	override protected void CheckQuestAIPatrol()
	{
	#ifdef EXPANSIONMODQUESTSOBJECTIVEDEBUG
		auto trace = EXTrace.Start(EXTrace.QUESTS, this);
	#endif

		if (!Class.CastTo(m_AICampConfig, m_ObjectiveConfig))
			return;

		CheckQuestAIPatrol(m_TotalUnitsAmount);
	}

	override void CreateQuestAIPatrol()
	{
	#ifdef EXPANSIONMODQUESTSOBJECTIVEDEBUG
		auto trace = EXTrace.Start(EXTrace.QUESTS, this);
	#endif

		if (!m_AICampConfig)
			return;

		m_UnitsToSpawn = m_TotalUnitsAmount - m_TotalKillCount;
		array<ref ExpansionQuestAISpawn> aiSpawns = m_AICampConfig.GetAISpawns();
		if (!aiSpawns)
			return;

		array<eAIQuestPatrol> questPatrols = new array<eAIQuestPatrol>;
		for (int i = 0; i < m_UnitsToSpawn; i++)
		{
			ExpansionQuestAISpawn aiSpawn = aiSpawns[i];
			eAIQuestPatrol questPatrol = aiSpawn.CreateAIQuestPatrol();
			questPatrols.Insert(questPatrol);
		}

		ExpansionQuestModule.GetModuleInstance().SetQuestPatrols(m_Quest.GetQuestConfig().GetID(), questPatrols);

		//! Create trigger and add it to global triggers array in quest module for further use when not created already by an other instance of the same objective
		if (m_AICampConfig.GetInfectedDeletionRadius() > 0 && !ExpansionQuestModule.GetModuleInstance().QuestTriggerExists(m_Quest.GetQuestConfig().GetID(), GetObjectiveType(), m_ObjectiveConfig.GetID()))
			CreateTrigger(m_AICampConfig.GetAISpawns()[0].GetWaypoints()[0], m_AICampConfig.GetInfectedDeletionRadius());

		if (m_AICampConfig.GetObjectiveText() != string.Empty)
			CreateMarkers();
	}

	protected void CreateTrigger(vector pos, int radius)
	{
	#ifdef EXPANSIONMODQUESTSOBJECTIVEDEBUG
		auto trace = EXTrace.Start(EXTrace.QUESTS, this);
	#endif

		array<ExpansionObjectiveTriggerBase> triggers = new array<ExpansionObjectiveTriggerBase>;
		ExpansionAICampObjectiveSphereTrigger trigger = ExpansionAICampObjectiveSphereTrigger.Cast(g_Game.CreateObjectEx("ExpansionAICampObjectiveSphereTrigger", pos, ECE_LOCAL));
		if (!trigger)
			return;

		trigger.SetPosition(pos);
		trigger.SetTriggerRadius(radius);
		trigger.SetObjectiveData(m_Quest.GetQuestConfig().GetID(), GetObjectiveType(), m_AICampConfig.GetID());
		triggers.Insert(trigger);

		ExpansionQuestModule.GetModuleInstance().SetQuestTriggers(m_Quest.GetQuestConfig().GetID(), triggers);
	}

	override void CreateMarkers()
	{
		string markerName = m_AICampConfig.GetObjectiveText();
		vector position = m_AICampConfig.GetAISpawns()[0].GetWaypoints()[0];
		if (position != "0 0 0")
			CreateObjectiveMarker(position, markerName);
	}

	override int GetObjectiveType()
	{
		return ExpansionQuestObjectiveType.AICAMP;
	}
};
#endif