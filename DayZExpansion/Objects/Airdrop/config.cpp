class CfgPatches
{
	class DayZExpansion_Objects_Airdrop
	{
		units[]={};
		weapons[]={};
		requiredVersion=0.1;
		requiredAddons[]=
		{
			"DZ_Data"
		};
	};
};
class CfgVehicles
{
	class HouseNoDestruct;
	class ExpansionAirdropPlane: HouseNoDestruct
	{
		vehicleClass="Expansion_Airdrop";
		scope=2;
		model="\DayZExpansion\Objects\Airdrop\AirdropC130J.p3d";
		forceFarBubble="true";
		class AnimationSources
		{
			class gear
			{
				source="user";
				animPeriod=0.0099999998;
				initPhase=1;
			};
			class rotor
			{
				source="user";
				animPeriod=0.00099999993;
				initPhase=0;
			};
		};
	};
};
