#define _ARMA_

class CfgPatches
{
	class DayZExpansion_Objects_BaseBuilding_Flags
	{
		requiredVersion = 0.1;
		requiredAddons[] = {"DZ_Data"};
		units[] = {"ExpansionFlagKitAustralia","ExpansionFlagKitAustraliaPlacing","ExpansionFlagKitCanada","ExpansionFlagKitCanadaPlacing","ExpansionFlagKitChernarus","ExpansionFlagKitChernarusPlacing","ExpansionFlagKitFrance","ExpansionFlagKitFrancePlacing","ExpansionFlagKitGermany","ExpansionFlagKitGermanyPlacing","ExpansionFlagKitLatvia","ExpansionFlagKitLatviaPlacing","ExpansionFlagKitLuxembourg","ExpansionFlagKitLuxembourgPlacing","ExpansionFlagKitMexico","ExpansionFlagKitMexicoPlacing","ExpansionFlagKitNetherlands","ExpansionFlagKitNetherlandsPlacing","ExpansionFlagKitNewZealand","ExpansionFlagKitNewZealandPlacing","ExpansionFlagKitNorway","ExpansionFlagKitNorwayPlacing","ExpansionFlagKitPoland","ExpansionFlagKitPolandPlacing","ExpansionFlagKitRussia","ExpansionFlagKitRussiaPlacing","ExpansionFlagKitUK","ExpansionFlagKitUKPlacing","ExpansionFlagKitUSA","ExpansionFlagKitUSAPlacing","ExpansionFlagKitExpansion","ExpansionFlagKitExpansionPlacing","ExpansionFlagKitWhite","ExpansionFlagKitWhitePlacing","ExpansionFlagBase","ExpansionTerritoryFlag","Expansion_Flag_Expansion","Expansion_Flag_White","Expansion_Flag_Australia","Expansion_Flag_Canada","Expansion_Flag_Chernarus","Expansion_Flag_France","Expansion_Flag_Germany","Expansion_Flag_Latvia","Expansion_Flag_Luxembourg","Expansion_Flag_Mexico","Expansion_Flag_Netherlands","Expansion_Flag_Newzealand","Expansion_Flag_Norway","Expansion_Flag_Poland","Expansion_Flag_Russia","Expansion_Flag_UK","Expansion_Flag_USA","Expansion_Flag_Scottish","Expansion_Flag_Finnish","Expansion_Flag_Swedish","Expansion_Flag_Spain","Expansion_Flag_Brazil","Expansion_Flag_Portugal","Expansion_Flag_Belgium","Expansion_Flag_Livonia","Expansion_Flag_Japan","Expansion_Flag_China","Expansion_Flag_SouthKorea","Expansion_Flag_UN","Expansion_Flag_NATO","Expansion_Flag_Pirate","Expansion_Flag_Chedaki","Expansion_Flag_Napa","Expansion_Flag_Cdf","Expansion_Flag_NuevoRico","Expansion_Flag_Borduriens","Expansion_Flag_Biohazard","Expansion_Flag_AnyoneInCherno","Expansion_Flag_Irish","Expansion_Flag_Wales","Expansion_Flag_Switzerland","Expansion_Flag_Skilanka","Expansion_Flag_SouthAfrica","Expansion_Flag_Sicily","Expansion_Flag_OffWithHead","Expansion_Flag_Gubraltar","Expansion_Flag_Czech","Expansion_Flag_Fari","Expansion_Flag_Finland","Expansion_Flag_DayZWhite","Expansion_Flag_DayZBlack","Expansion_Flag_DoubleAxe","Expansion_Flag_Grenade","Expansion_Flag_Red","Expansion_Flag_Blue","Expansion_Flag_Green","Expansion_Flag_Yellow","Expansion_Flag_Orange","Expansion_Flag_Pink","Expansion_Flag_Purple","Expansion_Flag_Rainbow"};
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
		hiddenSelectionsTextures[] = {"\DayZExpansion\Objects\Structures\Flags\data\logos\flag_expansion_co.paa"};
	};
	class ExpansionTerritoryFlag: ExpansionFlagBase
	{
		scope = 2;
		vehicleClass = "Expansion_Construction";
		displayName = "ExpansionTerritoryFlag";
		descriptionShort = "ExpansionTerritoryFlag";
	};
	class Flag_Base;
	class Expansion_Flag_Expansion: Flag_Base
	{
		scope = 2;
		hiddenSelectionsTextures[] = {"\DayZExpansion\Objects\Structures\Flags\data\logos\flag_expansion_co.paa"};
	};
	class Expansion_Flag_White: Flag_Base
	{
		scope = 2;
		hiddenSelectionsTextures[] = {"\DayZExpansion\Objects\Structures\Flags\data\colors\flag_white_co.paa"};
	};
	class Expansion_Flag_Australia: Flag_Base
	{
		scope = 2;
		hiddenSelectionsTextures[] = {"\DayZExpansion\Objects\Structures\Flags\data\countries\flag_australia_co.paa"};
	};
	class Expansion_Flag_Canada: Flag_Base
	{
		scope = 2;
		hiddenSelectionsTextures[] = {"\DayZExpansion\Objects\Structures\Flags\data\countries\flag_canada_co.paa"};
	};
	class Expansion_Flag_Chernarus: Flag_Base
	{
		scope = 2;
		hiddenSelectionsTextures[] = {"\DayZExpansion\Objects\Structures\Flags\data\countries\flag_chernarus_co.paa"};
	};
	class Expansion_Flag_France: Flag_Base
	{
		scope = 2;
		hiddenSelectionsTextures[] = {"\DayZExpansion\Objects\Structures\Flags\data\countries\flag_france_co.paa"};
	};
	class Expansion_Flag_Germany: Flag_Base
	{
		scope = 2;
		hiddenSelectionsTextures[] = {"\DayZExpansion\Objects\Structures\Flags\data\countries\flag_germany_co.paa"};
	};
	class Expansion_Flag_Latvia: Flag_Base
	{
		scope = 2;
		hiddenSelectionsTextures[] = {"\DayZExpansion\Objects\Structures\Flags\data\countries\flag_latvia_co.paa"};
	};
	class Expansion_Flag_Luxembourg: Flag_Base
	{
		scope = 2;
		hiddenSelectionsTextures[] = {"\DayZExpansion\Objects\Structures\Flags\data\countries\flag_luxembourg_co.paa"};
	};
	class Expansion_Flag_Mexico: Flag_Base
	{
		scope = 2;
		hiddenSelectionsTextures[] = {"\DayZExpansion\Objects\Structures\Flags\data\countries\flag_mexico_co.paa"};
	};
	class Expansion_Flag_Netherlands: Flag_Base
	{
		scope = 2;
		hiddenSelectionsTextures[] = {"\DayZExpansion\Objects\Structures\Flags\data\countries\flag_netherlands_co.paa"};
	};
	class Expansion_Flag_Newzealand: Flag_Base
	{
		scope = 2;
		hiddenSelectionsTextures[] = {"\DayZExpansion\Objects\Structures\Flags\data\countries\flag_new_zealand_co.paa"};
	};
	class Expansion_Flag_Norway: Flag_Base
	{
		scope = 2;
		hiddenSelectionsTextures[] = {"\DayZExpansion\Objects\Structures\Flags\data\countries\flag_norway_co.paa"};
	};
	class Expansion_Flag_Poland: Flag_Base
	{
		scope = 2;
		hiddenSelectionsTextures[] = {"\DayZExpansion\Objects\Structures\Flags\data\countries\flag_poland_co.paa"};
	};
	class Expansion_Flag_Russia: Flag_Base
	{
		scope = 2;
		hiddenSelectionsTextures[] = {"\DayZExpansion\Objects\Structures\Flags\data\countries\flag_russia_co.paa"};
	};
	class Expansion_Flag_UK: Flag_Base
	{
		scope = 2;
		hiddenSelectionsTextures[] = {"\DayZExpansion\Objects\Structures\Flags\data\countries\flag_uk_co.paa"};
	};
	class Expansion_Flag_USA: Flag_Base
	{
		scope = 2;
		hiddenSelectionsTextures[] = {"\DayZExpansion\Objects\Structures\Flags\data\countries\flag_usa_co.paa"};
	};
	class Expansion_Flag_Scottish: Flag_Base
	{
		scope = 2;
		hiddenSelectionsTextures[] = {"\DayZExpansion\Objects\Structures\Flags\data\countries\flag_scottish_co.paa"};
	};
	class Expansion_Flag_Finnish: Flag_Base
	{
		scope = 2;
		hiddenSelectionsTextures[] = {"\DayZExpansion\Objects\Structures\Flags\data\countries\flag_finnish_co.paa"};
	};
	class Expansion_Flag_Swedish: Flag_Base
	{
		scope = 2;
		hiddenSelectionsTextures[] = {"\DayZExpansion\Objects\Structures\Flags\data\countries\flag_swedish_co.paa"};
	};
	class Expansion_Flag_Spain: Flag_Base
	{
		scope = 2;
		hiddenSelectionsTextures[] = {"\DayZExpansion\Objects\Structures\Flags\data\countries\flag_spain_co.paa"};
	};
	class Expansion_Flag_Brazil: Flag_Base
	{
		scope = 2;
		hiddenSelectionsTextures[] = {"\DayZExpansion\Objects\Structures\Flags\data\countries\flag_brazil_co.paa"};
	};
	class Expansion_Flag_Portugal: Flag_Base
	{
		scope = 2;
		hiddenSelectionsTextures[] = {"\DayZExpansion\Objects\Structures\Flags\data\countries\flag_portugal_co.paa"};
	};
	class Expansion_Flag_Belgium: Flag_Base
	{
		scope = 2;
		hiddenSelectionsTextures[] = {"\DayZExpansion\Objects\Structures\Flags\data\countries\flag_belgium_co.paa"};
	};
	class Expansion_Flag_Livonia: Flag_Base
	{
		scope = 2;
		hiddenSelectionsTextures[] = {"\DayZExpansion\Objects\Structures\Flags\data\countries\flag_livonia_co.paa"};
	};
	class Expansion_Flag_Japan: Flag_Base
	{
		scope = 2;
		hiddenSelectionsTextures[] = {"\DayZExpansion\Objects\Structures\Flags\data\countries\flag_japan_co.paa"};
	};
	class Expansion_Flag_China: Flag_Base
	{
		scope = 2;
		hiddenSelectionsTextures[] = {"\DayZExpansion\Objects\Structures\Flags\data\countries\flag_china_co.paa"};
	};
	class Expansion_Flag_SouthKorea: Flag_Base
	{
		scope = 2;
		hiddenSelectionsTextures[] = {"\DayZExpansion\Objects\Structures\Flags\data\countries\flag_southkorea_co.paa"};
	};
	class Expansion_Flag_UN: Flag_Base
	{
		scope = 2;
		hiddenSelectionsTextures[] = {"\DayZExpansion\Objects\Structures\Flags\data\countries\flag_un_co.paa"};
	};
	class Expansion_Flag_NATO: Flag_Base
	{
		scope = 2;
		hiddenSelectionsTextures[] = {"\DayZExpansion\Objects\Structures\Flags\data\countries\flag_nato_co.paa"};
	};
	class Expansion_Flag_Pirate: Flag_Base
	{
		scope = 2;
		hiddenSelectionsTextures[] = {"\DayZExpansion\Objects\Structures\Flags\data\countries\flag_pirate_co.paa"};
	};
	class Expansion_Flag_Chedaki: Flag_Base
	{
		scope = 2;
		hiddenSelectionsTextures[] = {"\DayZExpansion\Objects\Structures\Flags\data\countries\flag_chedaki_co.paa"};
	};
	class Expansion_Flag_Napa: Flag_Base
	{
		scope = 2;
		hiddenSelectionsTextures[] = {"\DayZExpansion\Objects\Structures\Flags\data\countries\flag_napa_co.paa"};
	};
	class Expansion_Flag_Cdf: Flag_Base
	{
		scope = 2;
		hiddenSelectionsTextures[] = {"\DayZExpansion\Objects\Structures\Flags\data\countries\flag_cdf_co.paa"};
	};
	class Expansion_Flag_NuevoRico: Flag_Base
	{
		scope = 2;
		hiddenSelectionsTextures[] = {"\DayZExpansion\Objects\Structures\Flags\data\countries\flag_nuevorico_co.paa"};
	};
	class Expansion_Flag_Borduriens: Flag_Base
	{
		scope = 2;
		hiddenSelectionsTextures[] = {"\DayZExpansion\Objects\Structures\Flags\data\countries\flag_borduriens_co.paa"};
	};
	class Expansion_Flag_Biohazard: Flag_Base
	{
		scope = 2;
		hiddenSelectionsTextures[] = {"\DayZExpansion\Objects\Structures\Flags\data\logos\flag_biohazard_co.paa"};
	};
	class Expansion_Flag_AnyoneInCherno: Flag_Base
	{
		scope = 2;
		hiddenSelectionsTextures[] = {"\DayZExpansion\Objects\Structures\Flags\data\logos\flag_anyoneincherno_co.paa"};
	};
	class Expansion_Flag_Irish: Flag_Base
	{
		scope = 2;
		hiddenSelectionsTextures[] = {"\DayZExpansion\Objects\Structures\Flags\data\countries\flag_irish_co.paa"};
	};
	class Expansion_Flag_Wales: Flag_Base
	{
		scope = 2;
		hiddenSelectionsTextures[] = {"\DayZExpansion\Objects\Structures\Flags\data\countries\flag_wales_co.paa"};
	};
	class Expansion_Flag_Switzerland: Flag_Base
	{
		scope = 2;
		hiddenSelectionsTextures[] = {"\DayZExpansion\Objects\Structures\Flags\data\countries\flag_switzerland_co.paa"};
	};
	class Expansion_Flag_Skilanka: Flag_Base
	{
		scope = 2;
		hiddenSelectionsTextures[] = {"\DayZExpansion\Objects\Structures\Flags\data\countries\flag_skilanka_co.paa"};
	};
	class Expansion_Flag_SouthAfrica: Flag_Base
	{
		scope = 2;
		hiddenSelectionsTextures[] = {"\DayZExpansion\Objects\Structures\Flags\data\countries\flag_southafrica_co.paa"};
	};
	class Expansion_Flag_Sicily: Flag_Base
	{
		scope = 2;
		hiddenSelectionsTextures[] = {"\DayZExpansion\Objects\Structures\Flags\data\countries\flag_sicily_co.paa"};
	};
	class Expansion_Flag_OffWithHead: Flag_Base
	{
		scope = 2;
		hiddenSelectionsTextures[] = {"\DayZExpansion\Objects\Structures\Flags\data\countries\flag_offwithhead_co.paa"};
	};
	class Expansion_Flag_Gubraltar: Flag_Base
	{
		scope = 2;
		hiddenSelectionsTextures[] = {"\DayZExpansion\Objects\Structures\Flags\data\countries\flag_gibraltar_co.paa"};
	};
	class Expansion_Flag_Czech: Flag_Base
	{
		scope = 2;
		hiddenSelectionsTextures[] = {"\DayZExpansion\Objects\Structures\Flags\data\countries\flag_czech_co.paa"};
	};
	class Expansion_Flag_Fari: Flag_Base
	{
		scope = 2;
		hiddenSelectionsTextures[] = {"\DayZExpansion\Objects\Structures\Flags\data\countries\flag_fari_co.paa"};
	};
	class Expansion_Flag_Finland: Flag_Base
	{
		scope = 2;
		hiddenSelectionsTextures[] = {"\DayZExpansion\Objects\Structures\Flags\data\countries\flag_finland_co.paa"};
	};
	class Expansion_Flag_DayZWhite: Flag_Base
	{
		scope = 2;
		hiddenSelectionsTextures[] = {"\DayZExpansion\Objects\Structures\Flags\data\logos\flag_dayzwhite_co.paa"};
	};
	class Expansion_Flag_DayZBlack: Flag_Base
	{
		scope = 2;
		hiddenSelectionsTextures[] = {"\DayZExpansion\Objects\Structures\Flags\data\logos\flag_dayzblack_co.paa"};
	};
	class Expansion_Flag_DoubleAxe: Flag_Base
	{
		scope = 2;
		hiddenSelectionsTextures[] = {"\DayZExpansion\Objects\Structures\Flags\data\logos\flag_doubleaxe_co.paa"};
	};
	class Expansion_Flag_Grenade: Flag_Base
	{
		scope = 2;
		hiddenSelectionsTextures[] = {"\DayZExpansion\Objects\Structures\Flags\data\logos\flag_grenade_co.paa"};
	};
	class Expansion_Flag_Red: Flag_Base
	{
		scope = 2;
		hiddenSelectionsTextures[] = {"\DayZExpansion\Objects\Structures\Flags\data\colors\flag_red_co.paa"};
	};
	class Expansion_Flag_Blue: Flag_Base
	{
		scope = 2;
		hiddenSelectionsTextures[] = {"\DayZExpansion\Objects\Structures\Flags\data\colors\flag_blue_co.paa"};
	};
	class Expansion_Flag_Green: Flag_Base
	{
		scope = 2;
		hiddenSelectionsTextures[] = {"\DayZExpansion\Objects\Structures\Flags\data\colors\flag_green_co.paa"};
	};
	class Expansion_Flag_Yellow: Flag_Base
	{
		scope = 2;
		hiddenSelectionsTextures[] = {"\DayZExpansion\Objects\Structures\Flags\data\colors\flag_yellow_co.paa"};
	};
	class Expansion_Flag_Orange: Flag_Base
	{
		scope = 2;
		hiddenSelectionsTextures[] = {"\DayZExpansion\Objects\Structures\Flags\data\colors\flag_orange_co.paa"};
	};
	class Expansion_Flag_Pink: Flag_Base
	{
		scope = 2;
		hiddenSelectionsTextures[] = {"\DayZExpansion\Objects\Structures\Flags\data\colors\flag_pink_co.paa"};
	};
	class Expansion_Flag_Purple: Flag_Base
	{
		scope = 2;
		hiddenSelectionsTextures[] = {"\DayZExpansion\Objects\Structures\Flags\data\colors\flag_purple_co.paa"};
	};
	class Expansion_Flag_Rainbow: Flag_Base
	{
		scope = 2;
		hiddenSelectionsTextures[] = {"\DayZExpansion\Objects\Structures\Flags\data\colors\flag_rainbow_co.paa"};
	};
};
