#define _ARMA_

class CfgPatches
{
	class DayZExpansion_Vehicles_Ground_Truck_01
	{
		units[] = {};
		weapons[] = {};
		requiredVersion = 0.1;
		requiredAddons[] = {"DayZExpansion_Vehicles_Data"};
	};
};
class CfgVehicles
{
	class ExpansionWheelBase;
	class ExpansionVehicleCarBase;
	class CarDoor;
	class CarScript;
	class ExpansionCarDoorFix;
	class Crew;
	class Driver;
	class CoDriver;
	class SimulationModule;
	class Engine;
	class Axles;
	class Front;
	class Rear;
	class Wheels;
	class Left;
	class Right;
	class AnimationSources;
	class Truck_01_Base;
	class Truck_01_Covered: Truck_01_Base
	{
		doors[] = {"Truck_01_Door_1_1","Truck_01_Door_2_1","Truck_01_Hood"};
		defaultSkin = "Green";
	};
	class Truck_01_Door_1_1: CarDoor
	{
		defaultSkin = "Green";
	};
	class Truck_01_Door_2_1: Truck_01_Door_1_1
	{
		defaultSkin = "Green";
	};
	class Truck_01_Hood: CarDoor
	{
		defaultSkin = "Green";
	};
	class Expansion_Truck_01_Door_1_1: ExpansionCarDoorFix
	{
		newBase = "Truck_01_Door_1_1";
	};
	class Expansion_Truck_01_Door_2_1: ExpansionCarDoorFix
	{
		newBase = "Truck_01_Door_2_1";
	};
	class Expansion_Truck_01_Hood: ExpansionCarDoorFix
	{
		newBase = "Truck_01_Hood";
	};
	class Truck_01_Covered_Green: Truck_01_Covered
	{
		skinBase = "Truck_01_Covered";
		skinName = "Green";
	};
	class Truck_01_Door_1_1_Green: Truck_01_Door_1_1
	{
		skinBase = "Truck_01_Door_1_1";
		skinName = "Green";
	};
	class Truck_01_Door_2_1_Green: Truck_01_Door_2_1
	{
		skinBase = "Truck_01_Door_2_1";
		skinName = "Green";
	};
	class Truck_01_Hood_Green: Truck_01_Hood
	{
		skinBase = "Truck_01_Hood";
		skinName = "Green";
	};
	class Truck_01_Covered_Orange: Truck_01_Covered
	{
		skinBase = "Truck_01_Covered";
		skinName = "Orange";
	};
	class Truck_01_Door_1_1_Orange: Truck_01_Door_1_1
	{
		skinBase = "Truck_01_Door_1_1";
		skinName = "Orange";
	};
	class Truck_01_Door_2_1_Orange: Truck_01_Door_2_1
	{
		skinBase = "Truck_01_Door_2_1";
		skinName = "Orange";
	};
	class Truck_01_Hood_Orange: Truck_01_Hood
	{
		skinBase = "Truck_01_Hood";
		skinName = "Orange";
	};
	class Truck_01_Covered_Blue: Truck_01_Covered
	{
		skinBase = "Truck_01_Covered";
		skinName = "Blue";
	};
	class Truck_01_Door_1_1_Blue: Truck_01_Door_1_1
	{
		skinBase = "Truck_01_Door_1_1";
		skinName = "Blue";
	};
	class Truck_01_Door_2_1_Blue: Truck_01_Door_2_1
	{
		skinBase = "Truck_01_Door_2_1";
		skinName = "Blue";
	};
	class Truck_01_Hood_Blue: Truck_01_Hood
	{
		skinBase = "Truck_01_Hood";
		skinName = "Blue";
	};
	class Truck_01_Covered_Red: Truck_01_Covered
	{
		skinBase = "Truck_01_Covered";
		skinName = "Red";
	};
	class Truck_01_Door_1_1_Red: Truck_01_Door_1_1
	{
		skinBase = "Truck_01_Door_1_1";
		skinName = "Red";
	};
	class Truck_01_Door_2_1_Red: Truck_01_Door_2_1
	{
		skinBase = "Truck_01_Door_2_1";
		skinName = "Red";
	};
	class Truck_01_Hood_Red: Truck_01_Hood
	{
		skinBase = "Truck_01_Hood";
		skinName = "Red";
	};
	class Truck_01_Covered_White: Truck_01_Covered
	{
		skinBase = "Truck_01_Covered";
		skinName = "White";
	};
	class Truck_01_Door_1_1_White: Truck_01_Door_1_1
	{
		skinBase = "Truck_01_Door_1_1";
		skinName = "White";
	};
	class Truck_01_Door_2_1_White: Truck_01_Door_2_1
	{
		skinBase = "Truck_01_Door_2_1";
		skinName = "White";
	};
	class Truck_01_Hood_White: Truck_01_Hood
	{
		skinBase = "Truck_01_Hood";
		skinName = "White";
	};
	class Truck_01_Covered_Black: Truck_01_Covered
	{
		skinBase = "Truck_01_Covered";
		skinName = "Black";
	};
	class Truck_01_Door_1_1_Black: Truck_01_Door_1_1
	{
		skinBase = "Truck_01_Door_1_1";
		skinName = "Black";
	};
	class Truck_01_Door_2_1_Black: Truck_01_Door_2_1
	{
		skinBase = "Truck_01_Door_2_1";
		skinName = "Black";
	};
	class Truck_01_Hood_Black: Truck_01_Hood
	{
		skinBase = "Truck_01_Hood";
		skinName = "Black";
	};
	class Truck_01_Covered_Wine: Truck_01_Covered
	{
		skinBase = "Truck_01_Covered";
		skinName = "Wine";
	};
	class Truck_01_Door_1_1_Wine: Truck_01_Door_1_1
	{
		skinBase = "Truck_01_Door_1_1";
		skinName = "Wine";
	};
	class Truck_01_Door_2_1_Wine: Truck_01_Door_2_1
	{
		skinBase = "Truck_01_Door_2_1";
		skinName = "Wine";
	};
	class Truck_01_Hood_Wine: Truck_01_Hood
	{
		skinBase = "Truck_01_Hood";
		skinName = "Wine";
	};
	class Truck_01_Covered_Lime: Truck_01_Covered
	{
		skinBase = "Truck_01_Covered";
		skinName = "Lime";
	};
	class Truck_01_Door_1_1_Lime: Truck_01_Door_1_1
	{
		skinBase = "Truck_01_Door_1_1";
		skinName = "Lime";
	};
	class Truck_01_Door_2_1_Lime: Truck_01_Door_2_1
	{
		skinBase = "Truck_01_Door_2_1";
		skinName = "Lime";
	};
	class Truck_01_Hood_Lime: Truck_01_Hood
	{
		skinBase = "Truck_01_Hood";
		skinName = "Lime";
	};
	class Truck_01_Covered_Yellow: Truck_01_Covered
	{
		skinBase = "Truck_01_Covered";
		skinName = "Yellow";
	};
	class Truck_01_Door_1_1_Yellow: Truck_01_Door_1_1
	{
		skinBase = "Truck_01_Door_1_1";
		skinName = "Yellow";
	};
	class Truck_01_Door_2_1_Yellow: Truck_01_Door_2_1
	{
		skinBase = "Truck_01_Door_2_1";
		skinName = "Yellow";
	};
	class Truck_01_Hood_Yellow: Truck_01_Hood
	{
		skinBase = "Truck_01_Hood";
		skinName = "Yellow";
	};
	class Truck_01_Covered_YellowBright: Truck_01_Covered
	{
		skinBase = "Truck_01_Covered";
		skinName = "YellowBright";
	};
	class Truck_01_Door_1_1_YellowBright: Truck_01_Door_1_1
	{
		skinBase = "Truck_01_Door_1_1";
		skinName = "YellowBright";
	};
	class Truck_01_Door_2_1_YellowBright: Truck_01_Door_2_1
	{
		skinBase = "Truck_01_Door_2_1";
		skinName = "YellowBright";
	};
	class Truck_01_Hood_YellowBright: Truck_01_Hood
	{
		skinBase = "Truck_01_Hood";
		skinName = "YellowBright";
	};
};
