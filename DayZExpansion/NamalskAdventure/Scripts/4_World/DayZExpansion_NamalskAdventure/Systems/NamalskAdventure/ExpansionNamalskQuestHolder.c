/**
 * ExpansionNamalskModule.c
 *
 * DayZ Expansion Mod
 * www.dayzexpansion.com
 * © 2023 DayZ Expansion Mod Team
 *
 * This work is licensed under the Creative Commons Attribution-NonCommercial-NoDerivatives 4.0 International License.
 * To view a copy of this license, visit http://creativecommons.org/licenses/by-nc-nd/4.0/.
 *
*/

class ExpansionNamalskQuestHolder
{
	int ID;
	string ClassName;
	ref array<ref ExpansionNamalskQuestHolderPosition> Positions = new array<ref ExpansionNamalskQuestHolderPosition>;
	bool IsStatic;
	string Name;
	string DefaultText;
	int ObjectiveID;

	void ExpansionNamalskQuestHolder(int id, string className, int objectiveID, bool isStatic = true, string name = "Unknown", string text = "There is nothing to do here for you...")
	{
		ID = id;
		ClassName = className;
		ObjectiveID = objectiveID;
		IsStatic = isStatic;
		Name = name;
		DefaultText = text;
	}

	void AddPosition(ExpansionNamalskQuestHolderPosition pos)
	{
		Positions.Insert(pos);
	}
};

class ExpansionNamalskQuestHolderPosition
{
	vector Position;
	vector Orientation;

	void ExpansionNamalskQuestHolderPosition(vector pos, vector ori)
	{
		Position = pos;
		Orientation = ori;
	}
};