#define _ARMA_

class CfgPatches
{
	class Expansion_Gloves
	{
		units[] = {"ExpansionTacticalGlovesDesert"};
		weapons[] = {};
		requiredVersion = 0.1;
		requiredAddons[] = {"DZ_Characters_Gloves"};
	};
};
class CfgVehicles
{
	class TacticalGloves_ColorBase;
	class ExpansionTacticalGlovesDesert: TacticalGloves_ColorBase
	{
		scope = 2;
		hiddenSelectionsTextures[] = {"DayZExpansion\data\characters\gloves\data\tacticalgloves_desertmarpat_co.paa","DayZExpansion\data\characters\gloves\data\tacticalgloves_desertmarpat_co.paa","DayZExpansion\data\characters\gloves\data\tacticalgloves_desertmarpat_co.paa"};
	};
};
