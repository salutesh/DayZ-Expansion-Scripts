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
	class ExpansionPhysics_Static_Wall_Wood1_5: ExpansionPhysicsStructure
	{
		model = "\DZ\structures\walls\wall_wood1_5.p3d";
	};
	class ExpansionPhysics_Static_Wall_Wood1_5_2: ExpansionPhysicsStructure
	{
		model = "\DZ\structures\walls\wall_wood1_5_2.p3d";
	};
	class ExpansionPhysics_Static_Wall_Wood1_5_pole: ExpansionPhysicsStructure
	{
		model = "\DZ\structures\walls\wall_wood1_5_pole.p3d";
	};
	class ExpansionPhysics_Static_Wall_Wood1d_5: ExpansionPhysicsStructure
	{
		model = "\DZ\structures\walls\wall_wood1d_5.p3d";
	};
	class ExpansionPhysics_Static_Wall_Wood1d_5_2: ExpansionPhysicsStructure
	{
		model = "\DZ\structures\walls\wall_wood1d_5_2.p3d";
	};
	class ExpansionPhysics_Static_Wall_Wood1d_5_pole: ExpansionPhysicsStructure
	{
		model = "\DZ\structures\walls\wall_wood1d_5_pole.p3d";
	};
	class ExpansionPhysics_Static_Wall_Wood2_5: ExpansionPhysicsStructure
	{
		model = "\DZ\structures\walls\wall_wood2_5.p3d";
	};
	class ExpansionPhysics_Static_Wall_Wood2_5_d: ExpansionPhysicsStructure
	{
		model = "\DZ\structures\walls\wall_wood2_5_d.p3d";
	};
	class ExpansionPhysics_Static_Wall_Wood2_5_end: ExpansionPhysicsStructure
	{
		model = "\DZ\structures\walls\wall_wood2_5_end.p3d";
	};
	class ExpansionPhysics_Static_Wall_Wood2_5_end_2: ExpansionPhysicsStructure
	{
		model = "\DZ\structures\walls\wall_wood2_5_end_2.p3d";
	};
	class ExpansionPhysics_Static_Wall_Wood2_5_pole: ExpansionPhysicsStructure
	{
		model = "\DZ\structures\walls\wall_wood2_5_pole.p3d";
	};
	class ExpansionPhysics_Static_Wall_Wood3_4: ExpansionPhysicsStructure
	{
		model = "\DZ\structures\walls\wall_wood3_4.p3d";
	};
	class ExpansionPhysics_Static_Wall_Wood3_4_end: ExpansionPhysicsStructure
	{
		model = "\DZ\structures\walls\wall_wood3_4_end.p3d";
	};
	class ExpansionPhysics_Static_Wall_Wood4_4: ExpansionPhysicsStructure
	{
		model = "\DZ\structures\walls\wall_wood4_4.p3d";
	};
	class ExpansionPhysics_Static_Wall_Wood4_end1: ExpansionPhysicsStructure
	{
		model = "\DZ\structures\walls\wall_wood4_end1.p3d";
	};
	class ExpansionPhysics_Static_Wall_Wood4_end2: ExpansionPhysicsStructure
	{
		model = "\DZ\structures\walls\wall_wood4_end2.p3d";
	};
	class ExpansionPhysics_Static_Wall_Wood4_pole: ExpansionPhysicsStructure
	{
		model = "\DZ\structures\walls\wall_wood4_pole.p3d";
	};
	class ExpansionPhysics_Static_Wall_WoodF_5: ExpansionPhysicsStructure
	{
		model = "\DZ\structures\walls\wall_woodf_5.p3d";
	};
	class ExpansionPhysics_Static_Wall_WoodF_5_2: ExpansionPhysicsStructure
	{
		model = "\DZ\structures\walls\wall_woodf_5_2.p3d";
	};
	class ExpansionPhysics_Static_Wall_WoodF_5_end: ExpansionPhysicsStructure
	{
		model = "\DZ\structures\walls\wall_woodf_5_end.p3d";
	};
	class ExpansionPhysics_Static_Wall_WoodVil_4: ExpansionPhysicsStructure
	{
		model = "\DZ\structures\walls\wall_woodvil_4.p3d";
	};
	class ExpansionPhysics_Static_Wall_WoodVil_4_2: ExpansionPhysicsStructure
	{
		model = "\DZ\structures\walls\wall_woodvil_4_2.p3d";
	};
	class ExpansionPhysics_Static_Wall_WoodVil_4_pole: ExpansionPhysicsStructure
	{
		model = "\DZ\structures\walls\wall_woodvil_4_pole.p3d";
	};
};
class CfgNonAIVehicles
{
	class ExpansionStaticMapObject;
	class Static_Wall_Wood1_5: ExpansionStaticMapObject{};
	class Static_Wall_Wood1_5_2: ExpansionStaticMapObject{};
	class Static_Wall_Wood1_5_pole: ExpansionStaticMapObject{};
	class Static_Wall_Wood1d_5: ExpansionStaticMapObject{};
	class Static_Wall_Wood1d_5_2: ExpansionStaticMapObject{};
	class Static_Wall_Wood1d_5_pole: ExpansionStaticMapObject{};
	class Static_Wall_Wood2_5: ExpansionStaticMapObject{};
	class Static_Wall_Wood2_5_d: ExpansionStaticMapObject{};
	class Static_Wall_Wood2_5_end: ExpansionStaticMapObject{};
	class Static_Wall_Wood2_5_end_2: ExpansionStaticMapObject{};
	class Static_Wall_Wood2_5_pole: ExpansionStaticMapObject{};
	class Static_Wall_Wood3_4: ExpansionStaticMapObject{};
	class Static_Wall_Wood3_4_end: ExpansionStaticMapObject{};
	class Static_Wall_Wood4_4: ExpansionStaticMapObject{};
	class Static_Wall_Wood4_end1: ExpansionStaticMapObject{};
	class Static_Wall_Wood4_end2: ExpansionStaticMapObject{};
	class Static_Wall_Wood4_pole: ExpansionStaticMapObject{};
	class Static_Wall_WoodF_5: ExpansionStaticMapObject{};
	class Static_Wall_WoodF_5_2: ExpansionStaticMapObject{};
	class Static_Wall_WoodF_5_end: ExpansionStaticMapObject{};
	class Static_Wall_WoodVil_4: ExpansionStaticMapObject{};
	class Static_Wall_WoodVil_4_2: ExpansionStaticMapObject{};
	class Static_Wall_WoodVil_4_pole: ExpansionStaticMapObject{};
};
