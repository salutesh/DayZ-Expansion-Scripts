class CfgPatches
{
	class DayZExpansion_Objects_Structures_Signs
	{
		units[]=
		{
			"Expansion_Decal_WelcomeToHell"
		};
		weapons[]={};
		requiredVersion=0.1;
		requiredAddons[]=
		{
			"DZ_Data"
		};
	};
};
class CfgVehicles
{
	class HouseNoDestruct;
	class Expansion_Decal_WelcomeToHell: HouseNoDestruct
	{
		scope=2;
		model="\DayZExpansion\Objects\Structures\Signs\Decal_WelcomeToHell.p3d";
		physLayer="item_small";
		carveNavmesh=0;
	};
};
