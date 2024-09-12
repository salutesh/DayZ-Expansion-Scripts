/**
 * ExpansionCircuitBoardBase.c
 *
 * DayZ Expansion Mod
 * www.dayzexpansion.com
 * © 2022 DayZ Expansion Mod Team
 *
 * This work is licensed under the Creative Commons Attribution-NonCommercial-NoDerivatives 4.0 International License.
 * To view a copy of this license, visit http://creativecommons.org/licenses/by-nc-nd/4.0/.
 *
*/

class ExpansionCircuitBoardBase: ItemBase
{
	int Expansion_GetTier()
	{
		return -1;
	}
};

class ExpansionCircuitBoard_MK1: ExpansionCircuitBoardBase
{
	override int Expansion_GetTier()
	{
		return 1;
	}
};
class ExpansionCircuitBoard_MK2: ExpansionCircuitBoardBase
{
	override int Expansion_GetTier()
	{
		return 2;
	}
};
class ExpansionCircuitBoard_MK3: ExpansionCircuitBoardBase
{
	override int Expansion_GetTier()
	{
		return 3;
	}
};