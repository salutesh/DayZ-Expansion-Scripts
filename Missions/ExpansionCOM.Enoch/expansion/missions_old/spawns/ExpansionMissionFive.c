/**
 * ExpansionMissionFive.c
 *
 * DayZ Expansion Mod
 * www.dayzexpansion.com
 * © 2020 DayZ Expansion Mod Team
 *
 * This work is licensed under the Creative Commons Attribution-NonCommercial-NoDerivatives 4.0 International License. 
 * To view a copy of this license, visit http://creativecommons.org/licenses/by-nc-nd/4.0/.
 *
*/

static void ExpansionMissionFive()
{
	//! Notifications
	GetNotificationSystem().CreateNotification_Old("Mission", "A high tier soldiers mission appeared near Vybor", ARGB(255, 221, 38, 38), 7);

	//! Loot
	ref array< ref ExpansionMissionEventSoldierLoot > Loot = new array< ref ExpansionMissionEventSoldierLoot >;

	Loot.Insert( new ExpansionMissionEventSoldierLoot("AK74") );
	Loot.Insert( new ExpansionMissionEventSoldierLoot("AK101") );
	Loot.Insert( new ExpansionMissionEventSoldierLoot("SKS") );
	Loot.Insert( new ExpansionMissionEventSoldierLoot("M4A1") );
	Loot.Insert( new ExpansionMissionEventSoldierLoot("AKM") );
	Loot.Insert( new ExpansionMissionEventSoldierLoot("Mag_STANAG_30Rnd") );
	Loot.Insert( new ExpansionMissionEventSoldierLoot("Mag_AKM_30Rnd") );
	Loot.Insert( new ExpansionMissionEventSoldierLoot("Mag_SVD_10Rnd") );
	Loot.Insert( new ExpansionMissionEventSoldierLoot("Mag_VSS_10Rnd") );
	Loot.Insert( new ExpansionMissionEventSoldierLoot("PlateCarrierVest") );
	Loot.Insert( new ExpansionMissionEventSoldierLoot("PlaceCarrierHolster") );
	Loot.Insert( new ExpansionMissionEventSoldierLoot("PlaceCarrierPouches") );
	Loot.Insert( new ExpansionMissionEventSoldierLoot("Rangefinder") );

	ItemBase Barrel = ItemBase.Cast(GetDayZExpansion().SpawnObject("Barrel_Blue", "3762.160889 299.170624 7680.401367", "0.000000 0.000000 -0.000000", 60));
	Barrel.Open();

	GetDynamicMissions().SpawnMissionLoot(Loot, Barrel, 10);

	//! Soldiers
	ExpansionSoldierBase soldier1 = GetDayZExpansion().CreateExpansionSoldier("ExpansionSoldierLewis", "3765.903076 299.919983 7653.550781", "0 0 0", {"TTSKOPants", "TTSKOBoots", "TTsKOJacket_Camo", "AssaultBag_Ttsko","GorkaHelmet","GorkaHelmetVisor","HighCapacityVest_Olive"} );
	ExpansionSoldierBase soldier2 = GetDayZExpansion().CreateExpansionSoldier("ExpansionSoldierLewis", "3750.193848 299.896271 7675.122559", "0 0 0", {"TTSKOPants", "TTSKOBoots", "TTsKOJacket_Camo", "AssaultBag_Ttsko","GorkaHelmet","GorkaHelmetVisor","HighCapacityVest_Olive"} );
	ExpansionSoldierBase soldier3 = GetDayZExpansion().CreateExpansionSoldier("ExpansionSoldierLewis", "3761.679443 299.753815 7677.648926", "0 0 0", {"TTSKOPants", "TTSKOBoots", "TTsKOJacket_Camo", "AssaultBag_Ttsko","GorkaHelmet","GorkaHelmetVisor","HighCapacityVest_Olive"} );
	ExpansionSoldierBase soldier4 = GetDayZExpansion().CreateExpansionSoldier("ExpansionSoldierLewis", "3763.396973 300.019928 7668.386719", "0 0 0", {"TTSKOPants", "TTSKOBoots", "TTsKOJacket_Camo", "AssaultBag_Ttsko","GorkaHelmet","GorkaHelmetVisor","HighCapacityVest_Olive"} );

	soldier1.SetWeapon("AKM");
	soldier1.SetMagazine("Mag_AKM_30Rnd");
	soldier2.SetWeapon("AKM");
	soldier2.SetMagazine("Mag_AKM_30Rnd");
	soldier3.SetWeapon("AKM");
	soldier3.SetMagazine("Mag_AKM_30Rnd");
	soldier4.SetWeapon("AKM");
	soldier4.SetMagazine("Mag_AKM_30Rnd");

	//! Objects
	GetDayZExpansion().SpawnObject( "bldr_mil_artilery_rampart", "3772.952148 299.949280 7644.243652", "31.999992 0.076469 0.000000", 60);
	GetDayZExpansion().SpawnObject( "bldr_mil_artilery_rampart", "3779.061035 299.991638 7652.792969", "-147.000015 0.076466 0.381873", 60);
	GetDayZExpansion().SpawnObject( "Land_Guardhouse", "3767.790039 300.887421 7648.182617", "124.000008 0.076469 0.000000", 60);
	GetDayZExpansion().SpawnObject( "bldr_mil_artilery_rampart", "3771.712646 299.959961 7657.670898", "-146.000031 0.000000 0.000000", 60);
	GetDayZExpansion().SpawnObject( "bldr_Misc_RoadBarrier", "3779.810059 300.391174 7644.697754", "34.000000 0.000000 0.000000", 60);
	GetDayZExpansion().SpawnObject( "bldr_Misc_RoadBarrier", "3765.052734 300.308014 7657.647949", "-145.000015 0.000000 0.000000", 60);
	GetDayZExpansion().SpawnObject( "bldr_panel_concrete_dam", "3764.744873 300.649078 7649.191406", "34.000000 0.000000 0.000000", 60);
	GetDayZExpansion().SpawnObject( "bldr_panel_concrete_dam", "3762.257324 300.649078 7650.855957", "34.000000 0.000000 0.000000", 60);
	GetDayZExpansion().SpawnObject( "bldr_sign_roadblock_stop", "3780.829834 301.074005 7645.850098", "31.000002 0.305407 0.000000", 60);
	GetDayZExpansion().SpawnObject( "Land_Radio_PanelPAS", "3765.391602 300.069153 7646.434082", "123.999947 0.000000 0.076469", 60);
	GetDayZExpansion().SpawnObject( "bldr_flag_chernarus_pole", "3769.718750 303.160156 7650.933105", "-56.999989 0.000000 0.152938", 60);
	GetDayZExpansion().SpawnObject( "Land_Power_Pole_Conc1_Amp", "3768.802490 303.370880 7645.919922", "65.000000 0.000000 0.076469", 60);
	GetDayZExpansion().SpawnObject( "bldr_wall_tincom_9", "3767.809326 300.320007 7662.520508", "-54.999992 0.000000 0.000000", 60);
	GetDayZExpansion().SpawnObject( "bldr_wall_tincom_9", "3772.935059 300.032013 7669.857910", "-55.427124 0.000000 3.317371", 60);
	GetDayZExpansion().SpawnObject( "bldr_wall_tincom_9_2", "3772.069092 299.724640 7676.565918", "-142.022995 0.000000 -1.101710", 60);
	GetDayZExpansion().SpawnObject( "bldr_wall_tincom_9", "3764.517822 299.759003 7681.531250", "31.984289 -1.069015 0.804361", 60);
	GetDayZExpansion().SpawnObject( "bldr_wall_tincom_9", "3756.897217 299.829773 7686.300293", "31.978802 -1.221478 -0.173584", 60);
	GetDayZExpansion().SpawnObject( "bldr_wall_tincom_9_2", "3750.404297 300.124908 7685.017090", "122.546997 0.000000 -4.000000", 60);
	GetDayZExpansion().SpawnObject( "bldr_wall_tincom_9", "3745.745117 300.350708 7677.396484", "-56.999989 0.229172 0.178820", 60);
	GetDayZExpansion().SpawnObject( "bldr_wall_tincom_pole", "3748.153076 300.353943 7681.109863", "-60.999897 0.153000 3.149693", 60);
	GetDayZExpansion().SpawnObject( "bldr_hbarrier_big", "3761.488770 301.140991 7647.060059", "36.000000 0.000000 0.000000", 60);
	GetDayZExpansion().SpawnObject( "bldr_hbarrier_big", "3754.550293 301.091034 7651.653320", "35.000004 0.000000 0.000000", 60);
	GetDayZExpansion().SpawnObject( "bldr_hbarrier_big", "3746.468262 301.091034 7654.427734", "33.000000 0.076235 0.229172", 60);
	GetDayZExpansion().SpawnObject( "bldr_hbarrier_big", "3739.427002 301.055481 7658.879883", "35.000301 0.076235 0.229172", 60);
	GetDayZExpansion().SpawnObject( "bldr_wall_feng_pole", "3762.163818 300.496521 7653.706055", "34.000000 0.000000 0.000000", 60);
	GetDayZExpansion().SpawnObject( "Land_Wall_Gate_Ind1_R", "3758.700439 300.670074 7655.788574", "34.000000 0.000000 0.000000", 60);
	GetDayZExpansion().SpawnObject( "bldr_wall_feng_pole", "3759.648682 300.496521 7650.088867", "33.000000 0.000000 0.000000", 60);
	GetDayZExpansion().SpawnObject( "bldr_mil_fortified_nest_watchtower", "3743.044922 301.888672 7661.622070", "-56.999989 0.000000 0.000000", 60);
	GetDayZExpansion().SpawnObject( "bldr_mil_guardshed", "3740.194580 300.524933 7667.896973", "122.999985 0.076467 0.305407", 60);
	GetDayZExpansion().SpawnObject( "bldr_mil_camonet_roof_nato", "3751.181641 301.042480 7675.429199", "-145.000000 0.000000 0.000000", 60);
	GetDayZExpansion().SpawnObject( "Land_Mil_Barracks_Round", "3756.029297 300.504913 7681.219238", "-56.000000 0.000000 0.000000", 60);
	GetDayZExpansion().SpawnObject( "Land_Mil_Barracks_Round", "3767.291260 300.730957 7673.799805", "122.999992 0.000000 0.000000", 60);
	GetDayZExpansion().SpawnObject( "bldr_path_panels_damaged", "3761.743896 299.565002 7677.469238", "-57.999981 -2.000000 4.999999", 60);
	GetDayZExpansion().SpawnObject( "bldr_locker_open_blue_v2", "3766.127686 300.101593 7645.889160", "-147.000015 -0.042758 -0.063395", 60);
	GetDayZExpansion().SpawnObject( "bldr_locker_closed_blue_v2", "3766.429932 300.101593 7645.689941", "-147.000015 -0.042758 -0.063395", 60);
	GetDayZExpansion().SpawnObject( "bldr_box_c_multi", "3750.553223 299.836731 7678.725586", "0.000000 0.229173 0.000000", 60);
	GetDayZExpansion().SpawnObject( "bldr_lekarnicka", "3766.530029 301.269989 7648.673340", "122.999939 0.000000 0.000000", 60);
	GetDayZExpansion().SpawnObject( "bldr_pc", "3768.037354 300.774994 7646.396484", "28.000006 0.076469 0.000000", 60);
	GetDayZExpansion().SpawnObject( "bldr_phonebox", "3765.767090 300.369781 7648.936523", "122.999954 0.000000 0.076469", 60);
	GetDayZExpansion().SpawnObject( "bldr_fluor_lamp", "3768.067139 302.619324 7648.636230", "35.000004 0.000000 0.000000", 60);
	GetDayZExpansion().SpawnObject( "bldr_flag_chernarus_wall", "3765.669189 301.969482 7647.700195", "-56.000000 0.000000 0.000000", 60);
	GetDayZExpansion().SpawnObject( "bldr_school_map", "3768.183594 301.903381 7646.070801", "-55.999992 0.000000 0.000000", 60);
	GetDayZExpansion().SpawnObject( "bldr_dirtpile_small_house", "3767.790771 300.069977 7648.149902", "-17.000004 0.000000 0.000000", 60);
	GetDayZExpansion().SpawnObject( "bldr_Garbage_Container_Plastic", "3765.321045 300.661530 7660.343750", "-146.000031 0.000000 0.000000", 60);
	GetDayZExpansion().SpawnObject( "bldr_Garbage_Container_Paper", "3766.150146 300.661530 7661.826172", "-154.000031 0.000000 0.000000", 60);
	GetDayZExpansion().SpawnObject( "bldr_Garbage_GroundSq_5m", "3763.695068 299.820007 7659.177246", "0.000000 0.000000 0.000000", 60);
	GetDayZExpansion().SpawnObject( "bldr_Garbage_GroundSq_5m", "3764.911377 299.820007 7662.968262", "0.000000 0.000000 0.000000", 60);
	GetDayZExpansion().SpawnObject( "bldr_Garbage_Pile6", "3767.283447 299.820007 7664.049805", "-76.999969 0.000000 0.000000", 60);
	GetDayZExpansion().SpawnObject( "bldr_misc_pallets3", "3746.634766 300.472321 7676.941406", "42.000000 0.000000 0.229173", 60);
	GetDayZExpansion().SpawnObject( "bldr_misc_tirepile", "3753.494873 304.100647 7655.263672", "-137.999985 0.000000 0.000000", 60);
	GetDayZExpansion().SpawnObject( "bldr_tent_gunrack", "3770.199463 300.434875 7649.871582", "-56.999989 0.000000 0.000000", 60);
	GetDayZExpansion().SpawnObject( "bldr_tent_table", "3752.887207 300.233978 7675.946289", "32.000004 0.000000 0.000000", 60);
	GetDayZExpansion().SpawnObject( "bldr_tent_table", "3754.609375 300.229523 7674.835449", "28.000006 0.000000 0.000000", 60);
	GetDayZExpansion().SpawnObject( "bldr_misc_antenna", "3749.830078 302.683075 7655.962891", "0.001222 0.467607 0.536814", 60);
	GetDayZExpansion().SpawnObject( "bldr_misc_range_roof", "3766.306396 300.735992 7663.164551", "-56.999989 0.000000 0.000000", 60);
	GetDayZExpansion().SpawnObject( "bldr_Dead_MassGrave_15m", "3756.661865 299.934998 7646.246582", "28.959267 -1.908284 1.222831", 60);
	GetDayZExpansion().SpawnObject( "bldr_sign_attention", "3785.772461 300.752930 7645.696777", "-52.000000 0.000000 0.229173", 60);
	GetDayZExpansion().SpawnObject( "bldr_sign_attention", "3782.219238 300.738739 7640.641113", "-55.000000 0.152938 0.000000", 60);
	GetDayZExpansion().SpawnObject( "bldr_wreck_hmmwv", "3778.029053 300.695892 7646.908203", "-38.999210 0.305405 0.152940", 60);
	GetDayZExpansion().SpawnObject( "bldr_wreck_decal_small2", "3777.710449 299.618011 7647.526855", "-37.000004 0.000000 0.000000", 60);
	GetDayZExpansion().SpawnObject( "bldr_tent_chair", "3751.921387 300.326813 7675.293457", "0.000000 0.229173 0.000000", 60);
	GetDayZExpansion().SpawnObject( "bldr_tent_chair", "3754.495850 300.320221 7673.640137", "179.999985 -87.228996 -47.000092", 60);
	GetDayZExpansion().SpawnObject( "bldr_misc_barbedwire", "3739.447754 300.613281 7673.556641", "-17.000071 0.076469 0.152704", 60);
	GetDayZExpansion().SpawnObject( "bldr_plnt_l_plane_long", "3772.477539 300.669800 7648.513672", "32.000004 0.000000 0.000000", 60);
	GetDayZExpansion().SpawnObject( "bldr_plnt_l_plane_long", "3777.257324 299.770020 7650.260742", "33.000004 0.000000 0.000000", 60);
	GetDayZExpansion().SpawnObject( "bldr_plnt_l_long_pile_b", "3770.966553 299.914032 7649.961914", "107.999969 0.000000 0.000000", 60);
	GetDayZExpansion().SpawnObject( "bldr_plnt_l_long_pile_b", "3769.865479 299.914032 7651.157227", "-147.000015 0.000000 0.000000", 60);
	GetDayZExpansion().SpawnObject( "bldr_plnt_l_plane", "3773.287598 299.720032 7650.337402", "0.000000 0.000000 0.000000", 60);
	GetDayZExpansion().SpawnObject( "bldr_plnt_l_plane", "3765.274658 299.570068 7652.570313", "101.000008 0.000000 0.000000", 60);
	GetDayZExpansion().SpawnObject( "bldr_wreck_decal_small1", "3714.109375 300.024445 7677.154297", "-78.000000 0.000000 0.000000", 60);
	GetDayZExpansion().SpawnObject( "bldr_plnt_p_heracleum", "3768.076660 300.557831 7656.830078", "0.000000 0.000000 0.000000", 60);
	GetDayZExpansion().SpawnObject( "bldr_plnt_p_urtica", "3768.190186 300.224762 7652.147949", "0.000000 0.000000 0.000000", 60);
	GetDayZExpansion().SpawnObject( "bldr_plnt_p_heracleum", "3766.640137 300.557831 7666.309570", "0.000000 0.000000 0.000000", 60);
	GetDayZExpansion().SpawnObject( "bldr_plnt_p_urtica", "3748.694580 300.224762 7662.158203", "0.000000 0.000000 0.000000", 60);
	GetDayZExpansion().SpawnObject( "bldr_plnt_b_prunusSpinosa_1s", "3744.338379 301.315155 7673.157715", "0.000000 0.000000 0.000000", 60);
	GetDayZExpansion().SpawnObject( "bldr_plnt_b_rosaCanina_1s", "3758.249023 300.520630 7675.742188", "0.000000 0.000000 0.000000", 60);
	GetDayZExpansion().SpawnObject( "bldr_plnt_b_rosaCanina_1s", "3764.711182 300.760132 7658.890137", "0.000000 0.000000 0.000000", 60);
	GetDayZExpansion().SpawnObject( "bldr_plnt_l_long_pile_b", "3767.295410 299.955994 7650.932617", "-56.000019 0.076469 0.000000", 60);
}