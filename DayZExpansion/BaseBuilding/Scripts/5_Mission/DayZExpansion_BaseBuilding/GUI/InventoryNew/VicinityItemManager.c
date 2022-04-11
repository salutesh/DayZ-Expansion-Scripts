/**
 * VicinityItemManager.c
 *
 * DayZ Expansion Mod
 * www.dayzexpansion.com
 * © 2022 DayZ Expansion Mod Team
 *
 * This work is licensed under the Creative Commons Attribution-NonCommercial-NoDerivatives 4.0 International License. 
 * To view a copy of this license, visit http://creativecommons.org/licenses/by-nc-nd/4.0/.
 *
*/

modded class VicinityItemManager
{
	override void AddVicinityItems(Object object)
	{
		if ( ExpansionShouldShowInVicinity(object) )
			super.AddVicinityItems(object);
	}

	bool ExpansionShouldShowInVicinity(Object filtered_object)
	{
		if ( ExpansionHesco.Cast(filtered_object) )
			return true;
		
		if ( ExpansionCamoBox.Cast(filtered_object) )
			return true;
		
		if ( ExpansionCamoTent.Cast(filtered_object) )
			return true;

		ExpansionBaseBuilding basebuilding = ExpansionBaseBuilding.Cast(filtered_object);
		
		if ( basebuilding )
		{
			if ( basebuilding.IsInherited(ExpansionWallBase) )
			{
				if ( basebuilding.CanDisplayAttachmentCategory("Attachments") ) //! Always show Attachments if available
					return true;
			}
			
			/*
			//! Preparation for the basebuilding update
			if ( basebuilding.IsInherited(ExpansionFloorBase) )
			{
				if ( basebuilding.CanDisplayAttachmentCategory("Attachments") ) //! Always show Attachments if available
					return true;
			}
			*/

			if ( basebuilding.GetInventory().AttachmentCount() > 0 ) //! if there is more than one item, show it
				return true;

			//! GetHealth cannot be called on client. Using GetHealthLevel instead
			if ( basebuilding.GetHealthLevel() != GameConstants.STATE_PRISTINE ) //! if it's damaged show it
				return true;
				
			return !basebuilding.IsLastStage(); //! if it's the last stage hide it
		}

		return true;
	}
};

