#define _ARMA_

class CfgPatches
{
	class BuilderItems_Military
	{
		units[] = {"bldr_mil_blastcover1","bldr_mil_blastcover1_grass","bldr_mil_blastcover2","bldr_mil_blastcover2_grass","bldr_mil_blastcover3","bldr_mil_blastcover3_grass","bldr_mil_blastcover4","bldr_mil_blastcover4_grass_l","bldr_mil_blastcover4_grass_r","bldr_mil_reinforcedtank1","bldr_mil_reinforcedtank1_grass","bldr_mil_reinforcedtank2","bldr_mil_reinforcedtank2_grass_l","bldr_mil_reinforcedtank2_grass_r","bldr_mil_reinforcedtank2_grass_top","bldr_fusebox","bldr_high_outlet","bldr_high_outlet_c","bldr_light_cone","bldr_light_quad","bldr_mainframe","bldr_mil_radar_mobile_1_wires","bldr_mil_radar_mobile_2_wires","bldr_mil_radar_mobile_base","bldr_mil_radar_mobile_generator","bldr_mil_radar_mobile_head","bldr_mil_radar_tall_connections","bldr_mil_radar_tall_fuseboxs","bldr_mil_radar_tall_generator","bldr_mil_radar_tall_lightbulb","bldr_mil_radar_tall_lightcover","bldr_mil_radar_tall_lighttop","bldr_mil_radar_tall_mainframepart1","bldr_mil_radar_tall_mainframepart2","bldr_mil_radar_tall_mainframepart3","bldr_mil_radar_tall_planks","bldr_mil_radar_tall_radardish","bldr_mil_radar_tall_sheets","bldr_mil_radar_tall_stairs","bldr_mil_radar_tall_wires","bldr_mil_reinforcedtank2_shelter1","bldr_mil_reinforcedtank2_shelter2","bldr_mil_servicehangar_detail","bldr_mil_servicehangar_detail_r","bldr_outlet_1","bldr_outlet_2","bldr_proxy_atcbig_details1","bldr_proxy_atcdoors","bldr_proxy_cables","bldr_switch_lights","bldr_wires","bldr_tsygarbage","bldr_tsypipes","bldr_tsypipes_2","bldr_tsyradar_extra_parts","bldr_tsyradar_extra_parts_02","bldr_tsyradarb_base_parts_01","bldr_tsyradarplatform_20m_railing","bldr_tsyrailings","bldr_tsytisy_barracks_battary","bldr_tsytisy_barracks_battary2","bldr_tsytisy_barracks_doors","bldr_tsytisy_barracks_lamps","bldr_tsytisy_barracks_stairs","bldr_tsytisy_barracks_windows","bldr_tsytisy_barracks_windows2","bldr_tsywindows"};
		weapons[] = {};
		requiredVersion = 0.1;
		requiredAddons[] = {"DZ_Data","DZ_Structures_Military"};
	};
};
class CfgVehicles
{
	class HouseNoDestruct;
	class bldr_mil_blastcover1: HouseNoDestruct
	{
		scope = 2;
		model = "\DZ\structures\military\airfield\mil_blastcover1.p3d";
	};
	class bldr_mil_blastcover1_grass: HouseNoDestruct
	{
		scope = 2;
		model = "\DZ\structures\military\airfield\mil_blastcover1_grass.p3d";
	};
	class bldr_mil_blastcover2: HouseNoDestruct
	{
		scope = 2;
		model = "\DZ\structures\military\airfield\mil_blastcover2.p3d";
	};
	class bldr_mil_blastcover2_grass: HouseNoDestruct
	{
		scope = 2;
		model = "\DZ\structures\military\airfield\mil_blastcover2_grass.p3d";
	};
	class bldr_mil_blastcover3: HouseNoDestruct
	{
		scope = 2;
		model = "\DZ\structures\military\airfield\mil_blastcover3.p3d";
	};
	class bldr_mil_blastcover3_grass: HouseNoDestruct
	{
		scope = 2;
		model = "\DZ\structures\military\airfield\mil_blastcover3_grass.p3d";
	};
	class bldr_mil_blastcover4: HouseNoDestruct
	{
		scope = 2;
		model = "\DZ\structures\military\airfield\mil_blastcover4.p3d";
	};
	class bldr_mil_blastcover4_grass_l: HouseNoDestruct
	{
		scope = 2;
		model = "\DZ\structures\military\airfield\mil_blastcover4_grass_l.p3d";
	};
	class bldr_mil_blastcover4_grass_r: HouseNoDestruct
	{
		scope = 2;
		model = "\DZ\structures\military\airfield\mil_blastcover4_grass_r.p3d";
	};
	class bldr_mil_reinforcedtank1: HouseNoDestruct
	{
		scope = 2;
		model = "\DZ\structures\military\airfield\mil_reinforcedtank1.p3d";
	};
	class bldr_mil_reinforcedtank1_grass: HouseNoDestruct
	{
		scope = 2;
		model = "\DZ\structures\military\airfield\mil_reinforcedtank1_grass.p3d";
	};
	class bldr_mil_reinforcedtank2: HouseNoDestruct
	{
		scope = 2;
		model = "\DZ\structures\military\airfield\mil_reinforcedtank2.p3d";
	};
	class bldr_mil_reinforcedtank2_grass_l: HouseNoDestruct
	{
		scope = 2;
		model = "\DZ\structures\military\airfield\mil_reinforcedtank2_grass_l.p3d";
	};
	class bldr_mil_reinforcedtank2_grass_r: HouseNoDestruct
	{
		scope = 2;
		model = "\DZ\structures\military\airfield\mil_reinforcedtank2_grass_r.p3d";
	};
	class bldr_mil_reinforcedtank2_grass_top: HouseNoDestruct
	{
		scope = 2;
		model = "\DZ\structures\military\airfield\mil_reinforcedtank2_grass_top.p3d";
	};
	class bldr_fusebox: HouseNoDestruct
	{
		scope = 2;
		model = "\DZ\structures\military\airfield\proxy\fusebox.p3d";
	};
	class bldr_high_outlet: HouseNoDestruct
	{
		scope = 2;
		model = "\DZ\structures\military\airfield\proxy\high_outlet.p3d";
	};
	class bldr_high_outlet_c: HouseNoDestruct
	{
		scope = 2;
		model = "\DZ\structures\military\airfield\proxy\high_outlet_c.p3d";
	};
	class bldr_light_cone: HouseNoDestruct
	{
		scope = 2;
		model = "\DZ\structures\military\airfield\proxy\light_cone.p3d";
	};
	class bldr_light_quad: HouseNoDestruct
	{
		scope = 2;
		model = "\DZ\structures\military\airfield\proxy\light_quad.p3d";
	};
	class bldr_mainframe: HouseNoDestruct
	{
		scope = 2;
		model = "\DZ\structures\military\airfield\proxy\mainframe.p3d";
	};
	class bldr_mil_radar_mobile_1_wires: HouseNoDestruct
	{
		scope = 2;
		model = "\DZ\structures\military\airfield\proxy\mil_radar_mobile_1_wires.p3d";
	};
	class bldr_mil_radar_mobile_2_wires: HouseNoDestruct
	{
		scope = 2;
		model = "\DZ\structures\military\airfield\proxy\mil_radar_mobile_2_wires.p3d";
	};
	class bldr_mil_radar_mobile_base: HouseNoDestruct
	{
		scope = 2;
		model = "\DZ\structures\military\airfield\proxy\mil_radar_mobile_base.p3d";
	};
	class bldr_mil_radar_mobile_generator: HouseNoDestruct
	{
		scope = 2;
		model = "\DZ\structures\military\airfield\proxy\mil_radar_mobile_generator.p3d";
	};
	class bldr_mil_radar_mobile_head: HouseNoDestruct
	{
		scope = 2;
		model = "\DZ\structures\military\airfield\proxy\mil_radar_mobile_head.p3d";
	};
	class bldr_mil_radar_tall_connections: HouseNoDestruct
	{
		scope = 2;
		model = "\DZ\structures\military\airfield\proxy\mil_radar_tall_connections.p3d";
	};
	class bldr_mil_radar_tall_fuseboxs: HouseNoDestruct
	{
		scope = 2;
		model = "\DZ\structures\military\airfield\proxy\mil_radar_tall_fuseboxs.p3d";
	};
	class bldr_mil_radar_tall_generator: HouseNoDestruct
	{
		scope = 2;
		model = "\DZ\structures\military\airfield\proxy\mil_radar_tall_generator.p3d";
	};
	class bldr_mil_radar_tall_lightbulb: HouseNoDestruct
	{
		scope = 2;
		model = "\DZ\structures\military\airfield\proxy\mil_radar_tall_lightbulb.p3d";
	};
	class bldr_mil_radar_tall_lightcover: HouseNoDestruct
	{
		scope = 2;
		model = "\DZ\structures\military\airfield\proxy\mil_radar_tall_lightcover.p3d";
	};
	class bldr_mil_radar_tall_lighttop: HouseNoDestruct
	{
		scope = 2;
		model = "\DZ\structures\military\airfield\proxy\mil_radar_tall_lighttop.p3d";
	};
	class bldr_mil_radar_tall_mainframepart1: HouseNoDestruct
	{
		scope = 2;
		model = "\DZ\structures\military\airfield\proxy\mil_radar_tall_mainframepart1.p3d";
	};
	class bldr_mil_radar_tall_mainframepart2: HouseNoDestruct
	{
		scope = 2;
		model = "\DZ\structures\military\airfield\proxy\mil_radar_tall_mainframepart2.p3d";
	};
	class bldr_mil_radar_tall_mainframepart3: HouseNoDestruct
	{
		scope = 2;
		model = "\DZ\structures\military\airfield\proxy\mil_radar_tall_mainframepart3.p3d";
	};
	class bldr_mil_radar_tall_planks: HouseNoDestruct
	{
		scope = 2;
		model = "\DZ\structures\military\airfield\proxy\mil_radar_tall_planks.p3d";
	};
	class bldr_mil_radar_tall_radardish: HouseNoDestruct
	{
		scope = 2;
		model = "\DZ\structures\military\airfield\proxy\mil_radar_tall_radardish.p3d";
	};
	class bldr_mil_radar_tall_sheets: HouseNoDestruct
	{
		scope = 2;
		model = "\DZ\structures\military\airfield\proxy\mil_radar_tall_sheets.p3d";
	};
	class bldr_mil_radar_tall_stairs: HouseNoDestruct
	{
		scope = 2;
		model = "\DZ\structures\military\airfield\proxy\mil_radar_tall_stairs.p3d";
	};
	class bldr_mil_radar_tall_wires: HouseNoDestruct
	{
		scope = 2;
		model = "\DZ\structures\military\airfield\proxy\mil_radar_tall_wires.p3d";
	};
	class bldr_mil_reinforcedtank2_shelter1: HouseNoDestruct
	{
		scope = 2;
		model = "\DZ\structures\military\airfield\proxy\mil_reinforcedtank2_shelter1.p3d";
	};
	class bldr_mil_reinforcedtank2_shelter2: HouseNoDestruct
	{
		scope = 2;
		model = "\DZ\structures\military\airfield\proxy\mil_reinforcedtank2_shelter2.p3d";
	};
	class bldr_mil_servicehangar_detail: HouseNoDestruct
	{
		scope = 2;
		model = "\DZ\structures\military\airfield\proxy\mil_servicehangar_detail.p3d";
	};
	class bldr_mil_servicehangar_detail_r: HouseNoDestruct
	{
		scope = 2;
		model = "\DZ\structures\military\airfield\proxy\mil_servicehangar_detail_r.p3d";
	};
	class bldr_outlet_1: HouseNoDestruct
	{
		scope = 2;
		model = "\DZ\structures\military\airfield\proxy\outlet_1.p3d";
		physLayer = "item_small";
		carveNavmesh = 0;
	};
	class bldr_outlet_2: HouseNoDestruct
	{
		scope = 2;
		model = "\DZ\structures\military\airfield\proxy\outlet_2.p3d";
		physLayer = "item_small";
		carveNavmesh = 0;
	};
	class bldr_proxy_atcbig_details1: HouseNoDestruct
	{
		scope = 2;
		model = "\DZ\structures\military\airfield\proxy\proxy_atcbig_details1.p3d";
	};
	class bldr_proxy_atcdoors: HouseNoDestruct
	{
		scope = 2;
		model = "\DZ\structures\military\airfield\proxy\proxy_atcdoors.p3d";
	};
	class bldr_proxy_cables: HouseNoDestruct
	{
		scope = 2;
		model = "\DZ\structures\military\airfield\proxy\proxy_cables.p3d";
	};
	class bldr_switch_lights: HouseNoDestruct
	{
		scope = 2;
		model = "\DZ\structures\military\airfield\proxy\switch_lights.p3d";
		physLayer = "item_small";
		carveNavmesh = 0;
	};
	class bldr_wires: HouseNoDestruct
	{
		scope = 2;
		model = "\DZ\structures\military\airfield\proxy\wires.p3d";
	};
	class bldr_tsygarbage: HouseNoDestruct
	{
		scope = 2;
		model = "\DZ\structures\military\tisy\proxy\garbage.p3d";
	};
	class bldr_tsypipes: HouseNoDestruct
	{
		scope = 2;
		model = "\DZ\structures\military\tisy\proxy\pipes.p3d";
	};
	class bldr_tsypipes_2: HouseNoDestruct
	{
		scope = 2;
		model = "\DZ\structures\military\tisy\proxy\pipes_2.p3d";
	};
	class bldr_tsyradar_extra_parts: HouseNoDestruct
	{
		scope = 2;
		model = "\DZ\structures\military\tisy\proxy\radar_extra_parts.p3d";
	};
	class bldr_tsyradar_extra_parts_02: HouseNoDestruct
	{
		scope = 2;
		model = "\DZ\structures\military\tisy\proxy\radar_extra_parts_02.p3d";
	};
	class bldr_tsyradarb_base_parts_01: HouseNoDestruct
	{
		scope = 2;
		model = "\DZ\structures\military\tisy\proxy\radarb_base_parts_01.p3d";
	};
	class bldr_tsyradarplatform_20m_railing: HouseNoDestruct
	{
		scope = 2;
		model = "\DZ\structures\military\tisy\proxy\radarplatform_20m_railing.p3d";
	};
	class bldr_tsyrailings: HouseNoDestruct
	{
		scope = 2;
		model = "\DZ\structures\military\tisy\proxy\railings.p3d";
	};
	class bldr_tsytisy_barracks_battary: HouseNoDestruct
	{
		scope = 2;
		model = "\DZ\structures\military\tisy\proxy\tisy_barracks_battary.p3d";
	};
	class bldr_tsytisy_barracks_battary2: HouseNoDestruct
	{
		scope = 2;
		model = "\DZ\structures\military\tisy\proxy\tisy_barracks_battary2.p3d";
	};
	class bldr_tsytisy_barracks_doors: HouseNoDestruct
	{
		scope = 2;
		model = "\DZ\structures\military\tisy\proxy\tisy_barracks_doors.p3d";
	};
	class bldr_tsytisy_barracks_lamps: HouseNoDestruct
	{
		scope = 2;
		model = "\DZ\structures\military\tisy\proxy\tisy_barracks_lamps.p3d";
	};
	class bldr_tsytisy_barracks_stairs: HouseNoDestruct
	{
		scope = 2;
		model = "\DZ\structures\military\tisy\proxy\tisy_barracks_stairs.p3d";
	};
	class bldr_tsytisy_barracks_windows: HouseNoDestruct
	{
		scope = 2;
		model = "\DZ\structures\military\tisy\proxy\tisy_barracks_windows.p3d";
	};
	class bldr_tsytisy_barracks_windows2: HouseNoDestruct
	{
		scope = 2;
		model = "\DZ\structures\military\tisy\proxy\tisy_barracks_windows2.p3d";
	};
	class bldr_tsywindows: HouseNoDestruct
	{
		scope = 2;
		model = "\DZ\structures\military\tisy\proxy\windows.p3d";
	};
};
