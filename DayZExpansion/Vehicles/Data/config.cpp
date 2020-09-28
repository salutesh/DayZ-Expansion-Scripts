#define _ARMA_

class CfgPatches
{
	class DayZExpansion_Vehicles_Data
	{
		units[] = {"ExpansionCarDoorFix","ExpansionSpraycanBlack","ExpansionSpraycanGrey","ExpansionSpraycanWhite","ExpansionSpraycanGreen","ExpansionSpraycanRed","ExpansionSpraycanBlue","ExpansionSpraycanBanditKitty","ExpansionSpraycanYellow","ExpansionSpraycanWine"};
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
	class ExpansionSpraycanGrey: ExpansionSpraycanBase
	{
		scope = 2;
		displayName = "$STR_EXPANSION_SPRAYCAN_Grey_NAME";
		descriptionShort = "$STR_EXPANSION_SPRAYCAN_Grey_DESC";
		hiddenSelectionsTextures[] = {"\DayZExpansion\Objects\Gear\Consumables\data\spraycan_grey_co.paa"};
		skinName = "Grey";
	};
	class ExpansionSpraycanWhite: ExpansionSpraycanBase
	{
		scope = 2;
		displayName = "$STR_EXPANSION_SPRAYCAN_White_NAME";
		descriptionShort = "$STR_EXPANSION_SPRAYCAN_White_DESC";
		hiddenSelectionsTextures[] = {"\DayZExpansion\Objects\Gear\Consumables\data\spraycan_white_co.paa"};
		skinName = "White";
	};
	class ExpansionSpraycanGreen: ExpansionSpraycanBase
	{
		scope = 2;
		displayName = "$STR_EXPANSION_SPRAYCAN_Green_NAME";
		descriptionShort = "$STR_EXPANSION_SPRAYCAN_Green_DESC";
		hiddenSelectionsTextures[] = {"DZ\gear\consumables\data\spraycan_green_co.paa"};
		skinName = "Green";
	};
	class ExpansionSpraycanRed: ExpansionSpraycanBase
	{
		scope = 2;
		displayName = "$STR_EXPANSION_SPRAYCAN_Red_NAME";
		descriptionShort = "$STR_EXPANSION_SPRAYCAN_Red_DESC";
		hiddenSelectionsTextures[] = {"\DayZExpansion\Objects\Gear\Consumables\data\spraycan_red_co.paa"};
		skinName = "Red";
	};
	class ExpansionSpraycanBlue: ExpansionSpraycanBase
	{
		scope = 2;
		displayName = "$STR_EXPANSION_SPRAYCAN_Blue_NAME";
		descriptionShort = "$STR_EXPANSION_SPRAYCAN_Blue_DESC";
		hiddenSelectionsTextures[] = {"\DayZExpansion\Objects\Gear\Consumables\data\spraycan_blue_co.paa"};
		skinName = "Blue";
	};
	class ExpansionSpraycanBanditKitty: ExpansionSpraycanBase
	{
		scope = 2;
		displayName = "$STR_EXPANSION_SPRAYCAN_BanditKitty_NAME";
		descriptionShort = "$STR_EXPANSION_SPRAYCAN_BanditKitty_DESC";
		hiddenSelectionsTextures[] = {"\DayZExpansion\Objects\Gear\Consumables\data\spraycan_pink_co.paa"};
		skinName = "BanditKitty";
	};
	class ExpansionSpraycanYellow: ExpansionSpraycanBase
	{
		scope = 2;
		displayName = "$STR_EXPANSION_SPRAYCAN_Yellow_NAME";
		descriptionShort = "$STR_EXPANSION_SPRAYCAN_Yellow_DESC";
		hiddenSelectionsTextures[] = {"\DayZExpansion\Objects\Gear\Consumables\data\spraycan_yellow_co.paa"};
		skinName = "Yellow";
	};
	class ExpansionSpraycanWine: ExpansionSpraycanBase
	{
		scope = 2;
		displayName = "$STR_EXPANSION_SPRAYCAN_Wine_NAME";
		descriptionShort = "$STR_EXPANSION_SPRAYCAN_Wine_DESC";
		hiddenSelectionsTextures[] = {"\DayZExpansion\Objects\Gear\Consumables\data\spraycan_wine_co.paa"};
		skinName = "Wine";
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
