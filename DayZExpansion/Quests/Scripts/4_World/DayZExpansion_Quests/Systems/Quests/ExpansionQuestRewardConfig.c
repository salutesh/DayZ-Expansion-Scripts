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

class ExpansionQuestRewardConfigBase
{
	string ClassName;
	int Amount = 0;
	ref TStringArray Attachments = new TStringArray;
};

class ExpansionQuestRewardConfigV1: ExpansionQuestRewardConfigBase
{
	int HealthPercent;

	void Copy(ExpansionQuestRewardConfigBase rewardBase)
	{
		ClassName = rewardBase.ClassName;
		Amount = rewardBase.Amount;
		Attachments = rewardBase.Attachments;
	}
};

class ExpansionQuestRewardConfig: ExpansionQuestRewardConfigV1
{
	int DamagePercent = 0;

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

	void AddAttachment(string name)
	{
		if (!Attachments)
			Attachments = new TStringArray;

		Attachments.Insert(name);
	}

	TStringArray GetAttachments()
	{
		return Attachments;
	}

	int GetDamagePercent()
	{
		return DamagePercent;
	}

	void SetDamagePercent(int value)
	{
		DamagePercent = value;
	}

	void OnSend(ParamsWriteContext ctx)
	{
		ctx.Write(ClassName);
		ctx.Write(Amount);

		ctx.Write(Attachments.Count());
		foreach (string attach: Attachments)
		{
			ctx.Write(attach);
		}
	}

	bool OnRecieve(ParamsReadContext ctx)
	{
		if (!ctx.Read(ClassName))
			return false;

		if (!ctx.Read(Amount))
			return false;

		int attachmentsCount;
		if (!ctx.Read(attachmentsCount))
			return false;

		for (int i = 0; i < attachmentsCount; i++)
		{
			string attach;
			if (!ctx.Read(attach))
				return false;

			if (!Attachments)
				Attachments = new TStringArray;

			Attachments.Insert(attach);
		}

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