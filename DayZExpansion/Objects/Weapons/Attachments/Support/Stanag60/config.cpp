#define _ARMA_

class CfgPatches
{
	class DayZExpansion_Stanag60RND
	{
		units[] = {};
		weapons[] = {};
		requiredVersion = 0.1;
		requiredAddons[] = {"DZ_Weapons_Magazines"};
		magazines[] = {};
	};
};
class CfgMagazines
{
	class Magazine_Base;
	class Mag_STANAGCoupled_30Rnd: Magazine_Base
	{
		displayName = "$STR_EXPANSION_STANAG60";
		descriptionShort = "$STR_EXPANSION_STANAG60_DESC";
		model = "\DayZExpansion\Objects\Weapons\Attachments\Support\Stanag60\betterstanag60.p3d";
	};
};
