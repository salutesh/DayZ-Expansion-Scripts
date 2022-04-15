/**
 * Flag_Base.c
 *
 * DayZ Expansion Mod
 * www.dayzexpansion.com
 * © 2022 DayZ Expansion Mod Team
 *
 * This work is licensed under the Creative Commons Attribution-NonCommercial-NoDerivatives 4.0 International License. 
 * To view a copy of this license, visit http://creativecommons.org/licenses/by-nc-nd/4.0/.
 *
*/

modded class Flag_Base 
{
    override void OnWasAttached(EntityAI parent, int slot_id)
	{
		super.OnWasAttached(parent,slot_id);
		
		if ( parent.IsKindOf("TerritoryFlag") )
        	SetAllowDamage(false);
	}
	
	override void OnWasDetached(EntityAI parent, int slot_id)
	{
		super.OnWasDetached(parent,slot_id);
		
		if ( parent.IsKindOf("TerritoryFlag") )
        	SetAllowDamage(true);
	}
};
