#define _ARMA_

class CfgPatches
{
	class BuilderItems_Garbage
	{
		units[] = {"bldr_Garbage_Bin","bldr_Garbage_Container","bldr_Garbage_Container_Glass","bldr_Garbage_Container_Paper","bldr_Garbage_Container_Plastic","bldr_Garbage_Ground_6m","bldr_Garbage_GroundSq_3m","bldr_Garbage_GroundSq_5m","bldr_Garbage_Pile1","bldr_Garbage_Pile2","bldr_Garbage_Pile3","bldr_Garbage_Pile4","bldr_Garbage_Pile5","bldr_Garbage_Pile6"};
		weapons[] = {};
		requiredVersion = 0.1;
		requiredAddons[] = {"DZ_Structures_Residential"};
	};
};
class CfgVehicles
{
	class HouseNoDestruct;
	class bldr_Garbage_Bin: HouseNoDestruct
	{
		scope = 2;
		model = "\DZ\structures\residential\Misc\Garbage_Bin.p3d";
	};
	class bldr_Garbage_Container: HouseNoDestruct
	{
		scope = 2;
		model = "\DZ\structures\residential\Misc\Garbage_Container.p3d";
	};
	class bldr_Garbage_Container_Glass: HouseNoDestruct
	{
		scope = 2;
		model = "\DZ\structures\residential\Misc\Garbage_Container_Glass.p3d";
	};
	class bldr_Garbage_Container_Paper: HouseNoDestruct
	{
		scope = 2;
		model = "\DZ\structures\residential\Misc\Garbage_Container_Paper.p3d";
	};
	class bldr_Garbage_Container_Plastic: HouseNoDestruct
	{
		scope = 2;
		model = "\DZ\structures\residential\Misc\Garbage_Container_Plastic.p3d";
	};
	class bldr_Garbage_Ground_6m: HouseNoDestruct
	{
		scope = 2;
		model = "\DZ\structures\residential\Misc\Garbage_Ground_6m.p3d";
		physLayer = "item_small";
		carveNavmesh = 0;
	};
	class bldr_Garbage_GroundSq_3m: HouseNoDestruct
	{
		scope = 2;
		model = "\DZ\structures\residential\Misc\Garbage_GroundSq_3m.p3d";
		physLayer = "item_small";
		carveNavmesh = 0;
	};
	class bldr_Garbage_GroundSq_5m: HouseNoDestruct
	{
		scope = 2;
		model = "\DZ\structures\residential\Misc\Garbage_GroundSq_5m.p3d";
		physLayer = "item_small";
		carveNavmesh = 0;
	};
	class bldr_Garbage_Pile1: HouseNoDestruct
	{
		scope = 2;
		model = "\DZ\structures\residential\Misc\Garbage_Pile1.p3d";
	};
	class bldr_Garbage_Pile2: HouseNoDestruct
	{
		scope = 2;
		model = "\DZ\structures\residential\Misc\Garbage_Pile2.p3d";
	};
	class bldr_Garbage_Pile3: HouseNoDestruct
	{
		scope = 2;
		model = "\DZ\structures\residential\Misc\Garbage_Pile3.p3d";
	};
	class bldr_Garbage_Pile4: HouseNoDestruct
	{
		scope = 2;
		model = "\DZ\structures\residential\Misc\Garbage_Pile4.p3d";
	};
	class bldr_Garbage_Pile5: HouseNoDestruct
	{
		scope = 2;
		model = "\DZ\structures\residential\Misc\Garbage_Pile5.p3d";
	};
	class bldr_Garbage_Pile6: HouseNoDestruct
	{
		scope = 2;
		model = "\DZ\structures\residential\Misc\Garbage_Pile6.p3d";
	};
};
