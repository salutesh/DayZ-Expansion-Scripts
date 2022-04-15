/**
 * ExpansionBriefcase.c
 *
 * DayZ Expansion Mod
 * www.dayzexpansion.com
 * © 2022 DayZ Expansion Mod Team
 *
 * This work is licensed under the Creative Commons Attribution-NonCommercial-NoDerivatives 4.0 International License. 
 * To view a copy of this license, visit http://creativecommons.org/licenses/by-nc-nd/4.0/.
 *
*/

class ExpansionBriefcase : Container_Base 
{
	override int GetDamageSystemVersionChange()
	{
		return 110;
	}
	override bool CanReceiveItemIntoCargo( EntityAI item )
	{
		if (!super.CanReceiveItemIntoCargo(item))
			return false;
		
		return item.IsKindOf("ExpansionMoneyBase");
	}
	
	override bool CanLoadItemIntoCargo( EntityAI item )
	{
		if (!super.CanLoadItemIntoCargo(item))
			return false;
		
		return item.IsKindOf("ExpansionMoneyBase");
	}
	
};