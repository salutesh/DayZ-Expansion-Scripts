#define _ARMA_

class CfgPatches
{
	class DayZExpansion_AI_Gear
	{
		units[] = {};
		weapons[] = {};
		requiredVersion = 0.1;
		requiredAddons[] = {"DZ_Characters_Headgear","DZ_Characters_Vests"};
	};
};
class CfgVehicles
{
	class GorkaHelmet;
	class PlateCarrierVest;
	class PressVest_ColorBase;
	class Expansion_GorkaHelmet_Yeet: GorkaHelmet
	{
		scope = 2;
		visibilityModifier = 0.7;
		color = "Black";
		lootCategory = "Crafted";
		hiddenSelectionsTextures[] = {"\DayZExpansion\AI\Gear\Data\maska_yeet_co.paa","\DayZExpansion\AI\Gear\Data\maska_yeet_co.paa","\DayZExpansion\AI\Gear\Data\maska_yeet_co.paa"};
	};
	class Expansion_PlateCarrierVest_Yeet: PlateCarrierVest
	{
		hiddenSelections[] = {"camoGround","camoMale","camoFemale"};
		hiddenSelectionsTextures[] = {"\DayZExpansion\AI\Gear\Data\BallisticVest_Yeet_co.paa","\DayZExpansion\AI\Gear\Data\BallisticVest_Yeet_co.paa","\DayZExpansion\AI\Gear\Data\BallisticVest_Yeet_co.paa"};
	};
	class Expansion_PressVest_Blue_Yeet: PressVest_ColorBase
	{
		scope = 2;
		visibilityModifier = 0.95;
		hiddenSelectionsTextures[] = {"\DayZExpansion\AI\Gear\Data\PressVest_Blue_Yeet_co.paa","\DayZExpansion\AI\Gear\Data\PressVest_Blue_Yeet_co.paa","\DayZExpansion\AI\Gear\Data\PressVest_Blue_Yeet_co.paa"};
	};
};
