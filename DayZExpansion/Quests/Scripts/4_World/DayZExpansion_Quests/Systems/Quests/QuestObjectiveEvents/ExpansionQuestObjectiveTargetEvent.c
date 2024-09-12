/**
 * ExpansionQuestObjectiveTargetEvent.c
 *
 * DayZ Expansion Mod
 * www.dayzexpansion.com
 * © 2023 DayZ Expansion Mod Team
 *
 * This work is licensed under the Creative Commons Attribution-NonCommercial-NoDerivatives 4.0 International License.
 * To view a copy of this license, visit http://creativecommons.org/licenses/by-nc-nd/4.0/.
 *
*/

class ExpansionQuestObjectiveTargetEvent: ExpansionQuestObjectiveEventBase
{
	protected int m_Count;
	protected int m_Amount;
	protected ref ExpansionQuestObjectiveTargetConfig m_TargetConfig;

	override bool OnEventStart()
	{
		if (!super.OnEventStart())
			return false;

		if (!Class.CastTo(m_TargetConfig, m_ObjectiveConfig))
			return false;

		return true;
	}

	override bool OnContinue()
	{
		if (!super.OnContinue())
			return false;

		if (!Class.CastTo(m_TargetConfig, m_ObjectiveConfig))
			return false;

		m_Quest.QuestCompletionCheck(true);

		return true;
	}

	override void OnEntityKilled(EntityAI victim, EntityAI killer, Man killerPlayer = null, map<Man, ref ExpansionEntityHitInfo> hitMap = null)
	{
	#ifdef EXPANSIONMODQUESTSOBJECTIVEDEBUG
		auto trace = EXTrace.Start(EXTrace.QUESTS, this);
	#endif

		if (!m_TargetConfig)
			return;

		array<string> excludedClassNames = m_TargetConfig.GetExcludedClassNames();
		if (ExpansionStatic.IsAnyOf(victim, excludedClassNames, true))
			return;

		PlayerBase victimPlayer;
		if (Class.CastTo(victimPlayer, victim))
		{
			//! Check if this was a self-kill
			if (m_Quest.GetPlayer() == victimPlayer && !m_TargetConfig.CountSelfKill())
				return;

		#ifdef EXPANSIONMODAI
			//! Check if player is AI and if we can count it
			if (victimPlayer.IsAI() && !m_TargetConfig.CountAIPlayers())
				return;

			//! Check if target faction is in allowed factions of this objective
			array<string> allowedTargetFactions = m_TargetConfig.GetAllowedTargetFactions();
			if (allowedTargetFactions && allowedTargetFactions.Count() > 0)
			{
				bool foundFaction = false;
				eAIGroup victimGroup = victimPlayer.GetGroup();
				if (victimGroup)
				{
					eAIFaction victimFaction = victimGroup.GetFaction();
					if (victimFaction)
					{
						string victimFactionName = victimFaction.GetName();
						if (allowedTargetFactions.Find(victimFactionName) > -1)
							foundFaction = true;
					}
				}

				if (!foundFaction)
					return;
			}
		#endif

			//! PvP quest objective. Check if the victim is a quest player
			//! of this quest and if its a group quest make sure he was not in the involved party before.
			//! If he is in the related group or was in it we dont count the kill!
			if (m_Quest.GetQuestConfig().IsGroupQuest() && victimPlayer.GetIdentity())
			{
				string victimPlayerUID = victimPlayer.GetIdentity().GetId();
				
				#ifdef EXPANSIONMODGROUPS
				if (victimPlayer.Expansion_GetPartyID() == m_Quest.GetGroupID())
					return;
				#endif
				
				if (ExpansionQuestModule.GetModuleInstance().WasPlayerInGroup(victimPlayerUID, m_Quest.GetGroupID()))
					return;
			}
		}

		if (killerPlayer && !IsInRange(killerPlayer.GetPosition(), m_TargetConfig.GetPosition(), m_TargetConfig.GetMaxDistance(), m_TargetConfig.GetMinDistance()))
		{
			ObjectivePrint("Killer is out of legit kill range! Skip..");
			return;
		}

		//! If the target need to be killed with a special weapon check incoming killer class type
		TStringArray allowedWeapons = m_TargetConfig.GetAllowedWeapons();
		if (allowedWeapons.Count() > 0 && !ExpansionStatic.IsAnyOf(killer, allowedWeapons, true))
		{
			ObjectivePrint("Entity got not killed with any allowed weapon! Skip..");
			return;
		}
		
		//! If the target need to be a certain entity we compare the victim entity type with the configured list from the objective.
		TStringArray allowedClassNames = m_TargetConfig.GetClassNames();
		if (allowedClassNames.Count() > 0)
		{
			bool found = ExpansionStatic.IsAnyOf(victim, allowedClassNames, true);
			if (!found)
			{
				ObjectivePrint("Entity killed was not a valid entity! Skip..");
				return;
			}
		}
		
		//! If the target need to be hit at a certain zone we check the hit map for the last valid hit of the killer player on the victim entity.
		TStringArray allowedDamageZones = m_TargetConfig.GetAllowedDamageZones();
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
		
		ObjectivePrint("Killed valid target " + victim + " " + ExpansionString.JoinStrings(m_TargetConfig.GetClassNames()));
		if (m_Count < m_Amount)
		{
			m_Count++;
			m_Quest.QuestCompletionCheck(true);
		}
	}

	override bool CanComplete()
	{
	#ifdef EXPANSIONMODQUESTSOBJECTIVEDEBUG
		auto trace = EXTrace.Start(EXTrace.QUESTS, this);
	#endif
		ObjectivePrint("m_Count: " + m_Count);
		ObjectivePrint("m_Amount: " + m_Amount);

		if (m_Amount == 0)
			return false;

		if (m_Count == m_Amount)
			return true;
		return false;
	}

	void SetCount(int count)
	{
#ifdef EXTRACE
		auto trace = EXTrace.Start(EXTrace.QUESTS, this);
#endif

		m_Count = count;

		ObjectivePrint("Count: " + m_Count);

	}

	int GetCount()
	{
		return m_Count;
	}

	void SetAmount(int amount)
	{
		m_Amount = amount;
	}

	int GetAmount()
	{
		return m_Amount;
	}

	override int GetObjectiveType()
	{
		return ExpansionQuestObjectiveType.TARGET;
	}
};