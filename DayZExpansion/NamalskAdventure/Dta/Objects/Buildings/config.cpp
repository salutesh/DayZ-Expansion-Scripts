#define _ARMA_

class CfgPatches
{
	class DayZExpansion_NamalskAdventure_Objects_Buildings
	{
		units[] = {};
		weapons[] = {};
		requiredVersion = 0.1;
		requiredAddons[] = {"DZ_Data","DZ_Structures"};
	};
};
class CfgVehicles
{
	class HouseNoDestruct;
	class ExpansionQuestObjectWoodenCrate: HouseNoDestruct
	{
		scope = 1;
		model = "DZ\structures_bliss\Residential\Misc\Misc_WoodenCrate.p3d";
		forceFarBubble = "true";
	};
	class ExpansionQuestObjectCardboardBox: HouseNoDestruct
	{
		scope = 1;
		model = "DZ\structures\Furniture\Decoration\box_c\box_c.p3d";
		forceFarBubble = "true";
	};
	class ExpansionQuestObjectCardboardBoxMulti: HouseNoDestruct
	{
		scope = 1;
		model = "DZ\structures\Furniture\Decoration\box_c\box_c_multi.p3d";
		forceFarBubble = "true";
	};
};
