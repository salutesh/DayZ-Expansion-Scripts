#define _ARMA_

class CfgPatches
{
	class DayZExpansion_Structures_Walls
	{
		units[] = {};
		weapons[] = {};
		requiredVersion = 0.1;
		requiredAddons[] = {"DZ_Structures_Walls","DayZExpansion_Core_Scripts"};
	};
};
class CfgVehicles
{
	class ExpansionPhysicsStructure;
	class ExpansionPhysics_Static_Power_Pole_Wood1_Lamp: ExpansionPhysicsStructure
	{
		model = "\DZ\structures\industrial\Power\Power_Pole_Wood1_Lamp.p3d";
	};
	class ExpansionPhysics_Static_Power_Pole_Conc4_Lamp: ExpansionPhysicsStructure
	{
		model = "\DZ\structures\industrial\Power\Power_Pole_Conc4_Lamp.p3d";
	};
	class ExpansionPhysics_Static_Power_Pole_Wood1_Lamp_Amp: ExpansionPhysicsStructure
	{
		model = "\DZ\structures\industrial\Power\Power_Pole_Wood1_Lamp_Amp.p3d";
	};
	class ExpansionPhysics_Static_Power_Pole_Conc4_Lamp_Amp: ExpansionPhysicsStructure
	{
		model = "\DZ\structures\industrial\Power\Power_Pole_Conc4_Lamp_Amp.p3d";
	};
	class ExpansionPhysics_Static_Power_Pole_Conc4_Lamp_Trail_B: ExpansionPhysicsStructure
	{
		model = "\DZ\structures\industrial\Power\Power_Pole_Conc4_Lamp_Trail_B.p3d";
	};
	class ExpansionPhysics_Static_Power_Pole_Conc4_Lamp_Trail_G: ExpansionPhysicsStructure
	{
		model = "\DZ\structures\industrial\Power\Power_Pole_Conc4_Lamp_Trail_G.p3d";
	};
	class ExpansionPhysics_Static_Power_Pole_Conc4_Lamp_Trail_R: ExpansionPhysicsStructure
	{
		model = "\DZ\structures\industrial\Power\Power_Pole_Conc4_Lamp_Trail_R.p3d";
	};
	class ExpansionPhysics_Static_Power_Pole_Conc4_Lamp_Trail_Y: ExpansionPhysicsStructure
	{
		model = "\DZ\structures\industrial\Power\Power_Pole_Conc4_Lamp_Trail_Y.p3d";
	};
};
class CfgNonAIVehicles
{
	class ExpansionStaticMapObject;
	class ExpansionLampLightBase;
	class Static_Power_Pole_Wood1_Lamp: ExpansionLampLightBase
	{
		position = "-0.000977 3.65 0.418945";
	};
	class Static_Power_Pole_Conc4_Lamp: ExpansionLampLightBase
	{
		position = "-0.81982 5.0445 -0.01017";
	};
	class Static_Power_Pole_Wood1_Lamp_Amp: ExpansionLampLightBase
	{
		position = "-0.000977 3.65 0.418945";
	};
	class Static_Power_Pole_Conc4_Lamp_Amp: ExpansionLampLightBase
	{
		position = "-0.81982 5.0445 -0.01017";
	};
	class Static_Power_Pole_Conc4_Lamp_Trail_B: ExpansionStaticMapObject{};
	class Static_Power_Pole_Conc4_Lamp_Trail_G: ExpansionStaticMapObject{};
	class Static_Power_Pole_Conc4_Lamp_Trail_R: ExpansionStaticMapObject{};
	class Static_Power_Pole_Conc4_Lamp_Trail_Y: ExpansionStaticMapObject{};
};
