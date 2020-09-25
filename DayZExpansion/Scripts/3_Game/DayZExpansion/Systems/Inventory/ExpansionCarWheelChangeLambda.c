/**
 * ExpansionCarWheelChangeLambda.c
 *
 * DayZ Expansion Mod
 * www.dayzexpansion.com
 * © 2020 DayZ Expansion Mod Team
 *
 * This work is licensed under the Creative Commons Attribution-NonCommercial-NoDerivatives 4.0 International License. 
 * To view a copy of this license, visit http://creativecommons.org/licenses/by-nc-nd/4.0/.
 *
*/

class ExpansionCarWheelChangeLambda : ReplaceItemWithNewLambdaBase
{
 	override void CopyOldPropertiesToNew( notnull EntityAI old_item, EntityAI new_item )
	{
		hndDebugPrint("[inv] ReplaceItemWithNewLambdaBase Step E) Copying props " + old_item + " --> " + new_item);
		float oldHealth = old_item.GetHealth() / old_item.GetMaxHealth();
		new_item.SetHealth( new_item.GetMaxHealth() * oldHealth );
	}
};