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
	protected autoptr array<string> ClassNames = new array<string>;
	protected bool SpecialWeapon = false;
	protected autoptr array<string> AllowedWeapons = new array<string>;

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

	void OnSend(ParamsWriteContext ctx)
	{
		ctx.Write(Amount);

		int i;

		int namesCount = ClassNames.Count();
		ctx.Write(namesCount);

		for (i = 0; i < namesCount; i++)
		{
			ctx.Write(ClassNames[i]);
		}

		int allowedCount = AllowedWeapons.Count();
		ctx.Write(allowedCount);

		for (i = 0; i < allowedCount; i++)
		{
			ctx.Write(AllowedWeapons[i]);
		}
	}

	bool OnRecieve(ParamsReadContext ctx)
	{
		if (!ctx.Read(Amount))
			return false;

		int i;

		int namesCount;
		if (!ctx.Read(namesCount))
			return false;

		for (i = 0; i < namesCount; i++)
		{
			string name;
			if (!ctx.Read(name))
				return false;

			AddClassName(name);
		}

		int allowedCount;
		if (!ctx.Read(allowedCount))
			return false;

		for (i = 0; i < allowedCount; i++)
		{
			string allowed;
			if (!ctx.Read(allowed))
				return false;

			AddAllowedWeapon(name);
		}

		return true;
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