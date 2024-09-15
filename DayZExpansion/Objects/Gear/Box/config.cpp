class CfgPatches
{
	class DayZExpansion_Objects_Gear_Box
	{
		units[]=
		{
			"ExpansionHeroSet",
			"ExpansionBanditSet"
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
	class Inventory_Base;
	class ExpansionBoxSetBase: Inventory_Base
	{
		scope=0;
		weight=50;
		displayName="You Should Not seeing this";
		descriptionShort="You Should Not seeing this";
		model="\DayZExpansion\Objects\Gear\Box\box.p3d";
		itemSize[]={5,5};
	};
	class ExpansionHeroSet: ExpansionBoxSetBase
	{
		scope=2;
		displayName="$STR_EXPANSION_HERO_SET";
		descriptionShort="$STR_EXPANSION_HERO_SET_DESC";
	};
	class ExpansionBanditSet: ExpansionBoxSetBase
	{
		scope=2;
		displayName="$STR_EXPANSION_BANDIT_SET";
		descriptionShort="$STR_EXPANSION_BANDIT_SET_DESC";
	};
};
