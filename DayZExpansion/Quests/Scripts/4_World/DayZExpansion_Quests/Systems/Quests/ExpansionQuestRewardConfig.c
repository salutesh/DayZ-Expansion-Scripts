/**
 * ExpansionQuestRewardConfig.c
 *
 * DayZ Expansion Mod
 * www.dayzexpansion.com
 * © 2022 DayZ Expansion Mod Team
 *
 * This work is licensed under the Creative Commons Attribution-NonCommercial-NoDerivatives 4.0 International License.
 * To view a copy of this license, visit http://creativecommons.org/licenses/by-nc-nd/4.0/.
 *
*/
class ExpansionQuestRewardConfig
{
	//! Reward items
	protected string ClassName;
	protected int Amount = 0;

#ifdef EXPANSIONMODHARDLINE
	protected int Humanity = 0;
#endif

	void SetClassName(string name)
	{
		ClassName = name;
	}

	string GetClassName()
	{
		return ClassName;
	}

	void SetAmount(int amount)
	{
		Amount = amount;
	}

	int GetAmount()
	{
		return Amount;
	}

	bool IsVehicle()
	{
		return GetGame().IsKindOf(ClassName, "CarScript") || GetGame().IsKindOf(ClassName, "ExpansionVehicleBase");
	}

#ifdef EXPANSIONMODHARDLINE
	void SetHumanity(int humanity)
	{
		Humanity = humanity;
	}

	int GetHumanity()
	{
		return Humanity;
	}
#endif

	void OnSend(ParamsWriteContext ctx)
	{
		ctx.Write(ClassName);
		ctx.Write(Amount);
	#ifdef EXPANSIONMODHARDLINE
		ctx.Write(Humanity);
	#endif
	}

	bool OnRecieve(ParamsReadContext ctx)
	{
		if (!ctx.Read(ClassName))
			return false;

		if (!ctx.Read(Amount))
			return false;

	#ifdef EXPANSIONMODHARDLINE
		if (!ctx.Read(Humanity))
			return false;
	#endif

		return true;
	}

	void QuestDebug()
	{
	#ifdef EXPANSIONMODQUESTSINSTANCEDEBUG
		Print("------------------------------------------------------------");
		Print(ToString() + "::QuestDebug - ClassName: " + ClassName);
		Print(ToString() + "::QuestDebug - Amount: " + Amount);
		Print("------------------------------------------------------------");
	#endif
	}
};