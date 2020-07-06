#define _ARMA_

class CfgPatches
{
	class Expansion_Armbands
	{
		units[] = {"ExpansionArmbandBandit","ExpansionArmbandHero"};
		weapons[] = {};
		requiredVersion = 0.1;
		requiredAddons[] = {"DZ_Characters_Tops"};
	};
};
class CfgVehicles
{
	class Armband_ColorBase;
	class ExpansionArmbandBandit: Armband_ColorBase
	{
		scope = 2;
		color = "Black";
		hiddenSelectionsTextures[] = {"\dz\characters\tops\data\armbend_blue_g_co.paa","\dz\characters\tops\data\armbend_small_blue_co.paa","\dz\characters\tops\data\armbend_small_blue_co.paa","\DayZExpansion\Data\Characters\Armbands\Data\armband_Bandit_co.paa","\DayZExpansion\Data\Characters\Armbands\Data\armband_Bandit_co.paa"};
	};
	class ExpansionArmbandHero: Armband_ColorBase
	{
		scope = 2;
		color = "White";
		hiddenSelectionsTextures[] = {"\dz\characters\tops\data\armbend_white_g_co.paa","\dz\characters\tops\data\armbend_small_white_co.paa","\dz\characters\tops\data\armbend_small_white_co.paa","\DayZExpansion\Data\Characters\Armbands\Data\armband_Hero_co.paa","\DayZExpansion\Data\Characters\Armbands\Data\armband_Hero_co.paa"};
	};
};
