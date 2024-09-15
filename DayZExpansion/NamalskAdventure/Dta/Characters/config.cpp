class CfgPatches
{
	class DayZExpansion_NamalskAdventure_Characters
	{
		units[]=
		{
			"SurvivorMale",
			"SurvivorFemale",
			"SurvivorParts",
			"SurvivorPartsFemale",
			"SurvivorPartsMale",
			"zombieCoverallsBeige",
			"zombieCoverallsBlue",
			"zombieCoverallsGreen",
			"zombieCoverallsGrey",
			"zombieCoverallsWhite",
			"zombieJacketBeige",
			"zombieJacketBlack",
			"zombieJacketBlue",
			"zombieJacketBluechecks",
			"zombieJacketBrown",
			"zombieJacketGreenchecks",
			"zombieJacketGrey",
			"zombieJacketKhaki",
			"zombieJacketMagenta",
			"zombieJacketStripes",
			"zombieShortskirtBeige",
			"zombieShortskirtBlack",
			"zombieShortskirtBrown",
			"zombieShortskirtChecks",
			"zombieShortskirtGreen",
			"zombieShortskirtGrey",
			"zombieShortskirtRed",
			"zombieShortskirtStripes",
			"zombieShortskirtWhite",
			"zombieShortskirtYellow",
			"zombieJacketBeigeNew",
			"zombieJacketBlackNew",
			"zombieJacketBlueNew",
			"Hermit_NewAI"
		};
		weapons[]={};
		requiredVersion=0.1;
		requiredAddons[]=
		{
			"DZ_Data",
			"DZ_Characters",
			"Windstrideclothing",
			"SkullMask"
		};
	};
};
class CfgVehicles
{
	class Man;
	class DayZInfected;
	class SurvivorBase: Man
	{
		class Skinning
		{
			class HumanSkull
			{
				item="HumanSkull";
				count=1;
				quantityMinMaxCoef[]={0,0};
			};
		};
	};
	class ZombieBase: DayZInfected
	{
		class Skinning
		{
			class Guts
			{
				item="Guts";
				count=1;
				quantityMinMaxCoef[]={0.5,0.80000001};
			};
			class Lard
			{
				item="Lard";
				count=1;
				quantityMinMaxCoef[]={0.5,1};
			};
			class Bone
			{
				item="Bone";
				count=1;
				quantityMinMaxCoef[]={0.34999999,0.75};
			};
			class HumanSkull
			{
				item="HumanSkull";
				count=1;
				quantityMinMaxCoef[]={0,0};
			};
			class InfectedSteakMeat
			{
				item="InfectedSteakMeat";
				count=1;
				quantityMinMaxCoef[]={0.5,1};
			};
		};
	};
};
