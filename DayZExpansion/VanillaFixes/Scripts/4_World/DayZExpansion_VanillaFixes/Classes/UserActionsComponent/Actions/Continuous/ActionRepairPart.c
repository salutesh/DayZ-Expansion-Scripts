/**
 * ActionRepairPart.c
 *
 * DayZ Expansion Mod
 * www.dayzexpansion.com
 * © 2022 DayZ Expansion Mod Team
 *
 * This work is licensed under the Creative Commons Attribution-NonCommercial-NoDerivatives 4.0 International License.
 * To view a copy of this license, visit http://creativecommons.org/licenses/by-nc-nd/4.0/.
 *
*/

modded class ActionRepairPart
{
	//! Fix for T166880: Check if the part is even built before offering it for repair
	override protected bool RepairCondition(PlayerBase player, ActionTarget target, ItemBase item, bool camera_check)
	{
		if (!super.RepairCondition(player, target, item, camera_check))
			return false;

		Object target_object = target.GetObject();

		BaseBuildingBase base_building;
		if ( target_object && target_object.CanUseConstruction() && Class.CastTo(base_building, target_object) )
		{
			string part_name = target_object.GetActionComponentName( target.GetComponentIndex() );
			
			Construction construction = base_building.GetConstruction();
			ConstructionPart construction_part = construction.GetConstructionPart( part_name );
			
			if ( construction_part && !construction_part.IsBuilt() )
				return false;
		}

		return true;
	}
}
