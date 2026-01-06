class CfgPatches
{
	class DayZExpansion_Navigation_Worlds
	{
		units[]={};
		weapons[]={};
		requiredVersion=0.1;
		requiredAddons[]=
		{
			"DZ_Data"
		};
	};
};
class MapDefaults
{
	x=0.1;
	y=0.1;
	w=0.80000001;
	h=0.60000002;
	ptsPerSquareSea=8;
	ptsPerSquareTxt=10;
	ptsPerSquareCLn=10;
	ptsPerSquareExp=10;
	ptsPerSquareCost=10;
	ptsPerSquareFor=9;
	ptsPerSquareForEdge=15;
	ptsPerSquareRoad=4;
	ptsPerSquareObj=15;
	scaleMin=0.001;
	scaleMax=1;
	scaleDefault=0.16;
	maxSatelliteAlpha=1;
	alphaFadeStartScale=2;
	alphaFadeEndScale=2;
	userMapPath="";
	maxUserMapAlpha=0;
	alphaUserMapFadeStartScale=0.34;
	alphaUserMapFadeEndScale=0.34;
	showCountourInterval=1;
	colorBackground[]={0.96899998,0.95700002,0.949,1};
	colorSea[]={0.46700001,0.63099998,0.85100001,0.5};
	colorForest[]={0.38,0.47,0.23,0};
	colorForestBorder[]={0,0,0,0};
	colorRocks[]={0,0,0,0.30000001};
	colorRocksBorder[]={0,0,0,0};
	colorLevels[]={0.28600001,0.177,0.093999997,0.5};
	colorMainCountlines[]={0.57200003,0.354,0.18799999,0.5};
	colorCountlines[]={0.57200003,0.354,0.18799999,0};
	colorMainCountlinesWater[]={0.491,0.57700002,0.70200002,0.60000002};
	colorCountlinesWater[]={0.491,0.57700002,0.70200002,0.30000001};
	colorPowerLines[]={0.1,0.1,0.1,1};
	colorRailWay[]={0.80000001,0.2,0,1};
	colorNames[]={0.1,0.1,0.1,0.89999998};
	colorInactive[]={1,1,1,0};
	colorOutside[]={0,0,0,1};
	colorTracks[]={0.83999997,0.75999999,0.64999998,0.15000001};
	colorTracksFill[]={0.83999997,0.75999999,0.64999998,1};
	colorRoads[]={0.69999999,0.69999999,0.69999999,1};
	colorRoadsFill[]={1,1,1,1};
	colorMainRoads[]={0.89999998,0.5,0.30000001,1};
	colorMainRoadsFill[]={1,0.60000002,0.40000001,1};
	colorGrid[]={0.1,0.1,0.1,0.60000002};
	colorGridMap[]={0.1,0.1,0.1,0.60000002};
	colorTrails[]={0.83999997,0.75999999,0.64999998,0.15000001};
	colorTrailsFill[]={0.83999997,0.75999999,0.64999998,0.64999998};
	colorMountPoint[]={0.44999999,0.40000001,0.25,1};
	widthRailWay=4;
	fontLabel="gui/fonts/MetronBook-Bold58";
	sizeExLabel="(((((safezoneW / safezoneH) min 1.2) / 1.2) / 25) * 0.8)";
	fontGrid="gui/fonts/MetronBook-Book58";
	sizeExGrid=0.02;
	fontUnits="gui/fonts/MetronBook-Bold58";
	sizeExUnits="(((((safezoneW / safezoneH) min 1.2) / 1.2) / 25) * 0.8)";
	fontNames="gui/fonts/MetronBook-Bold58";
	sizeExNames="(((((safezoneW / safezoneH) min 1.2) / 1.2) / 25) * 0.8) * 2";
	fontInfo="gui/fonts/MetronBook-Bold58";
	sizeExInfo="(((((safezoneW / safezoneH) min 1.2) / 1.2) / 25) * 0.8)";
	fontLevel="gui/fonts/MetronBook-Book58";
	sizeExLevel=0.02;
	text="#(argb,8,8,3)color(1,1,1,1)";
	textureComboBoxColor="#(argb,8,8,3)color(1,1,1,1)";
	class Legend
	{
		x=0.050000001;
		y=0.85000002;
		w=0.40000001;
		h=0.1;
		font="gui/fonts/MetronBook-Bold58";
		sizeEx=0.02;
		colorBackground[]={1,1,1,0.5};
		color[]={0,0,0,1};
	};
	class Bush
	{
		icon="\dz\gear\navigation\data\map_bush_ca.paa";
		color[]={0.23,0.31999999,0.1,0.80000001};
		size=14;
		importance="0.2 * 14 * 0.05";
		coefMin=0.25;
		coefMax=4;
	};
	class SmallTree
	{
		icon="\dz\gear\navigation\data\map_smalltree_ca.paa";
		color[]={0.23,0.31999999,0.1,0.80000001};
		size=12;
		importance="0.6 * 12 * 0.05";
		coefMin=0.25;
		coefMax=4;
	};
	class Tree
	{
		icon="\dz\gear\navigation\data\map_tree_ca.paa";
		color[]={0.23,0.31999999,0.1,0.80000001};
		size=12;
		importance="0.9 * 16 * 0.05";
		coefMin=0.25;
		coefMax=4;
	};
	class Rock
	{
		icon="\dz\gear\navigation\data\map_rock_ca.paa";
		color[]={0.1,0.1,0.1,0.80000001};
		size=12;
		importance="0.5 * 12 * 0.05";
		coefMin=0.25;
		coefMax=4;
	};
	class Fuelstation
	{
		icon="\dz\gear\navigation\data\map_fuelstation_ca.paa";
		size=16;
		color[]={1,1,1,1};
		importance="2 * 16 * 0.05";
		coefMin=0.75;
		coefMax=4;
	};
	class Lighthouse
	{
		icon="\dz\gear\navigation\data\map_lighthouse_ca.paa";
		size=14;
		color[]={1,1,1,1};
		importance="3 * 16 * 0.05";
		coefMin=0.89999998;
		coefMax=4;
	};
	class Stack
	{
		icon="\dz\gear\navigation\data\map_stack_ca.paa";
		size=16;
		color[]={1,1,1,1};
		importance="2 * 16 * 0.05";
		coefMin=0.89999998;
		coefMax=4;
	};
	class Transmitter
	{
		icon="\dz\gear\navigation\data\map_transmitter_ca.paa";
		color[]={1,1,1,1};
		size=16;
		importance="2 * 16 * 0.05";
		coefMin=0.89999998;
		coefMax=4;
	};
	class Watertower
	{
		icon="\dz\gear\navigation\data\map_watertower_ca.paa";
		color[]={0.2,0.44999999,0.69999999,1};
		size=16;
		importance="1.2 * 16 * 0.05";
		coefMin=0.89999998;
		coefMax=4;
	};
	class Shipwreck
	{
		icon="\dz\gear\navigation\data\map_shipwreck_ca.paa";
		color[]={1,1,1,1};
		size=16;
		importance="1.2 * 16 * 0.05";
		coefMin=0.89999998;
		coefMax=4;
	};
	class Monument
	{
		icon="\dz\gear\navigation\data\map_monument_ca.paa";
		color[]={1,1,1,1};
		size=20;
		importance="1.2 * 16 * 0.05";
		coefMin=0.25;
		coefMax=4;
	};
	class BusStop
	{
		icon="\dz\gear\navigation\data\map_busstop_ca.paa";
		color[]={0.15000001,0.25999999,0.87,1};
		size=12;
		importance="1 * 10 * 0.05";
		coefMin=0.25;
		coefMax=4;
	};
	class Fountain
	{
		icon="\dz\gear\navigation\data\map_fountain_ca.paa";
		color[]={0.2,0.44999999,0.69999999,1};
		size=15;
		importance="1 * 12 * 0.05";
		coefMin=0.25;
		coefMax=4;
	};
	class Waterpump
	{
		icon="\dz\gear\navigation\data\map_waterpump_ca.paa";
		color[]={0.2,0.44999999,0.69999999,1};
		size=16;
		importance="1 * 12 * 0.05";
		coefMin=0.25;
		coefMax=4;
	};
	class Waterspring
	{
		icon="\dz\gear\navigation\data\map_waterspring_ca.paa";
		color[]={0.1,0.30000001,0.55000001,1};
		size=16;
		importance="1 * 12 * 0.05";
		coefMin=0.75;
		coefMax=4;
	};
	class Hospital
	{
		icon="\dz\gear\navigation\data\map_hospital_ca.paa";
		color[]={0.15000001,0.25999999,0.87,1};
		size=17;
		importance="2 * 16 * 0.05";
		coefMin=0.5;
		coefMax=4;
	};
	class Store
	{
		icon="\dz\gear\navigation\data\map_store_ca.paa";
		color[]={1,1,1,1};
		size=20;
		importance="2 * 16 * 0.05";
		coefMin=0.5;
		coefMax=4;
	};
	class Police
	{
		icon="\dz\gear\navigation\data\map_police_ca.paa";
		color[]={0.11,0.1,0.25,1};
		size=20;
		importance="2 * 16 * 0.05";
		coefMin=0.5;
		coefMax=4;
	};
	class FireDep
	{
		icon="\dz\gear\navigation\data\map_firedep_ca.paa";
		color[]={0.764,0,0,1};
		size=20;
		importance="2 * 16 * 0.05";
		coefMin=0.5;
		coefMax=4;
	};
	class TouristSign
	{
		icon="\dz\gear\navigation\data\map_tsign_ca.paa";
		color[]={1,1,1,1};
		size=18;
		importance="2 * 16 * 0.05";
		coefMin=0.5;
		coefMax=4;
	};
	class Tourism
	{
		icon="\dz\gear\navigation\data\map_tourism_ca.paa";
		color[]={1,1,1,1};
		size=14;
		importance="2 * 16 * 0.05";
		coefMin=0.5;
		coefMax=4;
	};
	class Cross
	{
		icon="\dz\gear\navigation\data\map_cross_ca.paa";
		size=16;
		color[]={1,1,1,1};
		importance="0.7 * 16 * 0.05";
		coefMin=0.25;
		coefMax=4;
	};
	class Chapel
	{
		icon="\dz\gear\navigation\data\map_chapel_ca.paa";
		color[]={1,1,1,1};
		size=16;
		importance="1 * 16 * 0.05";
		coefMin=0.89999998;
		coefMax=4;
	};
	class Church
	{
		icon="\dz\gear\navigation\data\map_church_ca.paa";
		color[]={1,1,1,1};
		size=10;
		importance="2 * 16 * 0.05";
		coefMin=0.89999998;
		coefMax=4;
	};
	class ViewTower
	{
		icon="\dz\gear\navigation\data\map_viewtower_ca.paa";
		color[]={1,1,1,1};
		size=16;
		importance="2.5 * 16 * 0.05";
		coefMin=0.5;
		coefMax=4;
	};
	class TouristShelter
	{
		icon="\dz\gear\navigation\data\map_tshelter_ca.paa";
		color[]={1,1,1,1};
		size=16;
		importance="2 * 16 * 0.05";
		coefMin=0.5;
		coefMax=4;
	};
	class Quay
	{
		icon="\dz\gear\navigation\data\map_quay_ca.paa";
		size=16;
		color[]={1,1,1,1};
		importance="2 * 16 * 0.05";
		coefMin=0.5;
		coefMax=4;
	};
	class Bunker
	{
		icon="\dz\gear\navigation\data\map_bunker_ca.paa";
		color[]={1,1,1,1};
		size=14;
		importance="1.5 * 14 * 0.05";
		coefMin=0.25;
		coefMax=4;
	};
	class Fortress
	{
		icon="\dz\gear\navigation\data\map_fortress_ca.paa";
		size=16;
		color[]={1,1,1,1};
		importance="2 * 16 * 0.05";
		coefMin=0.25;
		coefMax=4;
	};
	class Rocks
	{
		icon="\dz\gear\navigation\data\map_rock_ca.paa";
		color[]={0.1,0.1,0.1,0.80000001};
		size=12;
		importance="0.5 * 12 * 0.05";
		coefMin=0.25;
		coefMax=4;
	};
	class Ruin
	{
		icon="\dz\gear\navigation\data\map_ruin_ca.paa";
		size=16;
		color[]={1,1,1,1};
		importance="1.2 * 16 * 0.05";
		coefMin=1;
		coefMax=4;
	};
};
class RscMapControl: MapDefaults
{
	colorCountlines[]={0.57200003,0.354,0.18799999,0.2};
	colorTracks[]={0.2,0.13699999,0.078000002,0};
	colorTracksFill[]={0.2,0.13699999,0.078000002,0};
	colorRoads[]={0.66299999,0.66299999,0.66299999,1};
	colorRoadsFill[]={0.66299999,0.66299999,0.66299999,1};
	colorMainRoads[]={0.51800001,0.29800001,0.0080000004,1};
	colorMainRoadsFill[]={0.51800001,0.29800001,0.0080000004,1};
	class Bush
	{
		icon="\dz\gear\navigation\data\map_bush_ca.paa";
		color[]={0.23,0.31999999,0.1,0.2};
		size=14;
		importance="0.2 * 14 * 0.05";
		coefMin=0.25;
		coefMax=4;
	};
	class SmallTree
	{
		icon="\dz\gear\navigation\data\map_smalltree_ca.paa";
		color[]={0.23,0.31999999,0.1,0.30000001};
		size=12;
		importance="0.6 * 12 * 0.05";
		coefMin=0.25;
		coefMax=4;
	};
};
