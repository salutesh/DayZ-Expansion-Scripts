#define _ARMA_

class CfgPatches
{
	class BuilderItems_Industrial
	{
		units[] = {"bldr_cementworks_conveyor_up1","bldr_cementworks_conveyor_up2","bldr_cementworks_conveyor_up3","bldr_cementworks_conveyorfurnace","bldr_cementworks_conveyormilla","bldr_cementworks_conveyormillb","bldr_cementworks_conveyortowera","bldr_cementworks_conveyortowerb","bldr_cementworks_conveyortowerc","bldr_cementworks_silobig1bridge","bldr_coalplant_conveyor","bldr_container_1a","bldr_container_1b","bldr_container_1c","bldr_container_1d","bldr_container_2a","bldr_container_2b","bldr_container_2c","bldr_container_2d","bldr_container_2e","bldr_farm_hopper","bldr_farm_strawstack","bldr_barn_metal_lod1_proxy","bldr_barn_roofconstr_proxy","bldr_barnboxy_a","bldr_barnboxy_b","bldr_barnboxy_c","bldr_barnboxy_d","bldr_barnboxy_e","bldr_barnokna_a","bldr_barnokna_b","bldr_barntram_a","bldr_barntram_b","bldr_pier_concrete1","bldr_pier_concrete1_cornerin","bldr_pier_concrete1_cornerout","bldr_pier_concrete1_end","bldr_pier_concrete1_l","bldr_pier_concrete1_l10","bldr_pier_concrete1_l30","bldr_pier_concrete1_r","bldr_pier_concrete1_r10","bldr_pier_concrete1_r30","bldr_pier_concrete1_tbig","bldr_pier_concrete1_tsmall","bldr_pier_concrete2","bldr_pier_concrete2_end","bldr_pier_concrete3_1","bldr_pier_concrete3_2","bldr_pier_concrete3_end","bldr_pier_concrete3_fuel","bldr_pier_crane_rails","bldr_pier_crane_rails_end","bldr_pier_tube_big","bldr_pier_tube_small","bldr_pier_wooden1","bldr_pier_wooden1_end","bldr_mine_conveyer1_10","bldr_mine_conveyer1_begin","bldr_mine_conveyer1_endbuild","bldr_mine_conveyer2_build","bldr_mine_heap","bldr_mine_hopper_build","bldr_mine_rail","bldr_mine_rail_end","bldr_mine_rail_switch","bldr_mine_rail_tram","bldr_mine_tunnel","bldr_lamp_ind","bldr_lamp_rail","bldr_misc_boardspack1","bldr_misc_boardspack2","bldr_misc_boardspack3","bldr_misc_coil","bldr_misc_concretepanels","bldr_misc_concretepipe","bldr_misc_concretepipe_gate","bldr_misc_drainage","bldr_misc_gcontainer_big","bldr_misc_haybale","bldr_misc_haybale_decayed","bldr_misc_pallet","bldr_misc_pallets1","bldr_misc_pallets2","bldr_misc_pallets3","bldr_misc_pipes","bldr_misc_scaffolding","bldr_misc_through_static","bldr_misc_through_trailer","bldr_misc_timbers_log1","bldr_misc_timbers_log2","bldr_misc_timbers_log3","bldr_misc_timbers_log4","bldr_misc_timbers_log5","bldr_misc_timbers1","bldr_misc_timbers2","bldr_misc_timbers3","bldr_misc_timbers4","bldr_misc_tirepile","bldr_misc_tirepile_group","bldr_misc_woodpile_forest1","bldr_misc_woodpile_forest2","bldr_misc_woodpile_forest3","bldr_pipe_big_9m","bldr_pipe_big_18m","bldr_pipe_big_18m_ladder","bldr_pipe_big_smallconl","bldr_pipe_big_smallconr","bldr_pipe_big_support","bldr_pipe_med_9m","bldr_pipe_med_ground1","bldr_pipe_med_ground2","bldr_pipe_medl_buildr","bldr_pipe_medl_cornerl","bldr_pipe_medl_cornerr","bldr_pipe_medr_buildl","bldr_pipe_medr_buildr","bldr_pipe_small_20m","bldr_pipe_small_broken","bldr_pipe_small_ground1","bldr_pipe_small_ground2","bldr_pipe_small_l45","bldr_pipe_small_l90","bldr_pipe_small_r45","bldr_pipe_small_r90","bldr_pipe_small_stairs","bldr_pipe_small_u","bldr_pipe_small_valve","bldr_power_hv_line","bldr_power_hv_line_80m_damaged","bldr_power_hv_line_damaged","bldr_power_hv_line_turn","bldr_power_hv_line_turnl","bldr_power_hv_line_turnr","bldr_power_hv_tower","bldr_power_pole_conc1","bldr_power_pole_conc3","bldr_power_pole_conc4","bldr_power_pole_conc4_lamp","bldr_power_pole_conc4_lamp_trail_b","bldr_power_pole_conc4_lamp_trail_g","bldr_power_pole_conc4_lamp_trail_r","bldr_power_pole_conc4_lamp_trail_y","bldr_power_pole_conct","bldr_power_pole_wood1","bldr_power_pole_wood1_trail_b","bldr_power_pole_wood1_trail_g","bldr_power_pole_wood1_trail_r","bldr_power_pole_wood1_trail_y","bldr_power_pole_wood2","bldr_power_pole_wood3","bldr_power_transformer_small","bldr_power_transformerstation_cables","bldr_quarry_conveyer","bldr_quarry_conveyerlong","bldr_quarry_hammermill","bldr_tank_medium","bldr_tank_small_gas","bldr_tank_small_rusty","bldr_tank_small_white"};
		weapons[] = {};
		requiredVersion = 0.1;
		requiredAddons[] = {"DZ_Structures_Industrial"};
	};
};
class CfgVehicles
{
	class HouseNoDestruct;
	class bldr_cementworks_conveyor_up1: HouseNoDestruct
	{
		scope = 2;
		model = "\DZ\structures\industrial\cementworks\cementworks_conveyor_up1.p3d";
	};
	class bldr_cementworks_conveyor_up2: HouseNoDestruct
	{
		scope = 2;
		model = "\DZ\structures\industrial\cementworks\cementworks_conveyor_up2.p3d";
	};
	class bldr_cementworks_conveyor_up3: HouseNoDestruct
	{
		scope = 2;
		model = "\DZ\structures\industrial\cementworks\cementworks_conveyor_up3.p3d";
	};
	class bldr_cementworks_conveyorfurnace: HouseNoDestruct
	{
		scope = 2;
		model = "\DZ\structures\industrial\cementworks\cementworks_conveyorfurnace.p3d";
	};
	class bldr_cementworks_conveyormilla: HouseNoDestruct
	{
		scope = 2;
		model = "\DZ\structures\industrial\cementworks\cementworks_conveyormilla.p3d";
	};
	class bldr_cementworks_conveyormillb: HouseNoDestruct
	{
		scope = 2;
		model = "\DZ\structures\industrial\cementworks\cementworks_conveyormillb.p3d";
	};
	class bldr_cementworks_conveyortowera: HouseNoDestruct
	{
		scope = 2;
		model = "\DZ\structures\industrial\cementworks\cementworks_conveyortowera.p3d";
	};
	class bldr_cementworks_conveyortowerb: HouseNoDestruct
	{
		scope = 2;
		model = "\DZ\structures\industrial\cementworks\cementworks_conveyortowerb.p3d";
	};
	class bldr_cementworks_conveyortowerc: HouseNoDestruct
	{
		scope = 2;
		model = "\DZ\structures\industrial\cementworks\cementworks_conveyortowerc.p3d";
	};
	class bldr_cementworks_silobig1bridge: HouseNoDestruct
	{
		scope = 2;
		model = "\DZ\structures\industrial\cementworks\cementworks_silobig1bridge.p3d";
	};
	class bldr_coalplant_conveyor: HouseNoDestruct
	{
		scope = 2;
		model = "\DZ\structures\industrial\coalplant\coalplant_conveyor.p3d";
	};
	class bldr_container_1a: HouseNoDestruct
	{
		scope = 2;
		model = "\DZ\structures\industrial\containers\container_1a.p3d";
	};
	class bldr_container_1b: HouseNoDestruct
	{
		scope = 2;
		model = "\DZ\structures\industrial\containers\container_1b.p3d";
	};
	class bldr_container_1c: HouseNoDestruct
	{
		scope = 2;
		model = "\DZ\structures\industrial\containers\container_1c.p3d";
	};
	class bldr_container_1d: HouseNoDestruct
	{
		scope = 2;
		model = "\DZ\structures\industrial\containers\container_1d.p3d";
	};
	class bldr_container_2a: HouseNoDestruct
	{
		scope = 2;
		model = "\DZ\structures\industrial\containers\container_2a.p3d";
	};
	class bldr_container_2b: HouseNoDestruct
	{
		scope = 2;
		model = "\DZ\structures\industrial\containers\container_2b.p3d";
	};
	class bldr_container_2c: HouseNoDestruct
	{
		scope = 2;
		model = "\DZ\structures\industrial\containers\container_2c.p3d";
	};
	class bldr_container_2d: HouseNoDestruct
	{
		scope = 2;
		model = "\DZ\structures\industrial\containers\container_2d.p3d";
	};
	class bldr_container_2e: HouseNoDestruct
	{
		scope = 2;
		model = "\DZ\structures\industrial\containers\container_2e.p3d";
	};
	class bldr_farm_hopper: HouseNoDestruct
	{
		scope = 2;
		model = "\DZ\structures\industrial\farms\farm_hopper.p3d";
	};
	class bldr_farm_strawstack: HouseNoDestruct
	{
		scope = 2;
		model = "\DZ\structures\industrial\farms\farm_strawstack.p3d";
	};
	class bldr_barn_metal_lod1_proxy: HouseNoDestruct
	{
		scope = 2;
		model = "\DZ\structures\industrial\farms\proxy\barn_metal_lod1_proxy.p3d";
	};
	class bldr_barn_roofconstr_proxy: HouseNoDestruct
	{
		scope = 2;
		model = "\DZ\structures\industrial\farms\proxy\barn_roofconstr_proxy.p3d";
	};
	class bldr_barnboxy_a: HouseNoDestruct
	{
		scope = 2;
		model = "\DZ\structures\industrial\farms\proxy\boxy_a.p3d";
	};
	class bldr_barnboxy_b: HouseNoDestruct
	{
		scope = 2;
		model = "\DZ\structures\industrial\farms\proxy\boxy_b.p3d";
	};
	class bldr_barnboxy_c: HouseNoDestruct
	{
		scope = 2;
		model = "\DZ\structures\industrial\farms\proxy\boxy_c.p3d";
	};
	class bldr_barnboxy_d: HouseNoDestruct
	{
		scope = 2;
		model = "\DZ\structures\industrial\farms\proxy\boxy_d.p3d";
	};
	class bldr_barnboxy_e: HouseNoDestruct
	{
		scope = 2;
		model = "\DZ\structures\industrial\farms\proxy\boxy_e.p3d";
	};
	class bldr_barnokna_a: HouseNoDestruct
	{
		scope = 2;
		model = "\DZ\structures\industrial\farms\proxy\okna_a.p3d";
	};
	class bldr_barnokna_b: HouseNoDestruct
	{
		scope = 2;
		model = "\DZ\structures\industrial\farms\proxy\okna_b.p3d";
	};
	class bldr_barntram_a: HouseNoDestruct
	{
		scope = 2;
		model = "\DZ\structures\industrial\farms\proxy\tram_a.p3d";
	};
	class bldr_barntram_b: HouseNoDestruct
	{
		scope = 2;
		model = "\DZ\structures\industrial\farms\proxy\tram_b.p3d";
	};
	class bldr_pier_concrete1: HouseNoDestruct
	{
		scope = 2;
		model = "\DZ\structures\industrial\harbour\pier_concrete1.p3d";
	};
	class bldr_pier_concrete1_cornerin: HouseNoDestruct
	{
		scope = 2;
		model = "\DZ\structures\industrial\harbour\pier_concrete1_cornerin.p3d";
	};
	class bldr_pier_concrete1_cornerout: HouseNoDestruct
	{
		scope = 2;
		model = "\DZ\structures\industrial\harbour\pier_concrete1_cornerout.p3d";
	};
	class bldr_pier_concrete1_end: HouseNoDestruct
	{
		scope = 2;
		model = "\DZ\structures\industrial\harbour\pier_concrete1_end.p3d";
	};
	class bldr_pier_concrete1_l: HouseNoDestruct
	{
		scope = 2;
		model = "\DZ\structures\industrial\harbour\pier_concrete1_l.p3d";
	};
	class bldr_pier_concrete1_l10: HouseNoDestruct
	{
		scope = 2;
		model = "\DZ\structures\industrial\harbour\pier_concrete1_l10.p3d";
	};
	class bldr_pier_concrete1_l30: HouseNoDestruct
	{
		scope = 2;
		model = "\DZ\structures\industrial\harbour\pier_concrete1_l30.p3d";
	};
	class bldr_pier_concrete1_r: HouseNoDestruct
	{
		scope = 2;
		model = "\DZ\structures\industrial\harbour\pier_concrete1_r.p3d";
	};
	class bldr_pier_concrete1_r10: HouseNoDestruct
	{
		scope = 2;
		model = "\DZ\structures\industrial\harbour\pier_concrete1_r10.p3d";
	};
	class bldr_pier_concrete1_r30: HouseNoDestruct
	{
		scope = 2;
		model = "\DZ\structures\industrial\harbour\pier_concrete1_r30.p3d";
	};
	class bldr_pier_concrete1_tbig: HouseNoDestruct
	{
		scope = 2;
		model = "\DZ\structures\industrial\harbour\pier_concrete1_tbig.p3d";
	};
	class bldr_pier_concrete1_tsmall: HouseNoDestruct
	{
		scope = 2;
		model = "\DZ\structures\industrial\harbour\pier_concrete1_tsmall.p3d";
	};
	class bldr_pier_concrete2: HouseNoDestruct
	{
		scope = 2;
		model = "\DZ\structures\industrial\harbour\pier_concrete2.p3d";
	};
	class bldr_pier_concrete2_end: HouseNoDestruct
	{
		scope = 2;
		model = "\DZ\structures\industrial\harbour\pier_concrete2_end.p3d";
	};
	class bldr_pier_concrete3_1: HouseNoDestruct
	{
		scope = 2;
		model = "\DZ\structures\industrial\harbour\pier_concrete3_1.p3d";
	};
	class bldr_pier_concrete3_2: HouseNoDestruct
	{
		scope = 2;
		model = "\DZ\structures\industrial\harbour\pier_concrete3_2.p3d";
	};
	class bldr_pier_concrete3_end: HouseNoDestruct
	{
		scope = 2;
		model = "\DZ\structures\industrial\harbour\pier_concrete3_end.p3d";
	};
	class bldr_pier_concrete3_fuel: HouseNoDestruct
	{
		scope = 2;
		model = "\DZ\structures\industrial\harbour\pier_concrete3_fuel.p3d";
	};
	class bldr_pier_crane_rails: HouseNoDestruct
	{
		scope = 2;
		model = "\DZ\structures\industrial\harbour\pier_crane_rails.p3d";
	};
	class bldr_pier_crane_rails_end: HouseNoDestruct
	{
		scope = 2;
		model = "\DZ\structures\industrial\harbour\pier_crane_rails_end.p3d";
	};
	class bldr_pier_tube_big: HouseNoDestruct
	{
		scope = 2;
		model = "\DZ\structures\industrial\harbour\pier_tube_big.p3d";
	};
	class bldr_pier_tube_small: HouseNoDestruct
	{
		scope = 2;
		model = "\DZ\structures\industrial\harbour\pier_tube_small.p3d";
	};
	class bldr_pier_wooden1: HouseNoDestruct
	{
		scope = 2;
		model = "\DZ\structures\industrial\harbour\pier_wooden1.p3d";
	};
	class bldr_pier_wooden1_end: HouseNoDestruct
	{
		scope = 2;
		model = "\DZ\structures\industrial\harbour\pier_wooden1_end.p3d";
	};
	class bldr_mine_conveyer1_10: HouseNoDestruct
	{
		scope = 2;
		model = "\DZ\structures\industrial\mines\mine_conveyer1_10.p3d";
	};
	class bldr_mine_conveyer1_begin: HouseNoDestruct
	{
		scope = 2;
		model = "\DZ\structures\industrial\mines\mine_conveyer1_begin.p3d";
	};
	class bldr_mine_conveyer1_endbuild: HouseNoDestruct
	{
		scope = 2;
		model = "\DZ\structures\industrial\mines\mine_conveyer1_endbuild.p3d";
	};
	class bldr_mine_conveyer2_build: HouseNoDestruct
	{
		scope = 2;
		model = "\DZ\structures\industrial\mines\mine_conveyer2_build.p3d";
	};
	class bldr_mine_heap: HouseNoDestruct
	{
		scope = 2;
		model = "\DZ\structures\industrial\mines\mine_heap.p3d";
	};
	class bldr_mine_hopper_build: HouseNoDestruct
	{
		scope = 2;
		model = "\DZ\structures\industrial\mines\mine_hopper_build.p3d";
	};
	class bldr_mine_rail: HouseNoDestruct
	{
		scope = 2;
		model = "\DZ\structures\industrial\mines\mine_rail.p3d";
	};
	class bldr_mine_rail_end: HouseNoDestruct
	{
		scope = 2;
		model = "\DZ\structures\industrial\mines\mine_rail_end.p3d";
	};
	class bldr_mine_rail_switch: HouseNoDestruct
	{
		scope = 2;
		model = "\DZ\structures\industrial\mines\mine_rail_switch.p3d";
	};
	class bldr_mine_rail_tram: HouseNoDestruct
	{
		scope = 2;
		model = "\DZ\structures\industrial\mines\mine_rail_tram.p3d";
	};
	class bldr_mine_tunnel: HouseNoDestruct
	{
		scope = 2;
		model = "\DZ\structures\industrial\mines\mine_tunnel.p3d";
	};
	class bldr_lamp_ind: HouseNoDestruct
	{
		scope = 2;
		model = "\DZ\structures\industrial\misc\lamp_ind.p3d";
	};
	class bldr_lamp_rail: HouseNoDestruct
	{
		scope = 2;
		model = "\DZ\structures\industrial\misc\lamp_rail.p3d";
	};
	class bldr_misc_boardspack1: HouseNoDestruct
	{
		scope = 2;
		model = "\DZ\structures\industrial\misc\misc_boardspack1.p3d";
	};
	class bldr_misc_boardspack2: HouseNoDestruct
	{
		scope = 2;
		model = "\DZ\structures\industrial\misc\misc_boardspack2.p3d";
	};
	class bldr_misc_boardspack3: HouseNoDestruct
	{
		scope = 2;
		model = "\DZ\structures\industrial\misc\misc_boardspack3.p3d";
	};
	class bldr_misc_coil: HouseNoDestruct
	{
		scope = 2;
		model = "\DZ\structures\industrial\misc\misc_coil.p3d";
	};
	class bldr_misc_concretepanels: HouseNoDestruct
	{
		scope = 2;
		model = "\DZ\structures\industrial\misc\misc_concretepanels.p3d";
	};
	class bldr_misc_concretepipe: HouseNoDestruct
	{
		scope = 2;
		model = "\DZ\structures\industrial\misc\misc_concretepipe.p3d";
	};
	class bldr_misc_concretepipe_gate: HouseNoDestruct
	{
		scope = 2;
		model = "\DZ\structures\industrial\misc\misc_concretepipe_gate.p3d";
	};
	class bldr_misc_drainage: HouseNoDestruct
	{
		scope = 2;
		model = "\DZ\structures\industrial\misc\misc_drainage.p3d";
	};
	class bldr_misc_gcontainer_big: HouseNoDestruct
	{
		scope = 2;
		model = "\DZ\structures\industrial\misc\misc_gcontainer_big.p3d";
	};
	class bldr_misc_haybale: HouseNoDestruct
	{
		scope = 2;
		model = "\DZ\structures\industrial\misc\misc_haybale.p3d";
	};
	class bldr_misc_haybale_decayed: HouseNoDestruct
	{
		scope = 2;
		model = "\DZ\structures\industrial\misc\misc_haybale_decayed.p3d";
	};
	class bldr_misc_pallet: HouseNoDestruct
	{
		scope = 2;
		model = "\DZ\structures\industrial\misc\misc_pallet.p3d";
	};
	class bldr_misc_pallets1: HouseNoDestruct
	{
		scope = 2;
		model = "\DZ\structures\industrial\misc\misc_pallets1.p3d";
	};
	class bldr_misc_pallets2: HouseNoDestruct
	{
		scope = 2;
		model = "\DZ\structures\industrial\misc\misc_pallets2.p3d";
	};
	class bldr_misc_pallets3: HouseNoDestruct
	{
		scope = 2;
		model = "\DZ\structures\industrial\misc\misc_pallets3.p3d";
	};
	class bldr_misc_pipes: HouseNoDestruct
	{
		scope = 2;
		model = "\DZ\structures\industrial\misc\misc_pipes.p3d";
	};
	class bldr_misc_scaffolding: HouseNoDestruct
	{
		scope = 2;
		model = "\DZ\structures\industrial\misc\misc_scaffolding.p3d";
	};
	class bldr_misc_through_static: HouseNoDestruct
	{
		scope = 2;
		model = "\DZ\structures\industrial\misc\misc_through_static.p3d";
	};
	class bldr_misc_through_trailer: HouseNoDestruct
	{
		scope = 2;
		model = "\DZ\structures\industrial\misc\misc_through_trailer.p3d";
	};
	class bldr_misc_timbers_log1: HouseNoDestruct
	{
		scope = 2;
		model = "\DZ\structures\industrial\misc\misc_timbers_log1.p3d";
	};
	class bldr_misc_timbers_log2: HouseNoDestruct
	{
		scope = 2;
		model = "\DZ\structures\industrial\misc\misc_timbers_log2.p3d";
	};
	class bldr_misc_timbers_log3: HouseNoDestruct
	{
		scope = 2;
		model = "\DZ\structures\industrial\misc\misc_timbers_log3.p3d";
	};
	class bldr_misc_timbers_log4: HouseNoDestruct
	{
		scope = 2;
		model = "\DZ\structures\industrial\misc\misc_timbers_log4.p3d";
	};
	class bldr_misc_timbers_log5: HouseNoDestruct
	{
		scope = 2;
		model = "\DZ\structures\industrial\misc\misc_timbers_log5.p3d";
	};
	class bldr_misc_timbers1: HouseNoDestruct
	{
		scope = 2;
		model = "\DZ\structures\industrial\misc\misc_timbers1.p3d";
	};
	class bldr_misc_timbers2: HouseNoDestruct
	{
		scope = 2;
		model = "\DZ\structures\industrial\misc\misc_timbers2.p3d";
	};
	class bldr_misc_timbers3: HouseNoDestruct
	{
		scope = 2;
		model = "\DZ\structures\industrial\misc\misc_timbers3.p3d";
	};
	class bldr_misc_timbers4: HouseNoDestruct
	{
		scope = 2;
		model = "\DZ\structures\industrial\misc\misc_timbers4.p3d";
	};
	class bldr_misc_tirepile: HouseNoDestruct
	{
		scope = 2;
		model = "\DZ\structures\industrial\misc\misc_tirepile.p3d";
	};
	class bldr_misc_tirepile_group: HouseNoDestruct
	{
		scope = 2;
		model = "\DZ\structures\industrial\misc\misc_tirepile_group.p3d";
	};
	class bldr_misc_woodpile_forest1: HouseNoDestruct
	{
		scope = 2;
		model = "\DZ\structures\industrial\misc\misc_woodpile_forest1.p3d";
	};
	class bldr_misc_woodpile_forest2: HouseNoDestruct
	{
		scope = 2;
		model = "\DZ\structures\industrial\misc\misc_woodpile_forest2.p3d";
	};
	class bldr_misc_woodpile_forest3: HouseNoDestruct
	{
		scope = 2;
		model = "\DZ\structures\industrial\misc\misc_woodpile_forest3.p3d";
	};
	class bldr_pipe_big_9m: HouseNoDestruct
	{
		scope = 2;
		model = "\DZ\structures\industrial\pipes\pipe_big_9m.p3d";
	};
	class bldr_pipe_big_18m: HouseNoDestruct
	{
		scope = 2;
		model = "\DZ\structures\industrial\pipes\pipe_big_18m.p3d";
	};
	class bldr_pipe_big_18m_ladder: HouseNoDestruct
	{
		scope = 2;
		model = "\DZ\structures\industrial\pipes\pipe_big_18m_ladder.p3d";
	};
	class bldr_pipe_big_smallconl: HouseNoDestruct
	{
		scope = 2;
		model = "\DZ\structures\industrial\pipes\pipe_big_smallconl.p3d";
	};
	class bldr_pipe_big_smallconr: HouseNoDestruct
	{
		scope = 2;
		model = "\DZ\structures\industrial\pipes\pipe_big_smallconr.p3d";
	};
	class bldr_pipe_big_support: HouseNoDestruct
	{
		scope = 2;
		model = "\DZ\structures\industrial\pipes\pipe_big_support.p3d";
	};
	class bldr_pipe_med_9m: HouseNoDestruct
	{
		scope = 2;
		model = "\DZ\structures\industrial\pipes\pipe_med_9m.p3d";
	};
	class bldr_pipe_med_ground1: HouseNoDestruct
	{
		scope = 2;
		model = "\DZ\structures\industrial\pipes\pipe_med_ground1.p3d";
	};
	class bldr_pipe_med_ground2: HouseNoDestruct
	{
		scope = 2;
		model = "\DZ\structures\industrial\pipes\pipe_med_ground2.p3d";
	};
	class bldr_pipe_medl_buildr: HouseNoDestruct
	{
		scope = 2;
		model = "\DZ\structures\industrial\pipes\pipe_medl_buildr.p3d";
	};
	class bldr_pipe_medl_cornerl: HouseNoDestruct
	{
		scope = 2;
		model = "\DZ\structures\industrial\pipes\pipe_medl_cornerl.p3d";
	};
	class bldr_pipe_medl_cornerr: HouseNoDestruct
	{
		scope = 2;
		model = "\DZ\structures\industrial\pipes\pipe_medl_cornerr.p3d";
	};
	class bldr_pipe_medr_buildl: HouseNoDestruct
	{
		scope = 2;
		model = "\DZ\structures\industrial\pipes\pipe_medr_buildl.p3d";
	};
	class bldr_pipe_medr_buildr: HouseNoDestruct
	{
		scope = 2;
		model = "\DZ\structures\industrial\pipes\pipe_medr_buildr.p3d";
	};
	class bldr_pipe_small_20m: HouseNoDestruct
	{
		scope = 2;
		model = "\DZ\structures\industrial\pipes\pipe_small_20m.p3d";
	};
	class bldr_pipe_small_broken: HouseNoDestruct
	{
		scope = 2;
		model = "\DZ\structures\industrial\pipes\pipe_small_broken.p3d";
	};
	class bldr_pipe_small_ground1: HouseNoDestruct
	{
		scope = 2;
		model = "\DZ\structures\industrial\pipes\pipe_small_ground1.p3d";
	};
	class bldr_pipe_small_ground2: HouseNoDestruct
	{
		scope = 2;
		model = "\DZ\structures\industrial\pipes\pipe_small_ground2.p3d";
	};
	class bldr_pipe_small_l45: HouseNoDestruct
	{
		scope = 2;
		model = "\DZ\structures\industrial\pipes\pipe_small_l45.p3d";
	};
	class bldr_pipe_small_l90: HouseNoDestruct
	{
		scope = 2;
		model = "\DZ\structures\industrial\pipes\pipe_small_l90.p3d";
	};
	class bldr_pipe_small_r45: HouseNoDestruct
	{
		scope = 2;
		model = "\DZ\structures\industrial\pipes\pipe_small_r45.p3d";
	};
	class bldr_pipe_small_r90: HouseNoDestruct
	{
		scope = 2;
		model = "\DZ\structures\industrial\pipes\pipe_small_r90.p3d";
	};
	class bldr_pipe_small_stairs: HouseNoDestruct
	{
		scope = 2;
		model = "\DZ\structures\industrial\pipes\pipe_small_stairs.p3d";
	};
	class bldr_pipe_small_u: HouseNoDestruct
	{
		scope = 2;
		model = "\DZ\structures\industrial\pipes\pipe_small_u.p3d";
	};
	class bldr_pipe_small_valve: HouseNoDestruct
	{
		scope = 2;
		model = "\DZ\structures\industrial\pipes\pipe_small_valve.p3d";
	};
	class bldr_power_hv_line: HouseNoDestruct
	{
		scope = 2;
		model = "\DZ\structures\industrial\power\power_hv_line.p3d";
	};
	class bldr_power_hv_line_80m_damaged: HouseNoDestruct
	{
		scope = 2;
		model = "\DZ\structures\industrial\power\power_hv_line_80m_damaged.p3d";
	};
	class bldr_power_hv_line_damaged: HouseNoDestruct
	{
		scope = 2;
		model = "\DZ\structures\industrial\power\power_hv_line_damaged.p3d";
	};
	class bldr_power_hv_line_turn: HouseNoDestruct
	{
		scope = 2;
		model = "\DZ\structures\industrial\power\power_hv_line_turn.p3d";
	};
	class bldr_power_hv_line_turnl: HouseNoDestruct
	{
		scope = 2;
		model = "\DZ\structures\industrial\power\power_hv_line_turnl.p3d";
	};
	class bldr_power_hv_line_turnr: HouseNoDestruct
	{
		scope = 2;
		model = "\DZ\structures\industrial\power\power_hv_line_turnr.p3d";
	};
	class bldr_power_hv_tower: HouseNoDestruct
	{
		scope = 2;
		model = "\DZ\structures\industrial\power\power_hv_tower.p3d";
	};
	class bldr_power_pole_conc1: HouseNoDestruct
	{
		scope = 2;
		model = "\DZ\structures\industrial\power\power_pole_conc1.p3d";
	};
	class bldr_power_pole_conc3: HouseNoDestruct
	{
		scope = 2;
		model = "\DZ\structures\industrial\power\power_pole_conc3.p3d";
	};
	class bldr_power_pole_conc4: HouseNoDestruct
	{
		scope = 2;
		model = "\DZ\structures\industrial\power\power_pole_conc4.p3d";
	};
	class bldr_power_pole_conc4_lamp: HouseNoDestruct
	{
		scope = 2;
		model = "\DZ\structures\industrial\power\power_pole_conc4_lamp.p3d";
	};
	class bldr_power_pole_conc4_lamp_trail_b: HouseNoDestruct
	{
		scope = 2;
		model = "\DZ\structures\industrial\power\power_pole_conc4_lamp_trail_b.p3d";
	};
	class bldr_power_pole_conc4_lamp_trail_g: HouseNoDestruct
	{
		scope = 2;
		model = "\DZ\structures\industrial\power\power_pole_conc4_lamp_trail_g.p3d";
	};
	class bldr_power_pole_conc4_lamp_trail_r: HouseNoDestruct
	{
		scope = 2;
		model = "\DZ\structures\industrial\power\power_pole_conc4_lamp_trail_r.p3d";
	};
	class bldr_power_pole_conc4_lamp_trail_y: HouseNoDestruct
	{
		scope = 2;
		model = "\DZ\structures\industrial\power\power_pole_conc4_lamp_trail_y.p3d";
	};
	class bldr_power_pole_conct: HouseNoDestruct
	{
		scope = 2;
		model = "\DZ\structures\industrial\power\power_pole_conct.p3d";
	};
	class bldr_power_pole_wood1: HouseNoDestruct
	{
		scope = 2;
		model = "\DZ\structures\industrial\power\power_pole_wood1.p3d";
	};
	class bldr_power_pole_wood1_trail_b: HouseNoDestruct
	{
		scope = 2;
		model = "\DZ\structures\industrial\power\power_pole_wood1_trail_b.p3d";
	};
	class bldr_power_pole_wood1_trail_g: HouseNoDestruct
	{
		scope = 2;
		model = "\DZ\structures\industrial\power\power_pole_wood1_trail_g.p3d";
	};
	class bldr_power_pole_wood1_trail_r: HouseNoDestruct
	{
		scope = 2;
		model = "\DZ\structures\industrial\power\power_pole_wood1_trail_r.p3d";
	};
	class bldr_power_pole_wood1_trail_y: HouseNoDestruct
	{
		scope = 2;
		model = "\DZ\structures\industrial\power\power_pole_wood1_trail_y.p3d";
	};
	class bldr_power_pole_wood2: HouseNoDestruct
	{
		scope = 2;
		model = "\DZ\structures\industrial\power\power_pole_wood2.p3d";
	};
	class bldr_power_pole_wood3: HouseNoDestruct
	{
		scope = 2;
		model = "\DZ\structures\industrial\power\power_pole_wood3.p3d";
	};
	class bldr_power_transformer_small: HouseNoDestruct
	{
		scope = 2;
		model = "\DZ\structures\industrial\power\power_transformer_small.p3d";
	};
	class bldr_power_transformerstation_cables: HouseNoDestruct
	{
		scope = 2;
		model = "\DZ\structures\industrial\power\power_transformerstation_cables.p3d";
	};
	class bldr_quarry_conveyer: HouseNoDestruct
	{
		scope = 2;
		model = "\DZ\structures\industrial\quarry\quarry_conveyer.p3d";
	};
	class bldr_quarry_conveyerlong: HouseNoDestruct
	{
		scope = 2;
		model = "\DZ\structures\industrial\quarry\quarry_conveyerlong.p3d";
	};
	class bldr_quarry_hammermill: HouseNoDestruct
	{
		scope = 2;
		model = "\DZ\structures\industrial\quarry\quarry_hammermill.p3d";
	};
	class bldr_tank_medium: HouseNoDestruct
	{
		scope = 2;
		model = "\DZ\structures\industrial\tanks\tank_medium.p3d";
	};
	class bldr_tank_small_gas: HouseNoDestruct
	{
		scope = 2;
		model = "\DZ\structures\industrial\tanks\tank_small_gas.p3d";
	};
	class bldr_tank_small_rusty: HouseNoDestruct
	{
		scope = 2;
		model = "\DZ\structures\industrial\tanks\tank_small_rusty.p3d";
	};
	class bldr_tank_small_white: HouseNoDestruct
	{
		scope = 2;
		model = "\DZ\structures\industrial\tanks\tank_small_white.p3d";
	};
};
