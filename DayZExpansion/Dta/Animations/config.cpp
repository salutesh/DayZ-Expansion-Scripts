class CfgPatches
{
	class DayZExpansion_Dta_Animations
	{
		units[]={};
		weapons[]={};
		requiredVersion=0.1;
		requiredAddons[]=
		{
			"DZ_Characters"
		};
	};
};
class CfgVehicles
{
	class Man;
	class SurvivorBase: Man
	{
		class enfAnimSys
		{
			meshObject="dz\characters\bodies\player_testing.xob";
			graphName="DayZExpansion\Animations\Player\player_main.agr";
			defaultInstance="DayZExpansion\Animations\Player\player_main.asi";
			skeletonName="player_testing.xob";
			startNode="MasterControl";
		};
	};
};
