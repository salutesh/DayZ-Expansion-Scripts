#define _ARMA_

class CfgPatches
{
	class DayZExpansion_NamalskAdventure_Objects_Lore
	{
		units[] = {};
		weapons[] = {};
		requiredVersion = 0.1;
		requiredAddons[] = {"DZ_Data","DZ_Structures_Residential","ns2_build_a3"};
	};
};
class CfgVehicles
{
	class HouseNoDestruct;
	class Inventory_Base;
	class Container_Base;
	class Expansion_Satellite_Control: HouseNoDestruct
	{
		scope = 2;
		displayName = "Satellite Control";
		descriptionShort = "";
		model = "\nst\ns2\build\a3\proxy\a3_flaxbt_panel.p3d";
		handheld = "false";
		weight = 50000;
		physLayer = "item_large";
		carveNavmesh = 1;
		storageCategory = 10;
	};
	class Expansion_CommunityGoals_Board: HouseNoDestruct
	{
		scope = 2;
		model = "DZ\structures\Residential\Misc\Misc_NoticeBoard2.p3d";
	};
};
