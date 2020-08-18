#define _ARMA_

class CfgPatches
{
	class DayZExpansion_SmokeGrenades
	{
		units[] = {"Expansion_M18SmokeGrenade_Teargas"};
		weapons[] = {};
		requiredVersion = 0.1;
		requiredAddons[] = {"DZ_Weapons_Explosives"};
	};
};
class CfgVehicles
{
	class M18SmokeGrenade_ColorBase;
	class Expansion_M18SmokeGrenade_Teargas: M18SmokeGrenade_ColorBase
	{
		scope = 2;
		hiddenSelectionsTextures[] = {"DayZExpansion\Objects\Weapons\Throwables\Smokes\smokegrenade_teargas_co.paa"};
	};
};
