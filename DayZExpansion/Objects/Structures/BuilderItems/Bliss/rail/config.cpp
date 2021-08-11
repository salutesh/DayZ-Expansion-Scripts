#define _ARMA_

class CfgPatches
{
	class BuilderItems_Bliss_Rail
	{
		units[] = {};
		weapons[] = {};
		requiredVersion = 0.1;
		requiredAddons[] = {"DZ_Structures_Bliss_Rail"};
	};
};
class CfgVehicles
{
	class HouseNoDestruct;
	class bldr_Rail_Crossing_Block_Small: HouseNoDestruct
	{
		scope = 1;
		model = "\DZ\structures_bliss\rail\Misc\Rail_Crossing_Block_Small.p3d";
	};
	class bldr_Rail_LineBreak_Small: HouseNoDestruct
	{
		scope = 1;
		model = "\DZ\structures_bliss\rail\Misc\Rail_LineBreak_Small.p3d";
	};
	class bldr_Rail_TrackSmall_Switch: HouseNoDestruct
	{
		scope = 1;
		model = "\DZ\structures_bliss\rail\Misc\Rail_TrackSmall_Switch.p3d";
	};
	class bldr_Rail_TrackSmall_10m: HouseNoDestruct
	{
		scope = 1;
		model = "\DZ\structures_bliss\rail\Tracks\Rail_TrackSmall_10m.p3d";
	};
	class bldr_Rail_TrackSmall_15deg: HouseNoDestruct
	{
		scope = 1;
		model = "\DZ\structures_bliss\rail\Tracks\Rail_TrackSmall_15deg.p3d";
	};
	class bldr_Rail_TrackSmall_20m: HouseNoDestruct
	{
		scope = 1;
		model = "\DZ\structures_bliss\rail\Tracks\Rail_TrackSmall_20m.p3d";
	};
	class bldr_Rail_TrackSmall_30deg: HouseNoDestruct
	{
		scope = 1;
		model = "\DZ\structures_bliss\rail\Tracks\Rail_TrackSmall_30deg.p3d";
	};
	class bldr_Rail_TrackSmall_3m: HouseNoDestruct
	{
		scope = 1;
		model = "\DZ\structures_bliss\rail\Tracks\Rail_TrackSmall_3m.p3d";
	};
	class bldr_Rail_TrackSmall_7deg: HouseNoDestruct
	{
		scope = 1;
		model = "\DZ\structures_bliss\rail\Tracks\Rail_TrackSmall_7deg.p3d";
	};
	class bldr_Rail_TrackSmall_TurnOutL: HouseNoDestruct
	{
		scope = 1;
		model = "\DZ\structures_bliss\rail\Tracks\Rail_TrackSmall_TurnOutL.p3d";
	};
	class bldr_Rail_TrackSmall_TurnOutR: HouseNoDestruct
	{
		scope = 1;
		model = "\DZ\structures_bliss\rail\Tracks\Rail_TrackSmall_TurnOutR.p3d";
	};
};
