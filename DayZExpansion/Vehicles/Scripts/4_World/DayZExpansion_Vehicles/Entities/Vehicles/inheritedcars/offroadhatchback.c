/**
 * OffroadHatchback.c
 *
 * DayZ Expansion Mod
 * www.dayzexpansion.com
 * © 2022 DayZ Expansion Mod Team
 *
 * This work is licensed under the Creative Commons Attribution-NonCommercial-NoDerivatives 4.0 International License.
 * To view a copy of this license, visit http://creativecommons.org/licenses/by-nc-nd/4.0/.
 *
 */

modded class OffroadHatchback
{
	override string ExpansionGetWheelType(int slot_id)
	{
		return "HatchbackWheel";
	}
	
	override void OnDebugSpawn()
	{
		SpawnUniversalParts();
		SpawnAdditionalItems();
		FillUpCarFluids();

		GameInventory inventory = GetInventory();
		inventory.CreateInInventory("HatchbackWheel");
		inventory.CreateInInventory("HatchbackWheel");
		inventory.CreateInInventory("HatchbackWheel" );
		inventory.CreateInInventory("HatchbackWheel");
		
		string skinName = ConfigGetString("skinName");
		if (skinName)
			skinName = "_" + skinName;

		inventory.CreateInInventory("HatchbackDoors_Driver" + skinName);
		inventory.CreateInInventory("HatchbackDoors_CoDriver" + skinName);
		inventory.CreateInInventory("HatchbackHood" + skinName);
		inventory.CreateInInventory("HatchbackTrunk" + skinName);

		//-----IN CAR CARGO
		inventory.CreateInInventory("HatchbackWheel");
		inventory.CreateInInventory("HatchbackWheel");
	}
};
