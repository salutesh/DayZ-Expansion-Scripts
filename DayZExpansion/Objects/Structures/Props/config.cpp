#define _ARMA_

class CfgPatches
{
	class DayZExpansion_Objects_Structures_Props
	{
		units[] = {"ExpansionPropAirtank","ExpansionPropBag1","ExpansionPropBag2","ExpansionPropBattery","ExpansionPropBattery1","ExpansionPropBattery2","ExpansionPropBattery3","ExpansionPropBattery4","ExpansionPropBattery5","ExpansionPropBattery6","ExpansionPropBleach","ExpansionPropBoltCutters1","ExpansionPropBoltCutters2","ExpansionPropBucket1","ExpansionPropBucket2","ExpansionPropBucket3","ExpansionPropCarFilter","ExpansionPropCarJack","ExpansionPropCase1","ExpansionPropCase2","ExpansionPropChainsaw1","ExpansionPropChainsaw2","ExpansionPropChainsaw3","ExpansionPropChair1","ExpansionPropChair2","ExpansionPropCircularSaw1","ExpansionPropCircularSaw2","ExpansionPropCircularSaw3","ExpansionPropCircularSaw4","ExpansionPropClamp1","ExpansionPropClamp2","ExpansionPropClamp3","ExpansionPropClamp4","ExpansionPropClamp5","ExpansionPropClamp6","ExpansionPropClamp7","ExpansionPropClamp8","ExpansionPropClampLight1","ExpansionPropClampLight2","ExpansionPropClampLight3","ExpansionPropCooler1","ExpansionPropCooler2","ExpansionPropDetergent1","ExpansionPropDrill","ExpansionPropFan","ExpansionPropFishingPole1","ExpansionPropFishingPole2","ExpansionPropGasCan","ExpansionPropGloves","ExpansionPropGlue","ExpansionPropGrill1","ExpansionPropGrill2","ExpansionPropGrinder","ExpansionPropHammer","ExpansionPropHeadphones","ExpansionPropJigsaw","ExpansionPropLawnChair","ExpansionPropLawnMower1","ExpansionPropLawnMower2","ExpansionPropLeafBlower","ExpansionPropLevelrs","ExpansionPropMiniFridge","ExpansionPropNailgun","ExpansionPropOar","ExpansionPropPaintBrush","ExpansionPropPetCarrier","ExpansionPropPliers","ExpansionPropPliersSet1","ExpansionPropPliersSet2","ExpansionPropPot","ExpansionPropPreserver","ExpansionPropPropaneTank","ExpansionPropPropaneTorch","ExpansionPropRadio","ExpansionPropRakeset","ExpansionPropBin","ExpansionPropRespirator","ExpansionPropSawhorse","ExpansionPropSawset","ExpansionPropScrewdriver1","ExpansionPropScrewdriver2","ExpansionPropShed","ExpansionPropShelf1","ExpansionPropShelf2","ExpansionPropShelf3","ExpansionPropShelf4","ExpansionPropShelf5","ExpansionPropShelf6","ExpansionPropSprayCan","ExpansionPropSprayer","ExpansionPropStool1","ExpansionPropStool2","ExpansionPropStrapleGun","ExpansionPropTableSaw","ExpansionPropTackleBox1","ExpansionPropTackleBox2","ExpansionPropTarp","ExpansionPropTire","ExpansionPropTool1","ExpansionPropTool2","ExpansionPropToolBox1","ExpansionPropToolBox2","ExpansionPropToolChest","ExpansionPropToolShelf","ExpansionPropTrashBarrel1","ExpansionPropTrashBarrel2","ExpansionPropTrimmer","ExpansionPropUmbrella","ExpansionPropWacker","ExpansionPropWateringCan","ExpansionPropWelder","ExpansionPropWoodLadder","ExpansionPropWorkbench","ExpansionPropWorkbench2","ExpansionPropWorkbench3","ExpansionPropFlagBase","ExpansionPropFlagAustralia","ExpansionPropFlagCanada","ExpansionPropFlagFrance","ExpansionPropFlagGermany","ExpansionPropFlagLatvia","ExpansionPropFlagLuxembourg","ExpansionPropFlagMexico","ExpansionPropFlagNetherlands","ExpansionPropFlagNewZealand","ExpansionPropFlagNorway","ExpansionPropFlagPoland","ExpansionPropFlagRussia","ExpansionPropFlagUK","ExpansionPropFlagUSA","ExpansionPropFlagExpansion"};
		weapons[] = {};
		requiredVersion = 0.1;
		requiredAddons[] = {"DZ_Data"};
	};
};
class CfgVehicles
{
	class HouseNoDestruct;
	class ExpansionPropAirtank: HouseNoDestruct
	{
		scope = 2;
		model = "\DayZExpansion\Objects\Structures\Props\airtank1.p3d";
	};
	class ExpansionPropBag1: HouseNoDestruct
	{
		scope = 2;
		model = "\DayZExpansion\Objects\Structures\Props\bag1.p3d";
	};
	class ExpansionPropBag2: HouseNoDestruct
	{
		scope = 2;
		model = "\DayZExpansion\Objects\Structures\Props\bag2.p3d";
	};
	class ExpansionPropBattery: HouseNoDestruct
	{
		scope = 2;
		model = "\DayZExpansion\Objects\Structures\Props\battery.p3d";
	};
	class ExpansionPropBattery1: HouseNoDestruct
	{
		scope = 2;
		model = "\DayZExpansion\Objects\Structures\Props\battery1.p3d";
	};
	class ExpansionPropBattery2: HouseNoDestruct
	{
		scope = 2;
		model = "\DayZExpansion\Objects\Structures\Props\battery2.p3d";
	};
	class ExpansionPropBattery3: HouseNoDestruct
	{
		scope = 2;
		model = "\DayZExpansion\Objects\Structures\Props\battery3.p3d";
		physLayer = "item_small";
		carveNavmesh = 0;
	};
	class ExpansionPropBattery4: HouseNoDestruct
	{
		scope = 2;
		model = "\DayZExpansion\Objects\Structures\Props\battery4.p3d";
		physLayer = "item_small";
		carveNavmesh = 0;
	};
	class ExpansionPropBattery5: HouseNoDestruct
	{
		scope = 2;
		model = "\DayZExpansion\Objects\Structures\Props\battery5.p3d";
		physLayer = "item_small";
		carveNavmesh = 0;
	};
	class ExpansionPropBattery6: HouseNoDestruct
	{
		scope = 2;
		model = "\DayZExpansion\Objects\Structures\Props\battery6.p3d";
		physLayer = "item_small";
		carveNavmesh = 0;
	};
	class ExpansionPropBleach: HouseNoDestruct
	{
		scope = 2;
		model = "\DayZExpansion\Objects\Structures\Props\bleach.p3d";
	};
	class ExpansionPropBoltCutters1: HouseNoDestruct
	{
		scope = 2;
		model = "\DayZExpansion\Objects\Structures\Props\boltcutters1.p3d";
	};
	class ExpansionPropBoltCutters2: HouseNoDestruct
	{
		scope = 2;
		model = "\DayZExpansion\Objects\Structures\Props\boltcutters2.p3d";
	};
	class ExpansionPropBucket1: HouseNoDestruct
	{
		scope = 2;
		model = "\DayZExpansion\Objects\Structures\Props\bucket1.p3d";
	};
	class ExpansionPropBucket2: HouseNoDestruct
	{
		scope = 2;
		model = "\DayZExpansion\Objects\Structures\Props\bucket2.p3d";
	};
	class ExpansionPropBucket3: HouseNoDestruct
	{
		scope = 2;
		model = "\DayZExpansion\Objects\Structures\Props\bucket3.p3d";
	};
	class ExpansionPropCarFilter: HouseNoDestruct
	{
		scope = 2;
		model = "\DayZExpansion\Objects\Structures\Props\carfilter.p3d";
	};
	class ExpansionPropCarJack: HouseNoDestruct
	{
		scope = 2;
		model = "\DayZExpansion\Objects\Structures\Props\carjack.p3d";
	};
	class ExpansionPropCase1: HouseNoDestruct
	{
		scope = 2;
		model = "\DayZExpansion\Objects\Structures\Props\case1.p3d";
		physLayer = "item_small";
		carveNavmesh = 0;
	};
	class ExpansionPropCase2: HouseNoDestruct
	{
		scope = 2;
		model = "\DayZExpansion\Objects\Structures\Props\case2.p3d";
	};
	class ExpansionPropChainsaw1: HouseNoDestruct
	{
		scope = 2;
		model = "\DayZExpansion\Objects\Structures\Props\chainsaw.p3d";
	};
	class ExpansionPropChainsaw2: HouseNoDestruct
	{
		scope = 2;
		model = "\DayZExpansion\Objects\Structures\Props\chainsaw1.p3d";
	};
	class ExpansionPropChainsaw3: HouseNoDestruct
	{
		scope = 2;
		model = "\DayZExpansion\Objects\Structures\Props\chainsaw2.p3d";
	};
	class ExpansionPropChair1: HouseNoDestruct
	{
		scope = 2;
		model = "\DayZExpansion\Objects\Structures\Props\chair1.p3d";
	};
	class ExpansionPropChair2: HouseNoDestruct
	{
		scope = 2;
		model = "\DayZExpansion\Objects\Structures\Props\chair2.p3d";
	};
	class ExpansionPropCircularSaw1: HouseNoDestruct
	{
		scope = 2;
		model = "\DayZExpansion\Objects\Structures\Props\circularsaw1.p3d";
	};
	class ExpansionPropCircularSaw2: HouseNoDestruct
	{
		scope = 2;
		model = "\DayZExpansion\Objects\Structures\Props\circularsaw2.p3d";
	};
	class ExpansionPropCircularSaw3: HouseNoDestruct
	{
		scope = 2;
		model = "\DayZExpansion\Objects\Structures\Props\circularsaw3.p3d";
		physLayer = "item_small";
		carveNavmesh = 0;
	};
	class ExpansionPropCircularSaw4: HouseNoDestruct
	{
		scope = 2;
		model = "\DayZExpansion\Objects\Structures\Props\circularsaw4.p3d";
		physLayer = "item_small";
		carveNavmesh = 0;
	};
	class ExpansionPropClamp1: HouseNoDestruct
	{
		scope = 2;
		model = "\DayZExpansion\Objects\Structures\Props\clamp1.p3d";
		physLayer = "item_small";
		carveNavmesh = 0;
	};
	class ExpansionPropClamp2: HouseNoDestruct
	{
		scope = 2;
		model = "\DayZExpansion\Objects\Structures\Props\clamp2.p3d";
		physLayer = "item_small";
		carveNavmesh = 0;
	};
	class ExpansionPropClamp3: HouseNoDestruct
	{
		scope = 2;
		model = "\DayZExpansion\Objects\Structures\Props\clamp3.p3d";
		physLayer = "item_small";
		carveNavmesh = 0;
	};
	class ExpansionPropClamp4: HouseNoDestruct
	{
		scope = 2;
		model = "\DayZExpansion\Objects\Structures\Props\clamp4.p3d";
		physLayer = "item_small";
		carveNavmesh = 0;
	};
	class ExpansionPropClamp5: HouseNoDestruct
	{
		scope = 2;
		model = "\DayZExpansion\Objects\Structures\Props\clamp5.p3d";
		physLayer = "item_small";
		carveNavmesh = 0;
	};
	class ExpansionPropClamp6: HouseNoDestruct
	{
		scope = 2;
		model = "\DayZExpansion\Objects\Structures\Props\clamp6.p3d";
		physLayer = "item_small";
		carveNavmesh = 0;
	};
	class ExpansionPropClamp7: HouseNoDestruct
	{
		scope = 2;
		model = "\DayZExpansion\Objects\Structures\Props\clamp7.p3d";
		physLayer = "item_small";
		carveNavmesh = 0;
	};
	class ExpansionPropClamp8: HouseNoDestruct
	{
		scope = 2;
		model = "\DayZExpansion\Objects\Structures\Props\clamp8.p3d";
		physLayer = "item_small";
		carveNavmesh = 0;
	};
	class ExpansionPropClampLight1: HouseNoDestruct
	{
		scope = 2;
		model = "\DayZExpansion\Objects\Structures\Props\clamplight1.p3d";
	};
	class ExpansionPropClampLight2: HouseNoDestruct
	{
		scope = 2;
		model = "\DayZExpansion\Objects\Structures\Props\clamplight2.p3d";
	};
	class ExpansionPropClampLight3: HouseNoDestruct
	{
		scope = 2;
		model = "\DayZExpansion\Objects\Structures\Props\clamplight3.p3d";
	};
	class ExpansionPropCooler1: HouseNoDestruct
	{
		scope = 2;
		model = "\DayZExpansion\Objects\Structures\Props\cooler1.p3d";
	};
	class ExpansionPropCooler2: HouseNoDestruct
	{
		scope = 2;
		model = "\DayZExpansion\Objects\Structures\Props\cooler2.p3d";
	};
	class ExpansionPropDetergent1: HouseNoDestruct
	{
		scope = 2;
		model = "\DayZExpansion\Objects\Structures\Props\detergent1.p3d";
	};
	class ExpansionPropDrill: HouseNoDestruct
	{
		scope = 2;
		model = "\DayZExpansion\Objects\Structures\Props\drill.p3d";
		physLayer = "item_small";
		carveNavmesh = 0;
	};
	class ExpansionPropFan: HouseNoDestruct
	{
		scope = 2;
		model = "\DayZExpansion\Objects\Structures\Props\fan.p3d";
	};
	class ExpansionPropFishingPole1: HouseNoDestruct
	{
		scope = 2;
		model = "\DayZExpansion\Objects\Structures\Props\fishingpole.p3d";
	};
	class ExpansionPropFishingPole2: HouseNoDestruct
	{
		scope = 2;
		model = "\DayZExpansion\Objects\Structures\Props\fishingpole2.p3d";
	};
	class ExpansionPropGasCan: HouseNoDestruct
	{
		scope = 2;
		model = "\DayZExpansion\Objects\Structures\Props\gascan1.p3d";
	};
	class ExpansionPropGloves: HouseNoDestruct
	{
		scope = 2;
		model = "\DayZExpansion\Objects\Structures\Props\gloves.p3d";
		physLayer = "item_small";
		carveNavmesh = 0;
	};
	class ExpansionPropGlue: HouseNoDestruct
	{
		scope = 2;
		model = "\DayZExpansion\Objects\Structures\Props\glue.p3d";
	};
	class ExpansionPropGrill1: HouseNoDestruct
	{
		scope = 2;
		model = "\DayZExpansion\Objects\Structures\Props\grill1.p3d";
	};
	class ExpansionPropGrill2: HouseNoDestruct
	{
		scope = 2;
		model = "\DayZExpansion\Objects\Structures\Props\grill2.p3d";
	};
	class ExpansionPropGrinder: HouseNoDestruct
	{
		scope = 2;
		model = "\DayZExpansion\Objects\Structures\Props\grinder.p3d";
		physLayer = "item_small";
		carveNavmesh = 0;
	};
	class ExpansionPropHammer: HouseNoDestruct
	{
		scope = 2;
		model = "\DayZExpansion\Objects\Structures\Props\hammer.p3d";
		physLayer = "item_small";
		carveNavmesh = 0;
	};
	class ExpansionPropHeadphones: HouseNoDestruct
	{
		scope = 2;
		model = "\DayZExpansion\Objects\Structures\Props\headphones.p3d";
		physLayer = "item_small";
		carveNavmesh = 0;
	};
	class ExpansionPropJigsaw: HouseNoDestruct
	{
		scope = 2;
		model = "\DayZExpansion\Objects\Structures\Props\jigsaw.p3d";
	};
	class ExpansionPropLawnChair: HouseNoDestruct
	{
		scope = 2;
		model = "\DayZExpansion\Objects\Structures\Props\lawnchair.p3d";
	};
	class ExpansionPropLawnMower1: HouseNoDestruct
	{
		scope = 2;
		model = "\DayZExpansion\Objects\Structures\Props\lawnmower1.p3d";
	};
	class ExpansionPropLawnMower2: HouseNoDestruct
	{
		scope = 2;
		model = "\DayZExpansion\Objects\Structures\Props\lawnmower2.p3d";
	};
	class ExpansionPropLeafBlower: HouseNoDestruct
	{
		scope = 2;
		model = "\DayZExpansion\Objects\Structures\Props\leafblower.p3d";
	};
	class ExpansionPropLevelrs: HouseNoDestruct
	{
		scope = 2;
		model = "\DayZExpansion\Objects\Structures\Props\levelrs.p3d";
		physLayer = "item_small";
		carveNavmesh = 0;
	};
	class ExpansionPropMiniFridge: HouseNoDestruct
	{
		scope = 2;
		model = "\DayZExpansion\Objects\Structures\Props\minifridge.p3d";
	};
	class ExpansionPropNailgun: HouseNoDestruct
	{
		scope = 2;
		model = "\DayZExpansion\Objects\Structures\Props\nailgun.p3d";
		physLayer = "item_small";
		carveNavmesh = 0;
	};
	class ExpansionPropOar: HouseNoDestruct
	{
		scope = 2;
		model = "\DayZExpansion\Objects\Structures\Props\oar.p3d";
	};
	class ExpansionPropPaintBrush: HouseNoDestruct
	{
		scope = 2;
		model = "\DayZExpansion\Objects\Structures\Props\paintbrush.p3d";
		physLayer = "item_small";
		carveNavmesh = 0;
	};
	class ExpansionPropPetCarrier: HouseNoDestruct
	{
		scope = 2;
		model = "\DayZExpansion\Objects\Structures\Props\petcarrier.p3d";
	};
	class ExpansionPropPliers: HouseNoDestruct
	{
		scope = 2;
		model = "\DayZExpansion\Objects\Structures\Props\pliers.p3d";
	};
	class ExpansionPropPliersSet1: HouseNoDestruct
	{
		scope = 2;
		model = "\DayZExpansion\Objects\Structures\Props\pliersset1.p3d";
		physLayer = "item_small";
		carveNavmesh = 0;
	};
	class ExpansionPropPliersSet2: HouseNoDestruct
	{
		scope = 2;
		model = "\DayZExpansion\Objects\Structures\Props\pliersset1.p3d";
		physLayer = "item_small";
		carveNavmesh = 0;
	};
	class ExpansionPropPot: HouseNoDestruct
	{
		scope = 2;
		model = "\DayZExpansion\Objects\Structures\Props\pot1.p3d";
	};
	class ExpansionPropPreserver: HouseNoDestruct
	{
		scope = 2;
		model = "\DayZExpansion\Objects\Structures\Props\preserver.p3d";
		physLayer = "item_small";
		carveNavmesh = 0;
	};
	class ExpansionPropPropaneTank: HouseNoDestruct
	{
		scope = 2;
		model = "\DayZExpansion\Objects\Structures\Props\propanetank1.p3d";
	};
	class ExpansionPropPropaneTorch: HouseNoDestruct
	{
		scope = 2;
		model = "\DayZExpansion\Objects\Structures\Props\propanetorch1.p3d";
	};
	class ExpansionPropRadio: HouseNoDestruct
	{
		scope = 2;
		model = "\DayZExpansion\Objects\Structures\Props\radio.p3d";
	};
	class ExpansionPropRakeset: HouseNoDestruct
	{
		scope = 2;
		model = "\DayZExpansion\Objects\Structures\Props\rakeset.p3d";
	};
	class ExpansionPropBin: HouseNoDestruct
	{
		scope = 2;
		model = "\DayZExpansion\Objects\Structures\Props\recyclebin.p3d";
	};
	class ExpansionPropRespirator: HouseNoDestruct
	{
		scope = 2;
		model = "\DayZExpansion\Objects\Structures\Props\respirator.p3d";
		physLayer = "item_small";
		carveNavmesh = 0;
	};
	class ExpansionPropSawhorse: HouseNoDestruct
	{
		scope = 2;
		model = "\DayZExpansion\Objects\Structures\Props\sawhorse.p3d";
	};
	class ExpansionPropSawset: HouseNoDestruct
	{
		scope = 2;
		model = "\DayZExpansion\Objects\Structures\Props\sawset.p3d";
		physLayer = "item_small";
		carveNavmesh = 0;
	};
	class ExpansionPropScrewdriver1: HouseNoDestruct
	{
		scope = 2;
		model = "\DayZExpansion\Objects\Structures\Props\screwdriver1.p3d";
		physLayer = "item_small";
		carveNavmesh = 0;
	};
	class ExpansionPropScrewdriver2: HouseNoDestruct
	{
		scope = 2;
		model = "\DayZExpansion\Objects\Structures\Props\screwdriver2.p3d";
		physLayer = "item_small";
		carveNavmesh = 0;
	};
	class ExpansionPropShed: HouseNoDestruct
	{
		scope = 2;
		model = "\DayZExpansion\Objects\Structures\Props\shed1.p3d";
	};
	class ExpansionPropShelf1: HouseNoDestruct
	{
		scope = 2;
		model = "\DayZExpansion\Objects\Structures\Props\shelf1.p3d";
	};
	class ExpansionPropShelf2: HouseNoDestruct
	{
		scope = 2;
		model = "\DayZExpansion\Objects\Structures\Props\shelf2.p3d";
	};
	class ExpansionPropShelf3: HouseNoDestruct
	{
		scope = 2;
		model = "\DayZExpansion\Objects\Structures\Props\shelf3.p3d";
	};
	class ExpansionPropShelf4: HouseNoDestruct
	{
		scope = 2;
		model = "\DayZExpansion\Objects\Structures\Props\shelf4.p3d";
	};
	class ExpansionPropShelf5: HouseNoDestruct
	{
		scope = 2;
		model = "\DayZExpansion\Objects\Structures\Props\shelf5.p3d";
	};
	class ExpansionPropShelf6: HouseNoDestruct
	{
		scope = 2;
		model = "\DayZExpansion\Objects\Structures\Props\shelf6.p3d";
	};
	class ExpansionPropSprayCan: HouseNoDestruct
	{
		scope = 2;
		model = "\DayZExpansion\Objects\Structures\Props\spraycan.p3d";
		physLayer = "item_small";
		carveNavmesh = 0;
	};
	class ExpansionPropSprayer: HouseNoDestruct
	{
		scope = 2;
		model = "\DayZExpansion\Objects\Structures\Props\sprayer.p3d";
	};
	class ExpansionPropStool1: HouseNoDestruct
	{
		scope = 2;
		model = "\DayZExpansion\Objects\Structures\Props\stool.p3d";
	};
	class ExpansionPropStool2: HouseNoDestruct
	{
		scope = 2;
		model = "\DayZExpansion\Objects\Structures\Props\stool2.p3d";
	};
	class ExpansionPropStrapleGun: HouseNoDestruct
	{
		scope = 2;
		model = "\DayZExpansion\Objects\Structures\Props\straplegun.p3d";
		physLayer = "item_small";
		carveNavmesh = 0;
	};
	class ExpansionPropTableSaw: HouseNoDestruct
	{
		scope = 2;
		model = "\DayZExpansion\Objects\Structures\Props\tablesaw.p3d";
	};
	class ExpansionPropTackleBox1: HouseNoDestruct
	{
		scope = 2;
		model = "\DayZExpansion\Objects\Structures\Props\tacklebox.p3d";
	};
	class ExpansionPropTackleBox2: HouseNoDestruct
	{
		scope = 2;
		model = "\DayZExpansion\Objects\Structures\Props\tacklebox2.p3d";
	};
	class ExpansionPropTarp: HouseNoDestruct
	{
		scope = 2;
		model = "\DayZExpansion\Objects\Structures\Props\tarp.p3d";
		physLayer = "item_small";
		carveNavmesh = 0;
	};
	class ExpansionPropTire: HouseNoDestruct
	{
		scope = 2;
		model = "\DayZExpansion\Objects\Structures\Props\tire.p3d";
	};
	class ExpansionPropTool1: HouseNoDestruct
	{
		scope = 2;
		model = "\DayZExpansion\Objects\Structures\Props\tool1.p3d";
	};
	class ExpansionPropTool2: HouseNoDestruct
	{
		scope = 2;
		model = "\DayZExpansion\Objects\Structures\Props\tool2.p3d";
	};
	class ExpansionPropToolBox1: HouseNoDestruct
	{
		scope = 2;
		model = "\DayZExpansion\Objects\Structures\Props\toolbox1.p3d";
	};
	class ExpansionPropToolBox2: HouseNoDestruct
	{
		scope = 2;
		model = "\DayZExpansion\Objects\Structures\Props\toolbox2.p3d";
	};
	class ExpansionPropToolChest: HouseNoDestruct
	{
		scope = 2;
		model = "\DayZExpansion\Objects\Structures\Props\toolchest.p3d";
	};
	class ExpansionPropToolShelf: HouseNoDestruct
	{
		scope = 2;
		model = "\DayZExpansion\Objects\Structures\Props\toolshelf.p3d";
	};
	class ExpansionPropTrashBarrel1: HouseNoDestruct
	{
		scope = 2;
		model = "\DayZExpansion\Objects\Structures\Props\trashbarrel.p3d";
	};
	class ExpansionPropTrashBarrel2: HouseNoDestruct
	{
		scope = 2;
		model = "\DayZExpansion\Objects\Structures\Props\trashbarrel2.p3d";
	};
	class ExpansionPropTrimmer: HouseNoDestruct
	{
		scope = 2;
		model = "\DayZExpansion\Objects\Structures\Props\trimmer.p3d";
	};
	class ExpansionPropUmbrella: HouseNoDestruct
	{
		scope = 2;
		model = "\DayZExpansion\Objects\Structures\Props\umbrella.p3d";
	};
	class ExpansionPropWacker: HouseNoDestruct
	{
		scope = 2;
		model = "\DayZExpansion\Objects\Structures\Props\wacker.p3d";
	};
	class ExpansionPropWateringCan: HouseNoDestruct
	{
		scope = 2;
		model = "\DayZExpansion\Objects\Structures\Props\wateringcan.p3d";
	};
	class ExpansionPropWelder: HouseNoDestruct
	{
		scope = 2;
		model = "\DayZExpansion\Objects\Structures\Props\welder.p3d";
	};
	class ExpansionPropWoodLadder: HouseNoDestruct
	{
		scope = 2;
		model = "\DayZExpansion\Objects\Structures\Props\woodladder.p3d";
	};
	class ExpansionPropWorkbench: HouseNoDestruct
	{
		scope = 2;
		model = "\DayZExpansion\Objects\Structures\Props\workbench.p3d";
	};
	class ExpansionPropWorkbench2: HouseNoDestruct
	{
		scope = 2;
		model = "\DayZExpansion\Objects\Structures\Props\workbench2.p3d";
	};
	class ExpansionPropWorkbench3: HouseNoDestruct
	{
		scope = 2;
		model = "\DayZExpansion\Objects\Structures\Props\workbench3.p3d";
	};
	class ExpansionPropFlagBase: HouseNoDestruct
	{
		scope = 2;
		vehicleClass = "Expansion_Flags";
		displayName = "$STR_EXPANSION_FLAG_POLE_BASE";
		descriptionShort = "$STR_EXPANSION_FLAG_POLE_DESC";
		model = "DayZExpansion\Objects\Structures\Flags\flagpole.p3d";
		useEntityHierarchy = "true";
		forceFarBubble = "true";
		carveNavmesh = 1;
		bounding = "BSphere";
		physLayer = "item_large";
		allowOwnedCargoManipulation = 1;
		hiddenSelections[] = {"latka"};
		hiddenSelectionsTextures[] = {"\dayzexpansion\objects\structures\flags\data\countries\flag_chernarus_co.paa"};
	};
	class ExpansionPropFlagAustralia: ExpansionPropFlagBase
	{
		scope = 2;
		displayName = "$STR_EXPANSION_FLAG_POLE_AUS";
		hiddenSelectionsTextures[] = {"\dayzexpansion\objects\structures\flags\data\countries\flag_australia_co.paa"};
	};
	class ExpansionPropFlagCanada: ExpansionPropFlagBase
	{
		scope = 2;
		displayName = "$STR_EXPANSION_FLAG_POLE_CANADA";
		hiddenSelectionsTextures[] = {"\dayzexpansion\objects\structures\flags\data\countries\flag_canada_co.paa"};
	};
	class ExpansionPropFlagFrance: ExpansionPropFlagBase
	{
		scope = 2;
		displayName = "$STR_EXPANSION_FLAG_POLE_FRANCE";
		hiddenSelectionsTextures[] = {"\dayzexpansion\objects\structures\flags\data\countries\flag_france_co.paa"};
	};
	class ExpansionPropFlagGermany: ExpansionPropFlagBase
	{
		scope = 2;
		displayName = "$STR_EXPANSION_FLAG_POLE_GERMANY";
		hiddenSelectionsTextures[] = {"\dayzexpansion\objects\structures\flags\data\countries\flag_germany_co.paa"};
	};
	class ExpansionPropFlagLatvia: ExpansionPropFlagBase
	{
		scope = 2;
		displayName = "$STR_EXPANSION_FLAG_POLE_LATVIA";
		hiddenSelectionsTextures[] = {"\dayzexpansion\objects\structures\flags\data\countries\flag_latvia_co.paa"};
	};
	class ExpansionPropFlagLuxembourg: ExpansionPropFlagBase
	{
		scope = 2;
		displayName = "$STR_EXPANSION_FLAG_POLE_LUXEMBOURG";
		hiddenSelectionsTextures[] = {"\dayzexpansion\objects\structures\flags\data\countries\flag_luxembourg_co.paa"};
	};
	class ExpansionPropFlagMexico: ExpansionPropFlagBase
	{
		scope = 2;
		displayName = "$STR_EXPANSION_FLAG_POLE_MEXICO";
		hiddenSelectionsTextures[] = {"\dayzexpansion\objects\structures\flags\data\countries\flag_mexico_co.paa"};
	};
	class ExpansionPropFlagNetherlands: ExpansionPropFlagBase
	{
		scope = 2;
		displayName = "$STR_EXPANSION_FLAG_POLE_NETHERLANDS";
		hiddenSelectionsTextures[] = {"\dayzexpansion\objects\structures\flags\data\countries\flag_netherlands_co.paa"};
	};
	class ExpansionPropFlagNewZealand: ExpansionPropFlagBase
	{
		scope = 2;
		displayName = "$STR_EXPANSION_FLAG_POLE_NZ";
		hiddenSelectionsTextures[] = {"\dayzexpansion\objects\structures\flags\data\countries\flag_new_zealand_co.paa"};
	};
	class ExpansionPropFlagNorway: ExpansionPropFlagBase
	{
		scope = 2;
		displayName = "$STR_EXPANSION_FLAG_POLE_NORWAY";
		hiddenSelectionsTextures[] = {"\dayzexpansion\objects\structures\flags\data\countries\flag_norway_co.paa"};
	};
	class ExpansionPropFlagPoland: ExpansionPropFlagBase
	{
		scope = 2;
		displayName = "$STR_EXPANSION_FLAG_POLE_POLAND";
		hiddenSelectionsTextures[] = {"\dayzexpansion\objects\structures\flags\data\countries\flag_poland_co.paa"};
	};
	class ExpansionPropFlagRussia: ExpansionPropFlagBase
	{
		scope = 2;
		displayName = "$STR_EXPANSION_FLAG_POLE_RUSSIA";
		hiddenSelectionsTextures[] = {"\dayzexpansion\objects\structures\flags\data\countries\flag_russia_co.paa"};
	};
	class ExpansionPropFlagUK: ExpansionPropFlagBase
	{
		scope = 2;
		displayName = "$STR_EXPANSION_FLAG_POLE_UK";
		hiddenSelectionsTextures[] = {"\dayzexpansion\objects\structures\flags\data\countries\flag_uk_co.paa"};
	};
	class ExpansionPropFlagUSA: ExpansionPropFlagBase
	{
		scope = 2;
		displayName = "$STR_EXPANSION_FLAG_POLE_USA";
		hiddenSelectionsTextures[] = {"\dayzexpansion\objects\structures\flags\data\countries\flag_usa_co.paa"};
	};
	class ExpansionPropFlagExpansion: ExpansionPropFlagBase
	{
		scope = 2;
		displayName = "$STR_EXPANSION_FLAG_POLE_EXPANSION";
		hiddenSelectionsTextures[] = {"\dayzexpansion\objects\structures\flags\data\logos\flag_expansion_co.paa"};
	};
	class ExpansionTrainWagonFlat: HouseNoDestruct
	{
		vehicleClass = "Expansion_Static";
		scope = 1;
		model = "\DZ\structures\wrecks\Trains\Train_Wagon_Flat.p3d";
	};
	class ExpansionTrainWagonTanker: HouseNoDestruct
	{
		vehicleClass = "Expansion_Static";
		scope = 1;
		model = "\DZ\structures\wrecks\Trains\Train_Wagon_Tanker.p3d";
	};
	class ExpansionCementworksSiloBridge: HouseNoDestruct
	{
		vehicleClass = "Expansion_Static";
		scope = 1;
		model = "\DZ\structures\industrial\cementworks\cementworks_silobig1bridge.p3d";
	};
	class ExpansionPierConcrete2: HouseNoDestruct
	{
		vehicleClass = "Expansion_Static";
		scope = 1;
		model = "\DZ\structures\industrial\harbour\pier_concrete2_end.p3d";
	};
	class ExpansionPierConcrete1: HouseNoDestruct
	{
		vehicleClass = "Expansion_Static";
		scope = 1;
		model = "\DZ\structures\industrial\harbour\pier_concrete1_r.p3d";
	};
	class ExpansionPierConcrete2Stripes: HouseNoDestruct
	{
		vehicleClass = "Expansion_Static";
		scope = 1;
		model = "\DZ\structures\military\misc\misc_concreteblock2_stripes.p3d";
	};
	class ExpansionPierConcrete2End: HouseNoDestruct
	{
		scope = 1;
		model = "\DZ\structures\industrial\harbour\pier_concrete2_end.p3d";
	};
	class ExpansionContainer1a: HouseNoDestruct
	{
		vehicleClass = "Expansion_Static";
		scope = 1;
		model = "\DZ\structures\industrial\containers\container_1a.p3d";
	};
	class ExpansionContainer1b: HouseNoDestruct
	{
		vehicleClass = "Expansion_Static";
		scope = 1;
		model = "\DZ\structures\industrial\containers\container_1b.p3d";
	};
	class ExpansionContainer1c: HouseNoDestruct
	{
		vehicleClass = "Expansion_Static";
		scope = 1;
		model = "\DZ\structures\industrial\containers\container_1c.p3d";
	};
	class ExpansionContainer1d: HouseNoDestruct
	{
		scope = 1;
		model = "\DZ\structures\industrial\containers\container_1d.p3d";
	};
	class ExpansionContainer2a: HouseNoDestruct
	{
		vehicleClass = "Expansion_Static";
		scope = 1;
		model = "\DZ\structures\industrial\containers\container_2a.p3d";
	};
	class ExpansionContainer2b: HouseNoDestruct
	{
		vehicleClass = "Expansion_Static";
		scope = 1;
		model = "\DZ\structures\industrial\containers\container_2b.p3d";
	};
	class ExpansionContainer2c: HouseNoDestruct
	{
		vehicleClass = "Expansion_Static";
		scope = 1;
		model = "\DZ\structures\industrial\containers\container_2c.p3d";
	};
	class ExpansionContainer2d: HouseNoDestruct
	{
		vehicleClass = "Expansion_Static";
		scope = 1;
		model = "\DZ\structures\industrial\containers\container_2d.p3d";
	};
	class ExpansionContainer2e: HouseNoDestruct
	{
		vehicleClass = "Expansion_Static";
		scope = 1;
		model = "\DZ\structures\industrial\containers\container_2e.p3d";
	};
	class ExpansionWreckHmmwv: HouseNoDestruct
	{
		vehicleClass = "Expansion_Static";
		scope = 1;
		model = "\DZ\structures\wrecks\vehicles\wreck_hmmwv.p3d";
	};
	class ExpansionWreckBmp1: HouseNoDestruct
	{
		vehicleClass = "Expansion_Static";
		scope = 1;
		model = "\DZ\structures\wrecks\vehicles\wreck_bmp1.p3d";
	};
	class ExpansionWreckBrdm: HouseNoDestruct
	{
		vehicleClass = "Expansion_Static";
		scope = 1;
		model = "\DZ\structures\wrecks\vehicles\wreck_brdm.p3d";
	};
	class ExpansionRoadBarrier: HouseNoDestruct
	{
		vehicleClass = "Expansion_Static";
		scope = 1;
		model = "\DZ\structures\residential\misc\misc_roadbarrier.p3d";
	};
	class ExpansionRoadBarrierLong: HouseNoDestruct
	{
		vehicleClass = "Expansion_Static";
		scope = 1;
		model = "\DZ\structures\military\improvised\roadblock_cncblocks_long.p3d";
	};
	class ExpansionMedicalShowerTent: HouseNoDestruct
	{
		vehicleClass = "Expansion_Static";
		scope = 1;
		model = "\DZ\structures\residential\Medical\Medical_Tent_Shower.p3d";
	};
	class ExpansionMedicalShowerTentBig: HouseNoDestruct
	{
		vehicleClass = "Expansion_Static";
		scope = 1;
		model = "\DZ\structures\residential\Medical\Medical_Tent_Big.p3d";
	};
	class ExpansionBunkerGrassL: HouseNoDestruct
	{
		vehicleClass = "Expansion_Static";
		scope = 1;
		model = "\DZ\structures\military\Airfield\Mil_AircraftShelter_Grass_L.p3d";
		physLayer = "item_small";
		carveNavmesh = 0;
	};
	class ExpansionBunkerGrassR: HouseNoDestruct
	{
		vehicleClass = "Expansion_Static";
		scope = 1;
		model = "\DZ\structures\military\Airfield\Mil_AircraftShelter_Grass_R.p3d";
		physLayer = "item_small";
		carveNavmesh = 0;
	};
	class ExpansionDecal_damage_long1: HouseNoDestruct
	{
		vehicleClass = "Expansion_Static";
		scope = 1;
		model = "\DZ\structures\roads\Decals\Decal_damage_long1.p3d";
		physLayer = "item_small";
		carveNavmesh = 0;
	};
	class ExpansionDecal_damage_long2: HouseNoDestruct
	{
		vehicleClass = "Expansion_Static";
		scope = 1;
		model = "\DZ\structures\roads\Decals\Decal_damage_long2.p3d";
		physLayer = "item_small";
		carveNavmesh = 0;
	};
	class ExpansionDecal_damage_long3: HouseNoDestruct
	{
		vehicleClass = "Expansion_Static";
		scope = 1;
		model = "\DZ\structures\roads\Decals\Decal_damage_long3.p3d";
		physLayer = "item_small";
		carveNavmesh = 0;
	};
	class ExpansionDecal_damage_long4: HouseNoDestruct
	{
		vehicleClass = "Expansion_Static";
		scope = 1;
		model = "\DZ\structures\roads\Decals\Decal_damage_long4.p3d";
		physLayer = "item_small";
		carveNavmesh = 0;
	};
	class ExpansionDecal_damage_long5: HouseNoDestruct
	{
		vehicleClass = "Expansion_Static";
		scope = 1;
		model = "\DZ\structures\roads\Decals\Decal_damage_long5.p3d";
		physLayer = "item_small";
		carveNavmesh = 0;
	};
	class ExpansionDecal_damage_medium1: HouseNoDestruct
	{
		vehicleClass = "Expansion_Static";
		scope = 1;
		model = "\DZ\structures\roads\Decals\Decal_damage_medium1.p3d";
		physLayer = "item_small";
		carveNavmesh = 0;
	};
	class ExpansionDecal_damage_medium2: HouseNoDestruct
	{
		vehicleClass = "Expansion_Static";
		scope = 1;
		model = "\DZ\structures\roads\Decals\Decal_damage_medium2.p3d";
		physLayer = "item_small";
		carveNavmesh = 0;
	};
	class ExpansionDecal_damage_small1: HouseNoDestruct
	{
		vehicleClass = "Expansion_Static";
		scope = 1;
		model = "\DZ\structures\roads\Decals\Decal_damage_small1.p3d";
		physLayer = "item_small";
		carveNavmesh = 0;
	};
	class ExpansionDecal_damage_small2: HouseNoDestruct
	{
		vehicleClass = "Expansion_Static";
		scope = 1;
		model = "\DZ\structures\roads\Decals\Decal_damage_small2.p3d";
		physLayer = "item_small";
		carveNavmesh = 0;
	};
	class ExpansionMil_vojenskaBed: HouseNoDestruct
	{
		vehicleClass = "Expansion_Static";
		scope = 1;
		model = "\DZ\structures\furniture\Chairs\vojenska_palanda\Vojenska_palanda.p3d";
	};
	class ExpansionCihly1: HouseNoDestruct
	{
		vehicleClass = "Expansion_Static";
		scope = 1;
		model = "\DZ\structures\industrial\Construction\Proxy\cihly1.p3d";
	};
	class ExpansionCihly2: HouseNoDestruct
	{
		vehicleClass = "Expansion_Static";
		scope = 1;
		model = "\DZ\structures\industrial\Construction\Proxy\cihly2.p3d";
	};
	class ExpansionCihly3: HouseNoDestruct
	{
		vehicleClass = "Expansion_Static";
		scope = 1;
		model = "\DZ\structures\industrial\Construction\Proxy\cihly3.p3d";
	};
	class ExpansionCihly4: HouseNoDestruct
	{
		vehicleClass = "Expansion_Static";
		scope = 1;
		model = "\DZ\structures\industrial\Construction\Proxy\cihly4.p3d";
	};
	class ExpansionKoz: HouseNoDestruct
	{
		vehicleClass = "Expansion_Static";
		scope = 1;
		model = "\DZ\structures\industrial\Construction\Proxy\koz.p3d";
	};
	class ExpansionPipe_big: HouseNoDestruct
	{
		vehicleClass = "Expansion_Static";
		scope = 1;
		model = "\DZ\structures\industrial\Construction\Proxy\pipe-big.p3d";
	};
	class ExpansionPipe_small: HouseNoDestruct
	{
		vehicleClass = "Expansion_Static";
		scope = 1;
		model = "\DZ\structures\industrial\Construction\Proxy\pipe-small.p3d";
	};
	class ExpansionYtong: HouseNoDestruct
	{
		vehicleClass = "Expansion_Static";
		scope = 1;
		model = "\DZ\structures\industrial\Construction\Proxy\ytong.p3d";
	};
	class ExpansionMisc_HayBale_Decayed: HouseNoDestruct
	{
		vehicleClass = "Expansion_Static";
		scope = 1;
		model = "\DZ\structures\industrial\Misc\Misc_HayBale_Decayed.p3d";
	};
	class ExpansionCross: HouseNoDestruct
	{
		vehicleClass = "Expansion_Static";
		scope = 1;
		model = "\DZ\structures\residential\Medical\Proxy\Cross.p3d";
	};
	class ExpansionWreck_Trailer_Closed_base: HouseNoDestruct
	{
		vehicleClass = "Expansion_Static";
		scope = 1;
		model = "\DZ\structures\wrecks\Vehicles\Proxy\Wreck_Trailer_Closed_base.p3d";
	};
	class Expansion_ChernarusMap_Open: HouseNoDestruct
	{
		vehicleClass = "Expansion_Static";
		scope = 1;
		model = "\DZ\gear\navigation\Map_chernarus_open.p3d";
	};
};
