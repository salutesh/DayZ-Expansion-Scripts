#define _ARMA_

class CfgPatches
{
	class DayZExpansion_Dta_World
	{
		units[] = {"ExpansionAirCondBig","ExpansionAirCondSmall","ExpansionWoodDoorPlanks","ExpansionGasMeter","ExpansionBarricadingWindow","ExpansionLadderLong","ExpansionLadderMedium","ExpansionLadderShort","ExpansionLadderTop","ExpansionLightCondBottom","ExpansionLightCondConn","ExpansionLightCondCorn","ExpansionLightCondMid","ExpansionLightCondTop","ExpansionAirCondA","ExpansionAirCondB","ExpansionAntenaBigRoof","ExpansionAntenaSmallRoofA","ExpansionAntenaSmallRoofB"};
		weapons[] = {};
		requiredVersion = 0.1;
		requiredAddons[] = {"DZ_Vehicles_Wheeled"};
	};
};
class CfgVehicles
{
	class Inventory_Base;
	class HouseNoDestruct;
	class ExpansionAirCondBig: HouseNoDestruct
	{
		scope = 2;
		vehicleClass = "Expansion_Barricading";
		model = "\dz\structures\Proxy_BuildingParts\AirCondition\AirCond_big.p3d";
	};
	class ExpansionAirCondSmall: HouseNoDestruct
	{
		scope = 2;
		vehicleClass = "Expansion_Barricading";
		model = "\dz\structures\Proxy_BuildingParts\AirCondition\AirCond_small.p3d";
	};
	class ExpansionWoodDoorPlanks: HouseNoDestruct
	{
		scope = 2;
		vehicleClass = "Expansion_Barricading";
		model = "\dz\structures\Proxy_BuildingParts\Barricading\door_planks_01.p3d";
	};
	class ExpansionGasMeter: HouseNoDestruct
	{
		scope = 2;
		vehicleClass = "Expansion_Barricading";
		model = "\dz\structures\Proxy_BuildingParts\GasMeterExt\GasMeterExt.p3d";
	};
	class ExpansionBarricadingWindow: HouseNoDestruct
	{
		scope = 2;
		vehicleClass = "Expansion_Barricading";
		model = "\dz\structures\Proxy_BuildingParts\House\HouseV2_02_window.p3d";
	};
	class ExpansionLadderLong: HouseNoDestruct
	{
		scope = 2;
		vehicleClass = "Expansion_Barricading";
		model = "\dz\structures\Proxy_BuildingParts\Ladders\ladder_long_proxy.p3d";
	};
	class ExpansionLadderMedium: HouseNoDestruct
	{
		scope = 2;
		vehicleClass = "Expansion_Barricading";
		model = "\dz\structures\Proxy_BuildingParts\Ladders\ladder_medium_proxy.p3d";
	};
	class ExpansionLadderShort: HouseNoDestruct
	{
		scope = 2;
		vehicleClass = "Expansion_Barricading";
		model = "\dz\structures\Proxy_BuildingParts\Ladders\ladder_short_proxy.p3d";
	};
	class ExpansionLadderTop: HouseNoDestruct
	{
		scope = 2;
		vehicleClass = "Expansion_Barricading";
		model = "\dz\structures\Proxy_BuildingParts\Ladders\ladder_top_proxy.p3d";
	};
	class ExpansionLightCondBottom: HouseNoDestruct
	{
		scope = 2;
		vehicleClass = "Expansion_Barricading";
		model = "\dz\structures\Proxy_BuildingParts\LightningCond\LightningConductorBottom\LightningConductorBottom.p3d";
	};
	class ExpansionLightCondConn: HouseNoDestruct
	{
		scope = 2;
		vehicleClass = "Expansion_Barricading";
		model = "\dz\structures\Proxy_BuildingParts\LightningCond\LightningConductorConnection\LightningConductorConnection.p3d";
	};
	class ExpansionLightCondCorn: HouseNoDestruct
	{
		scope = 2;
		vehicleClass = "Expansion_Barricading";
		model = "\dz\structures\Proxy_BuildingParts\LightningCond\LightningConductorCorner\LightningConductorCorner.p3d";
	};
	class ExpansionLightCondMid: HouseNoDestruct
	{
		scope = 2;
		vehicleClass = "Expansion_Barricading";
		model = "\dz\structures\Proxy_BuildingParts\LightningCond\LightningConductorMiddle\LightningConductorMiddle.p3d";
	};
	class ExpansionLightCondTop: HouseNoDestruct
	{
		scope = 2;
		vehicleClass = "Expansion_Barricading";
		model = "\dz\structures\Proxy_BuildingParts\LightningCond\LightningConductorTop\LightningConductorTop.p3d";
	};
	class ExpansionAirCondA: HouseNoDestruct
	{
		scope = 2;
		vehicleClass = "Expansion_Barricading";
		model = "\dz\structures\Proxy_BuildingParts\Roof\AirCondition\AirCondition_A.p3d";
	};
	class ExpansionAirCondB: HouseNoDestruct
	{
		scope = 2;
		vehicleClass = "Expansion_Barricading";
		model = "\dz\structures\Proxy_BuildingParts\Roof\AirCondition\AirCondition_B.p3d";
	};
	class ExpansionAntenaBigRoof: HouseNoDestruct
	{
		scope = 2;
		vehicleClass = "Expansion_Barricading";
		model = "\dz\structures\Proxy_BuildingParts\Roof\AntennaBigRoof\antenna_big_roof.p3d";
	};
	class ExpansionAntenaSmallRoofA: HouseNoDestruct
	{
		scope = 2;
		vehicleClass = "Expansion_Barricading";
		model = "\dz\structures\Proxy_BuildingParts\Roof\AntennaSmallRoof\antenna_small_roof.p3d";
	};
	class ExpansionAntenaSmallRoofB: HouseNoDestruct
	{
		scope = 2;
		vehicleClass = "Expansion_Barricading";
		model = "\dz\structures\Proxy_BuildingParts\Roof\AntennaSmallRoof\antenna_small_roof_1.p3d";
	};
};
