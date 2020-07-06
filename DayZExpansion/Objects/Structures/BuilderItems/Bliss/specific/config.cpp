#define _ARMA_

class CfgPatches
{
	class BuilderItems_Bliss_Specific
	{
		units[] = {"bldr_Airfield_LandMark","bldr_Castle_Wall3","bldr_Castle_Wall3_D_L","bldr_Castle_Wall3_D_R","bldr_Casltes_Bastion2_etc","bldr_Casltes_Bastion2_rocks","bldr_Church5","bldr_Church5_Dam","bldr_Church5_Red_Dam","bldr_Church5_White","bldr_Church5_White_Dam","bldr_Church5_Yellow_Dam","bldr_Misc_Calvary1","bldr_Misc_Calvary2","bldr_Misc_Calvary3","bldr_Monument_Enoch1","bldr_Monument_Enoch2","bldr_Statue_Enoch1","bldr_Church4_details_proxy","bldr_Forest_Anthill","bldr_Forest_BeetleTrap1","bldr_Forest_BeetleTrap2","bldr_Forest_BeetleTrap3","bldr_Forest_DeerSkeleton_Dam","bldr_Forest_DeerSkeleton_Full","bldr_Forest_DeerSkeleton_Pile","bldr_Forest_DeerSkeleton_Skull","bldr_Forest_HumanSkeleton","bldr_FuelStation_Shed_Enoch","bldr_FuelStation_Sign_Enoch","bldr_Transmitter_Box","bldr_Transmitter_Small1","bldr_Transmitter_Small2","bldr_Tower_TC6_Ladder","bldr_Tribune"};
		weapons[] = {};
		requiredVersion = 0.1;
		requiredAddons[] = {"DZ_Structures_Bliss_Specific"};
	};
};
class CfgVehicles
{
	class HouseNoDestruct;
	class bldr_Airfield_LandMark: HouseNoDestruct
	{
		scope = 2;
		model = "\DZ\structures_bliss\specific\Airfields\Airfield_LandMark.p3d";
	};
	class bldr_Castle_Wall3: HouseNoDestruct
	{
		scope = 2;
		model = "\DZ\structures_bliss\specific\Castles\Castle_Wall3.p3d";
	};
	class bldr_Castle_Wall3_D_L: HouseNoDestruct
	{
		scope = 2;
		model = "\DZ\structures_bliss\specific\Castles\Castle_Wall3_D_L.p3d";
	};
	class bldr_Castle_Wall3_D_R: HouseNoDestruct
	{
		scope = 2;
		model = "\DZ\structures_bliss\specific\Castles\Castle_Wall3_D_R.p3d";
	};
	class bldr_Casltes_Bastion2_etc: HouseNoDestruct
	{
		scope = 2;
		model = "\DZ\structures_bliss\specific\Castles\Proxy\Casltes_Bastion2_etc.p3d";
	};
	class bldr_Casltes_Bastion2_rocks: HouseNoDestruct
	{
		scope = 2;
		model = "\DZ\structures_bliss\specific\Castles\Proxy\Casltes_Bastion2_rocks.p3d";
	};
	class bldr_Church5: HouseNoDestruct
	{
		scope = 2;
		model = "\DZ\structures_bliss\specific\Cultural\Church5.p3d";
	};
	class bldr_Church5_Dam: HouseNoDestruct
	{
		scope = 2;
		model = "\DZ\structures_bliss\specific\Cultural\Church5_Dam.p3d";
	};
	class bldr_Church5_Red_Dam: HouseNoDestruct
	{
		scope = 2;
		model = "\DZ\structures_bliss\specific\Cultural\Church5_Red_Dam.p3d";
	};
	class bldr_Church5_White: HouseNoDestruct
	{
		scope = 2;
		model = "\DZ\structures_bliss\specific\Cultural\Church5_White.p3d";
	};
	class bldr_Church5_White_Dam: HouseNoDestruct
	{
		scope = 2;
		model = "\DZ\structures_bliss\specific\Cultural\Church5_White_Dam.p3d";
	};
	class bldr_Church5_Yellow_Dam: HouseNoDestruct
	{
		scope = 2;
		model = "\DZ\structures_bliss\specific\Cultural\Church5_Yellow_Dam.p3d";
	};
	class bldr_Misc_Calvary1: HouseNoDestruct
	{
		scope = 2;
		model = "\DZ\structures_bliss\specific\Cultural\Misc_Calvary1.p3d";
	};
	class bldr_Misc_Calvary2: HouseNoDestruct
	{
		scope = 2;
		model = "\DZ\structures_bliss\specific\Cultural\Misc_Calvary2.p3d";
	};
	class bldr_Misc_Calvary3: HouseNoDestruct
	{
		scope = 2;
		model = "\DZ\structures_bliss\specific\Cultural\Misc_Calvary3.p3d";
	};
	class bldr_Monument_Enoch1: HouseNoDestruct
	{
		scope = 2;
		model = "\DZ\structures_bliss\specific\Cultural\Monument_Enoch1.p3d";
	};
	class bldr_Monument_Enoch2: HouseNoDestruct
	{
		scope = 2;
		model = "\DZ\structures_bliss\specific\Cultural\Monument_Enoch2.p3d";
	};
	class bldr_Statue_Enoch1: HouseNoDestruct
	{
		scope = 2;
		model = "\DZ\structures_bliss\specific\Cultural\Statue_Enoch1.p3d";
	};
	class bldr_Church4_details_proxy: HouseNoDestruct
	{
		scope = 2;
		model = "\DZ\structures_bliss\specific\Cultural\Proxy\Church4_details_proxy.p3d";
	};
	class bldr_Forest_Anthill: HouseNoDestruct
	{
		scope = 2;
		model = "\DZ\structures_bliss\specific\Forest\Forest_Anthill.p3d";
	};
	class bldr_Forest_BeetleTrap1: HouseNoDestruct
	{
		scope = 2;
		model = "\DZ\structures_bliss\specific\Forest\Forest_BeetleTrap1.p3d";
	};
	class bldr_Forest_BeetleTrap2: HouseNoDestruct
	{
		scope = 2;
		model = "\DZ\structures_bliss\specific\Forest\Forest_BeetleTrap2.p3d";
	};
	class bldr_Forest_BeetleTrap3: HouseNoDestruct
	{
		scope = 2;
		model = "\DZ\structures_bliss\specific\Forest\Forest_BeetleTrap3.p3d";
	};
	class bldr_Forest_DeerSkeleton_Dam: HouseNoDestruct
	{
		scope = 2;
		model = "\DZ\structures_bliss\specific\Forest\Forest_DeerSkeleton_Dam.p3d";
	};
	class bldr_Forest_DeerSkeleton_Full: HouseNoDestruct
	{
		scope = 2;
		model = "\DZ\structures_bliss\specific\Forest\Forest_DeerSkeleton_Full.p3d";
	};
	class bldr_Forest_DeerSkeleton_Pile: HouseNoDestruct
	{
		scope = 2;
		model = "\DZ\structures_bliss\specific\Forest\Forest_DeerSkeleton_Pile.p3d";
	};
	class bldr_Forest_DeerSkeleton_Skull: HouseNoDestruct
	{
		scope = 2;
		model = "\DZ\structures_bliss\specific\Forest\Forest_DeerSkeleton_Skull.p3d";
	};
	class bldr_Forest_HumanSkeleton: HouseNoDestruct
	{
		scope = 2;
		model = "\DZ\structures_bliss\specific\Forest\Forest_HumanSkeleton.p3d";
	};
	class bldr_FuelStation_Shed_Enoch: HouseNoDestruct
	{
		scope = 2;
		model = "\DZ\structures_bliss\specific\FuelStation\FuelStation_Shed_Enoch.p3d";
	};
	class bldr_FuelStation_Sign_Enoch: HouseNoDestruct
	{
		scope = 2;
		model = "\DZ\structures_bliss\specific\FuelStation\FuelStation_Sign_Enoch.p3d";
	};
	class bldr_Transmitter_Box: HouseNoDestruct
	{
		scope = 2;
		model = "\DZ\structures_bliss\specific\Towers\Transmitter_Box.p3d";
	};
	class bldr_Transmitter_Small1: HouseNoDestruct
	{
		scope = 2;
		model = "\DZ\structures_bliss\specific\Towers\Transmitter_Small1.p3d";
	};
	class bldr_Transmitter_Small2: HouseNoDestruct
	{
		scope = 2;
		model = "\DZ\structures_bliss\specific\Towers\Transmitter_Small2.p3d";
	};
	class bldr_Tower_TC6_Ladder: HouseNoDestruct
	{
		scope = 2;
		model = "\DZ\structures_bliss\specific\Towers\Proxy\Tower_TC6_Ladder.p3d";
	};
	class bldr_Tribune: HouseNoDestruct
	{
		scope = 2;
		model = "\DZ\structures_bliss\specific\Tribune\Tribune.p3d";
	};
};
