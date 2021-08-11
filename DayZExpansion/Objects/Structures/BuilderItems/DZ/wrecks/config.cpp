#define _ARMA_

class CfgPatches
{
	class BuilderItems_Wrecks
	{
		units[] = {};
		weapons[] = {};
		requiredVersion = 0.1;
		requiredAddons[] = {"DZ_Data","DZ_Structures_Wrecks"};
	};
};
class CfgVehicles
{
	class HouseNoDestruct;
	class bldr_ship_big_lifeboat: HouseNoDestruct
	{
		scope = 1;
		model = "\DZ\structures\wrecks\ships\ship_big_lifeboat.p3d";
	};
	class bldr_ship_medium_back: HouseNoDestruct
	{
		scope = 1;
		model = "\DZ\structures\wrecks\ships\ship_medium_back.p3d";
	};
	class bldr_ship_medium_front: HouseNoDestruct
	{
		scope = 1;
		model = "\DZ\structures\wrecks\ships\ship_medium_front.p3d";
	};
	class bldr_wreck_baler: HouseNoDestruct
	{
		scope = 1;
		model = "\DZ\structures\wrecks\vehicles\wreck_baler.p3d";
	};
	class bldr_wreck_bmp1: HouseNoDestruct
	{
		scope = 1;
		model = "\DZ\structures\wrecks\vehicles\wreck_bmp1.p3d";
	};
	class bldr_wreck_bmp2: HouseNoDestruct
	{
		scope = 1;
		model = "\DZ\structures\wrecks\vehicles\wreck_bmp2.p3d";
	};
	class bldr_wreck_brdm: HouseNoDestruct
	{
		scope = 1;
		model = "\DZ\structures\wrecks\vehicles\wreck_brdm.p3d";
	};
	class bldr_wreck_bulldozer: HouseNoDestruct
	{
		scope = 1;
		model = "\DZ\structures\wrecks\vehicles\wreck_bulldozer.p3d";
	};
	class bldr_wreck_decal_big: HouseNoDestruct
	{
		scope = 1;
		model = "\DZ\structures\wrecks\vehicles\wreck_decal_big.p3d";
	};
	class bldr_wreck_decal_small1: HouseNoDestruct
	{
		scope = 1;
		model = "\DZ\structures\wrecks\vehicles\wreck_decal_small1.p3d";
	};
	class bldr_wreck_decal_small2: HouseNoDestruct
	{
		scope = 1;
		model = "\DZ\structures\wrecks\vehicles\wreck_decal_small2.p3d";
	};
	class bldr_wreck_digger: HouseNoDestruct
	{
		scope = 1;
		model = "\DZ\structures\wrecks\vehicles\wreck_digger.p3d";
	};
	class bldr_wreck_harverster: HouseNoDestruct
	{
		scope = 1;
		model = "\DZ\structures\wrecks\vehicles\wreck_harverster.p3d";
	};
	class bldr_wreck_harvester_head_trailer: HouseNoDestruct
	{
		scope = 1;
		model = "\DZ\structures\wrecks\vehicles\wreck_harvester_head_trailer.p3d";
	};
	class bldr_wreck_hmmwv: HouseNoDestruct
	{
		scope = 1;
		model = "\DZ\structures\wrecks\vehicles\wreck_hmmwv.p3d";
	};
	class bldr_wreck_powgen: HouseNoDestruct
	{
		scope = 1;
		model = "\DZ\structures\wrecks\vehicles\wreck_powgen.p3d";
	};
	class bldr_wreck_s120_red: HouseNoDestruct
	{
		scope = 1;
		model = "\DZ\structures\wrecks\vehicles\wreck_s120_red.p3d";
	};
	class bldr_wreck_t72_chassis: HouseNoDestruct
	{
		scope = 1;
		model = "\DZ\structures\wrecks\vehicles\wreck_t72_chassis.p3d";
	};
	class bldr_wreck_t72_turret: HouseNoDestruct
	{
		scope = 1;
		model = "\DZ\structures\wrecks\vehicles\wreck_t72_turret.p3d";
	};
	class bldr_wreck_trailer_cistern: HouseNoDestruct
	{
		scope = 1;
		model = "\DZ\structures\wrecks\vehicles\wreck_trailer_cistern.p3d";
	};
	class bldr_wreck_trailer_flat: HouseNoDestruct
	{
		scope = 1;
		model = "\DZ\structures\wrecks\vehicles\wreck_trailer_flat.p3d";
	};
	class bldr_wreck_ural: HouseNoDestruct
	{
		scope = 1;
		model = "\DZ\structures\wrecks\vehicles\wreck_ural.p3d";
	};
};
