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
	private ref array<vector> Positions = new array<vector>;
	private ref map<string, int> Items = new map<string, int>;
	
	[NonSerialized()]
	private vector SelectedPosition;

	void AddItem(string className, int amount)
	{
		Items.Insert(className, amount);
	}

	map<string, int> GetItems()
	{
		return Items;
	}

	void AddPosition(vector pos)
	{
		Positions.Insert(pos);
	}

	array<vector> GetPositions()
	{
		return Positions;
	}
	
	void SetSelectedPosition(vector pos)
	{
		SelectedPosition = pos;
	}
	
	vector GetSelectedPosition()
	{
		return SelectedPosition;
	}

	void OnSend(ParamsWriteContext ctx)
	{
		ctx.Write(SelectedPosition);
		
		/*int posCount = Positions.Count();
		ctx.Write(posCount);
		foreach (vector pos: Positions)
		{
			ctx.Write(pos);
		}
		
		ctx.Write(Positions);*/

		/*int itemCount = Items.Count();
		ctx.Write(itemCount);
		
		foreach (string name, int amount: Items)
		{
			ctx.Write(name);
			ctx.Write(amount);
		}*/
	}

	bool OnRecieve(ParamsReadContext ctx)
	{
		if (!ctx.Read(SelectedPosition))
			return false;
		
		/*if (!Positions)
			Positions = new array<vector>;
		else
			Positions.Clear();

		int posCount;
		if (!ctx.Read(posCount))
			return false;
		
		for (int i = 0; i < posCount; i++)
		{
			vector pos;
			if (!ctx.Read(pos))
			return false;
			
			Positions.Insert(pos);
		}*/
		
		/*if (!Items)
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
		}*/

		return true;
	}

	void QuestDebug()
	{
	#ifdef EXPANSIONMODQUESTSOBJECTIVEDEBUG
		Print("------------------------------------------------------------");
		foreach (vector pos: Positions)
		{
			Print(ToString() + "::QuestDebug - Position: " + pos);
		}
		
		foreach (string item, int amount: Items)
		{
			Print(ToString() + "::QuestDebug - Item: " + item);
			Print(ToString() + "::QuestDebug - Item amount:" + amount);
		}
		Print("------------------------------------------------------------");
	#endif
	}
};