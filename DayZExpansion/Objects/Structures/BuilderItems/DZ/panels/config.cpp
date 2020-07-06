#define _ARMA_

class CfgPatches
{
	class BuilderItems_Panels
	{
		units[] = {"bldr_panel_concrete_1","bldr_panel_concrete_2","bldr_panel_concrete_dam","bldr_panels_concrete","bldr_path_panels","bldr_path_panels_damaged","bldr_concrete_panel_2_grass","bldr_concrete_panel_dam_grass","bldr_concrete_panel_grass","bldr_concrete_panels_main_grass"};
		weapons[] = {};
		requiredVersion = 0.1;
		requiredAddons[] = {"DZ_Data","DZ_Structures_Roads"};
	};
};
class CfgVehicles
{
	class HouseNoDestruct;
	class bldr_panel_concrete_1: HouseNoDestruct
	{
		scope = 2;
		model = "\DZ\structures\roads\panels\panel_concrete_1.p3d";
	};
	class bldr_panel_concrete_2: HouseNoDestruct
	{
		scope = 2;
		model = "\DZ\structures\roads\panels\panel_concrete_2.p3d";
	};
	class bldr_panel_concrete_dam: HouseNoDestruct
	{
		scope = 2;
		model = "\DZ\structures\roads\panels\panel_concrete_dam.p3d";
	};
	class bldr_panels_concrete: HouseNoDestruct
	{
		scope = 2;
		model = "\DZ\structures\roads\panels\panels_concrete.p3d";
	};
	class bldr_path_panels: HouseNoDestruct
	{
		scope = 2;
		model = "\DZ\structures\roads\panels\path_panels.p3d";
	};
	class bldr_path_panels_damaged: HouseNoDestruct
	{
		scope = 2;
		model = "\DZ\structures\roads\panels\path_panels_damaged.p3d";
	};
	class bldr_concrete_panel_2_grass: HouseNoDestruct
	{
		scope = 2;
		model = "\DZ\structures\roads\panels\proxy\concrete_panel_2_grass.p3d";
	};
	class bldr_concrete_panel_dam_grass: HouseNoDestruct
	{
		scope = 2;
		model = "\DZ\structures\roads\panels\proxy\concrete_panel_dam_grass.p3d";
	};
	class bldr_concrete_panel_grass: HouseNoDestruct
	{
		scope = 2;
		model = "\DZ\structures\roads\panels\proxy\concrete_panel_grass.p3d";
	};
	class bldr_concrete_panels_main_grass: HouseNoDestruct
	{
		scope = 2;
		model = "\DZ\structures\roads\panels\proxy\concrete_panels_main_grass.p3d";
	};
};
