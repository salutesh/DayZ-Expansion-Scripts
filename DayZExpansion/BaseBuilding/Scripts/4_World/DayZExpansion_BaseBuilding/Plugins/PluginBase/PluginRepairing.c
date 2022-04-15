/**
 * PluginRepairing.c
 *
 * DayZ Expansion Mod
 * www.dayzexpansion.com
 * © 2022 DayZ Expansion Mod Team
 *
 * This work is licensed under the Creative Commons Attribution-NonCommercial-NoDerivatives 4.0 International License. 
 * To view a copy of this license, visit http://creativecommons.org/licenses/by-nc-nd/4.0/.
 *
*/

modded class PluginRepairing
{
	override void CalculateHealth( PlayerBase player, ItemBase kit, Object item, float specialty_weight, string damage_zone = "", bool use_kit_qty = true )
	{
		super.CalculateHealth( player, kit, item, specialty_weight, damage_zone, use_kit_qty );

		ItemBase itemBase = ItemBase.Cast( item );
		if ( itemBase )
			itemBase.UpdateCurrentHealthMap();
	}
}
