/**
 * ActionDestroyPart.c
 *
 * DayZ Expansion Mod
 * www.dayzexpansion.com
 * © 2020 DayZ Expansion Mod Team
 *
 * This work is licensed under the Creative Commons Attribution-NonCommercial-NoDerivatives 4.0 International License. 
 * To view a copy of this license, visit http://creativecommons.org/licenses/by-nc-nd/4.0/.
 *
*/

modded class ActionDestroyPart
{
	override bool ActionCondition( PlayerBase player, ActionTarget target, ItemBase item )
	{
		/*
		//! Legacy Melee raiding
		if ( GetExpansionSettings().GetRaid() )
		{
			if ( GetExpansionSettings().GetRaid().AllowMeleeRaidingOnVanilla )
			{
				if ( item.IsInherited(FenceKit) || item.IsInherited(WatchtowerKit) )
				{
					super.ActionCondition( player, target, item );					
				}
			}
		}
		*/
		
		return false;
	}
}