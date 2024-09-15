class CfgPatches
{
	class DayZExpansion_Structures_Industrial_Misc
	{
		units[]={};
		weapons[]={};
		requiredVersion=0.1;
		requiredAddons[]=
		{
			"DZ_Structures_Walls",
			"DayZExpansion_Core_Scripts"
		};
	};
};
class CfgNonAIVehicles
{
	class ExpansionLampLightBase;
	class Static_Lamp_Ind: ExpansionLampLightBase
	{
	};
	class Static_Lamp_Rail: ExpansionLampLightBase
	{
	};
};
