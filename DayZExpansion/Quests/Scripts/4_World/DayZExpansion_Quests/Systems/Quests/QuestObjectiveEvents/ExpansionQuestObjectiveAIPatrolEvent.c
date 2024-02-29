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
class ExpansionQuestObjectiveAIPatrolEvent: ExpansionQuestObjectiveAIEventBase
{
	protected ref ExpansionQuestObjectiveAIPatrolConfig m_AIPatrolConfig;

	override bool OnEventStart()
	{
	#ifdef EXPANSIONMODQUESTSOBJECTIVEDEBUG
		auto trace = EXTrace.Start(EXTrace.QUESTS, this);
	#endif

		if (!Class.CastTo(m_AIPatrolConfig, m_ObjectiveConfig))
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

		if (!Class.CastTo(m_AIPatrolConfig, m_ObjectiveConfig))
			return false;
		
		if (!super.OnContinue())
			return false;

		return true;
	}

	override void CreateMarkers()
	{
		string markerName = m_AIPatrolConfig.GetObjectiveText();
		vector position = m_AIPatrolConfig.GetAISpawn().GetWaypoints()[0];
		if (position != "0 0 0")
			CreateObjectiveMarker(position, markerName);
	}

	override void OnEntityKilled(EntityAI victim, EntityAI killer, Man killerPlayer = null, map<Man, ref ExpansionEntityHitInfo> hitMap = null)
	{
	#ifdef EXPANSIONMODQUESTSOBJECTIVEDEBUG
		auto trace = EXTrace.Start(EXTrace.QUESTS, this);
	#endif

		if (!m_AIPatrolConfig)
			return;

		//! If the ai camp need to be killed with a special weapon check incoming killer class type.
		if (m_AIPatrolConfig.GetAllowedWeapons().Count() > 0)
		{
			if (!ExpansionStatic.IsAnyOf(killer, m_AIPatrolConfig.GetAllowedWeapons(), true))
			{
				ObjectivePrint("Entity got not killed with any allowed weapon! Skip..");
				return;
			}
		}
		
		//! Check if the killer player was in legit kill range.
		if (killerPlayer && !IsInRange(killerPlayer.GetPosition(), victim.GetPosition(), m_AIPatrolConfig.GetMaxDistance(), m_AIPatrolConfig.GetMinDistance()))
		{
			ObjectivePrint("Killer is out of legit kill range! Skip..");
			return;
		}
		
		//! If the target need to be hit at a certain zone we check the hit map for the last valid hit of the killer player on the victim entity.
		TStringArray allowedDamageZones = m_AIPatrolConfig.GetAllowedDamageZones();
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

		if (!m_AIPatrolConfig)
			return;

		CheckQuestAIPatrol(1);
	}

	override void CreateQuestAIPatrol()
	{
	#ifdef EXPANSIONMODQUESTSOBJECTIVEDEBUG
		auto trace = EXTrace.Start(EXTrace.QUESTS, this);
	#endif

		if (!m_AIPatrolConfig)
			return;

		ExpansionQuestAISpawn aiSpawn = m_AIPatrolConfig.GetAISpawn();
		if (!aiSpawn)
			return;

		array<eAIQuestPatrol> questPatrols = new array<eAIQuestPatrol>;
		eAIQuestPatrol questPatrol = aiSpawn.CreateAIQuestPatrol(m_TotalKillCount);
		questPatrols.Insert(questPatrol);

		ExpansionQuestModule.GetModuleInstance().SetQuestPatrols(m_Quest.GetQuestConfig().GetID(), questPatrols);

		if (m_AIPatrolConfig.GetObjectiveText() != string.Empty)
			CreateMarkers();
	}

	override int GetObjectiveType()
	{
		return ExpansionQuestObjectiveType.AIPATROL;
	}
};
#endif