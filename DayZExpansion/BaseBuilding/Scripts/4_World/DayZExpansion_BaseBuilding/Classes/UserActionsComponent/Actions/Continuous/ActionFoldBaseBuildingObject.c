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
		ItemBase targetItem = ItemBase.Cast( target.GetObject() );
		if (targetItem)
		{
			isDeployableConstruction = targetItem.IsInherited(ExpansionDeployableConstruction);
		#ifdef EXPANSIONMODGARAGE
			isParkingMeter = targetItem.IsInherited(ExpansionParkingMeter);
		#endif

			if (!isParkingMeter && targetItem.GetNumberOfItems() > 0)
				return false;
		}

		//! Standard checks
		if (!isDeployableConstruction && !super.ActionCondition(player, target, item))
			return false;

		//! Can fold if inside own territory, but not if in enemy territory unless whitelisted
		if (player.IsInTerritory())
		{
			if (GetExpansionSettings().GetBaseBuilding().DismantleInsideTerritory)
				return true;

			//! If it was deployable, it's also foldable
			if (ActionDeployObject.CanDeployInTerritory(player, targetItem))
				return true;

			if (GetGame().IsServer() && player.GetIdentity())
				ExpansionNotification("STR_EXPANSION_TERRITORY_TITLE", "STR_EXPANSION_TERRITORY_ENEMY_TERRITORY").Error(player.GetIdentity());

			return false;
		}

		return true;
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