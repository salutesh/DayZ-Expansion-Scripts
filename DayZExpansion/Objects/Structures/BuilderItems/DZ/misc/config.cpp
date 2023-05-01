#define _ARMA_

class CfgPatches
{
	class BuilderItems_Misc
	{
		units[] = {};
		weapons[] = {};
		requiredVersion = 0.1;
		requiredAddons[] = {"DZ_Data","DZ_Gear_Camping","DZ_Gear_Consumables","DZ_Gear_Cooking","DZ_Structures_Signs"};
	};
};
class CfgVehicles
{
	class HouseNoDestruct;
	class BarrelHoles_Red;
	class Fireplace;
	class Chemlight_ColorBase;
	class Chemlight_White;
	class Chemlight_Red;
	class Chemlight_Green;
	class Chemlight_Blue;
	class Chemlight_Yellow;
	class PortableGasLamp;
	class Candle;
	class Spotlight;
	class bldr_expansion_flare_blue: HouseNoDestruct
	{
		scope = 1;
		model = "\DZ\data\lightpoint.p3d";
	};
	class bldr_expansion_flare_red: HouseNoDestruct
	{
		scope = 1;
		model = "\DZ\data\lightpoint.p3d";
	};
	class bldr_expansion_flare_white: HouseNoDestruct
	{
		scope = 1;
		model = "\DZ\data\lightpoint.p3d";
	};
	class bldr_expansion_flare_yellow: HouseNoDestruct
	{
		scope = 1;
		model = "\DZ\data\lightpoint.p3d";
	};
	class bldr_sign_areoclub: HouseNoDestruct
	{
		scope = 1;
		model = "\DZ\structures\signs\misc\sign_areoclub.p3d";
	};
	class bldr_sign_build_altar: HouseNoDestruct
	{
		scope = 1;
		model = "\DZ\structures\signs\misc\sign_build_altar.p3d";
	};
	class bldr_sign_build_bar: HouseNoDestruct
	{
		scope = 1;
		model = "\DZ\structures\signs\misc\sign_build_bar.p3d";
	};
	class bldr_sign_build_bes: HouseNoDestruct
	{
		scope = 1;
		model = "\DZ\structures\signs\misc\sign_build_bes.p3d";
	};
	class bldr_sign_build_cerpecat: HouseNoDestruct
	{
		scope = 1;
		model = "\DZ\structures\signs\misc\sign_build_cerpecat.p3d";
	};
	class bldr_sign_build_chernobus: HouseNoDestruct
	{
		scope = 1;
		model = "\DZ\structures\signs\misc\sign_build_chernobus.p3d";
	};
	class bldr_sign_build_hotel: HouseNoDestruct
	{
		scope = 1;
		model = "\DZ\structures\signs\misc\sign_build_hotel.p3d";
	};
	class bldr_sign_build_produkty: HouseNoDestruct
	{
		scope = 1;
		model = "\DZ\structures\signs\misc\sign_build_produkty.p3d";
	};
	class bldr_sign_build_tec: HouseNoDestruct
	{
		scope = 1;
		model = "\DZ\structures\signs\misc\sign_build_tec.p3d";
	};
	class bldr_sign_build_zvezda: HouseNoDestruct
	{
		scope = 1;
		model = "\DZ\structures\signs\misc\sign_build_zvezda.p3d";
	};
	class bldr_sign_camp_100l: HouseNoDestruct
	{
		scope = 1;
		model = "\DZ\structures\signs\misc\sign_camp_100l.p3d";
	};
	class bldr_sign_camp_100r: HouseNoDestruct
	{
		scope = 1;
		model = "\DZ\structures\signs\misc\sign_camp_100r.p3d";
	};
	class bldr_sign_camp_300l: HouseNoDestruct
	{
		scope = 1;
		model = "\DZ\structures\signs\misc\sign_camp_300l.p3d";
	};
	class bldr_sign_camp_300r: HouseNoDestruct
	{
		scope = 1;
		model = "\DZ\structures\signs\misc\sign_camp_300r.p3d";
	};
	class bldr_sign_camp_arsenevo: HouseNoDestruct
	{
		scope = 1;
		model = "\DZ\structures\signs\misc\sign_camp_arsenevo.p3d";
	};
	class bldr_sign_camp_builder: HouseNoDestruct
	{
		scope = 1;
		model = "\DZ\structures\signs\misc\sign_camp_builder.p3d";
	};
	class bldr_sign_camp_camomile: HouseNoDestruct
	{
		scope = 1;
		model = "\DZ\structures\signs\misc\sign_camp_camomile.p3d";
	};
	class bldr_sign_camp_comet: HouseNoDestruct
	{
		scope = 1;
		model = "\DZ\structures\signs\misc\sign_camp_comet.p3d";
	};
	class bldr_sign_camp_friendship: HouseNoDestruct
	{
		scope = 1;
		model = "\DZ\structures\signs\misc\sign_camp_friendship.p3d";
	};
	class bldr_sign_camp_hope: HouseNoDestruct
	{
		scope = 1;
		model = "\DZ\structures\signs\misc\sign_camp_hope.p3d";
	};
	class bldr_sign_camp_pioneer: HouseNoDestruct
	{
		scope = 1;
		model = "\DZ\structures\signs\misc\sign_camp_pioneer.p3d";
	};
	class bldr_sign_camp_prchernarus: HouseNoDestruct
	{
		scope = 1;
		model = "\DZ\structures\signs\misc\sign_camp_prchernarus.p3d";
	};
	class bldr_sign_camp_scholar: HouseNoDestruct
	{
		scope = 1;
		model = "\DZ\structures\signs\misc\sign_camp_scholar.p3d";
	};
	class bldr_sign_camp_victory: HouseNoDestruct
	{
		scope = 1;
		model = "\DZ\structures\signs\misc\sign_camp_victory.p3d";
	};
	class bldr_sign_city_chernogorsk: HouseNoDestruct
	{
		scope = 1;
		model = "\DZ\structures\signs\misc\sign_city_chernogorsk.p3d";
	};
	class bldr_sign_city_elektro: HouseNoDestruct
	{
		scope = 1;
		model = "\DZ\structures\signs\misc\sign_city_elektro.p3d";
	};
	class bldr_sign_city_novo: HouseNoDestruct
	{
		scope = 1;
		model = "\DZ\structures\signs\misc\sign_city_novo.p3d";
	};
	class bldr_sign_decal_afs_01: HouseNoDestruct
	{
		scope = 1;
		model = "\DZ\structures\signs\misc\sign_decal_afs_01.p3d";
	};
	class bldr_sign_decal_afs_02: HouseNoDestruct
	{
		scope = 1;
		model = "\DZ\structures\signs\misc\sign_decal_afs_02.p3d";
	};
	class bldr_sign_decal_afs_03: HouseNoDestruct
	{
		scope = 1;
		model = "\DZ\structures\signs\misc\sign_decal_afs_03.p3d";
	};
	class bldr_sign_decal_afs_04: HouseNoDestruct
	{
		scope = 1;
		model = "\DZ\structures\signs\misc\sign_decal_afs_04.p3d";
	};
	class bldr_sign_decal_afs_05: HouseNoDestruct
	{
		scope = 1;
		model = "\DZ\structures\signs\misc\sign_decal_afs_05.p3d";
	};
	class bldr_sign_decal_afs_06: HouseNoDestruct
	{
		scope = 1;
		model = "\DZ\structures\signs\misc\sign_decal_afs_06.p3d";
	};
	class bldr_sign_decal_afs_07: HouseNoDestruct
	{
		scope = 1;
		model = "\DZ\structures\signs\misc\sign_decal_afs_07.p3d";
	};
	class bldr_sign_decal_afs_08: HouseNoDestruct
	{
		scope = 1;
		model = "\DZ\structures\signs\misc\sign_decal_afs_08.p3d";
	};
	class bldr_sign_decal_afs_09: HouseNoDestruct
	{
		scope = 1;
		model = "\DZ\structures\signs\misc\sign_decal_afs_09.p3d";
	};
	class bldr_sign_decal_afs_10: HouseNoDestruct
	{
		scope = 1;
		model = "\DZ\structures\signs\misc\sign_decal_afs_10.p3d";
	};
	class bldr_sign_decal_afs_11: HouseNoDestruct
	{
		scope = 1;
		model = "\DZ\structures\signs\misc\sign_decal_afs_11.p3d";
	};
	class bldr_sign_decal_afs_12: HouseNoDestruct
	{
		scope = 1;
		model = "\DZ\structures\signs\misc\sign_decal_afs_12.p3d";
	};
	class bldr_sign_decal_afs_13: HouseNoDestruct
	{
		scope = 1;
		model = "\DZ\structures\signs\misc\sign_decal_afs_13.p3d";
	};
	class bldr_sign_decal_afs_14: HouseNoDestruct
	{
		scope = 1;
		model = "\DZ\structures\signs\misc\sign_decal_afs_14.p3d";
	};
	class bldr_sign_decal_afs_15: HouseNoDestruct
	{
		scope = 1;
		model = "\DZ\structures\signs\misc\sign_decal_afs_15.p3d";
	};
	class bldr_sign_decal_afs_16: HouseNoDestruct
	{
		scope = 1;
		model = "\DZ\structures\signs\misc\sign_decal_afs_16.p3d";
	};
	class bldr_sign_decal_afs_17: HouseNoDestruct
	{
		scope = 1;
		model = "\DZ\structures\signs\misc\sign_decal_afs_17.p3d";
	};
	class bldr_sign_decal_afs_18: HouseNoDestruct
	{
		scope = 1;
		model = "\DZ\structures\signs\misc\sign_decal_afs_18.p3d";
	};
	class bldr_sign_decal_afs_19: HouseNoDestruct
	{
		scope = 1;
		model = "\DZ\structures\signs\misc\sign_decal_afs_19.p3d";
	};
	class bldr_sign_decal_afs_20: HouseNoDestruct
	{
		scope = 1;
		model = "\DZ\structures\signs\misc\sign_decal_afs_20.p3d";
	};
	class bldr_sign_decal_buffer: HouseNoDestruct
	{
		scope = 1;
		model = "\DZ\structures\signs\misc\sign_decal_buffer.p3d";
	};
	class bldr_sign_decal_campschedule: HouseNoDestruct
	{
		scope = 1;
		model = "\DZ\structures\signs\misc\sign_decal_campschedule.p3d";
	};
	class bldr_sign_decal_carserviceblackgar: HouseNoDestruct
	{
		scope = 1;
		model = "\DZ\structures\signs\misc\sign_decal_carserviceblackgar.p3d";
	};
	class bldr_sign_decal_carstoremig: HouseNoDestruct
	{
		scope = 1;
		model = "\DZ\structures\signs\misc\sign_decal_carstoremig.p3d";
	};
	class bldr_sign_decal_gov1: HouseNoDestruct
	{
		scope = 1;
		model = "\DZ\structures\signs\misc\sign_decal_gov1.p3d";
	};
	class bldr_sign_decal_gov2: HouseNoDestruct
	{
		scope = 1;
		model = "\DZ\structures\signs\misc\sign_decal_gov2.p3d";
	};
	class bldr_sign_decal_govpolice: HouseNoDestruct
	{
		scope = 1;
		model = "\DZ\structures\signs\misc\sign_decal_govpolice.p3d";
	};
	class bldr_sign_decal_govschool: HouseNoDestruct
	{
		scope = 1;
		model = "\DZ\structures\signs\misc\sign_decal_govschool.p3d";
	};
	class bldr_sign_decal_nosmoking: HouseNoDestruct
	{
		scope = 1;
		model = "\DZ\structures\signs\misc\sign_decal_nosmoking.p3d";
	};
	class bldr_sign_decal_postoffice: HouseNoDestruct
	{
		scope = 1;
		model = "\DZ\structures\signs\misc\sign_decal_postoffice.p3d";
	};
	class bldr_sign_decal_pubcz1: HouseNoDestruct
	{
		scope = 1;
		model = "\DZ\structures\signs\misc\sign_decal_pubcz1.p3d";
	};
	class bldr_sign_decal_pubcz2: HouseNoDestruct
	{
		scope = 1;
		model = "\DZ\structures\signs\misc\sign_decal_pubcz2.p3d";
	};
	class bldr_sign_decal_pubcz3: HouseNoDestruct
	{
		scope = 1;
		model = "\DZ\structures\signs\misc\sign_decal_pubcz3.p3d";
	};
	class bldr_sign_decal_pubru1: HouseNoDestruct
	{
		scope = 1;
		model = "\DZ\structures\signs\misc\sign_decal_pubru1.p3d";
	};
	class bldr_sign_decal_pubru2: HouseNoDestruct
	{
		scope = 1;
		model = "\DZ\structures\signs\misc\sign_decal_pubru2.p3d";
	};
	class bldr_sign_decal_pubru3: HouseNoDestruct
	{
		scope = 1;
		model = "\DZ\structures\signs\misc\sign_decal_pubru3.p3d";
	};
	class bldr_sign_easteregg1: HouseNoDestruct
	{
		scope = 1;
		model = "\DZ\structures\signs\misc\sign_easteregg1.p3d";
	};
	class bldr_sign_easteregg2: HouseNoDestruct
	{
		scope = 1;
		model = "\DZ\structures\signs\misc\sign_easteregg2.p3d";
	};
	class bldr_sign_nodumping: HouseNoDestruct
	{
		scope = 1;
		model = "\DZ\structures\signs\misc\sign_nodumping.p3d";
	};
	class bldr_sign_nofishing: HouseNoDestruct
	{
		scope = 1;
		model = "\DZ\structures\signs\misc\sign_nofishing.p3d";
	};
	class bldr_sign_pub_village: HouseNoDestruct
	{
		scope = 1;
		model = "\DZ\structures\signs\misc\sign_pub_village.p3d";
	};
	class bldr_sign_quarry: HouseNoDestruct
	{
		scope = 1;
		model = "\DZ\structures\signs\misc\sign_quarry.p3d";
	};
	class bldr_sign_rail_50: HouseNoDestruct
	{
		scope = 1;
		model = "\DZ\structures\signs\misc\sign_rail_50.p3d";
	};
	class bldr_sign_rail_balota: HouseNoDestruct
	{
		scope = 1;
		model = "\DZ\structures\signs\misc\sign_rail_balota.p3d";
	};
	class bldr_sign_rail_berezino: HouseNoDestruct
	{
		scope = 1;
		model = "\DZ\structures\signs\misc\sign_rail_berezino.p3d";
	};
	class bldr_sign_rail_chernogorsk: HouseNoDestruct
	{
		scope = 1;
		model = "\DZ\structures\signs\misc\sign_rail_chernogorsk.p3d";
	};
	class bldr_sign_rail_crossing_minor: HouseNoDestruct
	{
		scope = 1;
		model = "\DZ\structures\signs\misc\sign_rail_crossing_minor.p3d";
	};
	class bldr_sign_rail_crossing25: HouseNoDestruct
	{
		scope = 1;
		model = "\DZ\structures\signs\misc\sign_rail_crossing25.p3d";
	};
	class bldr_sign_rail_dobroe: HouseNoDestruct
	{
		scope = 1;
		model = "\DZ\structures\signs\misc\sign_rail_dobroe.p3d";
	};
	class bldr_sign_rail_elektrozavodsk: HouseNoDestruct
	{
		scope = 1;
		model = "\DZ\structures\signs\misc\sign_rail_elektrozavodsk.p3d";
	};
	class bldr_sign_rail_kamenka: HouseNoDestruct
	{
		scope = 1;
		model = "\DZ\structures\signs\misc\sign_rail_kamenka.p3d";
	};
	class bldr_sign_rail_kamyshovo: HouseNoDestruct
	{
		scope = 1;
		model = "\DZ\structures\signs\misc\sign_rail_kamyshovo.p3d";
	};
	class bldr_sign_rail_komarovo: HouseNoDestruct
	{
		scope = 1;
		model = "\DZ\structures\signs\misc\sign_rail_komarovo.p3d";
	};
	class bldr_sign_rail_novodmitrovsk: HouseNoDestruct
	{
		scope = 1;
		model = "\DZ\structures\signs\misc\sign_rail_novodmitrovsk.p3d";
	};
	class bldr_sign_rail_petrovka: HouseNoDestruct
	{
		scope = 1;
		model = "\DZ\structures\signs\misc\sign_rail_petrovka.p3d";
	};
	class bldr_sign_rail_platformend: HouseNoDestruct
	{
		scope = 1;
		model = "\DZ\structures\signs\misc\sign_rail_platformend.p3d";
	};
	class bldr_sign_rail_prigorodki: HouseNoDestruct
	{
		scope = 1;
		model = "\DZ\structures\signs\misc\sign_rail_prigorodki.p3d";
	};
	class bldr_sign_rail_severograd: HouseNoDestruct
	{
		scope = 1;
		model = "\DZ\structures\signs\misc\sign_rail_severograd.p3d";
	};
	class bldr_sign_rail_solnichnyi: HouseNoDestruct
	{
		scope = 1;
		model = "\DZ\structures\signs\misc\sign_rail_solnichnyi.p3d";
	};
	class bldr_sign_rail_svetloyarsk: HouseNoDestruct
	{
		scope = 1;
		model = "\DZ\structures\signs\misc\sign_rail_svetloyarsk.p3d";
	};
	class bldr_sign_rail_vavilovo: HouseNoDestruct
	{
		scope = 1;
		model = "\DZ\structures\signs\misc\sign_rail_vavilovo.p3d";
	};
	class bldr_sign_rail_zelenogorsk: HouseNoDestruct
	{
		scope = 1;
		model = "\DZ\structures\signs\misc\sign_rail_zelenogorsk.p3d";
	};
	class bldr_sign_relic_building: HouseNoDestruct
	{
		scope = 1;
		model = "\DZ\structures\signs\misc\sign_relic_building.p3d";
	};
	class bldr_sign_relic_tree: HouseNoDestruct
	{
		scope = 1;
		model = "\DZ\structures\signs\misc\sign_relic_tree.p3d";
	};
	class bldr_sign_relic_zagoria: HouseNoDestruct
	{
		scope = 1;
		model = "\DZ\structures\signs\misc\sign_relic_zagoria.p3d";
	};
	class bldr_sign_roadblock_arrow: HouseNoDestruct
	{
		scope = 1;
		model = "\DZ\structures\signs\misc\sign_roadblock_arrow.p3d";
	};
	class bldr_sign_roadblock_stop: HouseNoDestruct
	{
		scope = 1;
		model = "\DZ\structures\signs\misc\sign_roadblock_stop.p3d";
	};
	class bldr_sign_roof_bardak: HouseNoDestruct
	{
		scope = 1;
		model = "\DZ\structures\signs\misc\sign_roof_bardak.p3d";
	};
	class bldr_sign_roof_cerselprom: HouseNoDestruct
	{
		scope = 1;
		model = "\DZ\structures\signs\misc\sign_roof_cerselprom.p3d";
	};
	class bldr_sign_roof_chel: HouseNoDestruct
	{
		scope = 1;
		model = "\DZ\structures\signs\misc\sign_roof_chel.p3d";
	};
	class bldr_sign_roof_chernobus: HouseNoDestruct
	{
		scope = 1;
		model = "\DZ\structures\signs\misc\sign_roof_chernobus.p3d";
	};
	class bldr_sign_roof_cmc: HouseNoDestruct
	{
		scope = 1;
		model = "\DZ\structures\signs\misc\sign_roof_cmc.p3d";
	};
	class bldr_sign_roof_crr: HouseNoDestruct
	{
		scope = 1;
		model = "\DZ\structures\signs\misc\sign_roof_crr.p3d";
	};
	class bldr_sign_roof_czd: HouseNoDestruct
	{
		scope = 1;
		model = "\DZ\structures\signs\misc\sign_roof_czd.p3d";
	};
	class bldr_sign_roof_eldorado: HouseNoDestruct
	{
		scope = 1;
		model = "\DZ\structures\signs\misc\sign_roof_eldorado.p3d";
	};
	class bldr_sign_roof_interhotel: HouseNoDestruct
	{
		scope = 1;
		model = "\DZ\structures\signs\misc\sign_roof_interhotel.p3d";
	};
	class bldr_sign_roof_kolgaz: HouseNoDestruct
	{
		scope = 1;
		model = "\DZ\structures\signs\misc\sign_roof_kolgaz.p3d";
	};
	class bldr_sign_roof_moto: HouseNoDestruct
	{
		scope = 1;
		model = "\DZ\structures\signs\misc\sign_roof_moto.p3d";
	};
	class bldr_sign_roof_poctamt: HouseNoDestruct
	{
		scope = 1;
		model = "\DZ\structures\signs\misc\sign_roof_poctamt.p3d";
	};
	class bldr_sign_roof_tec: HouseNoDestruct
	{
		scope = 1;
		model = "\DZ\structures\signs\misc\sign_roof_tec.p3d";
	};
	class bldr_sign_roof_tes: HouseNoDestruct
	{
		scope = 1;
		model = "\DZ\structures\signs\misc\sign_roof_tes.p3d";
	};
	class bldr_sign_roof_univermag: HouseNoDestruct
	{
		scope = 1;
		model = "\DZ\structures\signs\misc\sign_roof_univermag.p3d";
	};
	class bldr_fire_barrel: BarrelHoles_Red
	{
		scope = 1;
	};
	class bldr_fireplace: Fireplace
	{
		scope = 1;
	};
	class bldr_Chemlight_ColorBase: Chemlight_ColorBase
	{
		scope = 1;
	};
	class bldr_Chemlight_White: Chemlight_White
	{
		scope = 1;
	};
	class bldr_Chemlight_Red: Chemlight_Red
	{
		scope = 1;
	};
	class bldr_Chemlight_Green: Chemlight_Green
	{
		scope = 1;
	};
	class bldr_Chemlight_Blue: Chemlight_Blue
	{
		scope = 1;
	};
	class bldr_Chemlight_Yellow: Chemlight_Yellow
	{
		scope = 1;
	};
	class bldr_PortableGasLamp_Prop: HouseNoDestruct
	{
		scope = 1;
		model = "\dz\gear\cooking\GasLight.p3d";
		hiddenSelections[] = {"glow","zbytek","glow_on","glow_off"};
		hiddenSelectionsMaterials[] = {"dz\gear\cooking\data\gaslight.rvmat","dz\gear\cooking\data\gaslight.rvmat","","dz\gear\cooking\data\gaslight.rvmat"};
	};
	class bldr_Candle: Candle
	{
		scope = 1;
	};
	class bldr_Spotlight: Spotlight
	{
		scope = 1;
	};
	class land_misc_barel_fire_1;
	class land_misc_barel_fire_2;
	class land_misc_barel_fire_3;
	class land_misc_barel_fire_4;
	class bldr_land_misc_barel_fire_4: land_misc_barel_fire_4
	{
		scope = 1;
	};
	class bldr_land_misc_barel_fire_3: land_misc_barel_fire_3
	{
		scope = 1;
	};
	class bldr_land_misc_barel_fire_2: land_misc_barel_fire_2
	{
		scope = 1;
	};
	class bldr_land_misc_barel_fire_1: land_misc_barel_fire_1
	{
		scope = 1;
	};
};
