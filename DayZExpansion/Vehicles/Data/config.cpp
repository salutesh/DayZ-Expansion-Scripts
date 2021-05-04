#define _ARMA_

class CfgPatches
{
	class DayZExpansion_Vehicles_Data
	{
		units[] = {"ExpansionCarDoorFix","ExpansionSpraycanBlack","ExpansionSpraycanBlackRust","ExpansionSpraycanGrey","ExpansionSpraycanGreyRust","ExpansionSpraycanWhite","ExpansionSpraycanWhiteRust","ExpansionSpraycanGreen","ExpansionSpraycanGreenRust","ExpansionSpraycanRed","ExpansionSpraycanRedRust","ExpansionSpraycanBlue","ExpansionSpraycanBlueRust","ExpansionSpraycanYellow","ExpansionSpraycanYellowRust","ExpansionSpraycanYellowBright","ExpansionSpraycanYellowBrightRust","ExpansionSpraycanWine","ExpansionSpraycanWineRust","ExpansionSpraycanLime","ExpansionSpraycanPolice","ExpansionSpraycanBanditKitty"};
		weapons[] = {};
		requiredVersion = 0.1;
		requiredAddons[] = {"DayZExpansion_Core"};
	};
};
class CfgVehicles
{
	class Inventory_Base;
	class CarScript;
	class CarDoor: Inventory_Base
	{
		hiddenSelections[] = {"dmgZone_doors"};
		hiddenSelectionsTextures[] = {""};
		hiddenSelectionsMaterials[] = {""};
	};
	class ExpansionCarDoorFix: CarDoor
	{
		scope = 2;
	};
	class ExpansionSpraycanBase;
	class ExpansionSpraycanBlack: ExpansionSpraycanBase
	{
		scope = 2;
		displayName = "$STR_EXPANSION_SPRAYCAN_Black_NAME";
		descriptionShort = "$STR_EXPANSION_SPRAYCAN_Black_DESC";
		hiddenSelectionsTextures[] = {"DZ\gear\consumables\data\spraycan_black_co.paa"};
		skinName = "Black";
	};
	class ExpansionSpraycanBlackRust: ExpansionSpraycanBase
	{
		scope = 2;
		displayName = "$STR_EXPANSION_SPRAYCAN_BlackRust_NAME";
		descriptionShort = "$STR_EXPANSION_SPRAYCAN_BlackRust_DESC";
		hiddenSelectionsTextures[] = {"DZ\gear\consumables\data\spraycan_black_co.paa"};
		skinName = "BlackRust";
	};
	class ExpansionSpraycanGrey: ExpansionSpraycanBase
	{
		scope = 2;
		displayName = "$STR_EXPANSION_SPRAYCAN_Grey_NAME";
		descriptionShort = "$STR_EXPANSION_SPRAYCAN_Grey_DESC";
		hiddenSelectionsTextures[] = {"\DayZExpansion\Objects\Gear\Spraycans\data\spraycan_grey_co.paa"};
		skinName = "Grey";
	};
	class ExpansionSpraycanGreyRust: ExpansionSpraycanBase
	{
		scope = 2;
		displayName = "$STR_EXPANSION_SPRAYCAN_GreyRust_NAME";
		descriptionShort = "$STR_EXPANSION_SPRAYCAN_GreyRust_DESC";
		hiddenSelectionsTextures[] = {"\DayZExpansion\Objects\Gear\Spraycans\data\spraycan_grey_co.paa"};
		skinName = "GreyRust";
	};
	class ExpansionSpraycanWhite: ExpansionSpraycanBase
	{
		scope = 2;
		displayName = "$STR_EXPANSION_SPRAYCAN_White_NAME";
		descriptionShort = "$STR_EXPANSION_SPRAYCAN_White_DESC";
		hiddenSelectionsTextures[] = {"\DayZExpansion\Objects\Gear\Spraycans\data\spraycan_white_co.paa"};
		skinName = "White";
	};
	class ExpansionSpraycanWhiteRust: ExpansionSpraycanBase
	{
		scope = 2;
		displayName = "$STR_EXPANSION_SPRAYCAN_WhiteRust_NAME";
		descriptionShort = "$STR_EXPANSION_SPRAYCAN_WhiteRust_DESC";
		hiddenSelectionsTextures[] = {"\DayZExpansion\Objects\Gear\Spraycans\data\spraycan_white_co.paa"};
		skinName = "WhiteRust";
	};
	class ExpansionSpraycanGreen: ExpansionSpraycanBase
	{
		scope = 2;
		displayName = "$STR_EXPANSION_SPRAYCAN_Green_NAME";
		descriptionShort = "$STR_EXPANSION_SPRAYCAN_Green_DESC";
		hiddenSelectionsTextures[] = {"DZ\gear\consumables\data\spraycan_green_co.paa"};
		skinName = "Green";
	};
	class ExpansionSpraycanGreenRust: ExpansionSpraycanBase
	{
		scope = 2;
		displayName = "$STR_EXPANSION_SPRAYCAN_GreenRust_NAME";
		descriptionShort = "$STR_EXPANSION_SPRAYCAN_GreenRust_DESC";
		hiddenSelectionsTextures[] = {"DZ\gear\consumables\data\spraycan_green_co.paa"};
		skinName = "GreenRust";
	};
	class ExpansionSpraycanRed: ExpansionSpraycanBase
	{
		scope = 2;
		displayName = "$STR_EXPANSION_SPRAYCAN_Red_NAME";
		descriptionShort = "$STR_EXPANSION_SPRAYCAN_Red_DESC";
		hiddenSelectionsTextures[] = {"\DayZExpansion\Objects\Gear\Spraycans\data\spraycan_red_co.paa"};
		skinName = "Red";
	};
	class ExpansionSpraycanRedRust: ExpansionSpraycanBase
	{
		scope = 2;
		displayName = "$STR_EXPANSION_SPRAYCAN_RedRust_NAME";
		descriptionShort = "$STR_EXPANSION_SPRAYCAN_RedRust_DESC";
		hiddenSelectionsTextures[] = {"\DayZExpansion\Objects\Gear\Spraycans\data\spraycan_red_co.paa"};
		skinName = "RedRust";
	};
	class ExpansionSpraycanBlue: ExpansionSpraycanBase
	{
		scope = 2;
		displayName = "$STR_EXPANSION_SPRAYCAN_Blue_NAME";
		descriptionShort = "$STR_EXPANSION_SPRAYCAN_Blue_DESC";
		hiddenSelectionsTextures[] = {"\DayZExpansion\Objects\Gear\Spraycans\data\spraycan_blue_co.paa"};
		skinName = "Blue";
	};
	class ExpansionSpraycanBlueRust: ExpansionSpraycanBase
	{
		scope = 2;
		displayName = "$STR_EXPANSION_SPRAYCAN_BlueRust_NAME";
		descriptionShort = "$STR_EXPANSION_SPRAYCAN_BlueRust_DESC";
		hiddenSelectionsTextures[] = {"\DayZExpansion\Objects\Gear\Spraycans\data\spraycan_blue_co.paa"};
		skinName = "BlueRust";
	};
	class ExpansionSpraycanYellow: ExpansionSpraycanBase
	{
		scope = 2;
		displayName = "$STR_EXPANSION_SPRAYCAN_Yellow_NAME";
		descriptionShort = "$STR_EXPANSION_SPRAYCAN_Yellow_DESC";
		hiddenSelectionsTextures[] = {"\DayZExpansion\Objects\Gear\Spraycans\data\spraycan_yellow_co.paa"};
		skinName = "Yellow";
	};
	class ExpansionSpraycanYellowRust: ExpansionSpraycanBase
	{
		scope = 2;
		displayName = "$STR_EXPANSION_SPRAYCAN_YellowRust_NAME";
		descriptionShort = "$STR_EXPANSION_SPRAYCAN_YellowRust_DESC";
		hiddenSelectionsTextures[] = {"\DayZExpansion\Objects\Gear\Spraycans\data\spraycan_yellow_co.paa"};
		skinName = "YellowRust";
	};
	class ExpansionSpraycanYellowBright: ExpansionSpraycanBase
	{
		scope = 2;
		displayName = "$STR_EXPANSION_SPRAYCAN_YellowBright_NAME";
		descriptionShort = "$STR_EXPANSION_SPRAYCAN_YellowBright_DESC";
		hiddenSelectionsTextures[] = {"\DayZExpansion\Objects\Gear\Spraycans\data\spraycan_yellow_co.paa"};
		skinName = "YellowBright";
	};
	class ExpansionSpraycanYellowBrightRust: ExpansionSpraycanBase
	{
		scope = 2;
		displayName = "$STR_EXPANSION_SPRAYCAN_YellowBrightRust_NAME";
		descriptionShort = "$STR_EXPANSION_SPRAYCAN_YellowBrightRust_DESC";
		hiddenSelectionsTextures[] = {"\DayZExpansion\Objects\Gear\Spraycans\data\spraycan_yellow_co.paa"};
		skinName = "YellowBrightRust";
	};
	class ExpansionSpraycanWine: ExpansionSpraycanBase
	{
		scope = 2;
		displayName = "$STR_EXPANSION_SPRAYCAN_Wine_NAME";
		descriptionShort = "$STR_EXPANSION_SPRAYCAN_Wine_DESC";
		hiddenSelectionsTextures[] = {"\DayZExpansion\Objects\Gear\Spraycans\data\spraycan_wine_co.paa"};
		skinName = "Wine";
	};
	class ExpansionSpraycanWineRust: ExpansionSpraycanBase
	{
		scope = 2;
		displayName = "$STR_EXPANSION_SPRAYCAN_WineRust_NAME";
		descriptionShort = "$STR_EXPANSION_SPRAYCAN_WineRust_DESC";
		hiddenSelectionsTextures[] = {"\DayZExpansion\Objects\Gear\Spraycans\data\spraycan_wine_co.paa"};
		skinName = "WineRust";
	};
	class ExpansionSpraycanLime: ExpansionSpraycanBase
	{
		scope = 2;
		displayName = "$STR_EXPANSION_SPRAYCAN_Lime_NAME";
		descriptionShort = "$STR_EXPANSION_SPRAYCAN_Lime_DESC";
		hiddenSelectionsTextures[] = {"DZ\gear\consumables\data\spraycan_green_co.paa"};
		skinName = "Lime";
	};
	class ExpansionSpraycanPolice: ExpansionSpraycanBase
	{
		scope = 2;
		displayName = "$STR_EXPANSION_SPRAYCAN_Police_NAME";
		descriptionShort = "$STR_EXPANSION_SPRAYCAN_Police_DESC";
		hiddenSelectionsTextures[] = {"\DayZExpansion\Objects\Gear\Spraycans\data\spraycan_white_co.paa"};
		skinName = "Police";
	};
	class ExpansionSpraycanBanditKitty: ExpansionSpraycanBase
	{
		scope = 2;
		displayName = "$STR_EXPANSION_SPRAYCAN_BanditKitty_NAME";
		descriptionShort = "$STR_EXPANSION_SPRAYCAN_BanditKitty_DESC";
		hiddenSelectionsTextures[] = {"\DayZExpansion\Objects\Gear\Spraycans\data\spraycan_pink_co.paa"};
		skinName = "BanditKitty";
	};
};
class CfgSurfaces
{
	class DZ_SurfacesInt;
	class DZ_SurfacesExt;
	class expansion_rubber: DZ_SurfacesExt
	{
		Density = 300;
		Thickness = 5;
		deflection = 0.2;
		bulletPenetrability = 600;
		bulletPenetrabilityWithThickness = 200;
		transparency = 0;
		rough = 0.1;
		dust = 0;
		audibility = 0.5;
		soundEnviron = "Empty";
		isLiquid = "false";
		friction = 0.0;
		restitution = 0.0;
		impact = "Hit_Rubber";
		soundHit = "rubber";
	};
};
