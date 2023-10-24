/**
 * ExpansionQuestObjectiveDelivery.c
 *
 * DayZ Expansion Mod
 * www.dayzexpansion.com
 * © 2022 DayZ Expansion Mod Team
 *
 * This work is licensed under the Creative Commons Attribution-NonCommercial-NoDerivatives 4.0 International License.
 * To view a copy of this license, visit http://creativecommons.org/licenses/by-nc-nd/4.0/.
 *
*/

class ExpansionQuestObjectiveDelivery
{
	protected int Amount = 0;
	protected string ClassName = string.Empty;
	protected int QuantityPercent = -1;
	protected int MinQuantityPercent = -1;

	void SetAmount(int amount)
	{
		Amount = amount;
	}

	int GetAmount()
	{
		return Amount;
	}

	void SetClassName(string name)
	{
		ClassName = name;
	}

	string GetClassName()
	{
		return ClassName;
	}

	string GetClassNameLower()
	{
		string className = ClassName;
		className.ToLower();
		return className;
	}
	
	void SetQuantity(int quantityPercent)
	{
		QuantityPercent = quantityPercent;
	}
	
	int GetQuantity()
	{
		return QuantityPercent;
	}
	
	int GetMinQuantity()
	{
		return MinQuantityPercent;
	}
	
	void SetMinQuantity(int quantityPercent)
	{
		MinQuantityPercent = quantityPercent;
	}
	
	void Copy(ExpansionQuestObjectiveDelivery data)
	{
		Amount = data.Amount;
		ClassName = data.ClassName;
		QuantityPercent = data.QuantityPercent;
		MinQuantityPercent = data.MinQuantityPercent;
	}

	void OnSend(ParamsWriteContext ctx)
	{
		ctx.Write(Amount);
		ctx.Write(ClassName);
		ctx.Write(QuantityPercent);
		ctx.Write(MinQuantityPercent);
	}

	bool OnRecieve(ParamsReadContext ctx)
	{
		if (!ctx.Read(Amount))
			return false;

		if (!ctx.Read(ClassName))
			return false;
		
		if (!ctx.Read(QuantityPercent))
			return false;
		
		if (!ctx.Read(MinQuantityPercent))
			return false;


		return true;
	}

	void QuestDebug()
	{
	#ifdef EXPANSIONMODQUESTSOBJECTIVEDEBUG
		Print("------------------------------------------------------------");
		Print(ToString() + "::QuestDebug - Amount: " + Amount);
		Print(ToString() + "::QuestDebug - ClassName: " + ClassName);
		Print(ToString() + "::QuestDebug - QuantityPercent: " + QuantityPercent);
		Print(ToString() + "::QuestDebug - MinQuantityPercent: " + MinQuantityPercent);
		Print("------------------------------------------------------------");
	#endif
	}
};