/**
 * ExpansionQuestObjectiveTargetEvent.c
 *
 * DayZ Expansion Mod
 * www.dayzexpansion.com
 * © 2022 DayZ Expansion Mod Team
 *
 * This work is licensed under the Creative Commons Attribution-NonCommercial-NoDerivatives 4.0 International License.
 * To view a copy of this license, visit http://creativecommons.org/licenses/by-nc-nd/4.0/.
 *
*/

class ExpansionQuestObjectiveTargetEvent: ExpansionQuestObjectiveEventBase
{
	protected int Count = 0;
	protected int Amount = 0;

	void SetCount(int count)
	{
		Count = count;
	}

	int GetCount()
	{
		return Count;
	}

	int GetAmount()
	{
		return Amount;
	}

	override bool OnStart()
	{
	#ifdef EXPANSIONTRACE
		auto trace = CF_Trace_0(ExpansionTracing.QUESTS, this, "OnStart");
	#endif
		
		if (!super.OnStart())
			return false;

		TargetEventStart();

		return true;
	}

	override bool OnContinue()
	{
	#ifdef EXPANSIONTRACE
		auto trace = CF_Trace_0(ExpansionTracing.QUESTS, this, "OnContinue");
	#endif
		
		if (!super.OnContinue())
			return false;

		TargetEventStart();

		return true;
	}

	protected void TargetEventStart()
	{
	#ifdef EXPANSIONTRACE
		auto trace = CF_Trace_0(ExpansionTracing.QUESTS, this, "TargetEventStart");
	#endif

		ExpansionQuestObjectiveTarget target = GetObjectiveConfig().GetTarget();
		if (!target)
			return;

		Amount = target.GetAmount();
	}

	void OnEntityKilled(EntityAI victim, EntityAI killer, Man killerPlayer = NULL)
	{
	#ifdef EXPANSIONTRACE
		auto trace = CF_Trace_0(ExpansionTracing.QUESTS, this, "OnEntityKilled");
	#endif

	#ifdef EXPANSIONMODAI
		//! XXX: This cannot happen, because killerPlayer identity is checked in Quest module and AI have no identity
		PlayerBase player;
		if (Class.CastTo(player, killer) && player.IsAI())
			return;
	#endif

		ExpansionQuestObjectiveTarget target = GetObjectiveConfig().GetTarget();
		if (!target)
			return;

		if (ExpansionStatic.IsAnyOf(victim.GetType(), target.GetExcludedClassNames(), victim.ClassName()))
			return;

		bool maxRangeCheck = false;

	#ifdef EXPANSIONMODQUESTSOBJECTIVEDEBUG
		Print(ToString() + "::OnEntityKilled - Victim class name: " + victim.ClassName());
		Print(ToString() + "::OnEntityKilled - Victim type: " + victim.GetType());
		Print(ToString() + "::OnEntityKilled - Victim type: " + victim.Type().ToString());
		Print(ToString() + "::OnEntityKilled - Killer class name: " + killer.ClassName());
		Print(ToString() + "::OnEntityKilled - Killer type: " + killer.GetType());
		Print(ToString() + "::OnEntityKilled - Killer type: " + killer.Type().ToString());
	#endif

		PlayerBase victimPlayer;
		if (Class.CastTo(victimPlayer, victim))
		{
			//! Check if this was a self-kill
			if (GetQuest().GetPlayer() == victimPlayer && !target.CountSelfKill)
				return;

		#ifdef EXPANSIONMODGROUPS
			//! PvP quest objective. Check if the victim is a quest player
			//! of this quest and if its a group quest make sure he was not in the involved party before.
			//! If he is in the related group or was in it we dont count the kill!
			if (GetQuest().IsGroupQuest() && victimPlayer.GetIdentity())
			{
				string victimPlayerUID = victimPlayer.GetIdentity().GetId();
				int groupID = GetQuest().GetGroupID();
				ExpansionPartyModule partymodule = ExpansionPartyModule.Cast(CF_ModuleCoreManager.Get(ExpansionPartyModule));
				if (!partymodule)
					return;

				ExpansionPartyPlayerData victimPartyData = partymodule.GetPartyPlayerData(victimPlayerUID);
				if (victimPartyData && victimPartyData.GetParty().GetPartyID() == groupID || GetQuest().GetQuestModule().WasPlayerInGroup(victimPlayerUID, groupID))
					return;
			}
		#endif
		}

		//! Use max range check if used in config
		if (GetObjectiveConfig().GetMaxDistance() > -1)
			maxRangeCheck = true;

		if (killerPlayer)
		{
			if (maxRangeCheck && !IsInMaxRange(killerPlayer.GetPosition()))
			{
				Print(ToString() + "::OnEntityKilled - Killer is out of legit kill range!");
				return;
			}
		}

		//! If the target need to be killed with a special weapon check incoming killer class type
		if (target.NeedSpecialWeapon())
		{
			if (!ExpansionStatic.IsAnyOf(killer.GetType(), target.GetAllowedWeapons(), killer.ClassName()))
				return;

		#ifdef EXPANSIONMODQUESTSOBJECTIVEDEBUG
			Print(ToString() + "::OnEntityKilled - Player has killed with special weapon!");
		#endif
		}

		int amount = target.GetAmount();
		Amount = amount;
		bool found = ExpansionStatic.IsAnyOf(victim.GetType(), target.GetClassNames(), victim.ClassName());

	#ifdef EXPANSIONMODQUESTSOBJECTIVEDEBUG
		Print(ToString() + "::OnEntityKilled - Target found: " + found);
	#endif

		if (found)
		{
			if (Count < amount)
			{
				Count++;

				if (GetQuest())
					GetQuest().UpdateQuestPlayersObjectiveData();
			}
		}

		if (Count >= amount)
		{
			SetCompleted(true);
			OnComplete();
		}
	}

	protected bool IsInMaxRange(vector playerPos)
	{
		vector position = GetObjectiveConfig().GetPosition();
		float maxDistance = GetObjectiveConfig().GetMaxDistance();
		float currentDistanceSq = vector.DistanceSq(playerPos, position);
		position[1] = GetGame().SurfaceY(position[0], position[2]);

		if (position != vector.Zero && currentDistanceSq <= maxDistance * maxDistance)
		{
			return true;
		}

		return false;
	}

	override int GetObjectiveType()
	{
		return ExpansionQuestObjectiveType.TARGET;
	}
};