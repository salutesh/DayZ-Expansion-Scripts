#define _ARMA_

class CfgPatches
{
	class BuilderItems_Rail
	{
		units[] = {};
		weapons[] = {};
		requiredVersion = 0.1;
		requiredAddons[] = {"DZ_Data","DZ_Structures_Rail"};
	};
};
class CfgVehicles
{
	class HouseNoDestruct;
	class bldr_rail_bridge_15: HouseNoDestruct
	{
		scope = 1;
		model = "\DZ\structures\rail\bridges\rail_bridge_15.p3d";
	};
	class bldr_rail_bridge_15_3tracks: HouseNoDestruct
	{
		scope = 1;
		model = "\DZ\structures\rail\bridges\rail_bridge_15_3tracks.p3d";
	};
	class bldr_rail_bridge_15_curve: HouseNoDestruct
	{
		scope = 1;
		model = "\DZ\structures\rail\bridges\rail_bridge_15_curve.p3d";
	};
	class bldr_rail_bridge_15_end: HouseNoDestruct
	{
		scope = 1;
		model = "\DZ\structures\rail\bridges\rail_bridge_15_end.p3d";
	};
	class bldr_rail_bridge_40: HouseNoDestruct
	{
		scope = 1;
		model = "\DZ\structures\rail\bridges\rail_bridge_40.p3d";
	};
	class bldr_rail_concreteramp: HouseNoDestruct
	{
		scope = 1;
		model = "\DZ\structures\rail\misc\rail_concreteramp.p3d";
	};
	class bldr_rail_crossing_barrier: HouseNoDestruct
	{
		scope = 1;
		model = "\DZ\structures\rail\misc\rail_crossing_barrier.p3d";
	};
	class bldr_rail_crossing_block_10: HouseNoDestruct
	{
		scope = 1;
		model = "\DZ\structures\rail\misc\rail_crossing_block_10.p3d";
	};
	class bldr_rail_crossing_block_25: HouseNoDestruct
	{
		scope = 1;
		model = "\DZ\structures\rail\misc\rail_crossing_block_25.p3d";
	};
	class bldr_rail_linebreak_concrete: HouseNoDestruct
	{
		scope = 1;
		model = "\DZ\structures\rail\misc\rail_linebreak_concrete.p3d";
	};
	class bldr_rail_linebreak_iron: HouseNoDestruct
	{
		scope = 1;
		model = "\DZ\structures\rail\misc\rail_linebreak_iron.p3d";
	};
	class bldr_rail_platform_cross: HouseNoDestruct
	{
		scope = 1;
		model = "\DZ\structures\rail\misc\rail_platform_cross.p3d";
	};
	class bldr_rail_platform_segment: HouseNoDestruct
	{
		scope = 1;
		model = "\DZ\structures\rail\misc\rail_platform_segment.p3d";
	};
	class bldr_rail_platform_start: HouseNoDestruct
	{
		scope = 1;
		model = "\DZ\structures\rail\misc\rail_platform_start.p3d";
	};
	class bldr_rail_polef: HouseNoDestruct
	{
		scope = 1;
		model = "\DZ\structures\rail\misc\rail_polef.p3d";
	};
	class bldr_rail_polet: HouseNoDestruct
	{
		scope = 1;
		model = "\DZ\structures\rail\misc\rail_polet.p3d";
	};
	class bldr_rail_polett1: HouseNoDestruct
	{
		scope = 1;
		model = "\DZ\structures\rail\misc\rail_polett1.p3d";
	};
	class bldr_rail_polett2: HouseNoDestruct
	{
		scope = 1;
		model = "\DZ\structures\rail\misc\rail_polett2.p3d";
	};
	class bldr_rail_signals: HouseNoDestruct
	{
		scope = 1;
		model = "\DZ\structures\rail\misc\rail_signals.p3d";
	};
	class bldr_rail_track_25: HouseNoDestruct
	{
		scope = 1;
		model = "\DZ\structures\rail\tracks\rail_track_25.p3d";
	};
	class bldr_rail_track_down_25: HouseNoDestruct
	{
		scope = 1;
		model = "\DZ\structures\rail\tracks\rail_track_down_25.p3d";
	};
	class bldr_rail_track_down_40: HouseNoDestruct
	{
		scope = 1;
		model = "\DZ\structures\rail\tracks\rail_track_down_40.p3d";
	};
	class bldr_rail_track_l25_10: HouseNoDestruct
	{
		scope = 1;
		model = "\DZ\structures\rail\tracks\rail_track_l25_10.p3d";
	};
	class bldr_rail_track_lb_re: HouseNoDestruct
	{
		scope = 1;
		model = "\DZ\structures\rail\tracks\rail_track_lb_re.p3d";
	};
	class bldr_rail_track_lb1_re: HouseNoDestruct
	{
		scope = 1;
		model = "\DZ\structures\rail\tracks\rail_track_lb1_re.p3d";
	};
	class bldr_rail_track_le_rb: HouseNoDestruct
	{
		scope = 1;
		model = "\DZ\structures\rail\tracks\rail_track_le_rb.p3d";
	};
	class bldr_rail_track_le1_rb: HouseNoDestruct
	{
		scope = 1;
		model = "\DZ\structures\rail\tracks\rail_track_le1_rb.p3d";
	};
	class bldr_rail_track_passing_10: HouseNoDestruct
	{
		scope = 1;
		model = "\DZ\structures\rail\tracks\rail_track_passing_10.p3d";
	};
	class bldr_rail_track_passing_25: HouseNoDestruct
	{
		scope = 1;
		model = "\DZ\structures\rail\tracks\rail_track_passing_25.p3d";
	};
	class bldr_rail_track_r25_10: HouseNoDestruct
	{
		scope = 1;
		model = "\DZ\structures\rail\tracks\rail_track_r25_10.p3d";
	};
	class bldr_rail_track_sp: HouseNoDestruct
	{
		scope = 1;
		model = "\DZ\structures\rail\tracks\rail_track_sp.p3d";
	};
	class bldr_rail_track_turnoutl: HouseNoDestruct
	{
		scope = 1;
		model = "\DZ\structures\rail\tracks\rail_track_turnoutl.p3d";
	};
	class bldr_rail_track_turnoutr: HouseNoDestruct
	{
		scope = 1;
		model = "\DZ\structures\rail\tracks\rail_track_turnoutr.p3d";
	};
	class bldr_rail_track_up_25: HouseNoDestruct
	{
		scope = 1;
		model = "\DZ\structures\rail\tracks\rail_track_up_25.p3d";
	};
	class bldr_rail_track_up_40: HouseNoDestruct
	{
		scope = 1;
		model = "\DZ\structures\rail\tracks\rail_track_up_40.p3d";
	};
	class bldr_rail_tracke_2: HouseNoDestruct
	{
		scope = 1;
		model = "\DZ\structures\rail\tracks\rail_tracke_2.p3d";
	};
	class bldr_rail_tracke_4: HouseNoDestruct
	{
		scope = 1;
		model = "\DZ\structures\rail\tracks\rail_tracke_4.p3d";
	};
	class bldr_rail_tracke_8: HouseNoDestruct
	{
		scope = 1;
		model = "\DZ\structures\rail\tracks\rail_tracke_8.p3d";
	};
	class bldr_rail_tracke_25: HouseNoDestruct
	{
		scope = 1;
		model = "\DZ\structures\rail\tracks\rail_tracke_25.p3d";
	};
	class bldr_rail_tracke_40: HouseNoDestruct
	{
		scope = 1;
		model = "\DZ\structures\rail\tracks\rail_tracke_40.p3d";
	};
	class bldr_rail_tracke_l25_5: HouseNoDestruct
	{
		scope = 1;
		model = "\DZ\structures\rail\tracks\rail_tracke_l25_5.p3d";
	};
	class bldr_rail_tracke_l25_10: HouseNoDestruct
	{
		scope = 1;
		model = "\DZ\structures\rail\tracks\rail_tracke_l25_10.p3d";
	};
	class bldr_rail_tracke_l30_20: HouseNoDestruct
	{
		scope = 1;
		model = "\DZ\structures\rail\tracks\rail_tracke_l30_20.p3d";
	};
	class bldr_rail_tracke_r25_5: HouseNoDestruct
	{
		scope = 1;
		model = "\DZ\structures\rail\tracks\rail_tracke_r25_5.p3d";
	};
	class bldr_rail_tracke_r25_10: HouseNoDestruct
	{
		scope = 1;
		model = "\DZ\structures\rail\tracks\rail_tracke_r25_10.p3d";
	};
	class bldr_rail_tracke_r30_20: HouseNoDestruct
	{
		scope = 1;
		model = "\DZ\structures\rail\tracks\rail_tracke_r30_20.p3d";
	};
	class bldr_rail_tracke_turnoutl: HouseNoDestruct
	{
		scope = 1;
		model = "\DZ\structures\rail\tracks\rail_tracke_turnoutl.p3d";
	};
	class bldr_rail_tracke_turnoutr: HouseNoDestruct
	{
		scope = 1;
		model = "\DZ\structures\rail\tracks\rail_tracke_turnoutr.p3d";
	};
};
