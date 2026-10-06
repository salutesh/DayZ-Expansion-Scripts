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
	class enfAnimSys;
	class SurvivorBase: Man
	{
		class enfAnimSys: enfAnimSys
		{
			graphName="DayZExpansion\Animations\Player\player_main.agr";
			defaultInstance="DayZExpansion\Animations\Player\player_main.asi";
		};
	};
};
