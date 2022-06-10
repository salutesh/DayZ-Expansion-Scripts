/**
 * ExpansionQuestObjectiveTreasureHunt.c
 *
 * DayZ Expansion Mod
 * www.dayzexpansion.com
 * © 2022 DayZ Expansion Mod Team
 *
 * This work is licensed under the Creative Commons Attribution-NonCommercial-NoDerivatives 4.0 International License.
 * To view a copy of this license, visit http://creativecommons.org/licenses/by-nc-nd/4.0/.
 *
*/

class ExpansionQuestObjectiveTreasureHunt
{
	private vector Position;
	private ref map<string, int> Items = new map<string, int>;

	void AddItem(string className, int amount)
	{
		Items.Insert(className, amount);
	}

	map<string, int> GetItems()
	{
		return Items;
	}

	void SetPosition(vector pos)
	{
		Position = pos;
	}

	vector GetPosition()
	{
		return Position;
	}

	void OnSend(ParamsWriteContext ctx)
	{
		ctx.Write(Position);

		int itemCount = Items.Count();
		ctx.Write(itemCount);
		
		foreach (string name, int amount: Items)
		{
			ctx.Write(name);
			ctx.Write(amount);
		}
	}

	bool OnRecieve(ParamsReadContext ctx)
	{
		vector pos;
		if (!ctx.Read(pos))
				return false;

		Position = pos;

		if (!Items)
			Items = new map<string, int>;
		else
			Items.Clear();

		int itemCount;
		if (!ctx.Read(itemCount))
			return false;

		for (int i = 0; i < itemCount; i++)
		{
			string name;
			if (!ctx.Read(name))
				return false;

			int amount;
			if (!ctx.Read(amount))
				return false;

			Items.Insert(name, amount);
		}

		return true;
	}

	void QuestDebug()
	{
	#ifdef EXPANSIONMODQUESTSOBJECTIVEDEBUG
		Print("------------------------------------------------------------");
		Print(ToString() + "::QuestDebug - Position: " + Position);
		foreach (string item, int amount: Items)
		{
			Print(ToString() + "::QuestDebug - Item: " + item);
			Print(ToString() + "::QuestDebug - Item amount:" + amount);
		}
		Print("------------------------------------------------------------");
	#endif
	}
};