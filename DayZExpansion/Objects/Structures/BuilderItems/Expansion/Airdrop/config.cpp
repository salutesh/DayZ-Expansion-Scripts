#define _ARMA_

class CfgPatches
{
	class BuilderItems_Expansion_Airdrop
	{
		units[] = {};
		weapons[] = {};
		requiredVersion = 0.1;
		requiredAddons[] = {"DZ_Structures_Signs"};
	};
};
class CfgVehicles
{
	class HouseNoDestruct;
	class bldr_expansion_Airdrop_Crate_Base: HouseNoDestruct
	{
		scope = 1;
		model = "\DayZExpansion\Objects\airdrop\container.p3d";
		hiddenSelections[] = {"body"};
		hiddenSelectionsTextures[] = {"DayZExpansion\Objects\Airdrop\data\Airdrop_co.paa"};
	};
	class bldr_expansion_Airdrop_Crate_Base_Grounded: bldr_expansion_Airdrop_Crate_Base
	{
		class AnimationSources
		{
			class Parachute
			{
				source = "user";
				animPeriod = 0.99999;
				initPhase = 1;
			};
		};
	};
	class bldr_expansion_Airdrop_Crate_Basebuilding: bldr_expansion_Airdrop_Crate_Base
	{
		hiddenSelectionsTextures[] = {"DayZExpansion\Objects\Airdrop\data\Airdrop_basebuilding_co.paa"};
	};
	class bldr_expansion_Airdrop_Crate_Military: bldr_expansion_Airdrop_Crate_Base
	{
		hiddenSelectionsTextures[] = {"DayZExpansion\Objects\Airdrop\data\Airdrop_mil_co.paa"};
	};
	class bldr_expansion_Airdrop_Crate_Medical: bldr_expansion_Airdrop_Crate_Base
	{
		hiddenSelectionsTextures[] = {"DayZExpansion\Objects\Airdrop\data\Airdrop_med_co.paa"};
	};
	class bldr_expansion_Airdrop_Crate_General: bldr_expansion_Airdrop_Crate_Base
	{
		hiddenSelectionsTextures[] = {"DayZExpansion\Objects\Airdrop\data\Airdrop_co.paa"};
	};
	class bldr_expansion_Airdrop_Crate_Basebuilding_Grounded: bldr_expansion_Airdrop_Crate_Base_Grounded
	{
		hiddenSelectionsTextures[] = {"DayZExpansion\Objects\Airdrop\data\Airdrop_basebuilding_co.paa"};
	};
	class bldr_expansion_Airdrop_Crate_Military_Grounded: bldr_expansion_Airdrop_Crate_Base_Grounded
	{
		hiddenSelectionsTextures[] = {"DayZExpansion\Objects\Airdrop\data\Airdrop_mil_co.paa"};
	};
	class bldr_expansion_Airdrop_Crate_Medical_Grounded: bldr_expansion_Airdrop_Crate_Base_Grounded
	{
		hiddenSelectionsTextures[] = {"DayZExpansion\Objects\Airdrop\data\Airdrop_med_co.paa"};
	};
	class bldr_expansion_Airdrop_Crate_General_Grounded: bldr_expansion_Airdrop_Crate_Base_Grounded
	{
		hiddenSelectionsTextures[] = {"DayZExpansion\Objects\Airdrop\data\Airdrop_co.paa"};
	};
	class bldr_expansion_Airdrop_Plane: HouseNoDestruct
	{
		scope = 1;
		model = "\DayZExpansion\Objects\Airdrop\AirdropC130J.p3d";
		class AnimationSources
		{
			class gear
			{
				source = "user";
				animPeriod = 0.01;
				initPhase = 1;
			};
			class rotor
			{
				source = "user";
				animPeriod = 0.0009999999;
				initPhase = 0;
			};
		};
	};
};
