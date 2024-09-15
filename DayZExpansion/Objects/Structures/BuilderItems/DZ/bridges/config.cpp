class CfgPatches
{
	class BuilderItems_Bridges
	{
		units[]={};
		weapons[]={};
		requiredVersion=0.1;
		requiredAddons[]=
		{
			"DZ_Data",
			"DZ_Structures_Roads"
		};
	};
};
class CfgVehicles
{
	class HouseNoDestruct;
	class bldr_bridge_metal_25: HouseNoDestruct
	{
		scope=1;
		model="\DZ\structures\roads\bridges\bridge_metal_25.p3d";
	};
	class bldr_bridge_metal_25_1: HouseNoDestruct
	{
		scope=1;
		model="\DZ\structures\roads\bridges\bridge_metal_25_1.p3d";
	};
	class bldr_bridge_stone_25: HouseNoDestruct
	{
		scope=1;
		model="\DZ\structures\roads\bridges\bridge_stone_25.p3d";
	};
	class bldr_bridge_wood_25: HouseNoDestruct
	{
		scope=1;
		model="\DZ\structures\roads\bridges\bridge_wood_25.p3d";
	};
	class bldr_bridge_wood_50: HouseNoDestruct
	{
		scope=1;
		model="\DZ\structures\roads\bridges\bridge_wood_50.p3d";
	};
};
