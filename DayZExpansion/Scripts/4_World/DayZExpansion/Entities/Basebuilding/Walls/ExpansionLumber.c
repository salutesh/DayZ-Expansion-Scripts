/**
 * ExpansionLumber.c
 *
 * DayZ Expansion Mod
 * www.dayzexpansion.com
 * © 2021 DayZ Expansion Mod Team
 *
 * This work is licensed under the Creative Commons Attribution-NonCommercial-NoDerivatives 4.0 International License. 
 * To view a copy of this license, visit http://creativecommons.org/licenses/by-nc-nd/4.0/.
 *
*/

class ExpansionLumberBase extends ItemBase
{
	override void SetActions()
	{
		super.SetActions();
		
		AddAction(ActionAttachToConstruction);
	}
};
class ExpansionLumber0_5 extends ExpansionLumberBase{};
class ExpansionLumber1 extends ExpansionLumberBase{};
class ExpansionLumber1_5 extends ExpansionLumberBase{};
class ExpansionLumber3 extends ExpansionLumberBase{};
