class CfgPatches
{
	class Expansion_Backpacks
	{
		units[]=
		{
			"ExpansionCoyoteBlack",
			"ExpansionDesertTortilla"
		};
		weapons[]={};
		requiredVersion=0.1;
		requiredAddons[]=
		{
			"DZ_Characters_Backpacks"
		};
	};
};
class CfgVehicles
{
	class Clothing_Base;
	class Clothing;
	class AliceBag_ColorBase;
	class ChildBag_ColorBase;
	class HuntingBag;
	class TaloonBag_ColorBase;
	class TortillaBag;
	class GhillieSuit_ColorBase;
	class ImprovisedBag;
	class AssaultBag_ColorBase;
	class CoyoteBag_ColorBase;
	class ExpansionCoyoteBlack: CoyoteBag_ColorBase
	{
		scope=2;
		visibilityModifier=0.94999999;
		hiddenSelectionsTextures[]=
		{
			"DayZExpansion\Data\characters\backpacks\data\CoyoteBP_black_co.paa",
			"DayZExpansion\Data\characters\backpacks\data\CoyoteBP_black_co.paa",
			"DayZExpansion\Data\characters\backpacks\data\CoyoteBP_black_co.paa"
		};
	};
	class ExpansionDesertTortilla: TortillaBag
	{
		scope=2;
		visibilityModifier=0.94999999;
		hiddenSelections[]=
		{
			"zbytek"
		};
		hiddenSelectionsTextures[]=
		{
			"DayZExpansion\Data\characters\backpacks\data\tortila_desertmarpat_co.paa"
		};
	};
};
