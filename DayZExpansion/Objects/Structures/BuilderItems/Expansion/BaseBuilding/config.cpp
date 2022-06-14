#define _ARMA_

class CfgPatches
{
	class BuilderItems_Expansion_BaseBuilding
	{
		units[] = {};
		weapons[] = {};
		requiredVersion = 0.1;
		requiredAddons[] = {"DZ_Structures_Signs"};
	};
};
class CfgVehicles
{
	class HouseNoDestruct;
	class bldr_expansion_BarbedWire: HouseNoDestruct
	{
		scope = 1;
		model = "\DayZExpansion\Objects\Basebuilding\Misc\BarbedWire\Wire.p3d";
	};
	class bldr_expansion_Hesco: HouseNoDestruct
	{
		scope = 1;
		model = "\DayZExpansion\Objects\Basebuilding\Misc\Hesco\Hesco.p3d";
	};
	class bldr_expansion_HescoKit: HouseNoDestruct
	{
		scope = 1;
		model = "\DayZExpansion\Objects\Basebuilding\Misc\Hesco\HescoKit.p3d";
	};
	class bldr_expansion_Helipad: HouseNoDestruct
	{
		scope = 1;
		model = "\DayZExpansion\Objects\Basebuilding\Misc\helipad\Helipad.p3d";
	};
	class bldr_expansionBarbedWire: HouseNoDestruct
	{
		scope = 1;
		model = "\DayZExpansion\Objects\Basebuilding\Misc\BarbedWire\Wire.p3d";
	};
	class bldr_expansion_Gunrack: HouseNoDestruct
	{
		scope = 1;
		model = "\DayZExpansion\Objects\Basebuilding\Misc\gunrack\gunrack.p3d";
	};
	class bldr_expansion_Cone: HouseNoDestruct
	{
		scope = 1;
		model = "\DayZExpansion\Objects\Basebuilding\Misc\Signs\Cone.p3d";
	};
	class bldr_expansion_Sign_Danger_01: HouseNoDestruct
	{
		scope = 1;
		model = "\DayZExpansion\Objects\Basebuilding\Misc\Signs\Danger.p3d";
	};
	class bldr_expansion_Sign_Danger_02: HouseNoDestruct
	{
		scope = 1;
		model = "\DayZExpansion\Objects\Basebuilding\Misc\Signs\Danger2.p3d";
	};
	class bldr_expansion_Sign_Danger_03: HouseNoDestruct
	{
		scope = 1;
		model = "\DayZExpansion\Objects\Basebuilding\Misc\Signs\Danger3.p3d";
	};
	class bldr_expansion_Sign_RoadBarrier: HouseNoDestruct
	{
		scope = 1;
		model = "\DayZExpansion\Objects\Basebuilding\Misc\Signs\Roadbarrier.p3d";
		hiddenSelections[] = {"light"};
		hiddenSelectionsMaterials[] = {"\DayZExpansion\Objects\Basebuilding\Misc\Signs\Data\bariera_lampa.rvmat"};
	};
	class bldr_expansion_Sign_RoadBarrier_LightOn: bldr_expansion_Sign_RoadBarrier
	{
		hiddenSelectionsMaterials[] = {"\DayZExpansion\Objects\Basebuilding\Misc\Signs\Data\bariera_lampa_on.rvmat"};
	};
	class bldr_expansion_Stove_Closed: HouseNoDestruct
	{
		scope = 1;
		model = "\DayZExpansion\Objects\Basebuilding\Misc\furniture\Kitchenstove_Elec.p3d";
	};
	class bldr_expansion_Stove_Open: bldr_expansion_Stove_Closed
	{
		class AnimationSources
		{
			class Doors1
			{
				source = "user";
				animPeriod = 0.8;
				initPhase = 1.0;
			};
		};
	};
	class bldr_expansion_CamoBox: HouseNoDestruct
	{
		scope = 1;
		model = "\DayZExpansion\Objects\Basebuilding\Tents\Camo_Box2.p3d";
	};
	class bldr_expansion_CamoTent: HouseNoDestruct
	{
		scope = 1;
		model = "\DayZExpansion\Objects\Basebuilding\Tents\ATent.p3d";
	};
	class bldr_expansion_Safe_Small_Closed: HouseNoDestruct
	{
		scope = 1;
		model = "\DayZExpansion\Objects\Basebuilding\Safes\Safe_02\Safe_02.p3d";
	};
	class bldr_expansion_Safe_Small_Opened: bldr_expansion_Safe_Small_Closed
	{
		class AnimationSources
		{
			class safe_door
			{
				source = "user";
				animPeriod = 1.0;
				initPhase = 1.0;
			};
		};
	};
	class bldr_expansion_Safe_Medium_Closed: HouseNoDestruct
	{
		scope = 1;
		model = "\DayZExpansion\Objects\Basebuilding\Safes\Safe_03\Safe_03.p3d";
	};
	class bldr_expansion_Safe_Medium_Opened: bldr_expansion_Safe_Medium_Closed
	{
		class AnimationSources
		{
			class safe_door
			{
				source = "user";
				animPeriod = 1.0;
				initPhase = 1.0;
			};
		};
	};
	class bldr_expansion_Safe_Large_Closed: HouseNoDestruct
	{
		scope = 1;
		model = "\DayZExpansion\Objects\Basebuilding\Safes\Safe_01\Safe_01.p3d";
	};
	class bldr_expansion_Safe_Large_Opened: bldr_expansion_Safe_Large_Closed
	{
		class AnimationSources
		{
			class safe_door
			{
				source = "user";
				animPeriod = 1.0;
				initPhase = 1.0;
			};
		};
	};
};
