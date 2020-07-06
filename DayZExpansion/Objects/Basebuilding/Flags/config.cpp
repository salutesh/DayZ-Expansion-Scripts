#define _ARMA_

class CfgPatches
{
	class DayZExpansion_Objects_BaseBuilding_Flags
	{
		requiredVersion = 0.1;
		requiredAddons[] = {"DZ_Data"};
		units[] = {"ExpansionFlagKitAustralia","ExpansionFlagKitAustraliaPlacing","ExpansionFlagKitCanada","ExpansionFlagKitCanadaPlacing","ExpansionFlagKitChernarus","ExpansionFlagKitChernarusPlacing","ExpansionFlagKitFrance","ExpansionFlagKitFrancePlacing","ExpansionFlagKitGermany","ExpansionFlagKitGermanyPlacing","ExpansionFlagKitLatvia","ExpansionFlagKitLatviaPlacing","ExpansionFlagKitLuxembourg","ExpansionFlagKitLuxembourgPlacing","ExpansionFlagKitMexico","ExpansionFlagKitMexicoPlacing","ExpansionFlagKitNetherlands","ExpansionFlagKitNetherlandsPlacing","ExpansionFlagKitNewZealand","ExpansionFlagKitNewZealandPlacing","ExpansionFlagKitNorway","ExpansionFlagKitNorwayPlacing","ExpansionFlagKitPoland","ExpansionFlagKitPolandPlacing","ExpansionFlagKitRussia","ExpansionFlagKitRussiaPlacing","ExpansionFlagKitUK","ExpansionFlagKitUKPlacing","ExpansionFlagKitUSA","ExpansionFlagKitUSAPlacing","ExpansionFlagKitExpansion","ExpansionFlagKitExpansionPlacing","ExpansionFlagKitWhite","ExpansionFlagKitWhitePlacing","ExpansionFlagBase","ExpansionTerritoryFlag"};
		weapons[] = {};
	};
};
class CfgVehicles
{
	class ExpansionKitSmall;
	class ExpansionBaseBuilding;
	class ExpansionFlagKitBase: ExpansionKitSmall
	{
		scope = 0;
		displayName = "$STR_EXPANSION_FLAG_KIT";
		descriptionShort = "$STR_EXPANSION_FLAG_KIT_DESC";
		model = "\DZ\gear\camping\fence_kit.p3d";
	};
	class ExpansionFlagKitPlacing: ExpansionFlagKitBase
	{
		scope = 1;
		model = "DayZExpansion\Objects\Structures\Flags\flagpole_preview.p3d";
		slopeTolerance = 0.85;
		hiddenSelections[] = {"placing"};
		hiddenSelectionsTextures[] = {"#(argb,8,8,3)color(1,1,1,1.0,co)"};
	};
	class ExpansionFlagKitAustralia: ExpansionFlagKitBase
	{
		scope = 2;
		displayName = "$STR_EXPANSION_FLAG_KIT_AUS";
	};
	class ExpansionFlagKitAustraliaPlacing: ExpansionFlagKitAustralia
	{
		scope = 2;
		model = "DayZExpansion\Objects\Structures\Flags\flagpole_preview.p3d";
	};
	class ExpansionFlagKitCanada: ExpansionFlagKitBase
	{
		scope = 2;
		displayName = "$STR_EXPANSION_FLAG_KIT_CANADA";
	};
	class ExpansionFlagKitCanadaPlacing: ExpansionFlagKitCanada
	{
		scope = 2;
		model = "DayZExpansion\Objects\Structures\Flags\flagpole_preview.p3d";
	};
	class ExpansionFlagKitChernarus: ExpansionFlagKitBase
	{
		scope = 2;
		displayName = "$STR_EXPANSION_FLAG_KIT_CHERNARUS";
	};
	class ExpansionFlagKitChernarusPlacing: ExpansionFlagKitChernarus
	{
		scope = 2;
		model = "DayZExpansion\Objects\Structures\Flags\flagpole_preview.p3d";
	};
	class ExpansionFlagKitFrance: ExpansionFlagKitBase
	{
		scope = 2;
		displayName = "$STR_EXPANSION_FLAG_KIT_FRANCE";
	};
	class ExpansionFlagKitFrancePlacing: ExpansionFlagKitFrance
	{
		scope = 2;
		model = "DayZExpansion\Objects\Structures\Flags\flagpole_preview.p3d";
	};
	class ExpansionFlagKitGermany: ExpansionFlagKitBase
	{
		scope = 2;
		displayName = "$STR_EXPANSION_FLAG_KIT_GERMANY";
	};
	class ExpansionFlagKitGermanyPlacing: ExpansionFlagKitGermany
	{
		scope = 2;
		model = "DayZExpansion\Objects\Structures\Flags\flagpole_preview.p3d";
	};
	class ExpansionFlagKitLatvia: ExpansionFlagKitBase
	{
		scope = 2;
		displayName = "$STR_EXPANSION_FLAG_KIT_LATVIA";
	};
	class ExpansionFlagKitLatviaPlacing: ExpansionFlagKitLatvia
	{
		scope = 2;
		model = "DayZExpansion\Objects\Structures\Flags\flagpole_preview.p3d";
	};
	class ExpansionFlagKitLuxembourg: ExpansionFlagKitBase
	{
		scope = 2;
		displayName = "$STR_EXPANSION_FLAG_KIT_LUXEMBOURG";
	};
	class ExpansionFlagKitLuxembourgPlacing: ExpansionFlagKitLuxembourg
	{
		scope = 2;
		model = "DayZExpansion\Objects\Structures\Flags\flagpole_preview.p3d";
	};
	class ExpansionFlagKitMexico: ExpansionFlagKitBase
	{
		scope = 2;
		displayName = "$STR_EXPANSION_FLAG_KIT_MEXICO";
	};
	class ExpansionFlagKitMexicoPlacing: ExpansionFlagKitMexico
	{
		scope = 2;
		model = "DayZExpansion\Objects\Structures\Flags\flagpole_preview.p3d";
	};
	class ExpansionFlagKitNetherlands: ExpansionFlagKitBase
	{
		scope = 2;
		displayName = "$STR_EXPANSION_FLAG_KIT_NETHERLANDS";
	};
	class ExpansionFlagKitNetherlandsPlacing: ExpansionFlagKitNetherlands
	{
		scope = 2;
		model = "DayZExpansion\Objects\Structures\Flags\flagpole_preview.p3d";
	};
	class ExpansionFlagKitNewZealand: ExpansionFlagKitBase
	{
		scope = 2;
		displayName = "$STR_EXPANSION_FLAG_KIT_NZ";
	};
	class ExpansionFlagKitNewZealandPlacing: ExpansionFlagKitNewZealand
	{
		scope = 2;
		model = "DayZExpansion\Objects\Structures\Flags\flagpole_preview.p3d";
	};
	class ExpansionFlagKitNorway: ExpansionFlagKitBase
	{
		scope = 2;
		displayName = "$STR_EXPANSION_FLAG_KIT_NORWAY";
	};
	class ExpansionFlagKitNorwayPlacing: ExpansionFlagKitNorway
	{
		scope = 2;
		model = "DayZExpansion\Objects\Structures\Flags\flagpole_preview.p3d";
	};
	class ExpansionFlagKitPoland: ExpansionFlagKitBase
	{
		scope = 2;
		displayName = "$STR_EXPANSION_FLAG_KIT_POLAND";
	};
	class ExpansionFlagKitPolandPlacing: ExpansionFlagKitPoland
	{
		scope = 2;
		model = "DayZExpansion\Objects\Structures\Flags\flagpole_preview.p3d";
	};
	class ExpansionFlagKitRussia: ExpansionFlagKitBase
	{
		scope = 2;
		displayName = "$STR_EXPANSION_FLAG_KIT_RUSSIA";
	};
	class ExpansionFlagKitRussiaPlacing: ExpansionFlagKitRussia
	{
		scope = 2;
		model = "DayZExpansion\Objects\Structures\Flags\flagpole_preview.p3d";
	};
	class ExpansionFlagKitUK: ExpansionFlagKitBase
	{
		scope = 2;
		displayName = "$STR_EXPANSION_FLAG_KIT_UK";
	};
	class ExpansionFlagKitUKPlacing: ExpansionFlagKitUK
	{
		scope = 2;
		model = "DayZExpansion\Objects\Structures\Flags\flagpole_preview.p3d";
	};
	class ExpansionFlagKitUSA: ExpansionFlagKitBase
	{
		scope = 2;
		displayName = "$STR_EXPANSION_FLAG_KIT_USA";
	};
	class ExpansionFlagKitUSAPlacing: ExpansionFlagKitUSA
	{
		scope = 2;
		model = "DayZExpansion\Objects\Structures\Flags\flagpole_preview.p3d";
	};
	class ExpansionFlagKitExpansion: ExpansionFlagKitBase
	{
		scope = 2;
		displayName = "$STR_EXPANSION_FLAG_KIT_EXPANSION";
	};
	class ExpansionFlagKitExpansionPlacing: ExpansionFlagKitExpansion
	{
		scope = 2;
		model = "DayZExpansion\Objects\Structures\Flags\flagpole_preview.p3d";
	};
	class ExpansionFlagKitWhite: ExpansionFlagKitBase
	{
		scope = 2;
		displayName = "$STR_EXPANSION_FLAG_KIT_WHITE";
	};
	class ExpansionFlagKitWhitePlacing: ExpansionFlagKitWhite
	{
		scope = 2;
		model = "DayZExpansion\Objects\Structures\Flags\flagpole_preview.p3d";
	};
	class ExpansionFlagBase: ExpansionBaseBuilding
	{
		scope = 2;
		vehicleClass = "Expansion_Construction";
		displayName = "$STR_EXPANSION_FLAG_POLE";
		descriptionShort = "$STR_EXPANSION_FLAG_KIT_DESC_2";
		model = "DayZExpansion\Objects\Structures\Flags\flagpole.p3d";
		useEntityHierarchy = "true";
		forceFarBubble = "true";
		carveNavmesh = 1;
		bounding = "BSphere";
		physLayer = "item_large";
		allowOwnedCargoManipulation = 1;
		hiddenSelections[] = {"latka"};
		hiddenSelectionsTextures[] = {"\DayZExpansion\Objects\Structures\Flags\data\countries\flag_chernarus_co.paa"};
	};
	class ExpansionTerritoryFlag: ExpansionFlagBase
	{
		scope = 2;
		vehicleClass = "Expansion_Construction";
		displayName = "ExpansionTerritoryFlag";
		descriptionShort = "ExpansionTerritoryFlag";
	};
};
