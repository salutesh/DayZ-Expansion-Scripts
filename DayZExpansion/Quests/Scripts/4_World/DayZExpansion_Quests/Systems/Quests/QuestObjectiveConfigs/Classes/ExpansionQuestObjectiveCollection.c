/**
 * ExpansionQuestObjectiveCollection.c
 *
 * DayZ Expansion Mod
 * www.dayzexpansion.com
 * © 2022 DayZ Expansion Mod Team
 *
 * This work is licensed under the Creative Commons Attribution-NonCommercial-NoDerivatives 4.0 International License.
 * To view a copy of this license, visit http://creativecommons.org/licenses/by-nc-nd/4.0/.
 *
*/

//! DEPRECATED BUT HERE FOR CONVERSION
class ExpansionQuestObjectiveCollection
{
	protected int Amount = 0;
	protected string ClassName = string.Empty;

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
};