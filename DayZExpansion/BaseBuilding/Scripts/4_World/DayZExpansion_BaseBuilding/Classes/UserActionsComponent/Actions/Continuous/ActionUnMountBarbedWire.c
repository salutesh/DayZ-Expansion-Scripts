/**
 * ActionUnmountBarbedWire.c
 *
 * DayZ Expansion Mod
 * www.dayzexpansion.com
 * © 2022 DayZ Expansion Mod Team
 *
 * This work is licensed under the Creative Commons Attribution-NonCommercial-NoDerivatives 4.0 International License.
 * To view a copy of this license, visit http://creativecommons.org/licenses/by-nc-nd/4.0/.
 *
*/

modded class ActionUnmountBarbedWire
{
	override bool ActionCondition( PlayerBase player, ActionTarget target, ItemBase item )
	{
		if (!GetExpansionSettings().GetBaseBuilding(false).IsLoaded())
			return false;

		bool isInTerritory = player.IsInTerritory();
		bool isInsideOwnTerritory;
		//! https://feedback.bistudio.com/T173348
		if (isInTerritory && player.IsInsideOwnTerritory())
			isInsideOwnTerritory = true;
		bool dismantleOutsideTerritory;
		if (!isInTerritory && GetExpansionSettings().GetBaseBuilding().DismantleOutsideTerritory)
			dismantleOutsideTerritory = true;

		if ( isInsideOwnTerritory || dismantleOutsideTerritory )
		{
			return super.ActionCondition(player, target, item);
		}

		return false;
	}
};
