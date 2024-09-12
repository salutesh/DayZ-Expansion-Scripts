/**
 * ExpansionQuestItemConfig.c
 *
 * DayZ Expansion Mod
 * www.dayzexpansion.com
 * © 2022 DayZ Expansion Mod Team
 *
 * This work is licensed under the Creative Commons Attribution-NonCommercial-NoDerivatives 4.0 International License.
 * To view a copy of this license, visit http://creativecommons.org/licenses/by-nc-nd/4.0/.
 *
*/

class ExpansionQuestItemConfig
{
	protected string ClassName = string.Empty;
	protected int Amount = -1;

	bool IsVehicle()
	{
		return ExpansionStatic.IsVehicle(ClassName);
	}

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

	void OnSend(ParamsWriteContext ctx)
	{
		ctx.Write(ClassName);
		ctx.Write(Amount);
	}

	bool OnRecieve(ParamsReadContext ctx)
	{
		if (!ctx.Read(ClassName))
			return false;

		if (!ctx.Read(Amount))
			return false;

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