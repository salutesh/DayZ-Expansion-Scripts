/**
 * AnimalBase.c
 *
 * DayZ Expansion Mod
 * www.dayzexpansion.com
 * © 2022 DayZ Expansion Mod Team
 *
 * This work is licensed under the Creative Commons Attribution-NonCommercial-NoDerivatives 4.0 International License.
 * To view a copy of this license, visit http://creativecommons.org/licenses/by-nc-nd/4.0/.
 *
*/

modded class AnimalBase
{
	protected static ref array<ref ExpansionQuestObjectiveEventBase> s_Expansion_AssignedQuestObjectives = new array<ref ExpansionQuestObjectiveEventBase>;
	protected ref ExpansionEntityHitHandler m_Expansion_HitHandler = new ExpansionEntityHitHandler(this);

	static void AssignQuestObjective(ExpansionQuestObjectiveEventBase objective)
	{
#ifdef EXTRACE
		auto trace = EXTrace.Start(EXTrace.QUESTS, AnimalBase);
#endif 

		int index = s_Expansion_AssignedQuestObjectives.Find(objective);
		if (index == -1)
		{
			s_Expansion_AssignedQuestObjectives.Insert(objective);
		#ifdef EXPANSIONMODQUESTSOBJECTIVEDEBUG
			EXTrace.Print(EXTrace.QUESTS, null, "Assigned quest objective: Type: " + objective.GetObjectiveType() + " | ID: " + objective.GetObjectiveConfig().GetID());
		#endif
		}
	#ifdef EXPANSIONMODQUESTSOBJECTIVEDEBUG
		else
		{
			EXTrace.Print(EXTrace.QUESTS, null, "Quest objective: Type: " + objective.GetObjectiveType() + " | ID: " + objective.GetObjectiveConfig().GetID() + " is already assigned to this entity! Skiped");
		}
	#endif
	}

	static void DeassignQuestObjective(ExpansionQuestObjectiveEventBase objective)
	{
#ifdef EXTRACE
		auto trace = EXTrace.Start(EXTrace.QUESTS, AnimalBase);
#endif

		int index = s_Expansion_AssignedQuestObjectives.Find(objective);
		if (index > -1)
		{
			s_Expansion_AssignedQuestObjectives.RemoveOrdered(index);
		#ifdef EXPANSIONMODQUESTSOBJECTIVEDEBUG
			EXTrace.Print(EXTrace.QUESTS, null, "Deassigned quest objective: Type: " + objective.GetObjectiveType() + " | ID: " + objective.GetObjectiveConfig().GetID());
		#endif
		}
	#ifdef EXPANSIONMODQUESTSOBJECTIVEDEBUG
		else
		{
			EXTrace.Print(EXTrace.QUESTS, null, "Quest objective: Type: " + objective.GetObjectiveType() + " | ID: " + objective.GetObjectiveConfig().GetID() + " is not assigned to this entity and cant be deassigned!");
		}
	#endif
	}

	protected void CheckAssignedObjectivesForEntity(Object killer)
	{
	#ifdef EXPANSIONMODQUESTSOBJECTIVEDEBUG
		auto trace = EXTrace.Start(EXTrace.QUESTS, this);
	#endif
	
		if (s_Expansion_AssignedQuestObjectives.Count() == 0)
			return;

		EntityAI killSource = EntityAI.Cast(killer);
		if (!killSource || killSource == this)
			return;

		Man killerPlayer = killSource.GetHierarchyRootPlayer();
		if (!killerPlayer)
			return;

		string killerUID;
		if (killerPlayer.GetIdentity())
			killerUID = killerPlayer.GetIdentity().GetId();

		ExpansionQuest quest;
		int failSafe = s_Expansion_AssignedQuestObjectives.Count() + 1;
		for (int i = 0, j = 0; i < s_Expansion_AssignedQuestObjectives.Count() && j < failSafe; j++)
		{
			ExpansionQuestObjectiveEventBase objective = s_Expansion_AssignedQuestObjectives[i];
			if (!objective)
			{
				EXTrace.Print(EXTrace.QUESTS, this, "WARNING: Objective is NULL!");
				s_Expansion_AssignedQuestObjectives.RemoveOrdered(i);
				continue;
			}

			quest = objective.GetQuest();
			if (!quest)
			{
				i++;
				continue;
			}
			
			//! Check if current quest players distance to our victim entity is in kill range proximity to count the kill when the killer is not a quest player of our current objective instance row
			//! We do this to share kills between quest objectives from players that are working on the same objective.
			if (killerUID == string.Empty || !quest.IsQuestPlayer(killerUID))
			{
			#ifdef DIAG_DEVELOPER
				EXPrint(ToString() + "::CheckAssignedObjectivesForEntity - Entity got killed by a player that is not part of this quest! Quest ID: " + quest.GetQuestConfig().GetID() + " | Killer UID: " + killerUID);
			#endif
				float maxDist = -1;
				switch (objective.GetObjectiveType())
				{
					case ExpansionQuestObjectiveType.TARGET:
					{
						ExpansionQuestObjectiveTargetConfig targetConfig;
						if (Class.CastTo(targetConfig, objective.GetObjectiveConfig()))
							maxDist = targetConfig.GetMaxDistance();
					}
					break;
				}

				if (maxDist <= 0)
					maxDist = 100.0;

				bool countKill = false;
				PlayerBase questPlayer;
				PlayerBase killerPB = PlayerBase.Cast(killerPlayer);
				if (!quest.GetQuestConfig().IsGroupQuest())
				{
					questPlayer = quest.GetPlayer();
					if (questPlayer && objective.IsInRange(questPlayer.GetPosition(), GetPosition(), maxDist) && (Expansion_HasHitEntity(questPlayer) || questPlayer.Expansion_IsHelper(killerPB)))
					{
						countKill = true;
					#ifdef DIAG_DEVELOPER
						EXPrint(ToString() + "::CheckAssignedObjectivesForEntity - Quest player in max range! Player position: " + questPlayer.GetPosition() + " | Victim position: " + GetPosition() + " | Max distance: " + maxDist);
					#endif
					}
				}
				else
				{
					set<string> playerUIDs = quest.GetPlayerUIDs();
					foreach (string memberUID: playerUIDs)
					{
						questPlayer = PlayerBase.GetPlayerByUID(memberUID);		
						if (questPlayer && objective.IsInRange(questPlayer.GetPosition(), GetPosition(), maxDist) && (Expansion_HasHitEntity(questPlayer) || questPlayer.Expansion_IsHelper(killerPB)))
						{
							countKill = true;
						#ifdef DIAG_DEVELOPER
							EXPrint(ToString() + "::CheckAssignedObjectivesForEntity - Quest player in max range! Player position: " + questPlayer.GetPosition() + " | Victim position: " + GetPosition() + " | Max distance: " + maxDist);
						#endif
							break;
						}
					}
				}
				
				//! If none of the current quest players is in kill range proximity then stop the current check and continue with the next objective row..
				if (!countKill)
				{
					i++;
					continue;
				}
			}

			switch (objective.GetObjectiveType())
			{
				case ExpansionQuestObjectiveType.TARGET:
					ExpansionQuestObjectiveTargetEvent targetEvent;
					if (Class.CastTo(targetEvent, objective))
						targetEvent.OnEntityKilled(this, killSource, killerPlayer, m_Expansion_HitHandler.GetHitMap());
					break;
			}

			if (objective.IsAssigned())
				i++;
		}

		//! @note should not happen, this could mean an objective was no longer marked assigned but still in the list
		if (j == failSafe)
			EXPrint(ToString() + " ::CheckAssignedObjectivesForEntity - WARNING: Reached end of loop unexpectedly!");
	}
	
	override void EEHitBy(TotalDamageResult damageResult, int damageType, EntityAI source, int component, string dmgZone, string ammo, vector modelPos, float speedCoef)
	{
		super.EEHitBy(damageResult, damageType, source, component, dmgZone, ammo, modelPos, speedCoef);
		
		m_Expansion_HitHandler.OnHit(damageResult, damageType, source, component, dmgZone, ammo, modelPos, speedCoef);
	}
	
	map<Man, ref ExpansionEntityHitInfo> Expansion_GetEntityHitMap()
	{
		return m_Expansion_HitHandler.GetHitMap();
	}
	
	bool Expansion_HasHitEntity(Man player)
	{
		return m_Expansion_HitHandler.WasHitBy(player);
	}

	override void EEKilled(Object killer)
	{
	#ifdef EXPANSIONMODQUESTSOBJECTIVEDEBUG
		auto trace = EXTrace.Start(EXTrace.QUESTS, this);
	#endif

		super.EEKilled(killer);

		//! Workaround for vanilla EEKilled returning the killed entity as killer since 1.20
		//! See VanillaFixes AnimalBase.c EEHitBy
		if (killer == this)
			killer = m_Expansion_LastDamageSource;

		CheckAssignedObjectivesForEntity(killer);
	}
};
