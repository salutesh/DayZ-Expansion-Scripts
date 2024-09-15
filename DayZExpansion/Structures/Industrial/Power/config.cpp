class CfgPatches
{
	class DayZExpansion_Structures_Industrial_Power
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
	class Static_Power_Pole_Wood1_Lamp: ExpansionLampLightBase
	{
		position="-0.000977 3.65 0.418945";
	};
	class Static_Power_Pole_Conc4_Lamp: ExpansionLampLightBase
	{
		position="-0.81982 5.0445 -0.01017";
	};
	class Static_Power_Pole_Wood1_Lamp_Amp: ExpansionLampLightBase
	{
		position="-0.000977 3.65 0.418945";
	};
	class Static_Power_Pole_Conc4_Lamp_Amp: ExpansionLampLightBase
	{
		position="-0.81982 5.0445 -0.01017";
	};
};
