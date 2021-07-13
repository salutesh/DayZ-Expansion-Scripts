#define _ARMA_

class CfgPatches
{
	class DayZExpansion_Weapons_Firearms_Vanilla_Override
	{
		units[] = {};
		weapons[] = {};
		requiredVersion = 0.1;
		requiredAddons[] = {"DZ_Weapons_Firearms"};
	};
};
class CfgWeapons
{
	class SVD_Base;
	class B95_Base;
	class Izh43Shotgun_Base;
	class SVD: SVD_Base
	{
		hiddenSelections[] = {"camo"};
		hiddenSelectionsTextures[] = {"dz\weapons\firearms\svd\data\svd_co.paa"};
		hiddenSelectionsMaterials[] = {"dz\weapons\firearms\svd\data\svd.rvmat"};
	};
	class B95: B95_Base
	{
		hiddenSelections[] = {"zbytek"};
		hiddenSelectionsTextures[] = {"dz\weapons\firearms\b95\data\b95_co.paa"};
		hiddenSelectionsMaterials[] = {"dz\weapons\firearms\b95\data\b95.rvmat"};
	};
	class Izh43Shotgun: Izh43Shotgun_Base
	{
		hiddenSelections[] = {"zbytek"};
		hiddenSelectionsTextures[] = {"dz\weapons\shotguns\izh43\data\izh43_co.paa"};
		hiddenSelectionsMaterials[] = {"dz\weapons\shotguns\izh43\data\izh43.rvmat"};
	};
};
