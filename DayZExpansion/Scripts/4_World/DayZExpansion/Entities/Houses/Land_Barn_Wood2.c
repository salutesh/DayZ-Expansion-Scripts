/**
 * Land_Garage_Big.c
 *
 * DayZ Expansion Mod
 * www.dayzexpansion.com
 * © 2021 DayZ Expansion Mod Team
 *
 * This work is licensed under the Creative Commons Attribution-NonCommercial-NoDerivatives 4.0 International License. 
 * To view a copy of this license, visit http://creativecommons.org/licenses/by-nc-nd/4.0/.
 *
*/

class Land_Barn_Wood2 extends BuildingSuper
{
	override bool HasInterior()
	{
		return true;
	}
	
	override void SpawnInterior()
	{
		SpawnInteriorObject("bldr_Misc_WoodenCrate_5x", "-5.200001 -1.370003 6.650002", "0.000000 0.000000 0.000000");
		SpawnInteriorObject("bldr_Misc_WoodenCrate_3x", "-5.507468 -1.623756 5.057117", "0.000000 -0.023367 0.000000");
		SpawnInteriorObject("bldr_Misc_WoodenCrate", "-3.930706 -2.033028 6.429312", "0.000000 -0.023367 0.000000");
		SpawnInteriorObject("bldr_workbench", "-5.694792 -2.331017 0.647865", "1.000000 0.000000 0.000000");
		SpawnInteriorObject("bldr_vise", "-5.050000 -1.626999  2.535000", "0.000000 0.000000 0.000000");
		SpawnInteriorObject("bldr_drill", "-5.723159 -1.144997  1.842319", "-178.999969 0.000000 0.000000");
		SpawnInteriorObject("bldr_grinder", "-5.500618 -1.400002  1.167805", "-89.000000 0.000000 0.000000");
		SpawnInteriorObject("bldr_metal_cutting_saw", "-5.622488 -1.211998 -0.356716", "0.000000 0.000000 0.000000");
		SpawnInteriorObject("ExpansionPropCircularSaw2", "-5.663294 -1.401398  0.188691", "129.999954 0.000000 0.000000");
		SpawnInteriorObject("ExpansionPropCircularSaw3", "-5.641544 -1.550003 0.710631", "179.999985 -89.000046 179.999985");
		SpawnInteriorObject("ExpansionPropToolChest", "-3.826445 -1.728508  0.381419", "-139.999985 -0.023367 0.000000");
		SpawnInteriorObject("ExpansionPropWorkbench2", "-6.057076 -1.828529 -2.727615", "-179.000061 0.000000 0.000000");
		SpawnInteriorObject("ExpansionPropWoodLadder", "-5.307195 -1.385002  3.802349", "-80.000008 0.000000 0.000000");
		SpawnInteriorObject("bldr_misc_through_trailer", "3.376301 -1.900116  -6.475227", "0.000000 0.000000 0.000000");
		SpawnInteriorObject("bldr_misc_pallets3", "4.572605 -1.570000 -4.132239", "0.000000 0.000000 0.000000");
		SpawnInteriorObject("bldr_misc_boardspack3", "4.689282 -1.620323  -1.572562", "-2.000000 0.000000 0.000000");
		SpawnInteriorObject("bldr_Garbage_Bin", "-5.940317 -1.769997 3.275367", "0.000000 -0.023367 0.000000");
		SpawnInteriorObject("bldr_wheel_cart_dz", "3.309037 -2.328133 -3.615693", "0.000000 0.000000 0.000000");
		SpawnInteriorObject("bldr_sawhorse_dz", "-5.100001 -2.334999  -6.800002", "0.000000 0.000000 0.000000");
		SpawnInteriorObject("bldr_canister_dz", "-6.188024 -1.970001 -6.521100", "91.022247 0.000000 0.000000");
		SpawnInteriorObject("bldr_radio_b", "-5.999801 -1.374001 -2.222562", "54.999992 90.000000 -0.000009");
		SpawnInteriorObject("bldr_box_c_multi", "4.306566 -2.330002 6.388833", "93.999969 0.000000 0.000000");
		SpawnInteriorObject("bldr_box_c", "3.287826 -2.022003 7.241081", "-4.000000 0.000000 0.000000");
	}
}