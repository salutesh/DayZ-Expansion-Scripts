#define _ARMA_

class CfgPatches
{
	class DayZExpansion_Vehicles_Ground_UAZ
	{
		units[] = {"ExpansionStaticWreckUAZ","ExpansionStaticWreckUAZRoofless","ExpansionUAZWheel","ExpansionUAZDoorDriver","ExpansionUAZDoorCoDriver","ExpansionUAZDoorCargo1","ExpansionUAZDoorCargo2","ExpansionUAZDoorTrunk","ExpansionUAZDoorHood","ExpansionUAZ","ExpansionUAZRoofless","ExpansionUAZCargoRoofless","ExpansionUAZCargo","ExpansionSpraycanGreen","ExpansionUAZ_Green","ExpansionUAZCargo_Green","ExpansionUAZRoofless_Green","ExpansionUAZCargoRoofless_Green","ExpansionUAZDoorDriver_Green","ExpansionUAZDoorCoDriver_Green","ExpansionUAZDoorCargo1_Green","ExpansionUAZDoorCargo2_Green","ExpansionUAZDoorHood_Green","ExpansionSpraycanBlack","ExpansionUAZ_Black","ExpansionUAZCargo_Black","ExpansionUAZRoofless_Black","ExpansionUAZCargoRoofless_Black","ExpansionUAZDoorDriver_Black","ExpansionUAZDoorCoDriver_Black","ExpansionUAZDoorCargo1_Black","ExpansionUAZDoorCargo2_Black","ExpansionUAZDoorHood_Black","ExpansionSpraycanLightGreen","ExpansionUAZ_LightGreen","ExpansionUAZCargo_LightGreen","ExpansionUAZRoofless_LightGreen","ExpansionUAZCargoRoofless_LightGreen","ExpansionUAZDoorDriver_LightGreen","ExpansionUAZDoorCoDriver_LightGreen","ExpansionUAZDoorCargo1_LightGreen","ExpansionUAZDoorCargo2_LightGreen","ExpansionUAZDoorHood_LightGreen"};
		weapons[] = {};
		requiredVersion = 0.1;
		requiredAddons[] = {"DZ_Vehicles_Wheeled"};
	};
};
class CfgSlots
{
	class Slot_uazwheel_1_1
	{
		name = "uazwheel_1_1";
		displayName = "$STR_EXPANSION_VEHICLE_UAZ_WHEEL";
		descriptionShort = "$STR_EXPANSION_VEHICLE_UAZ_WHEEL_DESC";
		selection = "wheel_1_1";
		ghostIcon = "wheel";
	};
	class Slot_uazwheel_1_2
	{
		name = "uazwheel_1_2";
		displayName = "$STR_EXPANSION_VEHICLE_UAZ_WHEEL";
		descriptionShort = "$STR_EXPANSION_VEHICLE_UAZ_WHEEL_DESC";
		selection = "wheel_1_2";
		ghostIcon = "wheel";
	};
	class Slot_uazwheel_2_1
	{
		name = "uazwheel_2_1";
		displayName = "$STR_EXPANSION_VEHICLE_UAZ_WHEEL";
		descriptionShort = "$STR_EXPANSION_VEHICLE_UAZ_WHEEL_DESC";
		selection = "wheel_2_1";
		ghostIcon = "wheel";
	};
	class Slot_uazwheel_2_2
	{
		name = "uazwheel_2_2";
		displayName = "$STR_EXPANSION_VEHICLE_UAZ_WHEEL";
		descriptionShort = "$STR_EXPANSION_VEHICLE_UAZ_WHEEL_DESC";
		selection = "wheel_2_2";
		ghostIcon = "wheel";
	};
	class Slot_uazdriverdoor
	{
		name = "uazdriverdoor";
		displayName = "$STR_EXPANSION_VEHICLE_UAZ_DOOR_FRONT_LEFT";
		descriptionShort = "";
		selection = "uazdriverdoor";
		ghostIcon = "doorfront";
	};
	class Slot_uazcodriverdoor
	{
		name = "uazcodriverdoor";
		displayName = "$STR_EXPANSION_VEHICLE_UAZ_DOOR_FRONT_RIGHT";
		descriptionShort = "";
		selection = "uazcodriverdoor";
		ghostIcon = "doorfront";
	};
	class Slot_uazcargo1door
	{
		name = "uazcargo1door";
		displayName = "$STR_EXPANSION_VEHICLE_UAZ_DOOR_BACK_LEFT";
		descriptionShort = "DESCRIPTION";
		selection = "uazcargo1door";
		ghostIcon = "doorrear";
	};
	class Slot_uazcargo2door
	{
		name = "uazcargo2door";
		displayName = "$STR_EXPANSION_VEHICLE_UAZ_DOOR_BACK_RIGHT";
		descriptionShort = "";
		selection = "uazcargo2door";
		ghostIcon = "doorrear";
	};
	class Slot_uazhooddoor
	{
		name = "uazhooddoor";
		displayName = "$STR_EXPANSION_VEHICLE_UAZ_DOOR_HOOD";
		descriptionShort = "";
		selection = "uazhooddoor";
		ghostIcon = "hood";
	};
	class Slot_uaztrunkdoor
	{
		name = "uaztrunkdoor";
		displayName = "$STR_EXPANSION_VEHICLE_UAZ_DOOR_TRUNK";
		descriptionShort = "";
		selection = "uaztrunkdoor";
		ghostIcon = "trunk";
	};
};
class CfgVehicles
{
	class CarScript;
	class HatchbackWheel;
	class Inventory_Base;
	class CivSedanDoors_Driver;
	class SimulationModule;
	class Axles;
	class Front;
	class Wheels;
	class Rear;
	class Left;
	class Right;
	class AnimationSources;
	class Crew;
	class Driver;
	class CoDriver;
	class CivilianSedan;
	class HouseNoDestruct;
	class ExpansionStaticWreckUAZ: HouseNoDestruct
	{
		scope = 2;
		model = "\DayZExpansion\Vehicles\Ground\UAZ\UAZ_Wreck_Roof.p3d";
		class AnimationSources: AnimationSources
		{
			class uazdriverdoor
			{
				source = "user";
				initPhase = 0;
				animPeriod = 0.5;
			};
			class uazcodriverdoor
			{
				source = "user";
				initPhase = 0.2;
				animPeriod = 0.5;
			};
			class uazcargo1door
			{
				source = "user";
				initPhase = 0.4;
				animPeriod = 0.5;
			};
			class uazcargo2door
			{
				source = "user";
				initPhase = 0.33;
				animPeriod = 0.5;
			};
			class uaztrunkdoor
			{
				source = "user";
				initPhase = 1;
				animPeriod = 0.5;
			};
			class uazhooddoor
			{
				source = "user";
				initPhase = 0.68;
				animPeriod = 0.5;
			};
			class steering_hub_1_1
			{
				source = "user";
				initPhase = 0.2;
				animPeriod = 0.5;
			};
			class steering_hub_1_2
			{
				source = "user";
				initPhase = 0.2;
				animPeriod = 0.5;
			};
		};
	};
	class ExpansionStaticWreckUAZRoofless: ExpansionStaticWreckUAZ
	{
		model = "\DayZExpansion\Vehicles\Ground\UAZ\UAZ_Wreck_Roofless.p3d";
		class AnimationSources: AnimationSources
		{
			class backseats
			{
				source = "user";
				animPeriod = 0.01;
				initPhase = 1;
			};
			class uazdriverdoor
			{
				source = "user";
				initPhase = 1;
				animPeriod = 0.5;
			};
			class uazcodriverdoor
			{
				source = "user";
				initPhase = 1;
				animPeriod = 0.5;
			};
			class uazcargo1door
			{
				source = "user";
				initPhase = 1;
				animPeriod = 0.5;
			};
			class uazcargo2door
			{
				source = "user";
				initPhase = 1;
				animPeriod = 0.5;
			};
			class uaztrunkdoor
			{
				source = "user";
				initPhase = 1;
				animPeriod = 0.5;
			};
			class uazhooddoor
			{
				source = "user";
				initPhase = 1;
				animPeriod = 0.5;
			};
			class steering_hub_1_1
			{
				source = "user";
				initPhase = 0.4;
				animPeriod = 0.5;
			};
			class steering_hub_1_2
			{
				source = "user";
				initPhase = 0.4;
				animPeriod = 0.5;
			};
		};
	};
	class ExpansionUAZWheel: HatchbackWheel
	{
		scope = 2;
		displayName = "$STR_EXPANSION_VEHICLE_UAZ_WHEEL";
		descriptionShort = "$STR_EXPANSION_VEHICLE_UAZ_WHEEL_DESC";
		model = "\DayZExpansion\Vehicles\Ground\UAZ\proxy\UAZWheel.p3d";
		weight = 15000;
		inventorySlot[] = {"uazwheel_1_1","uazwheel_1_2","uazwheel_2_1","uazwheel_2_2"};
		rotationFlags = 4;
		physLayer = "item_large";
		radiusByDamage[] = {0,0.343,0.3,0.4,0.9998,0.25,0.9999,0.2};
		radius = 0.416;
		width = 0.316;
		tyreRollResistance = 0.015;
		tyreTread = 0.7;
		class DamageSystem
		{
			class GlobalHealth
			{
				class Health
				{
					hitpoints = 200;
					healthLevels[] = {{1.0,{"DZ\vehicles\wheeled\OffroadHatchback\data\niva_wheel.rvmat"}},{0.7,{"DZ\vehicles\wheeled\OffroadHatchback\data\niva_wheel.rvmat"}},{0.5,{"DZ\vehicles\wheeled\OffroadHatchback\data\niva_wheel_dmg.rvmat"}},{0.3,{"DZ\vehicles\wheeled\OffroadHatchback\data\niva_wheel_dmg.rvmat"}},{0.0,{"DZ\vehicles\wheeled\OffroadHatchback\data\niva_wheel_dmg.rvmat"}}};
				};
			};
		};
	};
	class ExpansionUAZDoorDriver: CivSedanDoors_Driver
	{
		scope = 2;
		displayName = "$STR_EXPANSION_VEHICLE_UAZ_DOOR_FRONT_LEFT";
		descriptionShort = "$STR_EXPANSION_VEHICLE_UAZ_DOOR_FRONT_LEFT_DESC";
		model = "\DayZExpansion\Vehicles\Ground\UAZ\proxy\uazdriverdoor.p3d";
		hiddenSelections[] = {"camo"};
		hiddenSelectionsTextures[] = {"DayZExpansion\Vehicles\Ground\UAZ\new_data\uaz_out_01_co.paa"};
		hiddenSelectionsMaterials[] = {"DayZExpansion\Vehicles\Ground\UAZ\new_data\uaz_exterior_01.rvmat"};
		weight = 15000;
		itemSize[] = {10,10};
		itemBehaviour = 0;
		inventorySlot = "uazdriverdoor";
		rotationFlags = 8;
		physLayer = "item_large";
	};
	class ExpansionUAZDoorCoDriver: ExpansionUAZDoorDriver
	{
		displayName = "$STR_EXPANSION_VEHICLE_UAZ_DOOR_FRONT_RIGHT";
		descriptionShort = "$STR_EXPANSION_VEHICLE_UAZ_DOOR_FRONT_RIGHT_DESC";
		model = "\DayZExpansion\Vehicles\Ground\UAZ\proxy\uazcodriverdoor.p3d";
		inventorySlot = "uazcodriverdoor";
		rotationFlags = 4;
		itemBehaviour = 0;
	};
	class ExpansionUAZDoorCargo1: ExpansionUAZDoorDriver
	{
		displayName = "$STR_EXPANSION_VEHICLE_UAZ_DOOR_BACK_LEFT";
		descriptionShort = "$STR_EXPANSION_VEHICLE_UAZ_DOOR_BACK_LEFT_DESC";
		model = "\DayZExpansion\Vehicles\Ground\UAZ\proxy\uazcargo1door.p3d";
		inventorySlot = "uazcargo1door";
		rotationFlags = 4;
		itemBehaviour = 0;
	};
	class ExpansionUAZDoorCargo2: ExpansionUAZDoorDriver
	{
		displayName = "$STR_EXPANSION_VEHICLE_UAZ_DOOR_BACK_RIGHT";
		descriptionShort = "$STR_EXPANSION_VEHICLE_UAZ_DOOR_BACK_RIGHT_DESC";
		model = "\DayZExpansion\Vehicles\Ground\UAZ\proxy\uazcargo2door.p3d";
		inventorySlot = "uazcargo2door";
		rotationFlags = 4;
		itemBehaviour = 0;
	};
	class ExpansionUAZDoorTrunk: ExpansionUAZDoorDriver
	{
		displayName = "$STR_EXPANSION_VEHICLE_UAZ_DOOR_TRUNK";
		descriptionShort = "$STR_EXPANSION_VEHICLE_UAZ_DOOR_TRUNK_DESC";
		model = "\DayZExpansion\Vehicles\Ground\UAZ\proxy\uazTrunkdoor.p3d";
		inventorySlot = "uaztrunkdoor";
		rotationFlags = 4;
		itemBehaviour = 0;
	};
	class ExpansionUAZDoorHood: ExpansionUAZDoorDriver
	{
		displayName = "$STR_EXPANSION_VEHICLE_UAZ_DOOR_HOOD";
		descriptionShort = "$STR_EXPANSION_VEHICLE_UAZ_DOOR_HOOD_DESC";
		model = "\DayZExpansion\Vehicles\Ground\UAZ\proxy\uazHooddoor.p3d";
		inventorySlot = "uazhooddoor";
		rotationFlags = 2;
		itemBehaviour = 0;
	};
	class ExpansionUAZBase: CivilianSedan
	{
		scope = 0;
		displayName = "";
		model = "";
		vehicleClass = "Expansion_Car";
		attachments[] = {"CarBattery","Reflector_1_1","Reflector_2_1","CarRadiator","SparkPlug","uazwheel_1_1","uazwheel_1_2","uazwheel_2_1","uazwheel_2_2","uazdriverdoor","uazcodriverdoor","uazcargo1door","uazcargo2door""uazhooddoor"};
		doors[] = {"uazdriverdoor","uazcodriverdoor","uazcargo1door","uazcargo2door","uazhooddoor"};
		defaultSkin = "Green";
		fuelCapacity = 40;
		fuelConsumption = 10;
		class Crew: Crew
		{
			class Driver: Driver
			{
				actionSel = "seat_driver";
				proxyPos = "crewDriver";
				getInPos = "pos_driver";
				getInDir = "pos_driver_dir";
			};
			class CoDriver: CoDriver
			{
				actionSel = "seat_codriver";
				proxyPos = "crewCoDriver";
				getInPos = "pos_codriver";
				getInDir = "pos_codriver_dir";
			};
			class Cargo1
			{
				actionSel = "seat_cargo1";
				proxyPos = "crewCargo1";
				getInPos = "pos_cargo1";
				getInDir = "pos_cargo1_dir";
			};
			class Cargo2
			{
				actionSel = "seat_cargo2";
				proxyPos = "crewCargo2";
				getInPos = "pos_cargo2";
				getInDir = "pos_cargo2_dir";
			};
		};
		class SimulationModule: SimulationModule
		{
			drive = "DRIVE_AWD";
			centralDiffRatio = 1.45;
			airDragFrontTotal = 0.995;
			class Steering
			{
				increaseSpeed[] = {0,45,60,23,100,12};
				decreaseSpeed[] = {0,80,60,40,90,20};
				centeringSpeed[] = {0,0,15,25,60,40,100,60};
			};
			class Throttle
			{
				reactionTime = 1.25;
				defaultThrust = 0.85;
				gentleThrust = 0.7;
				turboCoef = 5;
				gentleCoef = 0.75;
			};
			braking[] = {0.0,0.1,1.0,0.8,2.5,0.9,3.0,1.0};
			class Engine
			{
				inertia = 0.15;
				steepness = 1.5;
				torqueMax = 136;
				torqueRpm = 3400;
				powerMax = 58.7;
				powerRpm = 5400;
				rpmIdle = 850;
				rpmMin = 900;
				rpmClutch = 1350;
				rpmRedline = 6000;
				rpmMax = 8000;
			};
			class Gearbox
			{
				reverse = 3.726;
				ratios[] = {3.967,2.3999999,1.5609999,1};
				timeToUncoupleClutch = 0.2;
				timeToCoupleClutch = 0.35;
				maxClutchTorque = 330;
			};
			class Axles: Axles
			{
				class Front: Front
				{
					maxSteeringAngle = 30;
					finalRatio = 4.1;
					brakeBias = 0.6;
					brakeForce = 4500;
					wheelHubMass = 5;
					wheelHubRadius = 0.15;
					class Suspension
					{
						swayBar = 1700;
						stiffness = 40000;
						compression = 2100;
						damping = 7500;
						travelMaxUp = 0.0882;
						travelMaxDown = 0.0833;
					};
					class Wheels: Wheels
					{
						class Left: Left
						{
							animDamper = "damper_1_1";
							inventorySlot = "uazwheel_1_1";
						};
						class Right: Right
						{
							animDamper = "damper_2_1";
							inventorySlot = "uazwheel_2_1";
						};
					};
				};
				class Rear: Rear
				{
					maxSteeringAngle = 0;
					finalRatio = 4.1;
					brakeBias = 0.4;
					brakeForce = 4200;
					wheelHubMass = 5;
					wheelHubRadius = 0.15;
					class Suspension
					{
						swayBar = 1800;
						stiffness = 21000;
						compression = 2200;
						damping = 7600;
						travelMaxUp = 0.1587;
						travelMaxDown = 0.1059;
					};
					class Wheels: Wheels
					{
						class Left: Left
						{
							animDamper = "damper_1_2";
							inventorySlot = "uazwheel_1_2";
						};
						class Right: Right
						{
							animDamper = "damper_2_2";
							inventorySlot = "uazwheel_2_2";
						};
					};
				};
			};
		};
		class Cargo
		{
			itemsCargoSize[] = {10,30};
			allowOwnedCargoManipulation = 1;
			openable = 1;
		};
		class AnimationSources: AnimationSources
		{
			class damper_1_1
			{
				source = "user";
				initPhase = 0.4857;
				animPeriod = 1;
			};
			class damper_2_1: damper_1_1{};
			class damper_1_2
			{
				source = "user";
				initPhase = 0.4002;
				animPeriod = 1;
			};
			class damper_2_2: damper_1_2{};
			class uazdriverdoor
			{
				source = "user";
				initPhase = 0;
				animPeriod = 0.5;
			};
			class uazcodriverdoor: uazdriverdoor{};
			class uazcargo1door: uazdriverdoor{};
			class uazcargo2door: uazdriverdoor{};
			class uaztrunkdoor: uazdriverdoor{};
			class uazhooddoor: uazdriverdoor{};
		};
		class Sounds
		{
			thrust = 0.6;
			thrustTurbo = 1;
			thrustGentle = 0.3;
			thrustSmoothCoef = 0.02;
			camposSmoothCoef = 0.03;
			soundSetsFilter[] = {"offroad_Engine_Offload_Ext_Rpm1_SoundSet","offroad_Engine_Offload_Ext_Rpm2_SoundSet","offroad_Engine_Offload_Ext_Rpm3_SoundSet","offroad_Engine_Offload_Ext_Rpm4_SoundSet","offroad_Engine_Offload_Ext_Rpm5_SoundSet","offroad_Engine_Ext_Rpm0_SoundSet","offroad_Engine_Ext_Rpm1_SoundSet","offroad_Engine_Ext_Rpm2_SoundSet","offroad_Engine_Ext_Rpm3_SoundSet","offroad_Engine_Ext_Rpm4_SoundSet","offroad_Engine_Ext_Rpm5_SoundSet","offroad_Engine_Ext_Broken_SoundSet","offroad_Tires_rock_slow_Ext_SoundSet","offroad_Tires_rock_fast_Ext_SoundSet","offroad_Tires_grass_slow_Ext_SoundSet","offroad_Tires_grass_fast_Ext_SoundSet","offroad_Tires_gravel_slow_Ext_SoundSet","offroad_Tires_gravel_fast_Ext_SoundSet","offroad_Tires_gravel_dust_fast_Ext_SoundSet","offroad_Tires_asphalt_slow_Ext_SoundSet","offroad_Tires_asphalt_fast_Ext_SoundSet","offroad_Tires_water_slow_Ext_SoundSet","offroad_Tires_water_fast_Ext_SoundSet","Offroad_skid_dirt_SoundSet","offroad_dirt_turn_SoundSet","offroad_Rain_Ext_SoundSet","offroad_damper_left_SoundSet","offroad_damper_right_SoundSet"};
			soundSetsInt[] = {"Offroad_Tires_Asphalt_Slow_General_Int_SoundSet","Offroad_Tires_Asphalt_Fast_General_Int_SoundSet","Offroad_Wind_SoundSet"};
		};
		class ObstacleGenerator
		{
			carve = 1;
			timeToStationary = 5;
			moveThreshold = 0.5;
			class Shapes
			{
				class Cylindric
				{
					class Cyl1
					{
						radius = 1;
						height = 1.5;
						center[] = {0,0,0.7};
					};
					class Cyl3
					{
						radius = 1;
						height = 1.5;
						center[] = {0,0,-0.7};
					};
				};
			};
		};
		class GUIInventoryAttachmentsProps
		{
			class Engine
			{
				name = "$STR_attachment_Engine0";
				description = "";
				icon = "cat_vehicle_engine";
				attachmentSlots[] = {"CarBattery","CarRadiator","SparkPlug"};
			};
			class Body
			{
				name = "$STR_attachment_Body0";
				description = "";
				icon = "cat_vehicle_body";
				attachmentSlots[] = {"Reflector_1_1","Reflector_2_1","uazdriverdoor","uazcodriverdoor","uazcargo1door","uazcargo2door","uazhooddoor"};
			};
			class Chassis
			{
				name = "$STR_attachment_Chassis0";
				description = "";
				icon = "cat_vehicle_chassis";
				attachmentSlots[] = {"uazwheel_1_1","uazwheel_1_2","uazwheel_2_1","uazwheel_2_2"};
			};
		};
	};
	class ExpansionUAZ: ExpansionUAZBase
	{
		scope = 2;
		displayname = "$STR_EXPANSION_VEHICLE_UAZ";
		model = "\DayZExpansion\Vehicles\Ground\UAZ\UAZ.p3d";
		modelZeroPointDistanceFromGround = 1.67;
		vehicleClass = "Expansion_Car";
		hiddenSelections[] = {"light_1_1","light_2_1","light_brake_1_2","light_brake_2_2","light_reverse_1_2","light_reverse_2_2","light_1_2","light_2_2","light_dashboard","camo1","camo2"};
		hiddenSelectionsTextures[] = {"dz\vehicles\wheeled\civiliansedan\data\sedan_glass_ca.paa","dz\vehicles\wheeled\civiliansedan\data\sedan_glass_ca.paa","","","","","","","","DayZExpansion\Vehicles\Ground\UAZ\new_data\uaz_out_01_co.paa","DayZExpansion\Vehicles\Ground\UAZ\new_data\uaz_int_01_co.paa"};
		hiddenSelectionsMaterials[] = {"dz\vehicles\wheeled\offroadhatchback\data\offroadhatchback_lights.rvmat","dz\vehicles\wheeled\offroadhatchback\data\offroadhatchback_lights.rvmat","dz\vehicles\wheeled\offroadhatchback\data\offroadhatchback_lights.rvmat","dz\vehicles\wheeled\offroadhatchback\data\offroadhatchback_lights.rvmat","dz\vehicles\wheeled\offroadhatchback\data\offroadhatchback_lights.rvmat","dz\vehicles\wheeled\offroadhatchback\data\offroadhatchback_lights.rvmat","dz\vehicles\wheeled\offroadhatchback\data\offroadhatchback_lights.rvmat","dz\vehicles\wheeled\offroadhatchback\data\offroadhatchback_lights.rvmat","dz\vehicles\wheeled\offroadhatchback\data\offroadhatchback_lights.rvmat","DayZExpansion\Vehicles\Ground\UAZ\new_data\uaz_exterior_01.rvmat","DayZExpansion\Vehicles\Ground\UAZ\new_data\uaz_interior_01.rvmat"};
		dashboardMatOn = "dz\vehicles\wheeled\transitbus\data\ikarus_dashboarde.rvmat";
		dashboardMatOff = "dz\vehicles\wheeled\transitbus\data\ikarus_dashboard.rvmat";
		frontReflectorMatOn = "dz\vehicles\wheeled\civiliansedan\data\civiliansedan_lights.rvmat";
		frontReflectorMatOff = "dz\vehicles\wheeled\offroadhatchback\data\offroadhatchback_lights.rvmat";
		brakeReflectorMatOn = "dz\vehicles\wheeled\civiliansedan\data\Sedan_TailLights_e.rvmat";
		brakeReflectorMatOff = "dz\vehicles\wheeled\civiliansedan\data\Sedan_TailLights.rvmat";
		ReverseReflectorMatOn = "dz\vehicles\wheeled\civiliansedan\data\Sedan_TailLights_e.rvmat";
		ReverseReflectorMatOff = "dz\vehicles\wheeled\civiliansedan\data\Sedan_TailLights.rvmat";
		TailReflectorMatOn = "dz\vehicles\wheeled\civiliansedan\data\Sedan_TailLights_e.rvmat";
		TailReflectorMatOff = "dz\vehicles\wheeled\civiliansedan\data\Sedan_TailLights.rvmat";
		attachments[] = {"CarBattery","CarRadiator","SparkPlug","Material_Shelter_Fabric","Reflector_1_1","Reflector_2_1","uazwheel_1_1","uazwheel_1_2","uazwheel_2_1","uazwheel_2_2","uazdriverdoor","uazcodriverdoor","uazcargo1door","uazcargo2door","uazhooddoor"};
		class AnimationSources: AnimationSources
		{
			class roof
			{
				source = "user";
				animPeriod = 0.01;
				initPhase = 1;
			};
			class frameroof
			{
				source = "user";
				animPeriod = 0.01;
				initPhase = 0;
			};
			class backseats
			{
				source = "user";
				animPeriod = 0.01;
				initPhase = 0;
			};
		};
		class GUIInventoryAttachmentsProps
		{
			class Engine
			{
				name = "$STR_attachment_Engine0";
				description = "";
				icon = "cat_vehicle_engine";
				attachmentSlots[] = {"CarBattery","CarRadiator","SparkPlug"};
			};
			class Body
			{
				name = "$STR_attachment_Body0";
				description = "";
				icon = "cat_vehicle_body";
				attachmentSlots[] = {"Reflector_1_1","Reflector_2_1","uazdriverdoor","uazcodriverdoor","uazcargo1door","uazcargo2door","uazhooddoor","Material_Shelter_Fabric"};
			};
			class Chassis
			{
				name = "$STR_attachment_Chassis0";
				description = "";
				icon = "cat_vehicle_chassis";
				attachmentSlots[] = {"uazwheel_1_1","uazwheel_1_2","uazwheel_2_1","uazwheel_2_2"};
			};
		};
	};
	class ExpansionUAZRoofless: ExpansionUAZ
	{
		scope = 2;
		displayname = "$STR_EXPANSION_VEHICLE_UAZ_ROOFLESS";
		vehicleClass = "Expansion_Car";
		attachments[] = {"CarBattery","CarRadiator","SparkPlug","Reflector_1_1","Reflector_2_1","uazwheel_1_1","uazwheel_1_2","uazwheel_2_1","uazwheel_2_2","uazdriverdoor","uazcodriverdoor","uazcargo1door","uazcargo2door","uazhooddoor"};
		class AnimationSources: AnimationSources
		{
			class roof
			{
				source = "user";
				animPeriod = 0.01;
				initPhase = 1;
			};
			class frameroof
			{
				source = "user";
				animPeriod = 0.01;
				initPhase = 1;
			};
			class backseats
			{
				source = "user";
				animPeriod = 0.01;
				initPhase = 0;
			};
		};
		class GUIInventoryAttachmentsProps
		{
			class Engine
			{
				name = "$STR_attachment_Engine0";
				description = "";
				icon = "cat_vehicle_engine";
				attachmentSlots[] = {"CarBattery","CarRadiator","SparkPlug"};
			};
			class Body
			{
				name = "$STR_attachment_Body0";
				description = "";
				icon = "cat_vehicle_body";
				attachmentSlots[] = {"Reflector_1_1","Reflector_2_1","uazdriverdoor","uazcodriverdoor","uazcargo1door","uazcargo2door","uazhooddoor"};
			};
			class Chassis
			{
				name = "$STR_attachment_Chassis0";
				description = "";
				icon = "cat_vehicle_chassis";
				attachmentSlots[] = {"uazwheel_1_1","uazwheel_1_2","uazwheel_2_1","uazwheel_2_2"};
			};
		};
	};
	class ExpansionUAZCargoRoofless: ExpansionUAZ
	{
		scope = 2;
		displayname = "$STR_EXPANSION_VEHICLE_UAZ_CARGO_ROOFLESS";
		model = "\DayZExpansion\Vehicles\Ground\UAZ\UAZCargoRoofless.p3d";
		vehicleClass = "Expansion_Car";
		attachments[] = {"CarBattery","CarRadiator","SparkPlug","Reflector_1_1","Reflector_2_1","uazwheel_1_1","uazwheel_1_2","uazwheel_2_1","uazwheel_2_2","uazdriverdoor","uazcodriverdoor","uazcargo1door","uazcargo2door","uazhooddoor"};
		class AnimationSources: AnimationSources
		{
			class roof
			{
				source = "user";
				animPeriod = 0.01;
				initPhase = 1;
			};
			class backseats
			{
				source = "user";
				animPeriod = 0.01;
				initPhase = 1;
			};
		};
		class Cargo
		{
			itemsCargoSize[] = {10,45};
			allowOwnedCargoManipulation = 1;
			openable = 1;
		};
		class Crew: Crew
		{
			class Driver: Driver
			{
				actionSel = "seat_driver";
				proxyPos = "crewDriver";
				getInPos = "pos_driver";
				getInDir = "pos_driver_dir";
			};
			class CoDriver: CoDriver
			{
				actionSel = "seat_codriver";
				proxyPos = "crewCoDriver";
				getInPos = "pos_codriver";
				getInDir = "pos_codriver_dir";
			};
			class Cargo1
			{
				actionSel = "";
				proxyPos = "";
				getInPos = "";
				getInDir = "";
			};
			class Cargo2
			{
				actionSel = "";
				proxyPos = "";
				getInPos = "";
				getInDir = "";
			};
		};
		class GUIInventoryAttachmentsProps
		{
			class Engine
			{
				name = "$STR_attachment_Engine0";
				description = "";
				icon = "cat_vehicle_engine";
				attachmentSlots[] = {"CarBattery","CarRadiator","SparkPlug"};
			};
			class Body
			{
				name = "$STR_attachment_Body0";
				description = "";
				icon = "cat_vehicle_body";
				attachmentSlots[] = {"Reflector_1_1","Reflector_2_1","uazdriverdoor","uazcodriverdoor","uazcargo1door","uazcargo2door","uazhooddoor"};
			};
			class Chassis
			{
				name = "$STR_attachment_Chassis0";
				description = "";
				icon = "cat_vehicle_chassis";
				attachmentSlots[] = {"uazwheel_1_1","uazwheel_1_2","uazwheel_2_1","uazwheel_2_2"};
			};
		};
	};
	class ExpansionUAZCargo: ExpansionUAZ
	{
		scope = 2;
		displayname = "$STR_EXPANSION_VEHICLE_UAZ_CARGO";
		vehicleClass = "Expansion_Car";
		class AnimationSources: AnimationSources
		{
			class backseats
			{
				source = "user";
				animPeriod = 0.01;
				initPhase = 1;
			};
		};
		class Cargo
		{
			itemsCargoSize[] = {10,45};
			allowOwnedCargoManipulation = 1;
			openable = 1;
		};
		class Crew: Crew
		{
			class Driver: Driver
			{
				actionSel = "seat_driver";
				proxyPos = "crewDriver";
				getInPos = "pos_driver";
				getInDir = "pos_driver_dir";
			};
			class CoDriver: CoDriver
			{
				actionSel = "seat_codriver";
				proxyPos = "crewCoDriver";
				getInPos = "pos_codriver";
				getInDir = "pos_codriver_dir";
			};
			class Cargo1
			{
				actionSel = "";
				proxyPos = "";
				getInPos = "";
				getInDir = "";
			};
			class Cargo2
			{
				actionSel = "";
				proxyPos = "";
				getInPos = "";
				getInDir = "";
			};
		};
	};
	class ExpansionSpraycanBase;
	class ExpansionSpraycanGreen: ExpansionSpraycanBase
	{
		scope = 2;
		displayName = "$STR_EXPANSION_SPRAYCAN_Green_NAME";
		descriptionShort = "$STR_EXPANSION_SPRAYCAN_Green_DESC";
		hiddenSelectionsTextures[] = {"DZ\gear\consumables\data\spraycan_green_co.paa"};
		skinName = "Green";
	};
	class ExpansionUAZ_Green: ExpansionUAZ
	{
		skinBase = "ExpansionUAZ";
		skinName = "Green";
	};
	class ExpansionUAZCargo_Green: ExpansionUAZCargo
	{
		skinBase = "ExpansionUAZ";
		skinName = "Green";
	};
	class ExpansionUAZRoofless_Green: ExpansionUAZRoofless
	{
		skinBase = "ExpansionUAZ";
		skinName = "Green";
	};
	class ExpansionUAZCargoRoofless_Green: ExpansionUAZCargoRoofless
	{
		skinBase = "ExpansionUAZ";
		skinName = "Green";
	};
	class ExpansionUAZDoorDriver_Green: ExpansionUAZDoorDriver
	{
		skinBase = "ExpansionUAZDoorDriver";
		skinName = "Green";
	};
	class ExpansionUAZDoorCoDriver_Green: ExpansionUAZDoorCoDriver
	{
		skinBase = "ExpansionUAZDoorCoDriver";
		skinName = "Green";
	};
	class ExpansionUAZDoorCargo1_Green: ExpansionUAZDoorCargo1
	{
		skinBase = "ExpansionUAZDoorCargo1";
		skinName = "Green";
	};
	class ExpansionUAZDoorCargo2_Green: ExpansionUAZDoorCargo2
	{
		skinBase = "ExpansionUAZDoorCargo2";
		skinName = "Green";
	};
	class ExpansionUAZDoorHood_Green: ExpansionUAZDoorHood
	{
		skinBase = "ExpansionUAZDoorHood";
		skinName = "Green";
	};
	class ExpansionSpraycanBlack: ExpansionSpraycanBase
	{
		scope = 2;
		displayName = "$STR_EXPANSION_SPRAYCAN_Black_NAME";
		descriptionShort = "$STR_EXPANSION_SPRAYCAN_Black_DESC";
		hiddenSelectionsTextures[] = {"DZ\gear\consumables\data\spraycan_black_co.paa"};
		skinName = "Black";
	};
	class ExpansionUAZ_Black: ExpansionUAZ
	{
		skinBase = "ExpansionUAZ";
		skinName = "Black";
	};
	class ExpansionUAZCargo_Black: ExpansionUAZCargo
	{
		skinBase = "ExpansionUAZ";
		skinName = "Black";
	};
	class ExpansionUAZRoofless_Black: ExpansionUAZRoofless
	{
		skinBase = "ExpansionUAZ";
		skinName = "Black";
	};
	class ExpansionUAZCargoRoofless_Black: ExpansionUAZCargoRoofless
	{
		skinBase = "ExpansionUAZ";
		skinName = "Black";
	};
	class ExpansionUAZDoorDriver_Black: ExpansionUAZDoorDriver
	{
		skinBase = "ExpansionUAZDoorDriver";
		skinName = "Black";
	};
	class ExpansionUAZDoorCoDriver_Black: ExpansionUAZDoorCoDriver
	{
		skinBase = "ExpansionUAZDoorCoDriver";
		skinName = "Black";
	};
	class ExpansionUAZDoorCargo1_Black: ExpansionUAZDoorCargo1
	{
		skinBase = "ExpansionUAZDoorCargo1";
		skinName = "Black";
	};
	class ExpansionUAZDoorCargo2_Black: ExpansionUAZDoorCargo2
	{
		skinBase = "ExpansionUAZDoorCargo2";
		skinName = "Black";
	};
	class ExpansionUAZDoorHood_Black: ExpansionUAZDoorHood
	{
		skinBase = "ExpansionUAZDoorHood";
		skinName = "Black";
	};
	class ExpansionSpraycanLightGreen: ExpansionSpraycanBase
	{
		scope = 2;
		displayName = "$STR_EXPANSION_SPRAYCAN_LightGreen_NAME";
		descriptionShort = "$STR_EXPANSION_SPRAYCAN_LightGreen_DESC";
		hiddenSelectionsTextures[] = {"DZ\gear\consumables\data\spraycan_green_co.paa"};
		skinName = "LightGreen";
	};
	class ExpansionUAZ_LightGreen: ExpansionUAZ
	{
		skinBase = "ExpansionUAZ";
		skinName = "LightGreen";
	};
	class ExpansionUAZCargo_LightGreen: ExpansionUAZCargo
	{
		skinBase = "ExpansionUAZ";
		skinName = "LightGreen";
	};
	class ExpansionUAZRoofless_LightGreen: ExpansionUAZRoofless
	{
		skinBase = "ExpansionUAZ";
		skinName = "LightGreen";
	};
	class ExpansionUAZCargoRoofless_LightGreen: ExpansionUAZCargoRoofless
	{
		skinBase = "ExpansionUAZ";
		skinName = "LightGreen";
	};
	class ExpansionUAZDoorDriver_LightGreen: ExpansionUAZDoorDriver
	{
		skinBase = "ExpansionUAZDoorDriver";
		skinName = "LightGreen";
	};
	class ExpansionUAZDoorCoDriver_LightGreen: ExpansionUAZDoorCoDriver
	{
		skinBase = "ExpansionUAZDoorCoDriver";
		skinName = "LightGreen";
	};
	class ExpansionUAZDoorCargo1_LightGreen: ExpansionUAZDoorCargo1
	{
		skinBase = "ExpansionUAZDoorCargo1";
		skinName = "LightGreen";
	};
	class ExpansionUAZDoorCargo2_LightGreen: ExpansionUAZDoorCargo2
	{
		skinBase = "ExpansionUAZDoorCargo2";
		skinName = "LightGreen";
	};
	class ExpansionUAZDoorHood_LightGreen: ExpansionUAZDoorHood
	{
		skinBase = "ExpansionUAZDoorHood";
		skinName = "LightGreen";
	};
};
class CfgNonAIVehicles
{
	class ProxyVehiclePart;
	class ProxyUAZWheel: ProxyVehiclePart
	{
		model = "\DayZExpansion\Vehicles\Ground\UAZ\proxy\UAZWheel.p3d";
		inventorySlot[] = {"uazwheel_1_1","uazwheel_1_2","uazwheel_2_1","uazwheel_2_2"};
	};
	class Proxyuazdriverdoor: ProxyVehiclePart
	{
		model = "\DayZExpansion\Vehicles\Ground\UAZ\proxy\uazdriverdoor.p3d";
		inventorySlot = "uazdriverdoor";
	};
	class Proxyuazcodriverdoor: ProxyVehiclePart
	{
		model = "\DayZExpansion\Vehicles\Ground\UAZ\proxy\uazcodriverdoor.p3d";
		inventorySlot = "uazcodriverdoor";
	};
	class Proxyuazcargo1door: ProxyVehiclePart
	{
		model = "\DayZExpansion\Vehicles\Ground\UAZ\proxy\uazcargo1door.p3d";
		inventorySlot = "uazcargo1door";
	};
	class Proxyuazcargo2door: ProxyVehiclePart
	{
		model = "\DayZExpansion\Vehicles\Ground\UAZ\proxy\uazcargo2door.p3d";
		inventorySlot = "uazcargo2door";
	};
	class Proxyuaztrunkdoor: ProxyVehiclePart
	{
		model = "\DayZExpansion\Vehicles\Ground\UAZ\proxy\uaztrunkdoor.p3d";
		inventorySlot = "uaztrunkdoor";
	};
	class Proxyuazhooddoor: ProxyVehiclePart
	{
		model = "\DayZExpansion\Vehicles\Ground\UAZ\proxy\uazhooddoor.p3d";
		inventorySlot = "uazhooddoor";
	};
};
