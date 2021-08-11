#define _ARMA_

class CfgPatches
{
	class BuilderItems_Expansion_Vehicles
	{
		units[] = {"bldr_expansion_Gyrocopter_Red","bldr_expansion_Gyrocopter_Animated_Red","bldr_expansion_C130J_Grey","bldr_expansion_C130J_Animated_Grey","bldr_expansion_An2_Green","bldr_expansion_An2_Animated_Green"};
		weapons[] = {};
		requiredVersion = 0.1;
		requiredAddons[] = {"DZ_Structures_Signs"};
	};
};
class CfgVehicles
{
	class HouseNoDestruct;
	class bldr_expansion_Mh6_Base: HouseNoDestruct
	{
		scope = 0;
		model = "\DayZExpansion\Vehicles\Air\Mh6\Mh6.p3d";
		hiddenSelections[] = {"camo1"};
		hiddenSelectionsTextures[] = {"dayzexpansion\vehicles\air\mh6\data\heli_light01_ext_co.paa"};
		hiddenSelectionsMaterials[] = {"dayzexpansion\vehicles\air\mh6\data\heli_light01_ext.rvmat"};
	};
	class bldr_expansion_Mh6_Animated_Base: bldr_expansion_Mh6_Base
	{
		scope = 0;
		class AnimationSources
		{
			class rotor
			{
				source = "user";
				animPeriod = 0.025;
				initPhase = 0;
			};
			class rearrotor
			{
				source = "user";
				animPeriod = 0.025;
				initPhase = 0;
			};
			class hiderotor
			{
				source = "user";
				animPeriod = 0.0009999999;
				initPhase = 1;
			};
		};
	};
	class bldr_expansion_Mh6_Green: bldr_expansion_Mh6_Base
	{
		hiddenSelectionsTextures[] = {"dayzexpansion\vehicles\air\mh6\data\heli_light01_ext_co.paa"};
	};
	class bldr_expansion_Mh6_Blueline: bldr_expansion_Mh6_Base
	{
		hiddenSelectionsTextures[] = {"dayzexpansion\vehicles\air\mh6\data\skins\heli_light01_ext_blueline_co.paa"};
	};
	class bldr_expansion_Mh6_Digital: bldr_expansion_Mh6_Base
	{
		hiddenSelectionsTextures[] = {"dayzexpansion\vehicles\air\mh6\data\skins\heli_light01_ext_digital_co.paa"};
	};
	class bldr_expansion_Mh6_Elliptical: bldr_expansion_Mh6_Base
	{
		hiddenSelectionsTextures[] = {"dayzexpansion\vehicles\air\mh6\data\skins\heli_light01_ext_elliptical_co.paa"};
	};
	class bldr_expansion_Mh6_Furious: bldr_expansion_Mh6_Base
	{
		hiddenSelectionsTextures[] = {"dayzexpansion\vehicles\air\mh6\data\skins\heli_light01_ext_furious_co.paa"};
	};
	class bldr_expansion_Mh6_GrayWatcher: bldr_expansion_Mh6_Base
	{
		hiddenSelectionsTextures[] = {"dayzexpansion\vehicles\air\mh6\data\skins\heli_light01_ext_graywatcher_co.paa"};
	};
	class bldr_expansion_Mh6_Jeans: bldr_expansion_Mh6_Base
	{
		hiddenSelectionsTextures[] = {"dayzexpansion\vehicles\air\mh6\data\skins\heli_light01_ext_jeans_co.paa"};
	};
	class bldr_expansion_Mh6_Resistance: bldr_expansion_Mh6_Base
	{
		hiddenSelectionsTextures[] = {"dayzexpansion\vehicles\air\mh6\data\skins\heli_light01_ext_resistance_co.paa"};
	};
	class bldr_expansion_Mh6_Shadow: bldr_expansion_Mh6_Base
	{
		hiddenSelectionsTextures[] = {"dayzexpansion\vehicles\air\mh6\data\skins\heli_light01_ext_shadow_co.paa"};
	};
	class bldr_expansion_Mh6_Sheriff: bldr_expansion_Mh6_Base
	{
		hiddenSelectionsTextures[] = {"dayzexpansion\vehicles\air\mh6\data\skins\heli_light01_ext_sheriff_co.paa"};
	};
	class bldr_expansion_Mh6_Speedy: bldr_expansion_Mh6_Base
	{
		hiddenSelectionsTextures[] = {"dayzexpansion\vehicles\air\mh6\data\skins\heli_light01_ext_speedy_co.paa"};
	};
	class bldr_expansion_Mh6_Sunset: bldr_expansion_Mh6_Base
	{
		hiddenSelectionsTextures[] = {"dayzexpansion\vehicles\air\mh6\data\skins\heli_light01_ext_sunset_co.paa"};
	};
	class bldr_expansion_Mh6_Vrana: bldr_expansion_Mh6_Base
	{
		hiddenSelectionsTextures[] = {"dayzexpansion\vehicles\air\mh6\data\skins\heli_light01_ext_vrana_co.paa"};
	};
	class bldr_expansion_Mh6_Wasp: bldr_expansion_Mh6_Base
	{
		hiddenSelectionsTextures[] = {"dayzexpansion\vehicles\air\mh6\data\skins\heli_light01_ext_wasp_co.paa"};
	};
	class bldr_expansion_Mh6_Wave: bldr_expansion_Mh6_Base
	{
		hiddenSelectionsTextures[] = {"dayzexpansion\vehicles\air\mh6\data\skins\heli_light01_ext_wave_co.paa"};
	};
	class bldr_expansion_Mh6_Animated_Green: bldr_expansion_Mh6_Animated_Base
	{
		hiddenSelectionsTextures[] = {"dayzexpansion\vehicles\air\mh6\data\heli_light01_ext_co.paa"};
	};
	class bldr_expansion_Mh6_Animated_Blueline: bldr_expansion_Mh6_Animated_Base
	{
		hiddenSelectionsTextures[] = {"dayzexpansion\vehicles\air\mh6\data\skins\heli_light01_ext_blueline_co.paa"};
	};
	class bldr_expansion_Mh6_Animated_Digital: bldr_expansion_Mh6_Animated_Base
	{
		hiddenSelectionsTextures[] = {"dayzexpansion\vehicles\air\mh6\data\skins\heli_light01_ext_digital_co.paa"};
	};
	class bldr_expansion_Mh6_Animated_Elliptical: bldr_expansion_Mh6_Animated_Base
	{
		hiddenSelectionsTextures[] = {"dayzexpansion\vehicles\air\mh6\data\skins\heli_light01_ext_elliptical_co.paa"};
	};
	class bldr_expansion_Mh6_Animated_Furious: bldr_expansion_Mh6_Animated_Base
	{
		hiddenSelectionsTextures[] = {"dayzexpansion\vehicles\air\mh6\data\skins\heli_light01_ext_furious_co.paa"};
	};
	class bldr_expansion_Mh6_Animated_GrayWatcher: bldr_expansion_Mh6_Animated_Base
	{
		hiddenSelectionsTextures[] = {"dayzexpansion\vehicles\air\mh6\data\skins\heli_light01_ext_graywatcher_co.paa"};
	};
	class bldr_expansion_Mh6_Animated_Jeans: bldr_expansion_Mh6_Animated_Base
	{
		hiddenSelectionsTextures[] = {"dayzexpansion\vehicles\air\mh6\data\skins\heli_light01_ext_jeans_co.paa"};
	};
	class bldr_expansion_Mh6_Animated_Resistance: bldr_expansion_Mh6_Animated_Base
	{
		hiddenSelectionsTextures[] = {"dayzexpansion\vehicles\air\mh6\data\skins\heli_light01_ext_resistance_co.paa"};
	};
	class bldr_expansion_Mh6_Animated_Shadow: bldr_expansion_Mh6_Animated_Base
	{
		hiddenSelectionsTextures[] = {"dayzexpansion\vehicles\air\mh6\data\skins\heli_light01_ext_shadow_co.paa"};
	};
	class bldr_expansion_Mh6_Animated_Sheriff: bldr_expansion_Mh6_Animated_Base
	{
		hiddenSelectionsTextures[] = {"dayzexpansion\vehicles\air\mh6\data\skins\heli_light01_ext_sheriff_co.paa"};
	};
	class bldr_expansion_Mh6_Animated_Speedy: bldr_expansion_Mh6_Animated_Base
	{
		hiddenSelectionsTextures[] = {"dayzexpansion\vehicles\air\mh6\data\skins\heli_light01_ext_speedy_co.paa"};
	};
	class bldr_expansion_Mh6_Animated_Sunset: bldr_expansion_Mh6_Animated_Base
	{
		hiddenSelectionsTextures[] = {"dayzexpansion\vehicles\air\mh6\data\skins\heli_light01_ext_sunset_co.paa"};
	};
	class bldr_expansion_Mh6_Animated_Vrana: bldr_expansion_Mh6_Animated_Base
	{
		hiddenSelectionsTextures[] = {"dayzexpansion\vehicles\air\mh6\data\skins\heli_light01_ext_vrana_co.paa"};
	};
	class bldr_expansion_Mh6_Animated_Wasp: bldr_expansion_Mh6_Animated_Base
	{
		hiddenSelectionsTextures[] = {"dayzexpansion\vehicles\air\mh6\data\skins\heli_light01_ext_wasp_co.paa"};
	};
	class bldr_expansion_Mh6_Animated_Wave: bldr_expansion_Mh6_Animated_Base
	{
		hiddenSelectionsTextures[] = {"dayzexpansion\vehicles\air\mh6\data\skins\heli_light01_ext_wave_co.paa"};
	};
	class bldr_expansion_Uh1h_Base: HouseNoDestruct
	{
		scope = 0;
		model = "\DayZExpansion\Vehicles\Air\Uh1h\uh1h.p3d";
		hiddenSelections[] = {"camo1","camo2"};
		hiddenSelectionsTextures[] = {"dayzexpansion\vehicles\air\uh1h\data\uh1d_co.paa","dayzexpansion\vehicles\air\uh1h\data\uh1d_in_co.paa"};
		hiddenSelectionsMaterials[] = {"dayzexpansion\vehicles\air\uh1h\data\uh1d.rvmat","dayzexpansion\vehicles\air\uh1h\data\uh1d_in.rvmat"};
	};
	class bldr_expansion_Uh1h_Animated_Base: bldr_expansion_Uh1h_Base
	{
		scope = 0;
		class AnimationSources
		{
			class rotor
			{
				source = "user";
				animPeriod = 0.025;
				initPhase = 0;
			};
			class rearrotor
			{
				source = "user";
				animPeriod = 0.025;
				initPhase = 0;
			};
			class hiderotor
			{
				source = "user";
				animPeriod = 0.0009999999;
				initPhase = 1;
			};
		};
	};
	class bldr_expansion_Uh1h_Green: bldr_expansion_Uh1h_Base
	{
		hiddenSelectionsTextures[] = {"dayzexpansion\vehicles\air\uh1h\data\uh1d_co.paa","dayzexpansion\vehicles\air\uh1h\data\uh1d_in_co.paa"};
	};
	class bldr_expansion_Uh1h_Civilian: bldr_expansion_Uh1h_Base
	{
		hiddenSelectionsTextures[] = {"dayzexpansion\vehicles\air\uh1h\data\civ\uh1d_civ_main_co.paa","dayzexpansion\vehicles\air\uh1h\data\civ\uh1d_civ_in_co.paa"};
	};
	class bldr_expansion_Uh1h_Medical: bldr_expansion_Uh1h_Base
	{
		hiddenSelectionsTextures[] = {"dayzexpansion\vehicles\air\uh1h\data\med\uh1d_med_main_co.paa","dayzexpansion\vehicles\air\uh1h\data\med\uh1d_med_in_co.paa"};
	};
	class bldr_expansion_Uh1h_Resistance: bldr_expansion_Uh1h_Base
	{
		hiddenSelectionsTextures[] = {"dayzexpansion\vehicles\air\uh1h\data\resistance\uh1d_resistance_2_co.paa","dayzexpansion\vehicles\air\uh1h\data\resistance\uh1d_resistance_2_in_co.paa"};
	};
	class bldr_expansion_Uh1h_Uber: bldr_expansion_Uh1h_Base
	{
		hiddenSelectionsTextures[] = {"dayzexpansion\vehicles\air\uh1h\data\uber\uh1d_uber_main_co.paa","dayzexpansion\vehicles\air\uh1h\data\uber\uh1d_uber_in_co.paa"};
	};
	class bldr_expansion_Uh1h_Animated_Green: bldr_expansion_Uh1h_Animated_Base
	{
		hiddenSelectionsTextures[] = {"dayzexpansion\vehicles\air\uh1h\data\uh1d_co.paa","dayzexpansion\vehicles\air\uh1h\data\uh1d_in_co.paa"};
	};
	class bldr_expansion_Uh1h_Animated_Civilian: bldr_expansion_Uh1h_Animated_Base
	{
		hiddenSelectionsTextures[] = {"dayzexpansion\vehicles\air\uh1h\data\civ\uh1d_civ_main_co.paa","dayzexpansion\vehicles\air\uh1h\data\civ\uh1d_civ_in_co.paa"};
	};
	class bldr_expansion_Uh1h_Animated_Medical: bldr_expansion_Uh1h_Animated_Base
	{
		hiddenSelectionsTextures[] = {"dayzexpansion\vehicles\air\uh1h\data\med\uh1d_med_main_co.paa","dayzexpansion\vehicles\air\uh1h\data\med\uh1d_med_in_co.paa"};
	};
	class bldr_expansion_Uh1h_Animated_Resistance: bldr_expansion_Uh1h_Animated_Base
	{
		hiddenSelectionsTextures[] = {"dayzexpansion\vehicles\air\uh1h\data\resistance\uh1d_resistance_2_co.paa","dayzexpansion\vehicles\air\uh1h\data\resistance\uh1d_resistance_2_in_co.paa"};
	};
	class bldr_expansion_Uh1h_Animated_Uber: bldr_expansion_Uh1h_Animated_Base
	{
		hiddenSelectionsTextures[] = {"dayzexpansion\vehicles\air\uh1h\data\uber\uh1d_uber_main_co.paa","dayzexpansion\vehicles\air\uh1h\data\uber\uh1d_uber_in_co.paa"};
	};
	class bldr_expansion_Merlin_Base: HouseNoDestruct
	{
		scope = 0;
		model = "\DayZExpansion\Vehicles\Air\Merlin\merlin.p3d";
		hiddenSelections[] = {"camo1","camo2","camo3"};
		hiddenSelectionsTextures[] = {"dayzexpansion\vehicles\air\merlin\data\heavy_mer_1_co.paa","dayzexpansion\vehicles\air\merlin\data\heavy_mer_2_co.paa","dayzexpansion\vehicles\air\merlin\data\heavy_mer_3_co.paa"};
		hiddenSelectionsMaterials[] = {"dayzexpansion\vehicles\air\merlin\data\heavy_mer_1_h.rvmat","dayzexpansion\vehicles\air\merlin\data\heavy_mer_2_h.rvmat","dayzexpansion\vehicles\air\merlin\data\heavy_mer_3_h.rvmat"};
	};
	class bldr_expansion_Merlin_Animated_Base: bldr_expansion_Merlin_Base
	{
		scope = 0;
		class AnimationSources
		{
			class rotor
			{
				source = "user";
				animPeriod = 0.025;
				initPhase = 0;
			};
			class rearrotor
			{
				source = "user";
				animPeriod = 0.025;
				initPhase = 0;
			};
			class hiderotor
			{
				source = "user";
				animPeriod = 0.0009999999;
				initPhase = 1;
			};
			class door_back_l
			{
				source = "user";
				animPeriod = 1.0;
				initPhase = 1.0;
			};
			class door_back_r
			{
				source = "user";
				animPeriod = 1.0;
				initPhase = 1.0;
			};
		};
	};
	class bldr_expansion_Merlin_Green: bldr_expansion_Merlin_Base
	{
		hiddenSelectionsTextures[] = {"dayzexpansion\vehicles\air\merlin\data\heavy_mer_1_co.paa","dayzexpansion\vehicles\air\merlin\data\heavy_mer_2_co.paa","dayzexpansion\vehicles\air\merlin\data\heavy_mer_3_co.paa"};
	};
	class bldr_expansion_Merlin_Dahoman: bldr_expansion_Merlin_Base
	{
		hiddenSelectionsTextures[] = {"dayzexpansion\vehicles\air\merlin\data\skins\heavy_mer_1_dahoman_co.paa","dayzexpansion\vehicles\air\merlin\data\skins\heavy_mer_2_dahoman_co.paa","dayzexpansion\vehicles\air\merlin\data\skins\heavy_mer_3_dahoman_co.paa"};
	};
	class bldr_expansion_Merlin_Ion: bldr_expansion_Merlin_Base
	{
		hiddenSelectionsTextures[] = {"dayzexpansion\vehicles\air\merlin\data\skins\heavy_mer_1_ion_co.paa","dayzexpansion\vehicles\air\merlin\data\skins\heavy_mer_2_ion_co.paa","dayzexpansion\vehicles\air\merlin\data\skins\heavy_mer_3_ion_co.paa"};
	};
	class bldr_expansion_Merlin_President: bldr_expansion_Merlin_Base
	{
		hiddenSelectionsTextures[] = {"dayzexpansion\vehicles\air\merlin\data\skins\heavy_mer_1_president_co.paa","dayzexpansion\vehicles\air\merlin\data\skins\heavy_mer_2_president_co.paa","dayzexpansion\vehicles\air\merlin\data\skins\heavy_mer_3_president_co.paa"};
	};
	class bldr_expansion_Merlin_Vrana: bldr_expansion_Merlin_Base
	{
		hiddenSelectionsTextures[] = {"dayzexpansion\vehicles\air\merlin\data\skins\heavy_mer_1_vrana_co.paa","dayzexpansion\vehicles\air\merlin\data\skins\heavy_mer_2_vrana_co.paa","dayzexpansion\vehicles\air\merlin\data\skins\heavy_mer_3_vrana_co.paa"};
	};
	class bldr_expansion_Merlin_Wave: bldr_expansion_Merlin_Base
	{
		hiddenSelectionsTextures[] = {"dayzexpansion\vehicles\air\merlin\data\skins\heavy_mer_1_wave_co.paa","dayzexpansion\vehicles\air\merlin\data\skins\heavy_mer_2_wave_co.paa","dayzexpansion\vehicles\air\merlin\data\skins\heavy_mer_3_wave_co.paa"};
	};
	class bldr_expansion_Merlin_Animated_Green: bldr_expansion_Merlin_Animated_Base
	{
		hiddenSelectionsTextures[] = {"dayzexpansion\vehicles\air\merlin\data\heavy_mer_1_co.paa","dayzexpansion\vehicles\air\merlin\data\heavy_mer_2_co.paa","dayzexpansion\vehicles\air\merlin\data\heavy_mer_3_co.paa"};
	};
	class bldr_expansion_Merlin_Animated_Dahoman: bldr_expansion_Merlin_Animated_Base
	{
		hiddenSelectionsTextures[] = {"dayzexpansion\vehicles\air\merlin\data\skins\heavy_mer_1_dahoman_co.paa","dayzexpansion\vehicles\air\merlin\data\skins\heavy_mer_2_dahoman_co.paa","dayzexpansion\vehicles\air\merlin\data\skins\heavy_mer_3_dahoman_co.paa"};
	};
	class bldr_expansion_Merlin_Animated_Ion: bldr_expansion_Merlin_Animated_Base
	{
		hiddenSelectionsTextures[] = {"dayzexpansion\vehicles\air\merlin\data\skins\heavy_mer_1_ion_co.paa","dayzexpansion\vehicles\air\merlin\data\skins\heavy_mer_2_ion_co.paa","dayzexpansion\vehicles\air\merlin\data\skins\heavy_mer_3_ion_co.paa"};
	};
	class bldr_expansion_Merlin_Animated_President: bldr_expansion_Merlin_Animated_Base
	{
		hiddenSelectionsTextures[] = {"dayzexpansion\vehicles\air\merlin\data\skins\heavy_mer_1_president_co.paa","dayzexpansion\vehicles\air\merlin\data\skins\heavy_mer_2_president_co.paa","dayzexpansion\vehicles\air\merlin\data\skins\heavy_mer_3_president_co.paa"};
	};
	class bldr_expansion_Merlin_Animated_Vrana: bldr_expansion_Merlin_Animated_Base
	{
		hiddenSelectionsTextures[] = {"dayzexpansion\vehicles\air\merlin\data\skins\heavy_mer_1_vrana_co.paa","dayzexpansion\vehicles\air\merlin\data\skins\heavy_mer_2_vrana_co.paa","dayzexpansion\vehicles\air\merlin\data\skins\heavy_mer_3_vrana_co.paa"};
	};
	class bldr_expansion_Merlin_Animated_Wave: bldr_expansion_Merlin_Animated_Base
	{
		hiddenSelectionsTextures[] = {"dayzexpansion\vehicles\air\merlin\data\skins\heavy_mer_1_wave_co.paa","dayzexpansion\vehicles\air\merlin\data\skins\heavy_mer_2_wave_co.paa","dayzexpansion\vehicles\air\merlin\data\skins\heavy_mer_3_wave_co.paa"};
	};
	class bldr_expansion_Gyrocopter_Base: HouseNoDestruct
	{
		scope = 0;
		model = "DayZExpansion\Vehicles\Air\Gyro\Gyrocopter.p3d";
	};
	class bldr_expansion_Gyrocopter_Animated_Base: bldr_expansion_Gyrocopter_Base
	{
		scope = 0;
		class AnimationSources
		{
			class rotor
			{
				source = "user";
				animPeriod = 0.025;
				initPhase = 0;
			};
			class rearrotor
			{
				source = "user";
				animPeriod = 0.025;
				initPhase = 0;
			};
			class hiderotor
			{
				source = "user";
				animPeriod = 0.0009999999;
				initPhase = 1;
			};
		};
	};
	class bldr_expansion_Gyrocopter_Red: bldr_expansion_Gyrocopter_Base
	{
		scope = 2;
	};
	class bldr_expansion_Gyrocopter_Animated_Red: bldr_expansion_Gyrocopter_Animated_Base
	{
		scope = 2;
	};
	class bldr_expansion_C130J_Base: HouseNoDestruct
	{
		scope = 0;
		model = "DayZExpansion\Vehicles\Air\C130J\C130J.p3d";
	};
	class bldr_expansion_C130J_Animated_Base: bldr_expansion_C130J_Base
	{
		scope = 0;
		class AnimationSources
		{
			class rotor
			{
				source = "user";
				animPeriod = 0.0009999999;
				initPhase = 0;
			};
			class hiderotor
			{
				source = "user";
				animPeriod = 0.0009999999;
				initPhase = 1;
			};
		};
	};
	class bldr_expansion_C130J_Grey: bldr_expansion_C130J_Base
	{
		scope = 2;
	};
	class bldr_expansion_C130J_Animated_Grey: bldr_expansion_C130J_Animated_Base
	{
		scope = 2;
	};
	class bldr_expansion_An2_Base: HouseNoDestruct
	{
		scope = 0;
		model = "DayZExpansion\Vehicles\Air\An2\an2.p3d";
	};
	class bldr_expansion_An2_Animated_Base: bldr_expansion_An2_Base
	{
		scope = 0;
		class AnimationSources
		{
			class rotor
			{
				source = "user";
				animPeriod = 0.0009999999;
				initPhase = 0;
			};
			class rotorwithoutblur
			{
				source = "user";
				animPeriod = 0.0009999999;
				initPhase = 1;
			};
		};
	};
	class bldr_expansion_An2_Green: bldr_expansion_An2_Base
	{
		scope = 2;
	};
	class bldr_expansion_An2_Animated_Green: bldr_expansion_An2_Animated_Base
	{
		scope = 2;
	};
};
