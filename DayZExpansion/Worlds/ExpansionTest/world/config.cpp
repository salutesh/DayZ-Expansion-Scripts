#define _ARMA_

class CfgPatches
{
	class DayZExpansion_Worlds_ExpansionTest_World
	{
		requiredAddons[] = {"DZ_Data"};
		units[] = {};
		weapons[] = {};
		worlds[] = {"ExpansionTest"};
	};
};
class CfgCharacterScenes
{
	class ExpansionTest
	{
		class loc1
		{
			target[] = {920.177,788.719,27.91};
			position[] = {920.046,780.009,1.1};
			fov = 0.7236;
			date[] = {2011,10,11,11,22};
			overcast = 0.15;
			rain = 0.0;
			fog = 0.0;
		};
	};
};
class CfgWorlds
{
	class CAWorld;
	class ExpansionTest: CAWorld
	{
		description = "Expansion Test";
		longitude = 50.0;
		latitude = -6.0;
		worldName = "DayZExpansion\Worlds\ExpansionTest\world\ExpansionTest.wrp";
		class Navmesh
		{
			navmeshName = "DayZExpansion\Worlds\ExpansionTest\navmesh\ExpansionTest.nm";
			filterIsolatedIslandsOnLoad = 1;
			visualiseOffset = 0.0;
			class GenParams
			{
				tileWidth = 50.0;
				cellSize1 = 0.25;
				cellSize2 = 0.1;
				cellSize3 = 0.1;
				filterIsolatedIslands = 1;
				seedPosition[] = {2560,0,2560};
				class Agent
				{
					diameter = 0.6;
					standHeight = 1.5;
					crouchHeight = 1.0;
					proneHeight = 0.5;
					maxStepHeight = 0.45;
					maxSlope = 60.0;
				};
				class Links
				{
					class ZedJump387_050
					{
						jumpLength = 1.5;
						jumpHeight = 0.5;
						minCenterHeight = 0.3;
						jumpDropdownMin = 0.5;
						jumpDropdownMax = -0.5;
						areaType = "jump0";
						flags[] = {"jumpOver"};
						color = 1727987712;
					};
					class ZedJump388_050
					{
						jumpLength = 1.5;
						jumpHeight = 0.5;
						minCenterHeight = -0.5;
						jumpDropdownMin = 0.5;
						jumpDropdownMax = -0.5;
						areaType = "jump0";
						flags[] = {"jumpOver"};
						color = 1725781248;
					};
					class ZedJump387_110
					{
						jumpLength = 3.9;
						jumpHeight = 1.1;
						minCenterHeight = 0.5;
						jumpDropdownMin = 0.5;
						jumpDropdownMax = -0.5;
						areaType = "jump0";
						flags[] = {"jumpOver"};
						color = 1711308800;
					};
					class ZedJump420_160
					{
						jumpLength = 4.0;
						jumpHeight = 1.6;
						minCenterHeight = 1.1;
						jumpDropdownMin = 0.5;
						jumpDropdownMax = -0.5;
						areaType = "jump0";
						flags[] = {"jumpOver"};
						color = 1711276287;
					};
					class ZedJump265_210
					{
						jumpLength = 2.45;
						jumpHeight = 2.5;
						minCenterHeight = 1.8;
						jumpDropdownMin = 0.5;
						jumpDropdownMax = -0.5;
						areaType = "jump0";
						flags[] = {"climb"};
						color = 1720975571;
					};
					class Fence50_110deer
					{
						typeId = 100;
						jumpLength = 8.0;
						jumpHeight = 1.1;
						minCenterHeight = 0.5;
						jumpDropdownMin = 1.0;
						jumpDropdownMax = -1.0;
						areaType = "jump2";
						flags[] = {"jumpOver"};
						color = 1722460927;
					};
					class Fence110_160deer
					{
						typeId = 101;
						jumpLength = 8.0;
						jumpHeight = 1.6;
						minCenterHeight = 1.1;
						jumpDropdownMin = 1.0;
						jumpDropdownMax = -1.0;
						areaType = "jump3";
						flags[] = {"jumpOver"};
						color = 1713700856;
					};
					class Fence50_110hen
					{
						typeId = 110;
						jumpLength = 4.0;
						jumpHeight = 1.1;
						minCenterHeight = 0.5;
						jumpDropdownMin = 0.5;
						jumpDropdownMax = -0.5;
						areaType = "jump4";
						flags[] = {"jumpOver"};
						color = -22016;
					};
					class Fence110_160hen
					{
						typeId = 111;
						jumpLength = 4.0;
						jumpHeight = 1.6;
						minCenterHeight = 1.1;
						jumpDropdownMin = 0.5;
						jumpDropdownMax = -0.5;
						areaType = "jump4";
						flags[] = {"jumpOver"};
						color = -22016;
					};
				};
			};
		};
		startTime = "14:20";
		startDate = "18/01/2019";
		centerPosition[] = {1024.0,1024.0,300.0};
		clutterGrid = 1.0;
		clutterDist = 125.0;
		noDetailDist = 40.0;
		fullDetailDist = 15.0;
		volFogOffset = 50;
		class UsedTerrainMaterials
		{
			material1 = "DZ\surfaces\data\terrain\cp_concrete1.rvmat";
			material2 = "DZ\surfaces_bliss\data\terrain\en_forest_dec.rvmat";
			material3 = "DZ\surfaces_bliss\data\terrain\en_grass1.rvmat";
			material4 = "DZ\surfaces_bliss\data\terrain\en_grass2.rvmat";
			material5 = "DZ\surfaces\data\terrain\cp_gravel.rvmat";
			material6 = "DZ\surfaces\data\terrain\cp_rock.rvmat";
		};
		midDetailTexture = "DayZExpansion\Worlds\ExpansionTest\data\expansionTest_middlesat_mco.paa";
		class OutsideTerrain
		{
			satellite = "DZ\worlds\chernarusplus\data\outside_sat_co.paa";
			enableTerrainSynth = 0;
			class Layers
			{
				class Layer0
				{
					nopx = "DZ\surfaces\data\terrain\cp_grass_nopx.paa";
					texture = "DZ\surfaces\data\terrain\cp_grass_ca.paa";
				};
			};
		};
		class Names{};
		ilsPosition[] = {3330.0,3610.0};
		ilsDirection[] = {-1.0,0.08,0.0};
		ilsTaxiOff[] = {3520.0,3610.0,4040.0,3610.0,4050.0,3600.0,4050.0,3580.0,4045.0,3575.0,4040.0,3572.0,3540.0,3572.0};
		ilsTaxiIn[] = {3540.0,3572.0,3388.0,3572.0,3382.0,3578.0,3382.0,3600.0,3400.0,3610.0};
		drawTaxiway = 0;
		class SecondaryAirports{};
		class Grid
		{
			offsetX = 0.0;
			offsetY = 0.0;
			class Zoom1
			{
				zoomMax = 0.15;
				format = "XY";
				formatX = "000";
				formatY = "000";
				stepX = 100.0;
				stepY = 100.0;
			};
			class Zoom2
			{
				zoomMax = 0.85;
				format = "XY";
				formatX = "00";
				formatY = "00";
				stepX = 1000.0;
				stepY = 1000.0;
			};
			class Zoom3
			{
				zoomMax = 1e+30;
				format = "XY";
				formatX = "0";
				formatY = "0";
				stepX = 10000.0;
				stepY = 10000.0;
			};
		};
		mapDisplayNameKey = "#STR_CfgVehicles_ChernarusMap0";
		mapDescriptionKey = "#STR_CfgVehicles_ChernarusMap1";
		mapTextureClosed = "dz\gear\navigation\data\map_chernarus_co.paa";
		mapTextureOpened = "dz\structures\signs\tourist\data\karta_co.paa";
		mapTextureLegend = "dz\structures\signs\tourist\data\karta_side_co.paa";
		userMapPath = "dz\gear\navigation\data\usermap";
		minTreesInForestSquare = 10;
		minRocksInRockSquare = 5;
		soundMapAttenCoef = 0.01;
		class SoundMapValues
		{
			treehard = 0.03;
			treesoft = 0.03;
			bushhard = 0.0;
			bushsoft = 0.0;
			forest = 1.0;
			house = 0.3;
			church = 0.5;
		};
		class Sounds
		{
			sounds[] = {};
		};
		class Ambient
		{
			class BigInsects
			{
				radius = 20;
				cost = "(5 - (2 * houses)) * (1 - night) * (1 - rain) * (1 - sea) * (1 - windy)";
				class Species{};
			};
			class BigInsectsAquatic
			{
				radius = 20;
				cost = "(3 * sea) * (1 - night) * (1 - rain) * (1 - windy)";
				class Species{};
			};
			class SmallInsects
			{
				radius = 3;
				cost = "(12 - 8 * hills) * (1 - night) * (1 - rain) * (1 - sea) * (1 - windy)";
				class Species{};
			};
			class NightInsects
			{
				radius = 3;
				cost = "(9 - 8 * hills) * night * (1 - rain) * (1 - sea) * (1 - windy)";
				class Species{};
			};
			class WindClutter
			{
				radius = 10;
				cost = "((20 - 5 * rain) * (3 * (windy factor [0.2, 0.5]))) * (1 - sea)";
				class Species
				{
					class FxWindGrass1
					{
						probability = "0.4 - 0.2 * hills - 0.2 * trees";
						cost = 1;
					};
					class FxWindGrass2
					{
						probability = "0.4 - 0.2 * hills - 0.2 * trees";
						cost = 1;
					};
					class FxWindRock1
					{
						probability = "0.4 * hills";
						cost = 1;
					};
					class FxCrWindLeaf1
					{
						probability = "0.2 * trees";
						cost = 1;
					};
					class FxCrWindLeaf2
					{
						probability = "0.1 * trees + 0.2";
						cost = 1;
					};
					class FxCrWindLeaf3
					{
						probability = "0.1 * trees";
						cost = 1;
					};
				};
			};
			class NoWindClutter
			{
				radius = 15;
				cost = 8;
				class Species
				{
					class FxWindPollen1
					{
						probability = 1;
						cost = 1;
					};
					class FxCrWindLeaf1
					{
						probability = "0.2 * trees";
						cost = 1;
					};
					class FxCrWindLeaf2
					{
						probability = "0.1 * trees + 0.2";
						cost = 1;
					};
					class FxCrWindLeaf3
					{
						probability = "0.1 * trees";
						cost = 1;
					};
				};
			};
		};
	};
};
class cfgWorldList
{
	class ExpansionTest{};
};
