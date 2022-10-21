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
	protected ref array<vector> Positions = new array<vector>;
	protected ref map<string, int> Items = new map<string, int>;

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

	void QuestDebug()
	{
	#ifdef EXPANSIONMODQUESTSOBJECTIVEDEBUG
		CF_Log.Debug("------------------------------------------------------------");
		foreach (vector pos: Positions)
		{
			CF_Log.Debug(ToString() + "::QuestDebug - Position: " + pos);
		}

		foreach (string item, int amount: Items)
		{
			CF_Log.Debug(ToString() + "::QuestDebug - Item: " + item);
			CF_Log.Debug(ToString() + "::QuestDebug - Item amount:" + amount);
		}
		CF_Log.Debug("------------------------------------------------------------");
	#endif
	}
};