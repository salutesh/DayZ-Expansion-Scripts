class CfgPatches
{
	class DayZExpansion_Objects_Firearms_Flaregun
	{
		units[]={};
		weapons[]={};
		requiredVersion=0.1;
		requiredAddons[]=
		{
			"DZ_Pistols_Flaregun"
		};
	};
};
class CfgWeapons
{
	class Flaregun;
	class ExpansionFlaregun: Flaregun
	{
		scope=0;
		displayName="$STR_cfgWeapons_Flaregun0";
		descriptionShort="The Flaregun is now part of the official weaponary of vanilla dayz. This item still exist so nobody lose their gear. Please do not spawn this item anymore. This item will be removed in a few months.";
	};
};
