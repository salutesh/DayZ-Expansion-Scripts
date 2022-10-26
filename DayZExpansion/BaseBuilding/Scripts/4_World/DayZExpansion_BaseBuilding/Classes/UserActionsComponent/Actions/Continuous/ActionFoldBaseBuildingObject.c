/**
 * ActionFoldBaseBuildingObject.c
 *
 * DayZ Expansion Mod
 * www.dayzexpansion.com
 * © 2022 DayZ Expansion Mod Team
 *
 * This work is licensed under the Creative Commons Attribution-NonCommercial-NoDerivatives 4.0 International License. 
 * To view a copy of this license, visit http://creativecommons.org/licenses/by-nc-nd/4.0/.
 *
*/

modded class ActionFoldBaseBuildingObject
{
	override bool ActionCondition( PlayerBase player, ActionTarget target, ItemBase item )
	{
		bool isDeployableConstruction;
		bool isParkingMeter;

		//! Don't allow to fold (e.g.) camo box and camo tent if not empty
		ItemBase item_base = ItemBase.Cast( target.GetObject() );
		if (item_base)
		{
			isDeployableConstruction = item_base.IsInherited(ExpansionDeployableConstruction);
		#ifdef EXPANSIONMODGARAGE
			isParkingMeter = item_base.IsInherited(ExpansionParkingMeter);
		#endif

			if (!isParkingMeter && item_base.GetNumberOfItems() > 0)
				return false;
		}

		//! Can fold if inside own territory, but not if in enemy territory
		if (player.IsInTerritory())
			return (isDeployableConstruction || super.ActionCondition(player, target, item)) && player.IsInsideOwnTerritory();

		//! Can fold if outside territory
		if (isDeployableConstruction)
			return true;

		return super.ActionCondition(player, target, item);
	}
	
	override void OnFinishProgressServer(ActionData action_data)
	{
		Object targetObject = action_data.m_Target.GetObject();
		if (targetObject && targetObject.IsInherited(ExpansionDeployableConstruction))
		{
			ExpansionDeployableConstruction deployable_construction = ExpansionDeployableConstruction.Cast(action_data.m_Target.GetObject());
			deployable_construction.CreateConstructionKit();
			deployable_construction.DestroyConstruction();
		}
		else
		{
			super.OnFinishProgressServer(action_data);
		}
	}
}