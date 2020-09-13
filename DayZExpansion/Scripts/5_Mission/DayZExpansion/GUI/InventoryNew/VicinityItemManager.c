/**
 * VicinityItemManager.c
 *
 * DayZ Expansion Mod
 * www.dayzexpansion.com
 * © 2020 DayZ Expansion Mod Team
 *
 * This work is licensed under the Creative Commons Attribution-NonCommercial-NoDerivatives 4.0 International License. 
 * To view a copy of this license, visit http://creativecommons.org/licenses/by-nc-nd/4.0/.
 *
*/

modded class VicinityItemManager
{
	override bool IsObstructed (Object filtered_object)
	{
		if ( ExpansionFlagBase.Cast(filtered_object) )
			return super.IsObstructed(filtered_object);					
		
		if ( ExpansionHesco.Cast(filtered_object) )
			return super.IsObstructed(filtered_object);
		
		if ( ExpansionCamoBox.Cast(filtered_object) )
			return super.IsObstructed(filtered_object);
		
		if ( ExpansionCamoTent.Cast(filtered_object) )
			return super.IsObstructed(filtered_object);			
		
		if ( ExpansionWallBase.Cast(filtered_object) )
		{
			return ExpansionWallBase.Cast(filtered_object).GetInventory().AttachmentCount() == 0 && !ExpansionWallBase.Cast(filtered_object).CanDisplayAttachmentCategory("Attachments"); //false
		}		
		
		if ( ExpansionBaseBuilding.Cast(filtered_object) )
		{
			return ExpansionBaseBuilding.Cast(filtered_object).GetInventory().AttachmentCount() == 0; //false
		}
		return super.IsObstructed(filtered_object);
	}
};

