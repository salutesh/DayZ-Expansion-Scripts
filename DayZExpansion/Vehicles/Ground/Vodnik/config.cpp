#define _ARMA_

class CfgPatches
{
	class DayZExpansion_Vehicles_Ground_Vodnik
	{
		units[] = {"ExpansionVodnikWheel","Vehicle_ExpansionVodnikWheel","ExpansionVodnikDoorDriver","ExpansionVodnikDoorCoDriver","ExpansionVodnik","Vehicle_ExpansionVodnik","ExpansionSpraycanCamo","ExpansionVodnik_Camo","Vehicle_ExpansionVodnik_Camo","ExpansionVodnikDoorDriver_Camo","ExpansionVodnikDoorCoDriver_Camo","ExpansionSpraycanCamoMedical","ExpansionVodnik_CamoMedical","Vehicle_ExpansionVodnik_CamoMedical","ExpansionVodnikDoorDriver_CamoMedical","ExpansionVodnikDoorCoDriver_CamoMedical"};
		weapons[] = {};
		requiredVersion = 0.1;
		requiredAddons[] = {"DZ_Vehicles_Wheeled"};
	};
};
class CfgSlots
{
	class Slot_vodnikwheel_1_1
	{
		name = "vodnikwheel_1_1";
		displayName = "$STR_EXPANSION_VEHICLE_VODNIK_WHEEL";
		selection = "wheel_1_1";
		ghostIcon = "wheel";
	};
	class Slot_vodnikwheel_1_2
	{
		name = "vodnikwheel_1_2";
		displayName = "$STR_EXPANSION_VEHICLE_VODNIK_WHEEL";
		selection = "wheel_1_2";
		ghostIcon = "wheel";
	};
	class Slot_vodnikwheel_2_1
	{
		name = "vodnikwheel_2_1";
		displayName = "$STR_EXPANSION_VEHICLE_VODNIK_WHEEL";
		selection = "wheel_2_1";
		ghostIcon = "wheel";
	};
	class Slot_vodnikwheel_2_2
	{
		name = "vodnikwheel_2_2";
		displayName = "$STR_EXPANSION_VEHICLE_VODNIK_WHEEL";
		selection = "wheel_2_2";
		ghostIcon = "wheel";
	};
	class Slot_vodnikdriverdoor
	{
		name = "vodnikdriverdoor";
		displayName = "$STR_EXPANSION_VEHICLE_VODNIK_DOOR_LEFT";
		selection = "vodnikdriverdoor";
		ghostIcon = "doorfront";
	};
	class Slot_vodnikcodriverdoor
	{
		name = "vodnikcodriverdoor";
		displayName = "$STR_EXPANSION_VEHICLE_VODNIK_DOOR_LEFT";
		selection = "vodnikcodriverdoor";
		ghostIcon = "doorfront";
	};
	class Slot_vodnikcargo1door
	{
		name = "vodnikcargo1door";
		displayName = "PLACEHOLDER Vodnik Cargo Door 01";
		selection = "vodnikcargo1door";
		ghostIcon = "doorrear";
	};
	class Slot_vodnikcargo2door
	{
		name = "vodnikcargo2door";
		displayName = "PLACEHOLDER Vodnik Cargo Door 02";
		selection = "vodnikcargo2door";
		ghostIcon = "doorrear";
	};
};
class CfgVehicles
{
	class CarScript;
	class Truck_01_WheelDouble;
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
	class OffroadHatchback;
	class ExpansionWheelBase;
	class ExpansionVehicleBoatBase;
	class ExpansionVodnikWheel: Truck_01_WheelDouble
	{
		scope = 2;
		displayName = "$STR_EXPANSION_VEHICLE_VODNIK_WHEEL";
		descriptionShort = "$STR_EXPANSION_VEHICLE_VODNIK_WHEEL_DESC";
		model = "\DayZExpansion\Vehicles\Ground\Vodnik\proxy\VodnikWheel.p3d";
		weight = 1500;
		inventorySlot[] = {"vodnikwheel_1_1","vodnikwheel_1_2","vodnikwheel_2_1","vodnikwheel_2_2"};
		rotationFlags = 4;
		physLayer = "item_large";
		radiusByDamage[] = {0,0.343,0.3,0.4,0.9998,0.25,0.9999,0.2};
		radius = 0.656;
		width = 0.316;
		tyreRollResistance = 0.015;
		tyreTread = 0.7;
		class DamageSystem
		{
			class GlobalHealth
			{
				class Health
				{
					hitpoints = 750;
					healthLevels[] = {{1.0,{"DZ\vehicles\wheeled\OffroadHatchback\data\niva_wheel.rvmat"}},{0.7,{"DZ\vehicles\wheeled\OffroadHatchback\data\niva_wheel.rvmat"}},{0.5,{"DZ\vehicles\wheeled\OffroadHatchback\data\niva_wheel_dmg.rvmat"}},{0.3,{"DZ\vehicles\wheeled\OffroadHatchback\data\niva_wheel_dmg.rvmat"}},{0.0,{"DZ\vehicles\wheeled\OffroadHatchback\data\niva_wheel_dmg.rvmat"}}};
				};
			};
		};
	};
	class Vehicle_ExpansionVodnikWheel: ExpansionWheelBase
	{
		scope = 2;
		displayName = "$STR_EXPANSION_VEHICLE_VODNIK_WHEEL";
		descriptionShort = "$STR_EXPANSION_VEHICLE_VODNIK_WHEEL_DESC";
		model = "\DayZExpansion\Vehicles\Ground\Vodnik\proxy\VodnikWheel.p3d";
		weight = 1500;
		inventorySlot[] = {"vodnikwheel_1_1","vodnikwheel_1_2","vodnikwheel_2_1","vodnikwheel_2_2"};
		rotationFlags = 4;
		physLayer = "item_large";
		radiusByDamage[] = {0,0.343,0.3,0.4,0.9998,0.25,0.9999,0.2};
		radius = 0.656;
		width = 0.316;
		tyreRollResistance = 0.015;
		tyreTread = 0.7;
		class DamageSystem
		{
			class GlobalHealth
			{
				class Health
				{
					hitpoints = 750;
					healthLevels[] = {{1.0,{"DZ\vehicles\wheeled\OffroadHatchback\data\niva_wheel.rvmat"}},{0.7,{"DZ\vehicles\wheeled\OffroadHatchback\data\niva_wheel.rvmat"}},{0.5,{"DZ\vehicles\wheeled\OffroadHatchback\data\niva_wheel_dmg.rvmat"}},{0.3,{"DZ\vehicles\wheeled\OffroadHatchback\data\niva_wheel_dmg.rvmat"}},{0.0,{"DZ\vehicles\wheeled\OffroadHatchback\data\niva_wheel_dmg.rvmat"}}};
				};
			};
		};
	};
	class ExpansionVodnikDoorDriver: CivSedanDoors_Driver
	{
		scope = 2;
		displayName = "$STR_EXPANSION_VEHICLE_VODNIK_DOOR_LEFT";
		descriptionShort = "$STR_EXPANSION_VEHICLE_VODNIK_DOOR_LEFT_DESC";
		model = "\DayZExpansion\Vehicles\Ground\Vodnik\proxy\vodnikdriverdoor.p3d";
		weight = 1500;
		itemSize[] = {10,10};
		itemBehaviour = 0;
		inventorySlot = "vodnikdriverdoor";
		rotationFlags = 8;
		physLayer = "item_large";
	};
	class ExpansionVodnikDoorCoDriver: ExpansionVodnikDoorDriver
	{
		displayName = "$STR_EXPANSION_VEHICLE_VODNIK_DOOR_LEFT";
		descriptionShort = "$STR_EXPANSION_VEHICLE_VODNIK_DOOR_RIGHT_DESC";
		model = "\DayZExpansion\Vehicles\Ground\Vodnik\proxy\vodnikcodriverdoor.p3d";
		inventorySlot = "vodnikcodriverdoor";
		rotationFlags = 4;
		itemBehaviour = 0;
	};
	class ExpansionVodnik: OffroadHatchback
	{
		scope = 2;
		displayname = "$STR_EXPANSION_VEHICLE_VODNIK";
		model = "\DayZExpansion\Vehicles\Ground\Vodnik\vodnik.p3d";
		vehicleClass = "Expansion_Car";
		hiddenSelections[] = {"light_1_1","light_2_1","light_brake_1_2","light_brake_2_2","light_reverse_1_2","light_reverse_2_2","light_1_2","light_2_2","light_dashboard","med"};
		hiddenSelectionsTextures[] = {"","","","","","","","","","","","dayzexpansion\vehicles\ground\vodnik\data\gaz39371_vodnik_mod_out_co.paa",""};
		hiddenSelectionsMaterials[] = {"","","","","","","","","","","dayzexpansion\vehicles\ground\vodnik\data\gaz39371_vodnik_module.rvmat",""};
		frontReflectorMatOn = "dayzexpansion\vehicles\ground\vodnik\data\gaz39371_vodnik_glass_on.rvmat";
		frontReflectorMatOff = "dayzexpansion\vehicles\ground\vodnik\data\gaz39371_vodnik_glass.rvmat";
		TailReflectorMatOn = "dayzexpansion\vehicles\ground\vodnik\data\gaz39371_vodnik_basis_01_on.rvmat";
		TailReflectorMatOff = "dayzexpansion\vehicles\ground\vodnik\data\gaz39371_vodnik_basis_01.rvmat";
		attachments[] = {"TruckBattery","Reflector_1_1","Reflector_2_1","CarRadiator","SparkPlug","vodnikwheel_1_1","vodnikwheel_1_2","vodnikwheel_2_1","vodnikwheel_2_2","vodnikdriverdoor","vodnikcodriverdoor"};
		doors[] = {"vodnikdriverdoor","vodnikcodriverdoor"};
		applySkinsTo[] = {"vodnikdriverdoor","vodnikcodriverdoor"};
		fuelCapacity = 42;
		fuelConsumption = 13;
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
				getInPos = "pos_cargo2";
				getInDir = "pos_cargo2_dir";
			};
			class Cargo2
			{
				actionSel = "seat_cargo2";
				proxyPos = "crewCargo2";
				getInPos = "pos_cargo1";
				getInDir = "pos_cargo1_dir";
			};
			class Cargo3
			{
				actionSel = "seat_cargo3";
				proxyPos = "crewCargo3";
				getInPos = "pos_cargo2";
				getInDir = "pos_cargo2_dir";
			};
			class Cargo4
			{
				actionSel = "seat_cargo4";
				proxyPos = "crewCargo4";
				getInPos = "pos_cargo1";
				getInDir = "pos_cargo1_dir";
			};
			class Cargo5
			{
				actionSel = "seat_cargo5";
				proxyPos = "crewCargo5";
				getInPos = "pos_cargo2";
				getInDir = "pos_cargo2_dir";
			};
			class Cargo6
			{
				actionSel = "seat_cargo6";
				proxyPos = "crewCargo6";
				getInPos = "pos_cargo1";
				getInDir = "pos_cargo1_dir";
			};
			class Cargo7
			{
				actionSel = "seat_cargo7";
				proxyPos = "crewCargo7";
				getInPos = "pos_cargo2";
				getInDir = "pos_cargo2_dir";
			};
			class Cargo8
			{
				actionSel = "seat_cargo8";
				proxyPos = "crewCargo8";
				getInPos = "pos_cargo1";
				getInDir = "pos_cargo1_dir";
			};
		};
		class SimulationModule: SimulationModule
		{
			drive = "DRIVE_AWD";
			airDragFrontTotal = 0.1;
			class Steering
			{
				increaseSpeed[] = {0,75,60,23,100,12};
				decreaseSpeed[] = {0,80,60,40,90,20};
				centeringSpeed[] = {0,30,15,25,60,40,100,60};
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
				torqueMax = 114;
				torqueRpm = 3400;
				powerMax = 53.7;
				powerRpm = 5400;
				rpmIdle = 850;
				rpmMin = 900;
				rpmClutch = 1850;
				rpmRedline = 11000;
				rpmMax = 14000;
			};
			class Gearbox
			{
				reverse = 3.426;
				ratios[] = {3.0670002,1.9999999,1.2609999,0.9};
				timeToUncoupleClutch = 0.2;
				timeToCoupleClutch = 0.35;
				maxClutchTorque = 420;
			};
			class Axles: Axles
			{
				class Front: Front
				{
					maxSteeringAngle = 40;
					finalRatio = 8.099999;
					brakeBias = 0.6;
					brakeForce = 4500;
					wheelHubMass = 5;
					wheelHubRadius = 0.14;
					class Suspension
					{
						swayBar = 300;
						stiffness = 40000;
						compression = 2100;
						damping = 5500;
						travelMaxUp = 0.2917994;
						travelMaxDown = 0.0833;
					};
					class Wheels: Wheels
					{
						class Left: Left
						{
							animDamper = "damper_1_1";
							inventorySlot = "vodnikwheel_1_1";
						};
						class Right: Right
						{
							animDamper = "damper_2_1";
							inventorySlot = "vodnikwheel_2_1";
						};
					};
				};
				class Rear: Rear
				{
					maxSteeringAngle = 0;
					finalRatio = 8.099999;
					brakeBias = 0.4;
					brakeForce = 4200;
					wheelHubMass = 5;
					wheelHubRadius = 0.14;
					class Suspension
					{
						swayBar = 300;
						stiffness = 39000;
						compression = 2100;
						damping = 5500;
						travelMaxUp = 0.2917994;
						travelMaxDown = 0.0833;
					};
					class Wheels: Wheels
					{
						class Left: Left
						{
							animDamper = "damper_1_2";
							inventorySlot = "vodnikwheel_1_2";
						};
						class Right: Right
						{
							animDamper = "damper_2_2";
							inventorySlot = "vodnikwheel_2_2";
						};
					};
				};
			};
		};
		class Cargo
		{
			itemsCargoSize[] = {10,27};
			allowOwnedCargoManipulation = 1;
			openable = 1;
		};
		class AnimationSources: AnimationSources
		{
			class med
			{
				source = "user";
				animPeriod = 0.01;
				initPhase = 1;
			};
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
			class vodnikdriverdoor
			{
				source = "user";
				initPhase = 0;
				animPeriod = 0.5;
			};
			class vodnikcodriverdoor: vodnikdriverdoor{};
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
				attachmentSlots[] = {"TruckBattery","CarRadiator","SparkPlug"};
			};
			class Body
			{
				name = "$STR_attachment_Body0";
				description = "";
				icon = "cat_vehicle_body";
				attachmentSlots[] = {"Reflector_1_1","Reflector_2_1"};
			};
			class Chassis
			{
				name = "$STR_attachment_Chassis0";
				description = "";
				icon = "cat_vehicle_chassis";
				attachmentSlots[] = {"vodnikwheel_1_1","vodnikwheel_1_2","vodnikwheel_2_1","vodnikwheel_2_2","vodnikdriverdoor","vodnikcodriverdoor"};
			};
		};
	};
	class Vehicle_ExpansionVodnik: ExpansionVehicleBoatBase
	{
		scope = 2;
		displayname = "[NOT READY]  VODNIK";
		model = "\DayZExpansion\Vehicles\Ground\Vodnik\vodnik.p3d";
		vehicleClass = "Expansion_Car";
		hiddenSelections[] = {"light_1_1","light_2_1","light_brake_1_2","light_brake_2_2","light_reverse_1_2","light_reverse_2_2","light_1_2","light_2_2","light_dashboard","med"};
		hiddenSelectionsTextures[] = {"","","","","","","","","","","","dayzexpansion\vehicles\ground\vodnik\data\gaz39371_vodnik_mod_out_co.paa",""};
		hiddenSelectionsMaterials[] = {"","","","","","","","","","","dayzexpansion\vehicles\ground\vodnik\data\gaz39371_vodnik_module.rvmat",""};
		frontReflectorMatOn = "dayzexpansion\vehicles\ground\vodnik\data\gaz39371_vodnik_glass_on.rvmat";
		frontReflectorMatOff = "dayzexpansion\vehicles\ground\vodnik\data\gaz39371_vodnik_glass.rvmat";
		TailReflectorMatOn = "dayzexpansion\vehicles\ground\vodnik\data\gaz39371_vodnik_basis_01_on.rvmat";
		TailReflectorMatOff = "dayzexpansion\vehicles\ground\vodnik\data\gaz39371_vodnik_basis_01.rvmat";
		attachments[] = {"TruckBattery","Reflector_1_1","Reflector_2_1","CarRadiator","SparkPlug","vodnikwheel_1_1","vodnikwheel_1_2","vodnikwheel_2_1","vodnikwheel_2_2","vodnikdriverdoor","vodnikcodriverdoor"};
		doors[] = {"vodnikdriverdoor","vodnikcodriverdoor"};
		applySkinsTo[] = {"vodnikdriverdoor","vodnikcodriverdoor"};
		fuelCapacity = 42;
		fuelConsumption = 13;
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
				getInPos = "pos_cargo2";
				getInDir = "pos_cargo2_dir";
			};
			class Cargo2
			{
				actionSel = "seat_cargo2";
				proxyPos = "crewCargo2";
				getInPos = "pos_cargo1";
				getInDir = "pos_cargo1_dir";
			};
			class Cargo3
			{
				actionSel = "seat_cargo3";
				proxyPos = "crewCargo3";
				getInPos = "pos_cargo2";
				getInDir = "pos_cargo2_dir";
			};
			class Cargo4
			{
				actionSel = "seat_cargo4";
				proxyPos = "crewCargo4";
				getInPos = "pos_cargo1";
				getInDir = "pos_cargo1_dir";
			};
			class Cargo5
			{
				actionSel = "seat_cargo5";
				proxyPos = "crewCargo5";
				getInPos = "pos_cargo2";
				getInDir = "pos_cargo2_dir";
			};
			class Cargo6
			{
				actionSel = "seat_cargo6";
				proxyPos = "crewCargo6";
				getInPos = "pos_cargo1";
				getInDir = "pos_cargo1_dir";
			};
			class Cargo7
			{
				actionSel = "seat_cargo7";
				proxyPos = "crewCargo7";
				getInPos = "pos_cargo2";
				getInDir = "pos_cargo2_dir";
			};
			class Cargo8
			{
				actionSel = "seat_cargo8";
				proxyPos = "crewCargo8";
				getInPos = "pos_cargo1";
				getInDir = "pos_cargo1_dir";
			};
		};
		class SimulationModule: SimulationModule
		{
			drive = "DRIVE_AWD";
			airDragFrontTotal = 0.1;
			class Steering
			{
				increaseSpeed[] = {0,75,60,23,100,12};
				decreaseSpeed[] = {0,80,60,40,90,20};
				centeringSpeed[] = {0,30,15,25,60,40,100,60};
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
				torqueMax = 114;
				torqueRpm = 3400;
				powerMax = 53.7;
				powerRpm = 5400;
				rpmIdle = 850;
				rpmMin = 900;
				rpmClutch = 1850;
				rpmRedline = 11000;
				rpmMax = 14000;
			};
			class Gearbox
			{
				reverse = 3.426;
				ratios[] = {3.0670002,1.9999999,1.2609999,0.9};
				timeToUncoupleClutch = 0.2;
				timeToCoupleClutch = 0.35;
				maxClutchTorque = 420;
			};
			class Axles: Axles
			{
				class Front: Front
				{
					maxSteeringAngle = 40;
					finalRatio = 8.099999;
					brakeBias = 0.6;
					brakeForce = 4500;
					wheelHubMass = 5;
					wheelHubRadius = 0.14;
					class Suspension
					{
						swayBar = 300;
						stiffness = 40000;
						compression = 2100;
						damping = 5500;
						travelMaxUp = 0.2917994;
						travelMaxDown = 0.0833;
					};
					class Wheels: Wheels
					{
						class Left: Left
						{
							inventorySlot = "vodnikwheel_1_1";
							animTurn = "turnfrontleft";
							animRotation = "wheelfrontleft";
							animDamper = "damper_1_1";
							wheelHub = "wheel_1_1_damper_land";
						};
						class Right: Right
						{
							inventorySlot = "vodnikwheel_2_1";
							animTurn = "turnfrontright";
							animRotation = "wheelfrontright";
							animDamper = "damper_2_1";
							wheelHub = "wheel_2_1_damper_land";
						};
					};
				};
				class Rear: Rear
				{
					maxSteeringAngle = 0;
					finalRatio = 8.099999;
					brakeBias = 0.4;
					brakeForce = 4200;
					wheelHubMass = 5;
					wheelHubRadius = 0.14;
					class Suspension
					{
						swayBar = 300;
						stiffness = 39000;
						compression = 2100;
						damping = 5500;
						travelMaxUp = 0.2917994;
						travelMaxDown = 0.0833;
					};
					class Wheels: Wheels
					{
						class Left: Left
						{
							inventorySlot = "vodnikwheel_1_2";
							animTurn = "turnbackleft";
							animRotation = "wheelbackleft";
							animDamper = "damper_1_2";
							wheelHub = "wheel_1_2_damper_land";
						};
						class Right: Right
						{
							inventorySlot = "vodnikwheel_2_2";
							animTurn = "turnbackright";
							animRotation = "wheelbackright";
							animDamper = "damper_2_2";
							wheelHub = "wheel_2_2_damper_land";
						};
					};
				};
			};
		};
		class Cargo
		{
			itemsCargoSize[] = {10,27};
			allowOwnedCargoManipulation = 1;
			openable = 1;
		};
		class AnimationSources: AnimationSources
		{
			class med
			{
				source = "user";
				animPeriod = 0.01;
				initPhase = 1;
			};
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
			class vodnikdriverdoor
			{
				source = "user";
				initPhase = 0;
				animPeriod = 0.5;
			};
			class vodnikcodriverdoor: vodnikdriverdoor{};
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
				attachmentSlots[] = {"TruckBattery","CarRadiator","SparkPlug"};
			};
			class Body
			{
				name = "$STR_attachment_Body0";
				description = "";
				icon = "cat_vehicle_body";
				attachmentSlots[] = {"Reflector_1_1","Reflector_2_1"};
			};
			class Chassis
			{
				name = "$STR_attachment_Chassis0";
				description = "";
				icon = "cat_vehicle_chassis";
				attachmentSlots[] = {"vodnikwheel_1_1","vodnikwheel_1_2","vodnikwheel_2_1","vodnikwheel_2_2","vodnikdriverdoor","vodnikcodriverdoor"};
			};
		};
	};
	class ExpansionSpraycanBase;
	class ExpansionSpraycanCamo: ExpansionSpraycanBase
	{
		scope = 2;
		displayName = "$STR_EXPANSION_SPRAYCAN_Camo_NAME";
		descriptionShort = "$STR_EXPANSION_SPRAYCAN_Camo_DESC";
		hiddenSelectionsTextures[] = {"\DayZExpansion\Objects\Gear\Spraycans\data\spraycan_camo_co.paa"};
		skinName = "Camo";
	};
	class ExpansionVodnik_Camo: ExpansionVodnik
	{
		skinBase = "ExpansionVodnik";
		skinName = "Camo";
	};
	class Vehicle_ExpansionVodnik_Camo: Vehicle_ExpansionVodnik
	{
		skinBase = "Vehicle_ExpansionVodnik";
		skinName = "Camo";
	};
	class ExpansionVodnikDoorDriver_Camo: ExpansionVodnikDoorDriver
	{
		skinBase = "ExpansionVodnikDoorDriver";
		skinName = "Camo";
	};
	class ExpansionVodnikDoorCoDriver_Camo: ExpansionVodnikDoorCoDriver
	{
		skinBase = "ExpansionVodnikDoorCoDriver";
		skinName = "Camo";
	};
	class ExpansionSpraycanCamoMedical: ExpansionSpraycanBase
	{
		scope = 2;
		displayName = "$STR_EXPANSION_SPRAYCAN_CamoMedical_NAME";
		descriptionShort = "$STR_EXPANSION_SPRAYCAN_CamoMedical_DESC";
		hiddenSelectionsTextures[] = {"\DayZExpansion\Objects\Gear\Spraycans\data\spraycan_camo_co.paa"};
		skinName = "CamoMedical";
	};
	class ExpansionVodnik_CamoMedical: ExpansionVodnik
	{
		skinBase = "ExpansionVodnik";
		skinName = "CamoMedical";
	};
	class Vehicle_ExpansionVodnik_CamoMedical: Vehicle_ExpansionVodnik
	{
		skinBase = "Vehicle_ExpansionVodnik";
		skinName = "CamoMedical";
	};
	class ExpansionVodnikDoorDriver_CamoMedical: ExpansionVodnikDoorDriver
	{
		skinBase = "ExpansionVodnikDoorDriver";
		skinName = "CamoMedical";
	};
	class ExpansionVodnikDoorCoDriver_CamoMedical: ExpansionVodnikDoorCoDriver
	{
		skinBase = "ExpansionVodnikDoorCoDriver";
		skinName = "CamoMedical";
	};
};
class CfgNonAIVehicles
{
	class ProxyVehiclePart;
	class Proxyvodnikwheel: ProxyVehiclePart
	{
		model = "\DayZExpansion\Vehicles\Ground\Vodnik\proxy\vodnikwheel.p3d";
		inventorySlot[] = {"vodnikwheel_1_1","vodnikwheel_1_2","vodnikwheel_2_1","vodnikwheel_2_2"};
	};
	class Proxyvodnikdriverdoor: ProxyVehiclePart
	{
		model = "\DayZExpansion\Vehicles\Ground\Vodnik\proxy\vodnikdriverdoor.p3d";
		inventorySlot = "vodnikdriverdoor";
	};
	class Proxyvodnikcodriverdoor: ProxyVehiclePart
	{
		model = "\DayZExpansion\Vehicles\Ground\Vodnik\proxy\vodnikcodriverdoor.p3d";
		inventorySlot = "vodnikcodriverdoor";
	};
};
