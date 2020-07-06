#define _ARMA_

class CfgPatches
{
	class BuilderItemsBliss_Plants
	{
		units[] = {"bldr_plnt_ivy_A","bldr_plnt_ivy_B","bldr_plnt_ivy_C","bldr_plnt_ivy_C2","bldr_plnt_ivy_D","bldr_plnt_ivy_dry_A","bldr_plnt_ivy_dry_B","bldr_plnt_ivy_dry_C","bldr_plnt_ivy_dry_D","bldr_plnt_ivy_dry_E","bldr_plnt_ivy_dry_F","bldr_plnt_ivy_dry_G","bldr_plnt_ivy_dry_H","bldr_plnt_ivy_dry_I","bldr_plnt_ivy_dry_J","bldr_plnt_ivy_F","bldr_plnt_ivy_G","bldr_plnt_ivy_H","bldr_plnt_ivy_I","bldr_plnt_ivy_J","bldr_plnt_ivy_K","bldr_plnt_ivy_L","bldr_plnt_ivy_M","bldr_plnt_ivy_N","bldr_plnt_ivy_O","bldr_plnt_ivy_P","bldr_plnt_ivy_R","bldr_plnt_ivy_roof_A","bldr_plnt_ivy_roof_B","bldr_plnt_ivy_S","bldr_plnt_ivy_special_A","bldr_plnt_ivy_special_B","bldr_plnt_ivy_special_C","bldr_plnt_ivy_special_D","bldr_plnt_ivy_special_E","bldr_plnt_ivy_special_F","bldr_plnt_ivy_T","bldr_plnt_ivy_U","bldr_plnt_ivy_V","bldr_plnt_ivy_X","bldr_plnt_poison_ivy","bldr_plnt_b_betulaNana_1s_summer","bldr_plnt_b_caraganaArborescens_2s_summer","bldr_plnt_b_corylusHeterophylla_1s_summer","bldr_plnt_b_corylusHeterophylla_2s_summer","bldr_plnt_b_FagusSylvatica_1f_summer","bldr_plnt_b_phragmitesAustralis_summer","bldr_plnt_b_prunusSpinosa_1s_summer","bldr_plnt_b_prunusSpinosa_2s_summer","bldr_plnt_b_rosaCanina_1s_summer","bldr_plnt_b_rosaCanina_2s_summer","bldr_plnt_b_sambucusNigra_1s_summer","bldr_plnt_b_sambucusNigra_2s_summer","bldr_plnt_c_beech_big_summer","bldr_plnt_c_beech_mid_summer","bldr_plnt_c_beech_small_summer","bldr_plnt_c_branches_deciduous_summer","bldr_plnt_c_branches_picea_summer","bldr_plnt_c_branchesBig_picea_summer","bldr_plnt_c_ConcreteGrass_bunch_summer","bldr_plnt_c_Cones_summer","bldr_plnt_c_DirtGrass_bunch_summer","bldr_plnt_c_Elytrigia_green_summer","bldr_plnt_c_Elytrigia_summer","bldr_plnt_c_ElytrigiaDirt_summer","bldr_plnt_c_ElytrigiaTall_summer","bldr_plnt_c_ElytrigiaTall2_summer","bldr_plnt_c_fern_summer","bldr_plnt_c_fernTall_summer","bldr_plnt_c_Grass_flower1_summer","bldr_plnt_c_Grass_summer","bldr_plnt_c_GrassAnthoxanthum_bended_summer","bldr_plnt_c_GrassAnthoxanthum_summer","bldr_plnt_c_GrassBaseCarduus_f_summer","bldr_plnt_c_GrassBaseHypericum_summer","bldr_plnt_c_GrassCropCarduus_f_summer","bldr_plnt_c_GrassCropHypericum_summer","bldr_plnt_c_GrassDry_summer","bldr_plnt_c_GrassDry2_summer","bldr_plnt_c_GrassDry3_summer","bldr_plnt_c_GrassDryCarduus_f_summer","bldr_plnt_c_GrassDryHypericum_summer","bldr_plnt_c_GrassDryPlant1_summer","bldr_plnt_c_GrassDryTall_summer","bldr_plnt_c_GrassDryTall2_summer","bldr_plnt_c_GrassDryTall3_summer","bldr_plnt_c_GrassFlowersPurple_summer","bldr_plnt_c_GrassFlowersPurpleShort_summer","bldr_plnt_c_GrassFlowersPurpleTall_summer","bldr_plnt_c_GrassFlowersWhite_summer","bldr_plnt_c_GrassFlowersWhiteShort_summer","bldr_plnt_c_GrassFlowersWhiteTall_summer","bldr_plnt_c_GrassFlowersYellow_summer","bldr_plnt_c_GrassFlowersYellowShort_summer","bldr_plnt_c_GrassFlowersYellowTall_summer","bldr_plnt_c_GrassLow_summer","bldr_plnt_c_GrassPlant1_summer","bldr_plnt_c_GrassPlant2_summer","bldr_plnt_c_GrassSmall_summer","bldr_plnt_c_GrassSmallConc_summer","bldr_plnt_c_GrassTall2_summer","bldr_plnt_c_GrassTall3_summer","bldr_plnt_c_GrassTiny_summer","bldr_plnt_c_GrassTinyConc_summer","bldr_plnt_c_leaves_dead_half_summer","bldr_plnt_c_leaves_dead_summer","bldr_plnt_c_Pines_summer","bldr_plnt_c_plantsSmall1_summer","bldr_plnt_c_plantsSmall2_summer","bldr_plnt_c_stubble_summer","bldr_plnt_c_Vaccinium_summer","bldr_plnt_c_wheat_summer","bldr_plnt_c_wheat1_summer","bldr_plnt_c_wheat2","bldr_plnt_c_wheatTall_summer","bldr_plnt_b_PiceaAbies_1f_summer","bldr_plnt_b_PiceaAbies_1fb_summer","bldr_plnt_d_BetulaPendula_fallen","bldr_plnt_d_BetulaPendula_stump","bldr_plnt_d_piceaabies_fallend","bldr_plnt_d_piceaabies_fallene","bldr_plnt_d_piceaabies_stumpc","bldr_plnt_d_piceaabies_stumpd","bldr_plnt_t_acer_2s_summer","bldr_plnt_t_BetulaPendulaE_1s_summer","bldr_plnt_t_BetulaPendulaE_2f_summer","bldr_plnt_t_BetulaPendulaE_2s_summer","bldr_plnt_t_BetulaPendulaE_2w_summer","bldr_plnt_t_BetulaPendulaE_3f_summer","bldr_plnt_t_BetulaPendulaE_3s_summer","bldr_plnt_t_FagusSylvatica_1f_summer","bldr_plnt_t_FagusSylvatica_1fb_summer","bldr_plnt_t_FagusSylvatica_1fc_summer","bldr_plnt_t_FagusSylvatica_1fd_summer","bldr_plnt_t_FagusSylvatica_1fe_summer","bldr_plnt_t_FagusSylvatica_1s_summer","bldr_plnt_t_FagusSylvatica_2d_summer","bldr_plnt_t_FagusSylvatica_2f_summer","bldr_plnt_t_FagusSylvatica_2fb_summer","bldr_plnt_t_FagusSylvatica_2fc_summer","bldr_plnt_t_FagusSylvatica_2s_summer","bldr_plnt_t_FagusSylvatica_2sb_summer","bldr_plnt_t_FagusSylvatica_2sb_Trail_B_summer","bldr_plnt_t_FagusSylvatica_2sb_Trail_G_summer","bldr_plnt_t_FagusSylvatica_2sb_Trail_R_summer","bldr_plnt_t_FagusSylvatica_2sb_Trail_Y_summer","bldr_plnt_t_FagusSylvatica_3d_summer","bldr_plnt_t_FagusSylvatica_3f_summer","bldr_plnt_t_FagusSylvatica_3fb_summer","bldr_plnt_t_FagusSylvatica_3s_summer","bldr_plnt_t_FagusSylvaticaE_2s_summer","bldr_plnt_t_FagusSylvaticaE_3f_summer","bldr_plnt_t_juglansRegia_2s_summer","bldr_plnt_t_juglansRegia_3s_summer","bldr_plnt_t_LarixDecidua_1f_summer","bldr_plnt_t_LarixDecidua_1s_summer","bldr_plnt_t_LarixDecidua_2f_summer","bldr_plnt_t_LarixDecidua_2fb_summer","bldr_plnt_t_LarixDecidua_2s_summer","bldr_plnt_t_LarixDecidua_3f_summer","bldr_plnt_t_LarixDecidua_3fb_summer","bldr_plnt_t_malusDomestica_2s_summer","bldr_plnt_t_malusDomestica_3s_summer","bldr_plnt_t_PiceaAbies_1f_summer","bldr_plnt_t_PiceaAbies_1s_summer","bldr_plnt_t_PiceaAbies_1sb_summer","bldr_plnt_t_PiceaAbies_2f_summer","bldr_plnt_t_PiceaAbies_2fb_summer","bldr_plnt_t_PiceaAbies_2s_summer","bldr_plnt_t_PiceaAbies_2sb_summer","bldr_plnt_t_PiceaAbies_3f_summer","bldr_plnt_t_PiceaAbies_3f_Trail_B_summer","bldr_plnt_t_PiceaAbies_3f_Trail_G_summer","bldr_plnt_t_PiceaAbies_3f_Trail_R_summer","bldr_plnt_t_PiceaAbies_3f_Trail_Y_summer","bldr_plnt_t_PiceaAbies_3s_summer","bldr_plnt_t_PinusSylvestris_1f_summer","bldr_plnt_t_PinusSylvestris_1s_summer","bldr_plnt_t_PinusSylvestris_2f_summer","bldr_plnt_t_PinusSylvestris_2fb_summer","bldr_plnt_t_PinusSylvestris_2s_summer","bldr_plnt_t_PinusSylvestris_2sb_summer","bldr_plnt_t_PinusSylvestris_3d_summer","bldr_plnt_t_PinusSylvestris_3f_summer","bldr_plnt_t_PinusSylvestris_3fb_summer","bldr_plnt_t_PinusSylvestris_3fc_summer","bldr_plnt_t_PinusSylvestris_3s_summer","bldr_plnt_t_populusNigra_3sb_summer","bldr_plnt_t_pyrusCommunis_2s_summer","bldr_plnt_t_pyrusCommunis_3s_summer","bldr_plnt_t_salixAlba_2sb_summer","bldr_plnt_t_sorbus_2s_summer"};
		weapons[] = {};
		requiredVersion = 0.1;
		requiredAddons[] = {"DZ_Plants_Bliss"};
	};
};
class CfgVehicles
{
	class HouseNoDestruct;
	class bldr_ivy: HouseNoDestruct
	{
		physLayer = "item_small";
		carveNavmesh = 0;
	};
	class bldr_plnt_ivy_A: bldr_ivy
	{
		scope = 2;
		model = "DZ\plants_bliss\building\ivy_A.p3d";
	};
	class bldr_plnt_ivy_B: bldr_ivy
	{
		scope = 2;
		model = "DZ\plants_bliss\building\ivy_B.p3d";
	};
	class bldr_plnt_ivy_C: bldr_ivy
	{
		scope = 2;
		model = "DZ\plants_bliss\building\ivy_C.p3d";
	};
	class bldr_plnt_ivy_C2: bldr_ivy
	{
		scope = 2;
		model = "DZ\plants_bliss\building\ivy_C2.p3d";
	};
	class bldr_plnt_ivy_D: bldr_ivy
	{
		scope = 2;
		model = "DZ\plants_bliss\building\ivy_D.p3d";
	};
	class bldr_plnt_ivy_dry_A: bldr_ivy
	{
		scope = 2;
		model = "DZ\plants_bliss\building\ivy_dry_A.p3d";
	};
	class bldr_plnt_ivy_dry_B: bldr_ivy
	{
		scope = 2;
		model = "DZ\plants_bliss\building\ivy_dry_B.p3d";
	};
	class bldr_plnt_ivy_dry_C: bldr_ivy
	{
		scope = 2;
		model = "DZ\plants_bliss\building\ivy_dry_C.p3d";
	};
	class bldr_plnt_ivy_dry_D: bldr_ivy
	{
		scope = 2;
		model = "DZ\plants_bliss\building\ivy_dry_D.p3d";
	};
	class bldr_plnt_ivy_dry_E: bldr_ivy
	{
		scope = 2;
		model = "DZ\plants_bliss\building\ivy_dry_E.p3d";
	};
	class bldr_plnt_ivy_dry_F: bldr_ivy
	{
		scope = 2;
		model = "DZ\plants_bliss\building\ivy_dry_F.p3d";
	};
	class bldr_plnt_ivy_dry_G: bldr_ivy
	{
		scope = 2;
		model = "DZ\plants_bliss\building\ivy_dry_G.p3d";
	};
	class bldr_plnt_ivy_dry_H: bldr_ivy
	{
		scope = 2;
		model = "DZ\plants_bliss\building\ivy_dry_H.p3d";
	};
	class bldr_plnt_ivy_dry_I: HouseNoDestruct
	{
		scope = 2;
		model = "DZ\plants_bliss\building\ivy_dry_I.p3d";
	};
	class bldr_plnt_ivy_dry_J: bldr_ivy
	{
		scope = 2;
		model = "DZ\plants_bliss\building\ivy_dry_J.p3d";
	};
	class bldr_plnt_ivy_F: bldr_ivy
	{
		scope = 2;
		model = "DZ\plants_bliss\building\ivy_F.p3d";
	};
	class bldr_plnt_ivy_G: bldr_ivy
	{
		scope = 2;
		model = "DZ\plants_bliss\building\ivy_G.p3d";
	};
	class bldr_plnt_ivy_H: bldr_ivy
	{
		scope = 2;
		model = "DZ\plants_bliss\building\ivy_H.p3d";
	};
	class bldr_plnt_ivy_I: bldr_ivy
	{
		scope = 2;
		model = "DZ\plants_bliss\building\ivy_I.p3d";
	};
	class bldr_plnt_ivy_J: bldr_ivy
	{
		scope = 2;
		model = "DZ\plants_bliss\building\ivy_J.p3d";
	};
	class bldr_plnt_ivy_K: bldr_ivy
	{
		scope = 2;
		model = "DZ\plants_bliss\building\ivy_K.p3d";
	};
	class bldr_plnt_ivy_L: bldr_ivy
	{
		scope = 2;
		model = "DZ\plants_bliss\building\ivy_L.p3d";
	};
	class bldr_plnt_ivy_M: bldr_ivy
	{
		scope = 2;
		model = "DZ\plants_bliss\building\ivy_M.p3d";
	};
	class bldr_plnt_ivy_N: bldr_ivy
	{
		scope = 2;
		model = "DZ\plants_bliss\building\ivy_N.p3d";
	};
	class bldr_plnt_ivy_O: bldr_ivy
	{
		scope = 2;
		model = "DZ\plants_bliss\building\ivy_O.p3d";
	};
	class bldr_plnt_ivy_P: bldr_ivy
	{
		scope = 2;
		model = "DZ\plants_bliss\building\ivy_P.p3d";
	};
	class bldr_plnt_ivy_R: bldr_ivy
	{
		scope = 2;
		model = "DZ\plants_bliss\building\ivy_R.p3d";
	};
	class bldr_plnt_ivy_roof_A: bldr_ivy
	{
		scope = 2;
		model = "DZ\plants_bliss\building\ivy_roof_A.p3d";
	};
	class bldr_plnt_ivy_roof_B: bldr_ivy
	{
		scope = 2;
		model = "DZ\plants_bliss\building\ivy_roof_B.p3d";
	};
	class bldr_plnt_ivy_S: bldr_ivy
	{
		scope = 2;
		model = "DZ\plants_bliss\building\ivy_S.p3d";
	};
	class bldr_plnt_ivy_special_A: bldr_ivy
	{
		scope = 2;
		model = "DZ\plants_bliss\building\ivy_special_A.p3d";
	};
	class bldr_plnt_ivy_special_B: bldr_ivy
	{
		scope = 2;
		model = "DZ\plants_bliss\building\ivy_special_B.p3d";
	};
	class bldr_plnt_ivy_special_C: bldr_ivy
	{
		scope = 2;
		model = "DZ\plants_bliss\building\ivy_special_C.p3d";
	};
	class bldr_plnt_ivy_special_D: bldr_ivy
	{
		scope = 2;
		model = "DZ\plants_bliss\building\ivy_special_D.p3d";
	};
	class bldr_plnt_ivy_special_E: bldr_ivy
	{
		scope = 2;
		model = "DZ\plants_bliss\building\ivy_special_E.p3d";
	};
	class bldr_plnt_ivy_special_F: bldr_ivy
	{
		scope = 2;
		model = "DZ\plants_bliss\building\ivy_special_F.p3d";
	};
	class bldr_plnt_ivy_T: bldr_ivy
	{
		scope = 2;
		model = "DZ\plants_bliss\building\ivy_T.p3d";
	};
	class bldr_plnt_ivy_U: bldr_ivy
	{
		scope = 2;
		model = "DZ\plants_bliss\building\ivy_U.p3d";
	};
	class bldr_plnt_ivy_V: bldr_ivy
	{
		scope = 2;
		model = "DZ\plants_bliss\building\ivy_V.p3d";
	};
	class bldr_plnt_ivy_X: bldr_ivy
	{
		scope = 2;
		model = "DZ\plants_bliss\building\ivy_X.p3d";
	};
	class bldr_plnt_poison_ivy: bldr_ivy
	{
		scope = 2;
		model = "DZ\plants_bliss\building\poison_ivy.p3d";
	};
	class bldr_plnt_b_betulaNana_1s_summer: HouseNoDestruct
	{
		scope = 2;
		model = "DZ\plants_bliss\bush\b_betulaNana_1s_summer.p3d";
		physLayer = "item_small";
		carveNavmesh = 0;
	};
	class bldr_plnt_b_caraganaArborescens_2s_summer: HouseNoDestruct
	{
		scope = 2;
		model = "DZ\plants_bliss\bush\b_caraganaArborescens_2s_summer.p3d";
		physLayer = "item_small";
		carveNavmesh = 0;
	};
	class bldr_plnt_b_corylusHeterophylla_1s_summer: HouseNoDestruct
	{
		scope = 2;
		model = "DZ\plants_bliss\bush\b_corylusHeterophylla_1s_summer.p3d";
		physLayer = "item_small";
		carveNavmesh = 0;
	};
	class bldr_plnt_b_corylusHeterophylla_2s_summer: HouseNoDestruct
	{
		scope = 2;
		model = "DZ\plants_bliss\bush\b_corylusHeterophylla_2s_summer.p3d";
		physLayer = "item_small";
		carveNavmesh = 0;
	};
	class bldr_plnt_b_FagusSylvatica_1f_summer: HouseNoDestruct
	{
		scope = 2;
		model = "DZ\plants_bliss\bush\b_FagusSylvatica_1f_summer.p3d";
		physLayer = "item_small";
		carveNavmesh = 0;
	};
	class bldr_plnt_b_phragmitesAustralis_summer: HouseNoDestruct
	{
		scope = 2;
		model = "DZ\plants_bliss\bush\b_phragmitesAustralis_summer.p3d";
		physLayer = "item_small";
		carveNavmesh = 0;
	};
	class bldr_plnt_b_prunusSpinosa_1s_summer: HouseNoDestruct
	{
		scope = 2;
		model = "DZ\plants_bliss\bush\b_prunusSpinosa_1s_summer.p3d";
		physLayer = "item_small";
		carveNavmesh = 0;
	};
	class bldr_plnt_b_prunusSpinosa_2s_summer: HouseNoDestruct
	{
		scope = 2;
		model = "DZ\plants_bliss\bush\b_prunusSpinosa_2s_summer.p3d";
		physLayer = "item_small";
		carveNavmesh = 0;
	};
	class bldr_plnt_b_rosaCanina_1s_summer: HouseNoDestruct
	{
		scope = 2;
		model = "DZ\plants_bliss\bush\b_rosaCanina_1s_summer.p3d";
		physLayer = "item_small";
		carveNavmesh = 0;
	};
	class bldr_plnt_b_rosaCanina_2s_summer: HouseNoDestruct
	{
		scope = 2;
		model = "DZ\plants_bliss\bush\b_rosaCanina_2s_summer.p3d";
		physLayer = "item_small";
		carveNavmesh = 0;
	};
	class bldr_plnt_b_sambucusNigra_1s_summer: HouseNoDestruct
	{
		scope = 2;
		model = "DZ\plants_bliss\bush\b_sambucusNigra_1s_summer.p3d";
		physLayer = "item_small";
		carveNavmesh = 0;
	};
	class bldr_plnt_b_sambucusNigra_2s_summer: HouseNoDestruct
	{
		scope = 2;
		model = "DZ\plants_bliss\bush\b_sambucusNigra_2s_summer.p3d";
		physLayer = "item_small";
		carveNavmesh = 0;
	};
	class bldr_clutter: HouseNoDestruct
	{
		physLayer = "item_small";
		carveNavmesh = 0;
	};
	class bldr_plnt_c_beech_big_summer: bldr_clutter
	{
		scope = 2;
		model = "DZ\plants_bliss\clutter\c_beech_big_summer.p3d";
	};
	class bldr_plnt_c_beech_mid_summer: bldr_clutter
	{
		scope = 2;
		model = "DZ\plants_bliss\clutter\c_beech_mid_summer.p3d";
	};
	class bldr_plnt_c_beech_small_summer: bldr_clutter
	{
		scope = 2;
		model = "DZ\plants_bliss\clutter\c_beech_small_summer.p3d";
	};
	class bldr_plnt_c_branches_deciduous_summer: bldr_clutter
	{
		scope = 2;
		model = "DZ\plants_bliss\clutter\c_branches_deciduous_summer.p3d";
	};
	class bldr_plnt_c_branches_picea_summer: bldr_clutter
	{
		scope = 2;
		model = "DZ\plants_bliss\clutter\c_branches_picea_summer.p3d";
	};
	class bldr_plnt_c_branchesBig_picea_summer: bldr_clutter
	{
		scope = 2;
		model = "DZ\plants_bliss\clutter\c_branchesBig_picea_summer.p3d";
	};
	class bldr_plnt_c_ConcreteGrass_bunch_summer: bldr_clutter
	{
		scope = 2;
		model = "DZ\plants_bliss\clutter\c_ConcreteGrass_bunch_summer.p3d";
	};
	class bldr_plnt_c_Cones_summer: bldr_clutter
	{
		scope = 2;
		model = "DZ\plants_bliss\clutter\c_Cones_summer.p3d";
	};
	class bldr_plnt_c_DirtGrass_bunch_summer: bldr_clutter
	{
		scope = 2;
		model = "DZ\plants_bliss\clutter\c_DirtGrass_bunch_summer.p3d";
	};
	class bldr_plnt_c_Elytrigia_green_summer: bldr_clutter
	{
		scope = 2;
		model = "DZ\plants_bliss\clutter\c_Elytrigia_green_summer.p3d";
	};
	class bldr_plnt_c_Elytrigia_summer: bldr_clutter
	{
		scope = 2;
		model = "DZ\plants_bliss\clutter\c_Elytrigia_summer.p3d";
	};
	class bldr_plnt_c_ElytrigiaDirt_summer: bldr_clutter
	{
		scope = 2;
		model = "DZ\plants_bliss\clutter\c_ElytrigiaDirt_summer.p3d";
	};
	class bldr_plnt_c_ElytrigiaTall_summer: bldr_clutter
	{
		scope = 2;
		model = "DZ\plants_bliss\clutter\c_ElytrigiaTall_summer.p3d";
	};
	class bldr_plnt_c_ElytrigiaTall2_summer: bldr_clutter
	{
		scope = 2;
		model = "DZ\plants_bliss\clutter\c_ElytrigiaTall2_summer.p3d";
	};
	class bldr_plnt_c_fern_summer: bldr_clutter
	{
		scope = 2;
		model = "DZ\plants_bliss\clutter\c_fern_summer.p3d";
	};
	class bldr_plnt_c_fernTall_summer: bldr_clutter
	{
		scope = 2;
		model = "DZ\plants_bliss\clutter\c_fernTall_summer.p3d";
	};
	class bldr_plnt_c_Grass_flower1_summer: bldr_clutter
	{
		scope = 2;
		model = "DZ\plants_bliss\clutter\c_Grass_flower1_summer.p3d";
	};
	class bldr_plnt_c_Grass_summer: bldr_clutter
	{
		scope = 2;
		model = "DZ\plants_bliss\clutter\c_Grass_summer.p3d";
	};
	class bldr_plnt_c_GrassAnthoxanthum_bended_summer: bldr_clutter
	{
		scope = 2;
		model = "DZ\plants_bliss\clutter\c_GrassAnthoxanthum_bended_summer.p3d";
	};
	class bldr_plnt_c_GrassAnthoxanthum_summer: bldr_clutter
	{
		scope = 2;
		model = "DZ\plants_bliss\clutter\c_GrassAnthoxanthum_summer.p3d";
	};
	class bldr_plnt_c_GrassBaseCarduus_f_summer: bldr_clutter
	{
		scope = 2;
		model = "DZ\plants_bliss\clutter\c_GrassBaseCarduus_f_summer.p3d";
	};
	class bldr_plnt_c_GrassBaseHypericum_summer: bldr_clutter
	{
		scope = 2;
		model = "DZ\plants_bliss\clutter\c_GrassBaseHypericum_summer.p3d";
	};
	class bldr_plnt_c_GrassCropCarduus_f_summer: bldr_clutter
	{
		scope = 2;
		model = "DZ\plants_bliss\clutter\c_GrassCropCarduus_f_summer.p3d";
	};
	class bldr_plnt_c_GrassCropHypericum_summer: bldr_clutter
	{
		scope = 2;
		model = "DZ\plants_bliss\clutter\c_GrassCropHypericum_summer.p3d";
	};
	class bldr_plnt_c_GrassDry_summer: bldr_clutter
	{
		scope = 2;
		model = "DZ\plants_bliss\clutter\c_GrassDry_summer.p3d";
	};
	class bldr_plnt_c_GrassDry2_summer: bldr_clutter
	{
		scope = 2;
		model = "DZ\plants_bliss\clutter\c_GrassDry2_summer.p3d";
	};
	class bldr_plnt_c_GrassDry3_summer: bldr_clutter
	{
		scope = 2;
		model = "DZ\plants_bliss\clutter\c_GrassDry3_summer.p3d";
	};
	class bldr_plnt_c_GrassDryCarduus_f_summer: bldr_clutter
	{
		scope = 2;
		model = "DZ\plants_bliss\clutter\c_GrassDryCarduus_f_summer.p3d";
	};
	class bldr_plnt_c_GrassDryHypericum_summer: bldr_clutter
	{
		scope = 2;
		model = "DZ\plants_bliss\clutter\c_GrassDryHypericum_summer.p3d";
	};
	class bldr_plnt_c_GrassDryPlant1_summer: bldr_clutter
	{
		scope = 2;
		model = "DZ\plants_bliss\clutter\c_GrassDryPlant1_summer.p3d";
	};
	class bldr_plnt_c_GrassDryTall_summer: bldr_clutter
	{
		scope = 2;
		model = "DZ\plants_bliss\clutter\c_GrassDryTall_summer.p3d";
	};
	class bldr_plnt_c_GrassDryTall2_summer: bldr_clutter
	{
		scope = 2;
		model = "DZ\plants_bliss\clutter\c_GrassDryTall2_summer.p3d";
	};
	class bldr_plnt_c_GrassDryTall3_summer: bldr_clutter
	{
		scope = 2;
		model = "DZ\plants_bliss\clutter\c_GrassDryTall3_summer.p3d";
	};
	class bldr_plnt_c_GrassFlowersPurple_summer: bldr_clutter
	{
		scope = 2;
		model = "DZ\plants_bliss\clutter\c_GrassFlowersPurple_summer.p3d";
	};
	class bldr_plnt_c_GrassFlowersPurpleShort_summer: bldr_clutter
	{
		scope = 2;
		model = "DZ\plants_bliss\clutter\c_GrassFlowersPurpleShort_summer.p3d";
	};
	class bldr_plnt_c_GrassFlowersPurpleTall_summer: bldr_clutter
	{
		scope = 2;
		model = "DZ\plants_bliss\clutter\c_GrassFlowersPurpleTall_summer.p3d";
	};
	class bldr_plnt_c_GrassFlowersWhite_summer: bldr_clutter
	{
		scope = 2;
		model = "DZ\plants_bliss\clutter\c_GrassFlowersWhite_summer.p3d";
	};
	class bldr_plnt_c_GrassFlowersWhiteShort_summer: bldr_clutter
	{
		scope = 2;
		model = "DZ\plants_bliss\clutter\c_GrassFlowersWhiteShort_summer.p3d";
	};
	class bldr_plnt_c_GrassFlowersWhiteTall_summer: bldr_clutter
	{
		scope = 2;
		model = "DZ\plants_bliss\clutter\c_GrassFlowersWhiteTall_summer.p3d";
	};
	class bldr_plnt_c_GrassFlowersYellow_summer: bldr_clutter
	{
		scope = 2;
		model = "DZ\plants_bliss\clutter\c_GrassFlowersYellow_summer.p3d";
	};
	class bldr_plnt_c_GrassFlowersYellowShort_summer: bldr_clutter
	{
		scope = 2;
		model = "DZ\plants_bliss\clutter\c_GrassFlowersYellowShort_summer.p3d";
	};
	class bldr_plnt_c_GrassFlowersYellowTall_summer: bldr_clutter
	{
		scope = 2;
		model = "DZ\plants_bliss\clutter\c_GrassFlowersYellowTall_summer.p3d";
	};
	class bldr_plnt_c_GrassLow_summer: bldr_clutter
	{
		scope = 2;
		model = "DZ\plants_bliss\clutter\c_GrassLow_summer.p3d";
	};
	class bldr_plnt_c_GrassPlant1_summer: bldr_clutter
	{
		scope = 2;
		model = "DZ\plants_bliss\clutter\c_GrassPlant1_summer.p3d";
	};
	class bldr_plnt_c_GrassPlant2_summer: bldr_clutter
	{
		scope = 2;
		model = "DZ\plants_bliss\clutter\c_GrassPlant2_summer.p3d";
	};
	class bldr_plnt_c_GrassSmall_summer: HouseNoDestruct
	{
		scope = 2;
		model = "DZ\plants_bliss\clutter\c_GrassSmall_summer.p3d";
	};
	class bldr_plnt_c_GrassSmallConc_summer: bldr_clutter
	{
		scope = 2;
		model = "DZ\plants_bliss\clutter\c_GrassSmallConc_summer.p3d";
	};
	class bldr_plnt_c_GrassTall2_summer: bldr_clutter
	{
		scope = 2;
		model = "DZ\plants_bliss\clutter\c_GrassTall2_summer.p3d";
	};
	class bldr_plnt_c_GrassTall3_summer: bldr_clutter
	{
		scope = 2;
		model = "DZ\plants_bliss\clutter\c_GrassTall3_summer.p3d";
	};
	class bldr_plnt_c_GrassTiny_summer: bldr_clutter
	{
		scope = 2;
		model = "DZ\plants_bliss\clutter\c_GrassTiny_summer.p3d";
	};
	class bldr_plnt_c_GrassTinyConc_summer: bldr_clutter
	{
		scope = 2;
		model = "DZ\plants_bliss\clutter\c_GrassTinyConc_summer.p3d";
	};
	class bldr_plnt_c_leaves_dead_half_summer: bldr_clutter
	{
		scope = 2;
		model = "DZ\plants_bliss\clutter\c_leaves_dead_half_summer.p3d";
	};
	class bldr_plnt_c_leaves_dead_summer: bldr_clutter
	{
		scope = 2;
		model = "DZ\plants_bliss\clutter\c_leaves_dead_summer.p3d";
	};
	class bldr_plnt_c_Pines_summer: bldr_clutter
	{
		scope = 2;
		model = "DZ\plants_bliss\clutter\c_Pines_summer.p3d";
	};
	class bldr_plnt_c_plantsSmall1_summer: bldr_clutter
	{
		scope = 2;
		model = "DZ\plants_bliss\clutter\c_plantsSmall1_summer.p3d";
	};
	class bldr_plnt_c_plantsSmall2_summer: bldr_clutter
	{
		scope = 2;
		model = "DZ\plants_bliss\clutter\c_plantsSmall2_summer.p3d";
	};
	class bldr_plnt_c_stubble_summer: bldr_clutter
	{
		scope = 2;
		model = "DZ\plants_bliss\clutter\c_stubble_summer.p3d";
	};
	class bldr_plnt_c_Vaccinium_summer: bldr_clutter
	{
		scope = 2;
		model = "DZ\plants_bliss\clutter\c_Vaccinium_summer.p3d";
	};
	class bldr_plnt_c_wheat_summer: bldr_clutter
	{
		scope = 2;
		model = "DZ\plants_bliss\clutter\c_wheat_summer.p3d";
	};
	class bldr_plnt_c_wheat1_summer: bldr_clutter
	{
		scope = 2;
		model = "DZ\plants_bliss\clutter\c_wheat1_summer.p3d";
	};
	class bldr_plnt_c_wheat2: bldr_clutter
	{
		scope = 2;
		model = "DZ\plants_bliss\clutter\c_wheat2.p3d";
	};
	class bldr_plnt_c_wheatTall_summer: bldr_clutter
	{
		scope = 2;
		model = "DZ\plants_bliss\clutter\c_wheatTall_summer.p3d";
	};
	class bldr_plnt_b_PiceaAbies_1f_summer: HouseNoDestruct
	{
		scope = 2;
		model = "DZ\plants_bliss\tree\b_PiceaAbies_1f_summer.p3d";
	};
	class bldr_plnt_b_PiceaAbies_1fb_summer: HouseNoDestruct
	{
		scope = 2;
		model = "DZ\plants_bliss\tree\b_PiceaAbies_1fb_summer.p3d";
	};
	class bldr_plnt_d_BetulaPendula_fallen: HouseNoDestruct
	{
		scope = 2;
		model = "DZ\plants_bliss\tree\d_BetulaPendula_fallen.p3d";
	};
	class bldr_plnt_d_BetulaPendula_stump: HouseNoDestruct
	{
		scope = 2;
		model = "DZ\plants_bliss\tree\d_BetulaPendula_stump.p3d";
	};
	class bldr_plnt_d_piceaabies_fallend: HouseNoDestruct
	{
		scope = 2;
		model = "DZ\plants_bliss\tree\d_piceaabies_fallend.p3d";
	};
	class bldr_plnt_d_piceaabies_fallene: HouseNoDestruct
	{
		scope = 2;
		model = "DZ\plants_bliss\tree\d_piceaabies_fallene.p3d";
	};
	class bldr_plnt_d_piceaabies_stumpc: HouseNoDestruct
	{
		scope = 2;
		model = "DZ\plants_bliss\tree\d_piceaabies_stumpc.p3d";
	};
	class bldr_plnt_d_piceaabies_stumpd: HouseNoDestruct
	{
		scope = 2;
		model = "DZ\plants_bliss\tree\d_piceaabies_stumpd.p3d";
	};
	class bldr_plnt_t_acer_2s_summer: HouseNoDestruct
	{
		scope = 2;
		model = "DZ\plants_bliss\tree\t_acer_2s_summer.p3d";
	};
	class bldr_plnt_t_BetulaPendulaE_1s_summer: HouseNoDestruct
	{
		scope = 2;
		model = "DZ\plants_bliss\tree\t_BetulaPendulaE_1s_summer.p3d";
	};
	class bldr_plnt_t_BetulaPendulaE_2f_summer: HouseNoDestruct
	{
		scope = 2;
		model = "DZ\plants_bliss\tree\t_BetulaPendulaE_2f_summer.p3d";
	};
	class bldr_plnt_t_BetulaPendulaE_2s_summer: HouseNoDestruct
	{
		scope = 2;
		model = "DZ\plants_bliss\tree\t_BetulaPendulaE_2s_summer.p3d";
	};
	class bldr_plnt_t_BetulaPendulaE_2w_summer: HouseNoDestruct
	{
		scope = 2;
		model = "DZ\plants_bliss\tree\t_BetulaPendulaE_2w_summer.p3d";
	};
	class bldr_plnt_t_BetulaPendulaE_3f_summer: HouseNoDestruct
	{
		scope = 2;
		model = "DZ\plants_bliss\tree\t_BetulaPendulaE_3f_summer.p3d";
	};
	class bldr_plnt_t_BetulaPendulaE_3s_summer: HouseNoDestruct
	{
		scope = 2;
		model = "DZ\plants_bliss\tree\t_BetulaPendulaE_3s_summer.p3d";
	};
	class bldr_plnt_t_FagusSylvatica_1f_summer: HouseNoDestruct
	{
		scope = 2;
		model = "DZ\plants_bliss\tree\t_FagusSylvatica_1f_summer.p3d";
	};
	class bldr_plnt_t_FagusSylvatica_1fb_summer: HouseNoDestruct
	{
		scope = 2;
		model = "DZ\plants_bliss\tree\t_FagusSylvatica_1fb_summer.p3d";
	};
	class bldr_plnt_t_FagusSylvatica_1fc_summer: HouseNoDestruct
	{
		scope = 2;
		model = "DZ\plants_bliss\tree\t_FagusSylvatica_1fc_summer.p3d";
	};
	class bldr_plnt_t_FagusSylvatica_1fd_summer: HouseNoDestruct
	{
		scope = 2;
		model = "DZ\plants_bliss\tree\t_FagusSylvatica_1fd_summer.p3d";
	};
	class bldr_plnt_t_FagusSylvatica_1fe_summer: HouseNoDestruct
	{
		scope = 2;
		model = "DZ\plants_bliss\tree\t_FagusSylvatica_1fe_summer.p3d";
	};
	class bldr_plnt_t_FagusSylvatica_1s_summer: HouseNoDestruct
	{
		scope = 2;
		model = "DZ\plants_bliss\tree\t_FagusSylvatica_1s_summer.p3d";
	};
	class bldr_plnt_t_FagusSylvatica_2d_summer: HouseNoDestruct
	{
		scope = 2;
		model = "DZ\plants_bliss\tree\t_FagusSylvatica_2d_summer.p3d";
	};
	class bldr_plnt_t_FagusSylvatica_2f_summer: HouseNoDestruct
	{
		scope = 2;
		model = "DZ\plants_bliss\tree\t_FagusSylvatica_2f_summer.p3d";
	};
	class bldr_plnt_t_FagusSylvatica_2fb_summer: HouseNoDestruct
	{
		scope = 2;
		model = "DZ\plants_bliss\tree\t_FagusSylvatica_2fb_summer.p3d";
	};
	class bldr_plnt_t_FagusSylvatica_2fc_summer: HouseNoDestruct
	{
		scope = 2;
		model = "DZ\plants_bliss\tree\t_FagusSylvatica_2fc_summer.p3d";
	};
	class bldr_plnt_t_FagusSylvatica_2s_summer: HouseNoDestruct
	{
		scope = 2;
		model = "DZ\plants_bliss\tree\t_FagusSylvatica_2s_summer.p3d";
	};
	class bldr_plnt_t_FagusSylvatica_2sb_summer: HouseNoDestruct
	{
		scope = 2;
		model = "DZ\plants_bliss\tree\t_FagusSylvatica_2sb_summer.p3d";
	};
	class bldr_plnt_t_FagusSylvatica_2sb_Trail_B_summer: HouseNoDestruct
	{
		scope = 2;
		model = "DZ\plants_bliss\tree\t_FagusSylvatica_2sb_Trail_B_summer.p3d";
	};
	class bldr_plnt_t_FagusSylvatica_2sb_Trail_G_summer: HouseNoDestruct
	{
		scope = 2;
		model = "DZ\plants_bliss\tree\t_FagusSylvatica_2sb_Trail_G_summer.p3d";
	};
	class bldr_plnt_t_FagusSylvatica_2sb_Trail_R_summer: HouseNoDestruct
	{
		scope = 2;
		model = "DZ\plants_bliss\tree\t_FagusSylvatica_2sb_Trail_R_summer.p3d";
	};
	class bldr_plnt_t_FagusSylvatica_2sb_Trail_Y_summer: HouseNoDestruct
	{
		scope = 2;
		model = "DZ\plants_bliss\tree\t_FagusSylvatica_2sb_Trail_Y_summer.p3d";
	};
	class bldr_plnt_t_FagusSylvatica_3d_summer: HouseNoDestruct
	{
		scope = 2;
		model = "DZ\plants_bliss\tree\t_FagusSylvatica_3d_summer.p3d";
	};
	class bldr_plnt_t_FagusSylvatica_3f_summer: HouseNoDestruct
	{
		scope = 2;
		model = "DZ\plants_bliss\tree\t_FagusSylvatica_3f_summer.p3d";
	};
	class bldr_plnt_t_FagusSylvatica_3fb_summer: HouseNoDestruct
	{
		scope = 2;
		model = "DZ\plants_bliss\tree\t_FagusSylvatica_3fb_summer.p3d";
	};
	class bldr_plnt_t_FagusSylvatica_3s_summer: HouseNoDestruct
	{
		scope = 2;
		model = "DZ\plants_bliss\tree\t_FagusSylvatica_3s_summer.p3d";
	};
	class bldr_plnt_t_FagusSylvaticaE_2s_summer: HouseNoDestruct
	{
		scope = 2;
		model = "DZ\plants_bliss\tree\t_FagusSylvaticaE_2s_summer.p3d";
	};
	class bldr_plnt_t_FagusSylvaticaE_3f_summer: HouseNoDestruct
	{
		scope = 2;
		model = "DZ\plants_bliss\tree\t_FagusSylvaticaE_3f_summer.p3d";
	};
	class bldr_plnt_t_juglansRegia_2s_summer: HouseNoDestruct
	{
		scope = 2;
		model = "DZ\plants_bliss\tree\t_juglansRegia_2s_summer.p3d";
	};
	class bldr_plnt_t_juglansRegia_3s_summer: HouseNoDestruct
	{
		scope = 2;
		model = "DZ\plants_bliss\tree\t_juglansRegia_3s_summer.p3d";
	};
	class bldr_plnt_t_LarixDecidua_1f_summer: HouseNoDestruct
	{
		scope = 2;
		model = "DZ\plants_bliss\tree\t_LarixDecidua_1f_summer.p3d";
	};
	class bldr_plnt_t_LarixDecidua_1s_summer: HouseNoDestruct
	{
		scope = 2;
		model = "DZ\plants_bliss\tree\t_LarixDecidua_1s_summer.p3d";
	};
	class bldr_plnt_t_LarixDecidua_2f_summer: HouseNoDestruct
	{
		scope = 2;
		model = "DZ\plants_bliss\tree\t_LarixDecidua_2f_summer.p3d";
	};
	class bldr_plnt_t_LarixDecidua_2fb_summer: HouseNoDestruct
	{
		scope = 2;
		model = "DZ\plants_bliss\tree\t_LarixDecidua_2fb_summer.p3d";
	};
	class bldr_plnt_t_LarixDecidua_2s_summer: HouseNoDestruct
	{
		scope = 2;
		model = "DZ\plants_bliss\tree\t_LarixDecidua_2s_summer.p3d";
	};
	class bldr_plnt_t_LarixDecidua_3f_summer: HouseNoDestruct
	{
		scope = 2;
		model = "DZ\plants_bliss\tree\t_LarixDecidua_3f_summer.p3d";
	};
	class bldr_plnt_t_LarixDecidua_3fb_summer: HouseNoDestruct
	{
		scope = 2;
		model = "DZ\plants_bliss\tree\t_LarixDecidua_3fb_summer.p3d";
	};
	class bldr_plnt_t_malusDomestica_2s_summer: HouseNoDestruct
	{
		scope = 2;
		model = "DZ\plants_bliss\tree\t_malusDomestica_2s_summer.p3d";
	};
	class bldr_plnt_t_malusDomestica_3s_summer: HouseNoDestruct
	{
		scope = 2;
		model = "DZ\plants_bliss\tree\t_malusDomestica_3s_summer.p3d";
	};
	class bldr_plnt_t_PiceaAbies_1f_summer: HouseNoDestruct
	{
		scope = 2;
		model = "DZ\plants_bliss\tree\t_PiceaAbies_1f_summer.p3d";
	};
	class bldr_plnt_t_PiceaAbies_1s_summer: HouseNoDestruct
	{
		scope = 2;
		model = "DZ\plants_bliss\tree\t_PiceaAbies_1s_summer.p3d";
	};
	class bldr_plnt_t_PiceaAbies_1sb_summer: HouseNoDestruct
	{
		scope = 2;
		model = "DZ\plants_bliss\tree\t_PiceaAbies_1sb_summer.p3d";
	};
	class bldr_plnt_t_PiceaAbies_2f_summer: HouseNoDestruct
	{
		scope = 2;
		model = "DZ\plants_bliss\tree\t_PiceaAbies_2f_summer.p3d";
	};
	class bldr_plnt_t_PiceaAbies_2fb_summer: HouseNoDestruct
	{
		scope = 2;
		model = "DZ\plants_bliss\tree\t_PiceaAbies_2fb_summer.p3d";
	};
	class bldr_plnt_t_PiceaAbies_2s_summer: HouseNoDestruct
	{
		scope = 2;
		model = "DZ\plants_bliss\tree\t_PiceaAbies_2s_summer.p3d";
	};
	class bldr_plnt_t_PiceaAbies_2sb_summer: HouseNoDestruct
	{
		scope = 2;
		model = "DZ\plants_bliss\tree\t_PiceaAbies_2sb_summer.p3d";
	};
	class bldr_plnt_t_PiceaAbies_3f_summer: HouseNoDestruct
	{
		scope = 2;
		model = "DZ\plants_bliss\tree\t_PiceaAbies_3f_summer.p3d";
	};
	class bldr_plnt_t_PiceaAbies_3f_Trail_B_summer: HouseNoDestruct
	{
		scope = 2;
		model = "DZ\plants_bliss\tree\t_PiceaAbies_3f_Trail_B_summer.p3d";
	};
	class bldr_plnt_t_PiceaAbies_3f_Trail_G_summer: HouseNoDestruct
	{
		scope = 2;
		model = "DZ\plants_bliss\tree\t_PiceaAbies_3f_Trail_G_summer.p3d";
	};
	class bldr_plnt_t_PiceaAbies_3f_Trail_R_summer: HouseNoDestruct
	{
		scope = 2;
		model = "DZ\plants_bliss\tree\t_PiceaAbies_3f_Trail_R_summer.p3d";
	};
	class bldr_plnt_t_PiceaAbies_3f_Trail_Y_summer: HouseNoDestruct
	{
		scope = 2;
		model = "DZ\plants_bliss\tree\t_PiceaAbies_3f_Trail_Y_summer.p3d";
	};
	class bldr_plnt_t_PiceaAbies_3s_summer: HouseNoDestruct
	{
		scope = 2;
		model = "DZ\plants_bliss\tree\t_PiceaAbies_3s_summer.p3d";
	};
	class bldr_plnt_t_PinusSylvestris_1f_summer: HouseNoDestruct
	{
		scope = 2;
		model = "DZ\plants_bliss\tree\t_PinusSylvestris_1f_summer.p3d";
	};
	class bldr_plnt_t_PinusSylvestris_1s_summer: HouseNoDestruct
	{
		scope = 2;
		model = "DZ\plants_bliss\tree\t_PinusSylvestris_1s_summer.p3d";
	};
	class bldr_plnt_t_PinusSylvestris_2f_summer: HouseNoDestruct
	{
		scope = 2;
		model = "DZ\plants_bliss\tree\t_PinusSylvestris_2f_summer.p3d";
	};
	class bldr_plnt_t_PinusSylvestris_2fb_summer: HouseNoDestruct
	{
		scope = 2;
		model = "DZ\plants_bliss\tree\t_PinusSylvestris_2fb_summer.p3d";
	};
	class bldr_plnt_t_PinusSylvestris_2s_summer: HouseNoDestruct
	{
		scope = 2;
		model = "DZ\plants_bliss\tree\t_PinusSylvestris_2s_summer.p3d";
	};
	class bldr_plnt_t_PinusSylvestris_2sb_summer: HouseNoDestruct
	{
		scope = 2;
		model = "DZ\plants_bliss\tree\t_PinusSylvestris_2sb_summer.p3d";
	};
	class bldr_plnt_t_PinusSylvestris_3d_summer: HouseNoDestruct
	{
		scope = 2;
		model = "DZ\plants_bliss\tree\t_PinusSylvestris_3d_summer.p3d";
	};
	class bldr_plnt_t_PinusSylvestris_3f_summer: HouseNoDestruct
	{
		scope = 2;
		model = "DZ\plants_bliss\tree\t_PinusSylvestris_3f_summer.p3d";
	};
	class bldr_plnt_t_PinusSylvestris_3fb_summer: HouseNoDestruct
	{
		scope = 2;
		model = "DZ\plants_bliss\tree\t_PinusSylvestris_3fb_summer.p3d";
	};
	class bldr_plnt_t_PinusSylvestris_3fc_summer: HouseNoDestruct
	{
		scope = 2;
		model = "DZ\plants_bliss\tree\t_PinusSylvestris_3fc_summer.p3d";
	};
	class bldr_plnt_t_PinusSylvestris_3s_summer: HouseNoDestruct
	{
		scope = 2;
		model = "DZ\plants_bliss\tree\t_PinusSylvestris_3s_summer.p3d";
	};
	class bldr_plnt_t_populusNigra_3sb_summer: HouseNoDestruct
	{
		scope = 2;
		model = "DZ\plants_bliss\tree\t_populusNigra_3sb_summer.p3d";
	};
	class bldr_plnt_t_pyrusCommunis_2s_summer: HouseNoDestruct
	{
		scope = 2;
		model = "DZ\plants_bliss\tree\t_pyrusCommunis_2s_summer.p3d";
	};
	class bldr_plnt_t_pyrusCommunis_3s_summer: HouseNoDestruct
	{
		scope = 2;
		model = "DZ\plants_bliss\tree\t_pyrusCommunis_3s_summer.p3d";
	};
	class bldr_plnt_t_salixAlba_2sb_summer: HouseNoDestruct
	{
		scope = 2;
		model = "DZ\plants_bliss\tree\t_salixAlba_2sb_summer.p3d";
	};
	class bldr_plnt_t_sorbus_2s_summer: HouseNoDestruct
	{
		scope = 2;
		model = "DZ\plants_bliss\tree\t_sorbus_2s_summer.p3d";
	};
};
