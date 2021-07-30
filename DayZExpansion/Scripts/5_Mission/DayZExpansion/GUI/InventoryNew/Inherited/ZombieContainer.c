/**
 * ZombieContainer.c
 *
 * DayZ Expansion Mod
 * www.dayzexpansion.com
 * © 2020 DayZ Expansion Mod Team
 *
 * This work is licensed under the Creative Commons Attribution-NonCommercial-NoDerivatives 4.0 International License. 
 * To view a copy of this license, visit http://creativecommons.org/licenses/by-nc-nd/4.0/.
 *
*/

modded class ZombieContainer
{
	override void InitGhostSlots()
	{
		super.InitGhostSlots();

		int row, column;
		SlotsIcon icon;

		string config_path_ghosts_slots = "CfgVehicles ZombieBase InventoryEquipment playerSlots";
		array<string> player_ghosts_slots = new array<string>;
		GetGame().ConfigGetTextArray( config_path_ghosts_slots, player_ghosts_slots );
		
		for ( int i = 0; i < player_ghosts_slots.Count(); i++ )
		{
			string slot_name = player_ghosts_slots.Get ( i );
			string path = "CfgSlots" + " " + slot_name;
			string ghostIconPath = "CfgSlots" + " " + slot_name + " expansionGhostIcon";

			if ( GetGame().ConfigIsExisting( path ) && GetGame().ConfigIsExisting( ghostIconPath ) )
			{
				string expansionGhost;
				GetGame().ConfigGetText( ghostIconPath, expansionGhost );

				icon = GetSlotsIcon( row, column );
				icon.GetGhostSlot().LoadImageFile( 0, expansionGhost );
			}
		}
	}
};