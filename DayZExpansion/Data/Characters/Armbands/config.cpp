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
	class ExpansionHardlineArmband: Armband_ColorBase
	{
		scope = 1;
	};
	class ExpansionArmbandBandit: ExpansionHardlineArmband
	{
		scope = 2;
		visibilityModifier = 0.95;
		color = "Pirates";
		hiddenSelectionsTextures[] = {"\dz\gear\camping\data\Flag_jolly_armband_co.paa","\dz\gear\camping\data\Flag_jolly_armband_co.paa","\dz\gear\camping\data\Flag_jolly_armband_co.paa","\dz\gear\camping\data\Flag_jolly_armband_co.paa","\dz\gear\camping\data\Flag_jolly_armband_co.paa","\dz\gear\camping\data\Flag_jolly_armband_co.paa","\dz\gear\camping\data\Flag_jolly_armband_co.paa","\dz\gear\camping\data\Flag_jolly_armband_co.paa","\dz\gear\camping\data\Flag_jolly_armband_co.paa"};
	};
	class ExpansionArmbandHero: ExpansionHardlineArmband
	{
		scope = 2;
		visibilityModifier = 0.95;
		color = "Wolf";
		hiddenSelectionsTextures[] = {"\dz\gear\camping\data\Flag_wolf_co.paa","\dz\gear\camping\data\Flag_wolf_co.paa","\dz\gear\camping\data\Flag_wolf_co.paa","\dz\gear\camping\data\Flag_wolf_co.paa","\dz\gear\camping\data\Flag_wolf_co.paa","\dz\gear\camping\data\Flag_wolf_co.paa","\dz\gear\camping\data\Flag_wolf_co.paa","\dz\gear\camping\data\Flag_wolf_co.paa","\dz\gear\camping\data\Flag_wolf_co.paa"};
	};
};
