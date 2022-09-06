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
	private int Amount = -1;
	private autoptr array<string> ClassNames = new array<string>;
	bool CountSelfKill;
	private bool SpecialWeapon = false;
	private autoptr array<string> AllowedWeapons = new array<string>;
	private autoptr array<string> ExcludedClassNames = new array<string>;

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