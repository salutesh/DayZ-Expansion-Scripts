#define _ARMA_

class CfgPatches
{
	class BuilderItems_StaticItems_Backpacks
	{
		units[] = {"bldr_backpack_alice_base","bldr_backpack_alice_green","bldr_backpack_alice_black","bldr_backpack_leathersack_base","bldr_backpack_leathersack_natural","bldr_backpack_leathersack_black"};
		weapons[] = {};
		requiredVersion = 0.1;
		requiredAddons[] = {"DZ_Data","DZ_Structures_Signs"};
	};
};
class CfgVehicles
{
	class HouseNoDestruct;
	class bldr_backpack_alice_base: HouseNoDestruct
	{
		scope = 2;
		model = "\dz\characters\backpacks\alicebackpack_g.p3d";
		hiddenSelections[] = {"camoGround","camoMale","camoFemale"};
	};
	class bldr_backpack_alice_green: bldr_backpack_alice_base
	{
		scope = 2;
		hiddenSelectionsTextures[] = {"\dz\characters\backpacks\data\alicebackpack_co.paa","\dz\characters\backpacks\data\alicebackpack_co.paa","\dz\characters\backpacks\data\alicebackpack_co.paa"};
	};
	class bldr_backpack_alice_black: bldr_backpack_alice_base
	{
		scope = 2;
		hiddenSelectionsTextures[] = {"\dz\characters\backpacks\data\alicebackpack_black_co.paa","\dz\characters\backpacks\data\alicebackpack_black_co.paa","\dz\characters\backpacks\data\alicebackpack_black_co.paa"};
	};
	class bldr_backpack_leathersack_base: HouseNoDestruct
	{
		scope = 2;
		model = "\dz\characters\backpacks\Bag_LeatherSack_g.p3d";
		hiddenSelections[] = {"camoGround","camoMale","camoFemale"};
	};
	class bldr_backpack_leathersack_natural: bldr_backpack_leathersack_base
	{
		scope = 2;
		model = "\dz\characters\backpacks\Bag_LeatherSack_g.p3d";
		hiddenSelectionsMaterials[] = {"dz\characters\backpacks\data\Bag_LeatherSack_natural.rvmat","dz\characters\backpacks\data\Bag_LeatherSack_natural.rvmat","dz\characters\backpacks\data\Bag_LeatherSack_natural.rvmat"};
	};
	class bldr_backpack_leathersack_black: bldr_backpack_leathersack_base
	{
		scope = 2;
		model = "\dz\characters\backpacks\Bag_LeatherSack_g.p3d";
		hiddenSelectionsMaterials[] = {"dz\characters\backpacks\data\Bag_LeatherSack_Black.rvmat","dz\characters\backpacks\data\Bag_LeatherSack_Black.rvmat","dz\characters\backpacks\data\Bag_LeatherSack_Black.rvmat"};
	};
};
