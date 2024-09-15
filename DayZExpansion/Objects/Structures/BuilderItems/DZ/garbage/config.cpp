class CfgPatches
{
	class BuilderItems_Garbage
	{
		units[]={};
		weapons[]={};
		requiredVersion=0.1;
		requiredAddons[]=
		{
			"DZ_Data",
			"DZ_Structures_Residential"
		};
	};
};
class CfgVehicles
{
	class HouseNoDestruct;
	class bldr_Garbage_Bin: HouseNoDestruct
	{
		scope=1;
		model="\DZ\structures\residential\Misc\Garbage_Bin.p3d";
	};
	class bldr_Garbage_Container: HouseNoDestruct
	{
		scope=1;
		model="\DZ\structures\residential\Misc\Garbage_Container.p3d";
	};
	class bldr_Garbage_Container_Glass: HouseNoDestruct
	{
		scope=1;
		model="\DZ\structures\residential\Misc\Garbage_Container_Glass.p3d";
	};
	class bldr_Garbage_Container_Paper: HouseNoDestruct
	{
		scope=1;
		model="\DZ\structures\residential\Misc\Garbage_Container_Paper.p3d";
	};
	class bldr_Garbage_Container_Plastic: HouseNoDestruct
	{
		scope=1;
		model="\DZ\structures\residential\Misc\Garbage_Container_Plastic.p3d";
	};
	class bldr_Garbage_Ground_6m: HouseNoDestruct
	{
		scope=1;
		model="\DZ\structures\residential\Misc\Garbage_Ground_6m.p3d";
	};
	class bldr_Garbage_GroundSq_3m: HouseNoDestruct
	{
		scope=1;
		model="\DZ\structures\residential\Misc\Garbage_GroundSq_3m.p3d";
	};
	class bldr_Garbage_GroundSq_5m: HouseNoDestruct
	{
		scope=1;
		model="\DZ\structures\residential\Misc\Garbage_GroundSq_5m.p3d";
	};
	class bldr_Garbage_Pile1: HouseNoDestruct
	{
		scope=1;
		model="\DZ\structures\residential\Misc\Garbage_Pile1.p3d";
	};
	class bldr_Garbage_Pile2: HouseNoDestruct
	{
		scope=1;
		model="\DZ\structures\residential\Misc\Garbage_Pile2.p3d";
	};
	class bldr_Garbage_Pile3: HouseNoDestruct
	{
		scope=1;
		model="\DZ\structures\residential\Misc\Garbage_Pile3.p3d";
	};
	class bldr_Garbage_Pile4: HouseNoDestruct
	{
		scope=1;
		model="\DZ\structures\residential\Misc\Garbage_Pile4.p3d";
	};
	class bldr_Garbage_Pile5: HouseNoDestruct
	{
		scope=1;
		model="\DZ\structures\residential\Misc\Garbage_Pile5.p3d";
	};
	class bldr_Garbage_Pile6: HouseNoDestruct
	{
		scope=1;
		model="\DZ\structures\residential\Misc\Garbage_Pile6.p3d";
	};
};
