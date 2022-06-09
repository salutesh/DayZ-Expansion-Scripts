#define _ARMA_

class CfgPatches
{
	class ExpansionProp_AntiAir
	{
		units[] = {"ExpansionProp_2S6M_Tunguska","ExpansionProp_ZSU_Insurgent","ExpansionProp_ZSU_Woodland","ExpansionProp_ZSU_Takistan","ExpansionProp_M2A2_Bradley"};
		weapons[] = {};
		requiredVersion = 0.1;
		requiredAddons[] = {"DZ_Data"};
	};
};
class CfgVehicles
{
	class HouseNoDestruct;
	class ExpansionProp_2S6M_Tunguska: HouseNoDestruct
	{
		scope = 2;
		model = "DayZExpansion\Vehicles\Objects\AntiAir\2S6M_Tunguska\2S6M_Tunguska.p3d";
	};
	class ExpansionProp_ZSU_Insurgent: HouseNoDestruct
	{
		scope = 2;
		model = "DayZExpansion\Vehicles\Objects\AntiAir\ZSU\ZSU.p3d";
		hiddenSelections[] = {"camo1","camo2","camo3"};
		hiddenSelectionsTextures[] = {"DayZExpansion\Vehicles\Objects\AntiAir\ZSU\data\zsu_01_co.paa","DayZExpansion\Vehicles\Objects\AntiAir\ZSU\data\zsu_02_co.paa","DayZExpansion\Vehicles\Objects\AntiAir\ZSU\data\zsu_03_co.paa"};
	};
	class ExpansionProp_ZSU_Woodland: HouseNoDestruct
	{
		scope = 2;
		model = "DayZExpansion\Vehicles\Objects\AntiAir\ZSU\ZSU.p3d";
		hiddenSelections[] = {"camo1","camo2","camo3"};
		hiddenSelectionsTextures[] = {"DayZExpansion\Vehicles\Objects\AntiAir\ZSU\data\zsu_01_camo_co.paa","DayZExpansion\Vehicles\Objects\AntiAir\ZSU\data\zsu_02_camo_co.paa","DayZExpansion\Vehicles\Objects\AntiAir\ZSU\data\zsu_03_camo_co.paa"};
	};
	class ExpansionProp_ZSU_Takistan: HouseNoDestruct
	{
		scope = 2;
		model = "DayZExpansion\Vehicles\Objects\AntiAir\ZSU\ZSU.p3d";
		hiddenSelections[] = {"camo1","camo2","camo3"};
		hiddenSelectionsTextures[] = {"DayZExpansion\Vehicles\Objects\AntiAir\ZSU\data\ZSU_01_TK_CO.paa","DayZExpansion\Vehicles\Objects\AntiAir\ZSU\data\ZSU_02_TK_CO.paa","DayZExpansion\Vehicles\Objects\AntiAir\ZSU\data\ZSU_03_TK_CO.paa"};
	};
	class ExpansionProp_M2A2_Bradley: HouseNoDestruct
	{
		scope = 2;
		model = "DayZExpansion\Vehicles\Objects\AntiAir\M2A2_Bradley\M2A2.p3d";
		hiddenSelections[] = {"camo1","camo2","camo3","selection_stinger"};
		hiddenSelectionsTextures[] = {"DayZExpansion\Vehicles\Objects\AntiAir\M2A2_Bradley\data\base_co.paa","DayZExpansion\Vehicles\Objects\AntiAir\M2A2_Bradley\data\a3_co.paa","DayZExpansion\Vehicles\Objects\AntiAir\M2A2_Bradley\data\ultralp_co.paa","DayZExpansion\Vehicles\Objects\AntiAir\M2A2_Bradley\data\base_co.paa"};
	};
};
