class CfgPatches
{
	class DayZExpansion_AI_Gear
	{
		units[]={};
		weapons[]={};
		requiredVersion=0.1;
		requiredAddons[]=
		{
			"DZ_Characters_Headgear",
			"DZ_Characters_Vests"
		};
	};
};
class CfgVehicles
{
	class GorkaHelmet;
	class PlateCarrierVest;
	class PressVest_ColorBase;
	class DamageSystem;
	class GlobalHealth;
	class Health;
	class Expansion_GorkaHelmet_Yeet: GorkaHelmet
	{
		scope=2;
		visibilityModifier=0.69999999;
		color="Black";
		lootCategory="Crafted";
		hiddenSelectionsTextures[]=
		{
			"\DayZExpansion\AI\Gear\Data\maska_yeet_co.paa",
			"\DayZExpansion\AI\Gear\Data\maska_yeet_co.paa",
			"\DayZExpansion\AI\Gear\Data\maska_yeet_co.paa"
		};
	};
	class Expansion_PlateCarrierVest_Yeet: PlateCarrierVest
	{
		hiddenSelections[]=
		{
			"camoGround",
			"camoMale",
			"camoFemale"
		};
		hiddenSelectionsTextures[]=
		{
			"\DayZExpansion\AI\Gear\Data\BallisticVest_Yeet_co.paa",
			"\DayZExpansion\AI\Gear\Data\BallisticVest_Yeet_co.paa",
			"\DayZExpansion\AI\Gear\Data\BallisticVest_Yeet_co.paa"
		};
	};
	class Expansion_PressVest_Blue_Yeet: PressVest_ColorBase
	{
		scope=2;
		visibilityModifier=0.94999999;
		hiddenSelectionsTextures[]=
		{
			"\DayZExpansion\AI\Gear\Data\PressVest_Blue_Yeet_co.paa",
			"\DayZExpansion\AI\Gear\Data\PressVest_Blue_Yeet_co.paa",
			"\DayZExpansion\AI\Gear\Data\PressVest_Blue_Yeet_co.paa"
		};
		hiddenSelectionsMaterials[]=
		{
			"DayZExpansion\AI\Gear\Data\pressvest_yeet.rvmat",
			"DayZExpansion\AI\Gear\Data\pressvest_yeet.rvmat",
			"DayZExpansion\AI\Gear\Data\pressvest_yeet.rvmat"
		};
		class DamageSystem: DamageSystem
		{
			class GlobalHealth: GlobalHealth
			{
				class Health: Health
				{
					healthLevels[]=
					{
						
						{
							1,
							
							{
								"DayZExpansion\AI\Gear\Data\pressvest_yeet.rvmat"
							}
						},
						
						{
							0.69999999,
							
							{
								"DayZExpansion\AI\Gear\Data\pressvest_yeet.rvmat"
							}
						},
						
						{
							0.5,
							
							{
								"DayZExpansion\AI\Gear\Data\pressvest_yeet_damage.rvmat"
							}
						},
						
						{
							0.30000001,
							
							{
								"DayZExpansion\AI\Gear\Data\pressvest_yeet_damage.rvmat"
							}
						},
						
						{
							0,
							
							{
								"DayZExpansion\AI\Gear\Data\pressvest_yeet_destruct.rvmat"
							}
						}
					};
				};
			};
		};
	};
};
