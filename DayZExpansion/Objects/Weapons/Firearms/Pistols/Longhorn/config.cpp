#define _ARMA_

class CfgPatches
{
	class DayZExpansion_Longhorn
	{
		units[] = {};
		weapons[] = {"Expansion_Longhorn"};
		requiredVersion = 0.1;
		requiredAddons[] = {"DZ_Pistols"};
	};
};
class cfgWeapons
{
	class LongHorn;
	class Expansion_Longhorn: LongHorn
	{
		scope = 2;
		displayName = "$STR_cfgWeapons_LongHorn0";
		descriptionShort = "The LongHorn is part of the official weaponry of vanilla DayZ 1.17. This item still exists so nobody loses their gear. Please do not spawn this item anymore. This item will be removed from the mod in a few months.";
	};
};
