/**
 * ExpansionQuestObjectiveTarget.c
 *
 * DayZ Expansion Mod
 * www.dayzexpansion.com
 * © 2022 DayZ Expansion Mod Team
 *
 * This work is licensed under the Creative Commons Attribution-NonCommercial-NoDerivatives 4.0 International License.
 * To view a copy of this license, visit http://creativecommons.org/licenses/by-nc-nd/4.0/.
 *
*/

class ExpansionQuestObjectiveTarget
{
	protected int Amount = -1;
	protected ref array<string> ClassNames = new array<string>;
	protected bool CountSelfKill;
	protected bool SpecialWeapon = false;
	protected ref array<string> AllowedWeapons = new array<string>;
	protected ref array<string> ExcludedClassNames = new array<string>;
#ifdef EXPANSIONMODAI
	protected bool CountAIPlayers = false;
	protected ref array<string> AllowedTargetFactions = new array<string>;
#endif

	void SetAmount(int amount)
	{
		Amount = amount;
	}

	int GetAmount()
	{
		return Amount;
	}

	void AddClassName(string name)
	{
		ClassNames.Insert(name);
	}

	array<string> GetClassNames()
	{
		return ClassNames;
	}

	void SetNeedSpecialWeapon(bool state)
	{
		SpecialWeapon = state;
	}

	bool NeedSpecialWeapon()
	{
		return SpecialWeapon;
	}

	void AddAllowedWeapon(string name)
	{
		AllowedWeapons.Insert(name);
	}

	array<string> GetAllowedWeapons()
	{
		return AllowedWeapons;
	}

	void AddExcludedClassName(string name)
	{
		ExcludedClassNames.Insert(name);
	}

	array<string> GetExcludedClassNames()
	{
		return ExcludedClassNames;
	}
	
	void SetCountSelfKill(bool state)
	{
		CountSelfKill = state;
	}
	
	bool CountSelfKill()
	{
		return CountSelfKill;
	}
	
#ifdef EXPANSIONMODAI
	void SetCountAIPlayers(bool state)
	{
		CountAIPlayers = state;
	}
	
	bool CountAIPlayers()
	{
		return CountAIPlayers;
	}
	
	void AddAllowedFaction(string factionName)
	{
		AllowedTargetFactions.Insert(factionName);
	}

	array<string> GetAllowedTargetFactions()
	{
		return AllowedTargetFactions;
	}
#endif

	void QuestDebug()
	{
	#ifdef EXPANSIONMODQUESTSOBJECTIVEDEBUG
		Print("------------------------------------------------------------");
		Print(ToString() + "::QuestDebug - Amount: " + Amount);
		int i;
		for (i = 0; i < ClassNames.Count(); i++)
		{
			Print(ToString() + "::QuestDebug - ClassName" + i + ": " +ClassNames[i]);
		}
		Print(ToString() + "::QuestDebug - SpecialWeapon: " + SpecialWeapon);
		for (i = 0; i < AllowedWeapons.Count(); i++)
		{
			Print(ToString() + "::QuestDebug - ClassName" + i + ": " + AllowedWeapons[i]);
		}
		Print("------------------------------------------------------------");
	#endif
	}
};