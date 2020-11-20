#define _ARMA_

class CfgPatches
{
	class BuilderItems_Furniture
	{
		units[] = {"bldr_basin_a","bldr_bath","bldr_sink","bldr_toilet_b_02","bldr_postel_panelak1","bldr_almara","bldr_almara_open","bldr_case_a","bldr_case_a_open","bldr_case_bedroom_a","bldr_case_bedroom_b","bldr_case_cans_b","bldr_case_d","bldr_library_a","bldr_library_a_open","bldr_lobby_case","bldr_locker_closed_blue_v1","bldr_locker_closed_blue_v2","bldr_locker_closed_blue_v3","bldr_locker_closed_v1","bldr_locker_closed_v2","bldr_locker_closed_v3","bldr_locker_open_blue_v1","bldr_locker_open_blue_v2","bldr_locker_open_blue_v3","bldr_locker_open_v1","bldr_locker_open_v2","bldr_locker_open_v3","bldr_metalcase_01","bldr_metalcase_02","bldr_metalcrate","bldr_metalcrate_02","bldr_sarcophagus","bldr_skrin_bar","bldr_case_wall_unit_part_c","bldr_case_wall_unit_part_d","bldr_ch_mod_c","bldr_ch_mod_h","bldr_ch_office_b","bldr_church_chair","bldr_hospital_bench","bldr_kitchen_chair_a","bldr_lobby_chair","bldr_office_chair","bldr_postel_manz_kov","bldr_chairpostel_panelak1","bldr_sofa_leather_new","bldr_sofa_leather_old","bldr_sofacorner","bldr_box_c","bldr_box_c_multi","bldr_candle","bldr_extinguisher","bldr_bucket","bldr_carpet_big","bldr_flowers_01","bldr_flowers_02","bldr_lekarnicka","bldr_piano","bldr_picture_a","bldr_picture_a_02","bldr_picture_a_03","bldr_picture_a_04","bldr_picture_a_05","bldr_picture_b","bldr_picture_b_02","bldr_picture_c","bldr_picture_c_02","bldr_picture_d","bldr_picture_d_02","bldr_picture_e","bldr_picture_e_02","bldr_picture_f","bldr_picture_f_02","bldr_picture_g","bldr_picture_g_02","bldr_wall_board","bldr_wall_board_02","bldr_wall_board_03","bldr_pc","bldr_phonebox","bldr_pokladna","bldr_radio_b","bldr_tv_a","bldr_vending_machine","bldr_washing_machine","bldr_foldingbed_open","bldr_cashier","bldr_icebox","bldr_pult","bldr_pultskasou","bldr_scale","bldr_shelf","bldr_shelf_1side","bldr_hospital_transport_bed","bldr_case_sink_a","bldr_dkamna_bila","bldr_dkamna_uhli","bldr_fireplace_grill","bldr_fridge","bldr_kitchendesk_a","bldr_kitchendesk_b","bldr_kitchenstove_elec","bldr_fluor_lamp","bldr_hangar_lamp","bldr_light_bathroom","bldr_light_kitchen_03","bldr_luxury_lamp","bldr_luxury_light","bldr_medical_table","bldr_patient_monitor","bldr_prison_sink","bldr_prison_toilet","bldr_radar_electronics","bldr_radar_panel","bldr_radar_panel_flat","bldr_radar_rack","bldr_radar_rack_quad","bldr_anatomy_poster_1","bldr_anatomy_poster_2","bldr_basketball_korb","bldr_blackboard","bldr_class_case_a_closed","bldr_class_case_a_open","bldr_class_case_b_closed","bldr_class_case_b_open","bldr_desk_globe","bldr_flag_chernarus_pole","bldr_flag_chernarus_wall","bldr_gym_ladder","bldr_intercom_speaker","bldr_lab_beaker","bldr_lab_bench","bldr_lab_bunsen","bldr_lab_cylinder_beaker","bldr_lab_dropper","bldr_lab_erlenmeyer_flask","bldr_lab_microscope","bldr_lab_petri_dish","bldr_lab_sink","bldr_lab_teacher_bench","bldr_lab_triplebeam","bldr_lab_vial","bldr_lab_volume_beaker","bldr_long_bench","bldr_periodic_table","bldr_radiator","bldr_school_cage","bldr_school_clock","bldr_school_map","bldr_school_ringbell","bldr_student_chair","bldr_student_desk","bldr_tac_board","bldr_tac_board_posters_level1","bldr_tac_board_posters_level2","bldr_teacher_desk","bldr_vaulting_box_large","bldr_vaulting_box_small","bldr_vaulting_goat","bldr_wall_hanger","bldr_yellow_star","bldr_conference_table_a","bldr_kitchen_table_a","bldr_lobby_counter","bldr_lobby_table","bldr_office_desk","bldr_office_table_a","bldr_stul_kuch1","bldr_table_drawer","bldr_table_umakart","bldr_anvil","bldr_bag_dz","bldr_bar_shelves_dz","bldr_bar_tap_dz","bldr_boots_dz","bldr_canister_dz","bldr_carpet_2_dz","bldr_chest_dz","bldr_couch_beige84x213x75_dz","bldr_couch_dz","bldr_debris_small_house","bldr_dirtpile_small_house","bldr_drapes","bldr_drapes_long","bldr_drill","bldr_flagholder","bldr_fusebox","bldr_grinder","bldr_kitchen_unit_a_dz","bldr_metal_cutting_saw","bldr_rack_dz","bldr_sawhorse_dz","bldr_shelf_dz","bldr_soustruh_proxy","bldr_table_dz","bldr_tools_racking_dz","bldr_vise","bldr_wheel_cart_dz","bldr_workbench","bldr_workbench_dz"};
		weapons[] = {};
		requiredVersion = 0.1;
		requiredAddons[] = {"DZ_Structures_Furniture"};
	};
};
class CfgVehicles
{
	class HouseNoDestruct;
	class bldr_basin_a: HouseNoDestruct
	{
		scope = 2;
		model = "\DZ\structures\furniture\bathroom\basin_a\basin_a.p3d";
	};
	class bldr_bath: HouseNoDestruct
	{
		scope = 2;
		model = "\DZ\structures\furniture\bathroom\bath\bath.p3d";
	};
	class bldr_sink: HouseNoDestruct
	{
		scope = 2;
		model = "\DZ\structures\furniture\bathroom\sink\sink.p3d";
	};
	class bldr_toilet_b_02: HouseNoDestruct
	{
		scope = 2;
		model = "\DZ\structures\furniture\bathroom\toilet_b_02\toilet_b_02.p3d";
	};
	class bldr_postel_panelak1: HouseNoDestruct
	{
		scope = 2;
		model = "\DZ\structures\furniture\beds\postel_panelak1.p3d";
	};
	class bldr_almara: HouseNoDestruct
	{
		scope = 2;
		model = "\DZ\structures\furniture\cases\almara\almara.p3d";
	};
	class bldr_almara_open: HouseNoDestruct
	{
		scope = 2;
		model = "\DZ\structures\furniture\cases\almara\almara_open.p3d";
	};
	class bldr_case_a: HouseNoDestruct
	{
		scope = 2;
		model = "\DZ\structures\furniture\cases\case_a\case_a.p3d";
	};
	class bldr_case_a_open: HouseNoDestruct
	{
		scope = 2;
		model = "\DZ\structures\furniture\cases\case_a\case_a_open.p3d";
	};
	class bldr_case_bedroom_a: HouseNoDestruct
	{
		scope = 2;
		model = "\DZ\structures\furniture\cases\case_bedroom_a\case_bedroom_a.p3d";
	};
	class bldr_case_bedroom_b: HouseNoDestruct
	{
		scope = 2;
		model = "\DZ\structures\furniture\cases\case_bedroom_b\case_bedroom_b.p3d";
	};
	class bldr_case_cans_b: HouseNoDestruct
	{
		scope = 2;
		model = "\DZ\structures\furniture\cases\case_cans_b\case_cans_b.p3d";
	};
	class bldr_case_d: HouseNoDestruct
	{
		scope = 2;
		model = "\DZ\structures\furniture\cases\case_d\case_d.p3d";
	};
	class bldr_library_a: HouseNoDestruct
	{
		scope = 2;
		model = "\DZ\structures\furniture\cases\library_a\library_a.p3d";
	};
	class bldr_library_a_open: HouseNoDestruct
	{
		scope = 2;
		model = "\DZ\structures\furniture\cases\library_a\library_a_open.p3d";
	};
	class bldr_lobby_case: HouseNoDestruct
	{
		scope = 2;
		model = "\DZ\structures\furniture\cases\lobby_case\lobby_case.p3d";
	};
	class bldr_locker_closed_blue_v1: HouseNoDestruct
	{
		scope = 2;
		model = "\DZ\structures\furniture\cases\locker\locker_closed_blue_v1.p3d";
	};
	class bldr_locker_closed_blue_v2: HouseNoDestruct
	{
		scope = 2;
		model = "\DZ\structures\furniture\cases\locker\locker_closed_blue_v2.p3d";
	};
	class bldr_locker_closed_blue_v3: HouseNoDestruct
	{
		scope = 2;
		model = "\DZ\structures\furniture\cases\locker\locker_closed_blue_v3.p3d";
	};
	class bldr_locker_closed_v1: HouseNoDestruct
	{
		scope = 2;
		model = "\DZ\structures\furniture\cases\locker\locker_closed_v1.p3d";
	};
	class bldr_locker_closed_v2: HouseNoDestruct
	{
		scope = 2;
		model = "\DZ\structures\furniture\cases\locker\locker_closed_v2.p3d";
	};
	class bldr_locker_closed_v3: HouseNoDestruct
	{
		scope = 2;
		model = "\DZ\structures\furniture\cases\locker\locker_closed_v3.p3d";
	};
	class bldr_locker_open_blue_v1: HouseNoDestruct
	{
		scope = 2;
		model = "\DZ\structures\furniture\cases\locker\locker_open_blue_v1.p3d";
	};
	class bldr_locker_open_blue_v2: HouseNoDestruct
	{
		scope = 2;
		model = "\DZ\structures\furniture\cases\locker\locker_open_blue_v2.p3d";
	};
	class bldr_locker_open_blue_v3: HouseNoDestruct
	{
		scope = 2;
		model = "\DZ\structures\furniture\cases\locker\locker_open_blue_v3.p3d";
	};
	class bldr_locker_open_v1: HouseNoDestruct
	{
		scope = 2;
		model = "\DZ\structures\furniture\cases\locker\locker_open_v1.p3d";
	};
	class bldr_locker_open_v2: HouseNoDestruct
	{
		scope = 2;
		model = "\DZ\structures\furniture\cases\locker\locker_open_v2.p3d";
	};
	class bldr_locker_open_v3: HouseNoDestruct
	{
		scope = 2;
		model = "\DZ\structures\furniture\cases\locker\locker_open_v3.p3d";
	};
	class bldr_metalcase_01: HouseNoDestruct
	{
		scope = 2;
		model = "\DZ\structures\furniture\cases\metalcase\metalcase_01.p3d";
	};
	class bldr_metalcase_02: HouseNoDestruct
	{
		scope = 2;
		model = "\DZ\structures\furniture\cases\metalcase\metalcase_02.p3d";
	};
	class bldr_metalcrate: HouseNoDestruct
	{
		scope = 2;
		model = "\DZ\structures\furniture\cases\metalcrate\metalcrate.p3d";
	};
	class bldr_metalcrate_02: HouseNoDestruct
	{
		scope = 2;
		model = "\DZ\structures\furniture\cases\metalcrate_02\metalcrate_02.p3d";
	};
	class bldr_sarcophagus: HouseNoDestruct
	{
		scope = 2;
		model = "\DZ\structures\furniture\cases\sarcophagus\sarcophagus.p3d";
	};
	class bldr_skrin_bar: HouseNoDestruct
	{
		scope = 2;
		model = "\DZ\structures\furniture\cases\skrin_bar\skrin_bar.p3d";
	};
	class bldr_case_wall_unit_part_c: HouseNoDestruct
	{
		scope = 2;
		model = "\DZ\structures\furniture\cases\case_wall_unit_part_c.p3d";
	};
	class bldr_case_wall_unit_part_d: HouseNoDestruct
	{
		scope = 2;
		model = "\DZ\structures\furniture\cases\case_wall_unit_part_d.p3d";
	};
	class bldr_ch_mod_c: HouseNoDestruct
	{
		scope = 2;
		model = "\DZ\structures\furniture\chairs\ch_mod_c\ch_mod_c.p3d";
	};
	class bldr_ch_mod_h: HouseNoDestruct
	{
		scope = 2;
		model = "\DZ\structures\furniture\chairs\ch_mod_h\ch_mod_h.p3d";
	};
	class bldr_ch_office_b: HouseNoDestruct
	{
		scope = 2;
		model = "\DZ\structures\furniture\chairs\ch_office_b\ch_office_b.p3d";
	};
	class bldr_church_chair: HouseNoDestruct
	{
		scope = 2;
		model = "\DZ\structures\furniture\chairs\church_chair\church_chair.p3d";
	};
	class bldr_hospital_bench: HouseNoDestruct
	{
		scope = 2;
		model = "\DZ\structures\furniture\chairs\hospital_bench\hospital_bench.p3d";
	};
	class bldr_kitchen_chair_a: HouseNoDestruct
	{
		scope = 2;
		model = "\DZ\structures\furniture\chairs\kitchen_chair_a\kitchen_chair_a.p3d";
	};
	class bldr_lobby_chair: HouseNoDestruct
	{
		scope = 2;
		model = "\DZ\structures\furniture\chairs\lobby_chair\lobby_chair.p3d";
	};
	class bldr_office_chair: HouseNoDestruct
	{
		scope = 2;
		model = "\DZ\structures\furniture\chairs\office_chair\office_chair.p3d";
	};
	class bldr_postel_manz_kov: HouseNoDestruct
	{
		scope = 2;
		model = "\DZ\structures\furniture\chairs\postel_manz_kov\postel_manz_kov.p3d";
	};
	class bldr_chairpostel_panelak1: HouseNoDestruct
	{
		scope = 2;
		model = "\DZ\structures\furniture\chairs\postel_panelak1\postel_panelak1.p3d";
	};
	class bldr_sofa_leather_new: HouseNoDestruct
	{
		scope = 2;
		model = "\DZ\structures\furniture\chairs\sofa_leather\sofa_leather_new.p3d";
	};
	class bldr_sofa_leather_old: HouseNoDestruct
	{
		scope = 2;
		model = "\DZ\structures\furniture\chairs\sofa_leather\sofa_leather_old.p3d";
	};
	class bldr_sofacorner: HouseNoDestruct
	{
		scope = 2;
		model = "\DZ\structures\furniture\chairs\sofacorner\sofacorner.p3d";
	};
	class bldr_box_c: HouseNoDestruct
	{
		scope = 2;
		model = "\DZ\structures\furniture\decoration\box_c\box_c.p3d";
	};
	class bldr_box_c_multi: HouseNoDestruct
	{
		scope = 2;
		model = "\DZ\structures\furniture\decoration\box_c\box_c_multi.p3d";
	};
	class bldr_candle: HouseNoDestruct
	{
		scope = 2;
		model = "\dz\gear\tools\candle.p3d";
		physLayer = "item_small";
		carveNavmesh = 0;
	};
	class bldr_extinguisher: HouseNoDestruct
	{
		scope = 2;
		model = "\dz\gear\tools\FireExtinguisher.p3d";
		physLayer = "item_small";
		carveNavmesh = 0;
	};
	class bldr_bucket: HouseNoDestruct
	{
		scope = 2;
		model = "\DZ\structures\furniture\decoration\bucket\bucket.p3d";
	};
	class bldr_carpet_big: HouseNoDestruct
	{
		scope = 2;
		model = "\DZ\structures\furniture\decoration\carpet_big\carpet_big.p3d";
	};
	class bldr_flowers_01: HouseNoDestruct
	{
		scope = 2;
		model = "\DZ\structures\furniture\decoration\flowers\flower_01.p3d";
		physLayer = "item_small";
		carveNavmesh = 0;
	};
	class bldr_flowers_02: HouseNoDestruct
	{
		scope = 2;
		model = "\DZ\structures\furniture\decoration\flowers\flower_02.p3d";
		physLayer = "item_small";
		carveNavmesh = 0;
	};
	class bldr_lekarnicka: HouseNoDestruct
	{
		scope = 2;
		model = "\DZ\structures\furniture\decoration\lekarnicka\lekarnicka.p3d";
	};
	class bldr_piano: HouseNoDestruct
	{
		scope = 2;
		model = "\DZ\structures\furniture\decoration\piano\piano.p3d";
	};
	class bldr_picture: HouseNoDestruct
	{
		physLayer = "item_small";
		carveNavmesh = 0;
	};
	class bldr_picture_a: bldr_picture
	{
		scope = 2;
		model = "\DZ\structures\furniture\decoration\wallboards\picture_a\picture_a.p3d";
	};
	class bldr_picture_a_02: bldr_picture
	{
		scope = 2;
		model = "\DZ\structures\furniture\decoration\wallboards\picture_a_02\picture_a_02.p3d";
	};
	class bldr_picture_a_03: bldr_picture
	{
		scope = 2;
		model = "\DZ\structures\furniture\decoration\wallboards\picture_a_03\picture_a_03.p3d";
	};
	class bldr_picture_a_04: bldr_picture
	{
		scope = 2;
		model = "\DZ\structures\furniture\decoration\wallboards\picture_a_04\picture_a_04.p3d";
	};
	class bldr_picture_a_05: bldr_picture
	{
		scope = 2;
		model = "\DZ\structures\furniture\decoration\wallboards\picture_a_05\picture_a_05.p3d";
	};
	class bldr_picture_b: bldr_picture
	{
		scope = 2;
		model = "\DZ\structures\furniture\decoration\wallboards\picture_b\picture_b.p3d";
	};
	class bldr_picture_b_02: bldr_picture
	{
		scope = 2;
		model = "\DZ\structures\furniture\decoration\wallboards\picture_b_02\picture_b_02.p3d";
	};
	class bldr_picture_c: bldr_picture
	{
		scope = 2;
		model = "\DZ\structures\furniture\decoration\wallboards\picture_c\picture_c.p3d";
	};
	class bldr_picture_c_02: bldr_picture
	{
		scope = 2;
		model = "\DZ\structures\furniture\decoration\wallboards\picture_c_02\picture_c_02.p3d";
	};
	class bldr_picture_d: bldr_picture
	{
		scope = 2;
		model = "\DZ\structures\furniture\decoration\wallboards\picture_d\picture_d.p3d";
	};
	class bldr_picture_d_02: bldr_picture
	{
		scope = 2;
		model = "\DZ\structures\furniture\decoration\wallboards\picture_d_02\picture_d_02.p3d";
	};
	class bldr_picture_e: bldr_picture
	{
		scope = 2;
		model = "\DZ\structures\furniture\decoration\wallboards\picture_e\picture_e.p3d";
	};
	class bldr_picture_e_02: bldr_picture
	{
		scope = 2;
		model = "\DZ\structures\furniture\decoration\wallboards\picture_e_02\picture_e_02.p3d";
	};
	class bldr_picture_f: bldr_picture
	{
		scope = 2;
		model = "\DZ\structures\furniture\decoration\wallboards\picture_f\picture_f.p3d";
	};
	class bldr_picture_f_02: HouseNoDestruct
	{
		scope = 2;
		model = "\DZ\structures\furniture\decoration\wallboards\picture_f_02\picture_f_02.p3d";
	};
	class bldr_picture_g: bldr_picture
	{
		scope = 2;
		model = "\DZ\structures\furniture\decoration\wallboards\picture_g\picture_g.p3d";
	};
	class bldr_picture_g_02: bldr_picture
	{
		scope = 2;
		model = "\DZ\structures\furniture\decoration\wallboards\picture_g_02\picture_g_02.p3d";
	};
	class bldr_wall_board: HouseNoDestruct
	{
		scope = 2;
		model = "\DZ\structures\furniture\decoration\wallboards\wall_board\wall_board.p3d";
		physLayer = "item_small";
		carveNavmesh = 0;
	};
	class bldr_wall_board_02: HouseNoDestruct
	{
		scope = 2;
		model = "\DZ\structures\furniture\decoration\wallboards\wall_board_02\wall_board_02.p3d";
		physLayer = "item_small";
		carveNavmesh = 0;
	};
	class bldr_wall_board_03: HouseNoDestruct
	{
		scope = 2;
		model = "\DZ\structures\furniture\decoration\wallboards\wall_board_03\wall_board_03.p3d";
		physLayer = "item_small";
		carveNavmesh = 0;
	};
	class bldr_pc: HouseNoDestruct
	{
		scope = 2;
		model = "\DZ\structures\furniture\eletrical_appliances\pc\pc.p3d";
	};
	class bldr_phonebox: HouseNoDestruct
	{
		scope = 2;
		model = "\DZ\structures\furniture\eletrical_appliances\phonebox\phonebox.p3d";
	};
	class bldr_pokladna: HouseNoDestruct
	{
		scope = 2;
		model = "\DZ\structures\furniture\eletrical_appliances\pokladna\pokladna.p3d";
	};
	class bldr_radio_b: HouseNoDestruct
	{
		scope = 2;
		model = "\DZ\structures\furniture\eletrical_appliances\radio_b\radio_b.p3d";
		physLayer = "item_small";
		carveNavmesh = 0;
	};
	class bldr_tv_a: HouseNoDestruct
	{
		scope = 2;
		model = "\DZ\structures\furniture\eletrical_appliances\tv_a\tv_a.p3d";
	};
	class bldr_vending_machine: HouseNoDestruct
	{
		scope = 2;
		model = "\DZ\structures\furniture\eletrical_appliances\vending_machine\vending_machine.p3d";
	};
	class bldr_washing_machine: HouseNoDestruct
	{
		scope = 2;
		model = "\DZ\structures\furniture\eletrical_appliances\washing_machine\washing_machine.p3d";
	};
	class bldr_foldingbed_open: HouseNoDestruct
	{
		scope = 2;
		model = "\DZ\structures\furniture\folding_beds\foldingbed_open.p3d";
	};
	class bldr_cashier: HouseNoDestruct
	{
		scope = 2;
		model = "\DZ\structures\furniture\generalstore\cashier.p3d";
	};
	class bldr_icebox: HouseNoDestruct
	{
		scope = 2;
		model = "\DZ\structures\furniture\generalstore\icebox.p3d";
	};
	class bldr_pult: HouseNoDestruct
	{
		scope = 2;
		model = "\DZ\structures\furniture\generalstore\pult.p3d";
	};
	class bldr_pultskasou: HouseNoDestruct
	{
		scope = 2;
		model = "\DZ\structures\furniture\generalstore\pultskasou.p3d";
	};
	class bldr_scale: HouseNoDestruct
	{
		scope = 2;
		model = "\DZ\structures\furniture\generalstore\scale.p3d";
	};
	class bldr_shelf: HouseNoDestruct
	{
		scope = 2;
		model = "\DZ\structures\furniture\generalstore\shelf.p3d";
	};
	class bldr_shelf_1side: HouseNoDestruct
	{
		scope = 2;
		model = "\DZ\structures\furniture\generalstore\shelf_1side.p3d";
	};
	class bldr_hospital_transport_bed: HouseNoDestruct
	{
		scope = 2;
		model = "\DZ\structures\furniture\hospital_transport_bed\hospital_transport_bed.p3d";
	};
	class bldr_case_sink_a: HouseNoDestruct
	{
		scope = 2;
		model = "\DZ\structures\furniture\kitchen\case_sink_a\case_sink_a.p3d";
	};
	class bldr_dkamna_bila: HouseNoDestruct
	{
		scope = 2;
		model = "\DZ\structures\furniture\kitchen\dkamna_uhli\dkamna_bila.p3d";
	};
	class bldr_dkamna_uhli: HouseNoDestruct
	{
		scope = 2;
		model = "\DZ\structures\furniture\kitchen\dkamna_uhli\dkamna_uhli.p3d";
	};
	class bldr_fireplace_grill: HouseNoDestruct
	{
		scope = 2;
		model = "\DZ\structures\furniture\kitchen\fireplace_grill\fireplace_grill.p3d";
	};
	class bldr_fridge: HouseNoDestruct
	{
		scope = 2;
		model = "\DZ\structures\furniture\kitchen\fridge\fridge.p3d";
	};
	class bldr_kitchendesk_a: HouseNoDestruct
	{
		scope = 2;
		model = "\DZ\structures\furniture\kitchen\kitchendesk\kicthen_desk_a.p3d";
	};
	class bldr_kitchendesk_b: HouseNoDestruct
	{
		scope = 2;
		model = "\DZ\structures\furniture\kitchen\kitchendesk\kicthen_desk_b.p3d";
	};
	class bldr_kitchenstove_elec: HouseNoDestruct
	{
		scope = 2;
		model = "\DZ\structures\furniture\kitchen\kitchenstove_elec\kitchenstove_elec.p3d";
	};
	class bldr_fluor_lamp: HouseNoDestruct
	{
		scope = 2;
		model = "\DZ\structures\furniture\lighting\fluor_lamp\fluor_lamp.p3d";
	};
	class bldr_hangar_lamp: HouseNoDestruct
	{
		scope = 2;
		model = "\DZ\structures\furniture\lighting\hangar_lamp\hangar_lamp.p3d";
	};
	class bldr_light_bathroom: HouseNoDestruct
	{
		scope = 2;
		model = "\DZ\structures\furniture\lighting\light_bathroom\light_bathroom.p3d";
	};
	class bldr_light_kitchen_03: HouseNoDestruct
	{
		scope = 2;
		model = "\DZ\structures\furniture\lighting\light_kitchen_03\light_kitchen_03.p3d";
		physLayer = "item_small";
		carveNavmesh = 0;
	};
	class bldr_luxury_lamp: HouseNoDestruct
	{
		scope = 2;
		model = "\DZ\structures\furniture\lighting\luxury_lamp\lamp.p3d";
		physLayer = "item_small";
		carveNavmesh = 0;
	};
	class bldr_luxury_light: HouseNoDestruct
	{
		scope = 2;
		model = "\DZ\structures\furniture\lighting\luxury_light\luxury_light.p3d";
		physLayer = "item_small";
		carveNavmesh = 0;
	};
	class bldr_medical_table: HouseNoDestruct
	{
		scope = 2;
		model = "\DZ\structures\furniture\medical\medical_table\medical_table.p3d";
	};
	class bldr_patient_monitor: HouseNoDestruct
	{
		scope = 2;
		model = "\DZ\structures\furniture\medical\patient_monitor\patient_monitor.p3d";
	};
	class bldr_prison_sink: HouseNoDestruct
	{
		scope = 2;
		model = "\DZ\structures\furniture\prison\prison_sink.p3d";
	};
	class bldr_prison_toilet: HouseNoDestruct
	{
		scope = 2;
		model = "\DZ\structures\furniture\prison\prison_toilet.p3d";
	};
	class bldr_radar_electronics: HouseNoDestruct
	{
		scope = 2;
		model = "\DZ\structures\furniture\radar_equipment\radar_electronics.p3d";
	};
	class bldr_radar_panel: HouseNoDestruct
	{
		scope = 2;
		model = "\DZ\structures\furniture\radar_equipment\radar_panel.p3d";
	};
	class bldr_radar_panel_flat: HouseNoDestruct
	{
		scope = 2;
		model = "\DZ\structures\furniture\radar_equipment\radar_panel_flat.p3d";
	};
	class bldr_radar_rack: HouseNoDestruct
	{
		scope = 2;
		model = "\DZ\structures\furniture\radar_equipment\radar_rack.p3d";
	};
	class bldr_radar_rack_quad: HouseNoDestruct
	{
		scope = 2;
		model = "\DZ\structures\furniture\radar_equipment\radar_rack_quad.p3d";
	};
	class bldr_anatomy_poster_1: HouseNoDestruct
	{
		scope = 2;
		model = "\DZ\structures\furniture\school_equipment\anatomy_poster_1.p3d";
	};
	class bldr_anatomy_poster_2: HouseNoDestruct
	{
		scope = 2;
		model = "\DZ\structures\furniture\school_equipment\anatomy_poster_2.p3d";
	};
	class bldr_basketball_korb: HouseNoDestruct
	{
		scope = 2;
		model = "\DZ\structures\furniture\school_equipment\basketball_korb.p3d";
	};
	class bldr_blackboard: HouseNoDestruct
	{
		scope = 2;
		model = "\DZ\structures\furniture\school_equipment\blackboard.p3d";
	};
	class bldr_class_case_a_closed: HouseNoDestruct
	{
		scope = 2;
		model = "\DZ\structures\furniture\school_equipment\class_case_a_closed.p3d";
	};
	class bldr_class_case_a_open: HouseNoDestruct
	{
		scope = 2;
		model = "\DZ\structures\furniture\school_equipment\class_case_a_open.p3d";
	};
	class bldr_class_case_b_closed: HouseNoDestruct
	{
		scope = 2;
		model = "\DZ\structures\furniture\school_equipment\class_case_b_closed.p3d";
	};
	class bldr_class_case_b_open: HouseNoDestruct
	{
		scope = 2;
		model = "\DZ\structures\furniture\school_equipment\class_case_b_open.p3d";
	};
	class bldr_desk_globe: HouseNoDestruct
	{
		scope = 2;
		model = "\DZ\structures\furniture\school_equipment\desk_globe.p3d";
		physLayer = "item_small";
		carveNavmesh = 0;
	};
	class bldr_flag_chernarus_pole: HouseNoDestruct
	{
		scope = 2;
		model = "\DZ\structures\furniture\school_equipment\flag_chernarus_pole.p3d";
	};
	class bldr_flag_chernarus_wall: HouseNoDestruct
	{
		scope = 2;
		model = "\DZ\structures\furniture\school_equipment\flag_chernarus_wall.p3d";
	};
	class bldr_gym_ladder: HouseNoDestruct
	{
		scope = 2;
		model = "\DZ\structures\furniture\school_equipment\gym_ladder.p3d";
	};
	class bldr_intercom_speaker: HouseNoDestruct
	{
		scope = 2;
		model = "\DZ\structures\furniture\school_equipment\intercom_speaker.p3d";
	};
	class bldr_lab_beaker: HouseNoDestruct
	{
		scope = 2;
		model = "\DZ\structures\furniture\school_equipment\lab_beaker.p3d";
	};
	class bldr_lab_bench: HouseNoDestruct
	{
		scope = 2;
		model = "\DZ\structures\furniture\school_equipment\lab_bench.p3d";
	};
	class bldr_lab_bunsen: HouseNoDestruct
	{
		scope = 2;
		model = "\DZ\structures\furniture\school_equipment\lab_bunsen.p3d";
	};
	class bldr_lab_cylinder_beaker: HouseNoDestruct
	{
		scope = 2;
		model = "\DZ\structures\furniture\school_equipment\lab_cylinder_beaker.p3d";
	};
	class bldr_lab_dropper: HouseNoDestruct
	{
		scope = 2;
		model = "\DZ\structures\furniture\school_equipment\lab_dropper.p3d";
	};
	class bldr_lab_erlenmeyer_flask: HouseNoDestruct
	{
		scope = 2;
		model = "\DZ\structures\furniture\school_equipment\lab_erlenmeyer_flask.p3d";
	};
	class bldr_lab_microscope: HouseNoDestruct
	{
		scope = 2;
		model = "\DZ\structures\furniture\school_equipment\lab_microscope.p3d";
	};
	class bldr_lab_petri_dish: HouseNoDestruct
	{
		scope = 2;
		model = "\DZ\structures\furniture\school_equipment\lab_petri_dish.p3d";
	};
	class bldr_lab_sink: HouseNoDestruct
	{
		scope = 2;
		model = "\DZ\structures\furniture\school_equipment\lab_sink.p3d";
	};
	class bldr_lab_teacher_bench: HouseNoDestruct
	{
		scope = 2;
		model = "\DZ\structures\furniture\school_equipment\lab_teacher_bench.p3d";
	};
	class bldr_lab_triplebeam: HouseNoDestruct
	{
		scope = 2;
		model = "\DZ\structures\furniture\school_equipment\lab_triplebeam.p3d";
	};
	class bldr_lab_vial: HouseNoDestruct
	{
		scope = 2;
		model = "\DZ\structures\furniture\school_equipment\lab_vial.p3d";
	};
	class bldr_lab_volume_beaker: HouseNoDestruct
	{
		scope = 2;
		model = "\DZ\structures\furniture\school_equipment\lab_volume_beaker.p3d";
	};
	class bldr_long_bench: HouseNoDestruct
	{
		scope = 2;
		model = "\DZ\structures\furniture\school_equipment\long_bench.p3d";
	};
	class bldr_periodic_table: HouseNoDestruct
	{
		scope = 2;
		model = "\DZ\structures\furniture\school_equipment\periodic_table.p3d";
	};
	class bldr_radiator: HouseNoDestruct
	{
		scope = 2;
		model = "\DZ\structures\furniture\school_equipment\radiator.p3d";
	};
	class bldr_school_cage: HouseNoDestruct
	{
		scope = 2;
		model = "\DZ\structures\furniture\school_equipment\school_cage.p3d";
	};
	class bldr_school_clock: HouseNoDestruct
	{
		scope = 2;
		model = "\DZ\structures\furniture\school_equipment\school_clock.p3d";
	};
	class bldr_school_map: HouseNoDestruct
	{
		scope = 2;
		model = "\DZ\structures\furniture\school_equipment\school_map.p3d";
	};
	class bldr_school_ringbell: HouseNoDestruct
	{
		scope = 2;
		model = "\DZ\structures\furniture\school_equipment\school_ringbell.p3d";
	};
	class bldr_student_chair: HouseNoDestruct
	{
		scope = 2;
		model = "\DZ\structures\furniture\school_equipment\student_chair.p3d";
	};
	class bldr_student_desk: HouseNoDestruct
	{
		scope = 2;
		model = "\DZ\structures\furniture\school_equipment\student_desk.p3d";
	};
	class bldr_tac_board: HouseNoDestruct
	{
		scope = 2;
		model = "\DZ\structures\furniture\school_equipment\tac_board.p3d";
	};
	class bldr_tac_board_posters_level1: HouseNoDestruct
	{
		scope = 2;
		model = "\DZ\structures\furniture\school_equipment\tac_board_posters_level1.p3d";
	};
	class bldr_tac_board_posters_level2: HouseNoDestruct
	{
		scope = 2;
		model = "\DZ\structures\furniture\school_equipment\tac_board_posters_level2.p3d";
	};
	class bldr_teacher_desk: HouseNoDestruct
	{
		scope = 2;
		model = "\DZ\structures\furniture\school_equipment\teacher_desk.p3d";
	};
	class bldr_vaulting_box_large: HouseNoDestruct
	{
		scope = 2;
		model = "\DZ\structures\furniture\school_equipment\vaulting_box_large.p3d";
	};
	class bldr_vaulting_box_small: HouseNoDestruct
	{
		scope = 2;
		model = "\DZ\structures\furniture\school_equipment\vaulting_box_small.p3d";
	};
	class bldr_vaulting_goat: HouseNoDestruct
	{
		scope = 2;
		model = "\DZ\structures\furniture\school_equipment\vaulting_goat.p3d";
	};
	class bldr_wall_hanger: HouseNoDestruct
	{
		scope = 2;
		model = "\DZ\structures\furniture\school_equipment\wall_hanger.p3d";
	};
	class bldr_yellow_star: HouseNoDestruct
	{
		scope = 2;
		model = "\DZ\structures\furniture\school_equipment\yellow_star.p3d";
	};
	class bldr_conference_table_a: HouseNoDestruct
	{
		scope = 2;
		model = "\DZ\structures\furniture\tables\conference_table_a\conference_table_a.p3d";
	};
	class bldr_kitchen_table_a: HouseNoDestruct
	{
		scope = 2;
		model = "\DZ\structures\furniture\tables\kitchen_table_a\kitchen_table_a.p3d";
	};
	class bldr_lobby_counter: HouseNoDestruct
	{
		scope = 2;
		model = "\DZ\structures\furniture\tables\lobby_counter\lobby_counter.p3d";
	};
	class bldr_lobby_table: HouseNoDestruct
	{
		scope = 2;
		model = "\DZ\structures\furniture\tables\lobby_table\lobby_table.p3d";
	};
	class bldr_office_desk: HouseNoDestruct
	{
		scope = 2;
		model = "\DZ\structures\furniture\tables\office_desk\desk_office.p3d";
	};
	class bldr_office_table_a: HouseNoDestruct
	{
		scope = 2;
		model = "\DZ\structures\furniture\tables\office_table_a\office_table_a.p3d";
	};
	class bldr_stul_kuch1: HouseNoDestruct
	{
		scope = 2;
		model = "\DZ\structures\furniture\tables\stul_kuch1\stul_kuch1.p3d";
	};
	class bldr_table_drawer: HouseNoDestruct
	{
		scope = 2;
		model = "\DZ\structures\furniture\tables\table_drawer\table_drawer.p3d";
	};
	class bldr_table_umakart: HouseNoDestruct
	{
		scope = 2;
		model = "\DZ\structures\furniture\tables\table_umakart\table_umakart.p3d";
	};
	class bldr_anvil: HouseNoDestruct
	{
		scope = 2;
		model = "\DZ\structures\furniture\various\anvil.p3d";
	};
	class bldr_bag_dz: HouseNoDestruct
	{
		scope = 2;
		model = "\DZ\structures\furniture\various\bag_dz.p3d";
	};
	class bldr_bar_shelves_dz: HouseNoDestruct
	{
		scope = 2;
		model = "\DZ\structures\furniture\various\bar_shelves_dz.p3d";
	};
	class bldr_bar_tap_dz: HouseNoDestruct
	{
		scope = 2;
		model = "\DZ\structures\furniture\various\bar_tap_dz.p3d";
	};
	class bldr_boots_dz: HouseNoDestruct
	{
		scope = 2;
		model = "\DZ\structures\furniture\various\boots_dz.p3d";
	};
	class bldr_canister_dz: HouseNoDestruct
	{
		scope = 2;
		model = "\DZ\structures\furniture\various\canister_dz.p3d";
	};
	class bldr_carpet_2_dz: HouseNoDestruct
	{
		scope = 2;
		model = "\DZ\structures\furniture\various\carpet_2_dz.p3d";
	};
	class bldr_chest_dz: HouseNoDestruct
	{
		scope = 2;
		model = "\DZ\structures\furniture\various\chest_dz.p3d";
	};
	class bldr_couch_beige84x213x75_dz: HouseNoDestruct
	{
		scope = 2;
		model = "\DZ\structures\furniture\various\couch_beige84x213x75_dz.p3d";
	};
	class bldr_couch_dz: HouseNoDestruct
	{
		scope = 2;
		model = "\DZ\structures\furniture\various\couch_dz.p3d";
	};
	class bldr_debris_small_house: HouseNoDestruct
	{
		scope = 2;
		model = "\DZ\structures\furniture\various\debris_small_house.p3d";
		physLayer = "item_small";
		carveNavmesh = 0;
	};
	class bldr_dirtpile_small_house: HouseNoDestruct
	{
		scope = 2;
		model = "\DZ\structures\furniture\various\dirtpile_small_house.p3d";
	};
	class bldr_drapes: HouseNoDestruct
	{
		scope = 2;
		model = "\DZ\structures\furniture\various\drapes.p3d";
	};
	class bldr_drapes_long: HouseNoDestruct
	{
		scope = 2;
		model = "\DZ\structures\furniture\various\drapes_long.p3d";
	};
	class bldr_drill: HouseNoDestruct
	{
		scope = 2;
		model = "\DZ\structures\furniture\various\drill.p3d";
	};
	class bldr_flagholder: HouseNoDestruct
	{
		scope = 2;
		model = "\DZ\structures\furniture\various\flagholder.p3d";
	};
	class bldr_fusebox: HouseNoDestruct
	{
		scope = 2;
		model = "\DZ\structures\furniture\various\fusebox.p3d";
	};
	class bldr_grinder: HouseNoDestruct
	{
		scope = 2;
		model = "\DZ\structures\furniture\various\grinder.p3d";
	};
	class bldr_kitchen_unit_a_dz: HouseNoDestruct
	{
		scope = 2;
		model = "\DZ\structures\furniture\various\kitchen_unit_a_dz.p3d";
	};
	class bldr_metal_cutting_saw: HouseNoDestruct
	{
		scope = 2;
		model = "\DZ\structures\furniture\various\metal_cutting_saw.p3d";
	};
	class bldr_rack_dz: HouseNoDestruct
	{
		scope = 2;
		model = "\DZ\structures\furniture\various\rack_dz.p3d";
	};
	class bldr_sawhorse_dz: HouseNoDestruct
	{
		scope = 2;
		model = "\DZ\structures\furniture\various\sawhorse_dz.p3d";
	};
	class bldr_shelf_dz: HouseNoDestruct
	{
		scope = 2;
		model = "\DZ\structures\furniture\various\shelf_dz.p3d";
	};
	class bldr_soustruh_proxy: HouseNoDestruct
	{
		scope = 2;
		model = "\DZ\structures\furniture\various\soustruh_proxy.p3d";
	};
	class bldr_table_dz: HouseNoDestruct
	{
		scope = 2;
		model = "\DZ\structures\furniture\various\table_dz.p3d";
	};
	class bldr_tools_racking_dz: HouseNoDestruct
	{
		scope = 2;
		model = "\DZ\structures\furniture\various\tools_racking_dz.p3d";
	};
	class bldr_vise: HouseNoDestruct
	{
		scope = 2;
		model = "\DZ\structures\furniture\various\vise.p3d";
	};
	class bldr_wheel_cart_dz: HouseNoDestruct
	{
		scope = 2;
		model = "\DZ\structures\furniture\various\wheel_cart_dz.p3d";
	};
	class bldr_workbench: HouseNoDestruct
	{
		scope = 2;
		model = "\DZ\structures\furniture\various\workbench.p3d";
	};
	class bldr_workbench_dz: HouseNoDestruct
	{
		scope = 2;
		model = "\DZ\structures\furniture\various\workbench_dz.p3d";
	};
};
