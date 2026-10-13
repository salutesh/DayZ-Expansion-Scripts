/**
 * CivilianSedan.c
 *
 * DayZ Expansion Mod
 * www.dayzexpansion.com
 * © 2022 DayZ Expansion Mod Team
 *
 * This work is licensed under the Creative Commons Attribution-NonCommercial-NoDerivatives 4.0 International License.
 * To view a copy of this license, visit http://creativecommons.org/licenses/by-nc-nd/4.0/.
 *
 */

modded class CivilianSedan
{
	override string ExpansionGetWheelType(int slot_id)
	{
		return "CivSedanWheel";
	}

	override void OnDebugSpawn()
	{
		SpawnUniversalParts();
		SpawnAdditionalItems();
		FillUpCarFluids();

		GameInventory inventory = GetInventory();
		inventory.CreateInInventory("CivSedanWheel");
		inventory.CreateInInventory("CivSedanWheel");
		inventory.CreateInInventory("CivSedanWheel");
		inventory.CreateInInventory("CivSedanWheel");
		
		string skinName = ConfigGetString("skinName");
		if (skinName)
			skinName = "_" + skinName;

		inventory.CreateInInventory("CivSedanDoors_Driver" + skinName);
		inventory.CreateInInventory("CivSedanDoors_CoDriver" + skinName);
		inventory.CreateInInventory("CivSedanDoors_BackLeft" + skinName);
		inventory.CreateInInventory("CivSedanDoors_BackRight" + skinName);
		inventory.CreateInInventory("CivSedanHood" + skinName);
		inventory.CreateInInventory("CivSedanTrunk" + skinName);

		//-----IN CAR CARGO
		inventory.CreateInInventory("CivSedanWheel");
		inventory.CreateInInventory("CivSedanWheel");
	}
};
