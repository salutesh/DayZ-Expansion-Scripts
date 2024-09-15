class CfgPatches
{
	class DayZExpansion_NamalskAdventure_Weapons_Archery
	{
		units[]={};
		weapons[]={};
		requiredVersion=0.1;
		requiredAddons[]=
		{
			"DZ_Data",
			"DZ_Weapons_Archery_Crossbow",
			"DayZExpansion_NamalskAdventure_Items"
		};
	};
};
class cfgWeapons
{
	class Archery_Base;
	class Crossbow_Base: Archery_Base
	{
		chamberableFrom[]+=
		{
			"Expansion_Ammo_BoltAnomaly_Ice",
			"Expansion_Ammo_BoltAnomaly_Warper"
		};
	};
};
