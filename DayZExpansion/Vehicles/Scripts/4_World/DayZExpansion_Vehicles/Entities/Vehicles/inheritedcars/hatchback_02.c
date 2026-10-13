/**
 * Hatchback_02.c
 *
 * DayZ Expansion Mod
 * www.dayzexpansion.com
 * © 2022 DayZ Expansion Mod Team
 *
 * This work is licensed under the Creative Commons Attribution-NonCommercial-NoDerivatives 4.0 International License.
 * To view a copy of this license, visit http://creativecommons.org/licenses/by-nc-nd/4.0/.
 *
 */

modded class Hatchback_02
{
	override string ExpansionGetWheelType(int slot_id)
	{
		return "Hatchback_02_Wheel";
	}

	override void OnDebugSpawn()
	{
		SpawnUniversalParts();
		SpawnAdditionalItems();
		FillUpCarFluids();

		GameInventory inventory = GetInventory();
		inventory.CreateInInventory("Hatchback_02_Wheel");
		inventory.CreateInInventory("Hatchback_02_Wheel");
		inventory.CreateInInventory("Hatchback_02_Wheel");
		inventory.CreateInInventory("Hatchback_02_Wheel");
		
		string skinName = ConfigGetString("skinName");
		if (skinName)
			skinName = "_" + skinName;

		inventory.CreateInInventory("Hatchback_02_Door_1_1" + skinName);
		inventory.CreateInInventory("Hatchback_02_Door_1_2" + skinName);
		inventory.CreateInInventory("Hatchback_02_Door_2_1" + skinName);
		inventory.CreateInInventory("Hatchback_02_Door_2_2" + skinName);
		inventory.CreateInInventory("Hatchback_02_Hood" + skinName);
		inventory.CreateInInventory("Hatchback_02_Trunk" + skinName);

		//-----IN CAR CARGO
		inventory.CreateInInventory("Hatchback_02_Wheel");
		inventory.CreateInInventory("Hatchback_02_Wheel");
	}
};
