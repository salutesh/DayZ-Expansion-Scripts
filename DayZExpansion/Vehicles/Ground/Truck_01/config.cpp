#define _ARMA_

class CfgPatches
{
	class DayZExpansion_Vehicles_Ground_Truck_01
	{
		units[] = {"Truck_01_Covered","Vehicle_Truck_01_Covered","Vehicle_Truck_01_Wheel","Vehicle_Truck_01_Wheel_Ruined","Vehicle_Truck_01_WheelDouble","Vehicle_Truck_01_WheelDouble_Ruined"};
		weapons[] = {};
		requiredVersion = 0.1;
		requiredAddons[] = {"DayZExpansion_Vehicles_Data"};
	};
};
class CfgVehicles
{
	class ExpansionWheelBase;
	class ExpansionVehicleCarBase;
	class CarDoor;
	class CarScript;
	class ExpansionCarDoorFix;
	class Crew;
	class Driver;
	class CoDriver;
	class SimulationModule;
	class Axles;
	class Front;
	class Rear;
	class Wheels;
	class Left;
	class Right;
	class AnimationSources;
	class Truck_01_Base;
	class Truck_01_Covered: Truck_01_Base
	{
		scope = 2;
		displayName = "$STR_v3s_covered0";
		model = "\dz\vehicles\wheeled\Truck_01\Truck_01_Covered.p3d";
		attachments[] = {"TruckBattery","Reflector_1_1","Reflector_2_1","Truck_01_Door_1_1","Truck_01_Door_2_1","Truck_01_Hood","Truck_01_Wheel_1_1","Truck_01_Wheel_1_2","Truck_01_Wheel_1_3","Truck_01_Wheel_2_1","Truck_01_Wheel_2_2","Truck_01_Wheel_2_3","Truck_01_Wheel_Spare_1","Truck_01_Wheel_Spare_2","Truck_01_WoodenLogs","Truck_01_WoodenPlanks","Truck_01_MetalSheets","Truck_01_Barrel1","Truck_01_Barrel2","Truck_01_Barrel3","Truck_01_Barrel4","Truck_01_WoodenCrate1","Truck_01_WoodenCrate2","Truck_01_WoodenCrate3","Truck_01_WoodenCrate4"};
		class Cargo
		{
			itemsCargoSize[] = {10,60};
			allowOwnedCargoManipulation = 1;
			openable = 0;
		};
		class GUIInventoryAttachmentsProps
		{
			class Engine
			{
				name = "$STR_attachment_Engine0";
				description = "";
				icon = "cat_vehicle_engine";
				attachmentSlots[] = {"TruckBattery"};
			};
			class Body
			{
				name = "$STR_attachment_Body0";
				description = "";
				icon = "cat_vehicle_body";
				attachmentSlots[] = {"Reflector_1_1","Reflector_2_1","Truck_01_Door_1_1","Truck_01_Door_2_1","Truck_01_Hood"};
			};
			class Chassis
			{
				name = "$STR_attachment_Chassis0";
				description = "";
				icon = "cat_vehicle_chassis";
				attachmentSlots[] = {"Truck_01_Wheel_1_1","Truck_01_Wheel_1_2","Truck_01_Wheel_1_3","Truck_01_Wheel_2_1","Truck_01_Wheel_2_2","Truck_01_Wheel_2_3","Truck_01_Wheel_Spare_1","Truck_01_Wheel_Spare_2"};
			};
			class BaseBuildingAttachments
			{
				name = "$STR_cfgvehicles_fence_att_category_materials";
				description = "";
				icon = "cat_common_cargo";
				attachmentSlots[] = {"Truck_01_WoodenLogs","Truck_01_WoodenPlanks","Truck_01_MetalSheets"};
			};
			class BaseBuildingContainers
			{
				name = "$STR_attachment_containers0";
				description = "";
				icon = "cat_common_cargo";
				attachmentSlots[] = {"Truck_01_Barrel1","Truck_01_Barrel2","Truck_01_Barrel3","Truck_01_Barrel4","Truck_01_WoodenCrate1","Truck_01_WoodenCrate2","Truck_01_WoodenCrate3","Truck_01_WoodenCrate4"};
			};
		};
	};
	class Vehicle_Truck_01_Base: ExpansionVehicleCarBase
	{
		scope = 0;
		displayName = "Truck 01 Base";
		attachments[] = {"TruckBattery","Reflector_1_1","Reflector_2_1","Truck_01_Door_1_1","Truck_01_Door_2_1","Truck_01_Hood","Truck_01_Wheel_1_1","Truck_01_Wheel_1_2","Truck_01_Wheel_1_3","Truck_01_Wheel_2_1","Truck_01_Wheel_2_2","Truck_01_Wheel_2_3","Truck_01_Wheel_Spare_1","Truck_01_Wheel_Spare_2"};
		hiddenSelections[] = {"light_1_1","light_2_1","light_brake_1_2","light_brake_2_2","light_reverse_1_2","light_reverse_2_2","light_1_2","light_2_2","light_dashboard","dmgZone_chassis","dmgZone_front","dmgZone_back","dmgZone_roof","dmgZone_fueltank","dmgZone_fender_1_1","dmgZone_fender_1_2","dmgZone_fender_2_1","dmgZone_fender_2_2","dmgZone_wood"};
		hiddenSelectionsTextures[] = {"","","","","","","","","","#(argb,8,8,3)color(0.521569,0.427451,0.054902,1.0,co)","#(argb,8,8,3)color(0.521569,0.427451,0.054902,1.0,co)","#(argb,8,8,3)color(0.521569,0.427451,0.054902,1.0,co)","#(argb,8,8,3)color(0.521569,0.427451,0.054902,1.0,co)","#(argb,8,8,3)color(0.521569,0.427451,0.054902,1.0,co)","#(argb,8,8,3)color(0.521569,0.427451,0.054902,1.0,co)","#(argb,8,8,3)color(0.521569,0.427451,0.054902,1.0,co)","#(argb,8,8,3)color(0.521569,0.427451,0.054902,1.0,co)","#(argb,8,8,3)color(0.521569,0.427451,0.054902,1.0,co)","#(argb,8,8,3)color(0.521569,0.427451,0.054902,1.0,co)"};
		dashboardMatOn = "dz\vehicles\wheeled\Truck_01\data\Truck_01_dashboard_light.rvmat";
		dashboardMatOff = "dz\vehicles\wheeled\Truck_01\data\Truck_01_dashboard.rvmat";
		frontReflectorMatOn = "dz\vehicles\wheeled\Truck_01\data\Truck_01_cab_lights.rvmat";
		frontReflectorMatOff = "dz\vehicles\wheeled\Truck_01\data\Truck_01_cab_chrome.rvmat";
		brakeReflectorMatOn = "dz\vehicles\wheeled\civiliansedan\data\Sedan_TailLights_e.rvmat";
		brakeReflectorMatOff = "dz\vehicles\wheeled\civiliansedan\data\Sedan_TailLights.rvmat";
		ReverseReflectorMatOn = "dz\vehicles\wheeled\civiliansedan\data\Sedan_TailLights_e.rvmat";
		ReverseReflectorMatOff = "dz\vehicles\wheeled\civiliansedan\data\Sedan_TailLights.rvmat";
		TailReflectorMatOn = "dz\vehicles\wheeled\civiliansedan\data\Sedan_TailLights_e.rvmat";
		TailReflectorMatOff = "dz\vehicles\wheeled\civiliansedan\data\Sedan_TailLights.rvmat";
		fuelCapacity = 120;
		fuelConsumption = 30;
		class SimulationModule: SimulationModule
		{
			drive = "DRIVE_662";
			centralDiffRatio = 2.15;
			airDragFrontTotal = 0.8;
			class Steering
			{
				increaseSpeed[] = {0,25,50,15};
				decreaseSpeed[] = {0,50,50,40};
				centeringSpeed[] = {0,25,50.0,15};
			};
			class Throttle
			{
				reactionTime = 0.5;
				defaultThrust = 0.9;
				gentleThrust = 0.75;
				turboCoef = 1.5;
				gentleCoef = 0.5;
			};
			braking[] = {0.0,0.3,0.15,0.4,0.5,0.5,3.0,1.0};
			class Engine
			{
				inertia = 22.5;
				powerMax = 72;
				powerRpm = 1900;
				steepness = 18;
				torqueMax = 360;
				torqueRpm = 1400;
				rpmIdle = 700;
				rpmMin = 750;
				rpmClutch = 850;
				rpmRedline = 2100;
				rpmMax = 2500;
			};
			class Gearbox
			{
				reverse = 6.28;
				ratios[] = {6.19,3.13,1.75,1.0};
				timeToUncoupleClutch = 0.25;
				timeToCoupleClutch = 1.75;
				maxClutchTorque = 450;
			};
			class Axles: Axles
			{
				class Front: Front
				{
					maxSteeringAngle = 35;
					finalRatio = 3.9;
					brakeBias = 0.4;
					brakeForce = 7000;
					wheelHubMass = 25;
					wheelHubRadius = 0.3;
					wheelHubRatio = 2.14;
					class Suspension
					{
						stiffness = 55000;
						compression = 3000;
						damping = 7000;
						travelMaxUp = 0.14;
						travelMaxDown = 0.15;
					};
					class Wheels: Wheels
					{
						class Left: Left
						{
							animDamper = "damper_1_1";
							inventorySlot = "Truck_01_Wheel_1_1";
						};
						class Right: Right
						{
							animDamper = "damper_2_1";
							inventorySlot = "Truck_01_Wheel_2_1";
						};
					};
				};
				class Middle: Rear
				{
					maxSteeringAngle = 0;
					finalRatio = 3.9;
					brakeBias = 0.3;
					brakeForce = 8000;
					wheelHubMass = 45;
					wheelHubRadius = 0.3;
					wheelHubRatio = 2.14;
					class Suspension
					{
						stiffness = 55000;
						compression = 2500;
						damping = 8000;
						travelMaxUp = 0.095;
						travelMaxDown = 0.125;
					};
					class Wheels: Wheels
					{
						class Left
						{
							inventorySlot = "Truck_01_Wheel_1_2";
							animTurn = "turnmiddleleft";
							animRotation = "wheelmiddleleft";
							animDamper = "damper_1_2";
							wheelHub = "wheel_1_2_damper_land";
						};
						class Right
						{
							inventorySlot = "Truck_01_Wheel_2_2";
							animTurn = "turnmiddletright";
							animRotation = "wheelmiddleright";
							animDamper = "damper_2_2";
							wheelHub = "wheel_2_2_damper_land";
						};
					};
				};
				class Rear: Rear
				{
					maxSteeringAngle = 0;
					finalRatio = 3.9;
					brakeBias = 0.3;
					brakeForce = 8000;
					wheelHubMass = 45;
					wheelHubRadius = 0.3;
					wheelHubRatio = 2.14;
					class Suspension
					{
						stiffness = 55000;
						compression = 2500;
						damping = 8000;
						travelMaxUp = 0.095;
						travelMaxDown = 0.125;
					};
					class Wheels: Wheels
					{
						class Left: Left
						{
							animDamper = "damper_1_3";
							inventorySlot = "Truck_01_Wheel_1_3";
							wheelHub = "wheel_1_3_damper_land";
						};
						class Right: Right
						{
							animDamper = "damper_2_3";
							inventorySlot = "Truck_01_Wheel_2_3";
							wheelHub = "wheel_2_3_damper_land";
						};
					};
				};
			};
		};
		class Cargo
		{
			itemsCargoSize[] = {10,40};
			allowOwnedCargoManipulation = 1;
			openable = 0;
		};
		class AnimationSources: AnimationSources
		{
			class WheelSidePlate1
			{
				source = "user";
				initPhase = 0;
				animPeriod = 1.3;
			};
			class WheelSidePlate2: WheelSidePlate1{};
			class Damper_1_1
			{
				source = "user";
				initPhase = 0.4828;
				animPeriod = 1;
			};
			class Damper_2_1: Damper_1_1{};
			class Damper_1_2
			{
				source = "user";
				initPhase = 0.4318;
				animPeriod = 1;
			};
			class Damper_2_2: Damper_1_2{};
			class Damper_1_3: Damper_1_2{};
			class Damper_2_3: Damper_1_2{};
		};
		class DamageSystem
		{
			class GlobalHealth
			{
				class Health
				{
					hitpoints = 1500;
					healthLevels[] = {{1.0,{}},{0.7,{}},{0.5,{}},{0.3,{}},{0.0,{}}};
				};
			};
			class DamageZones
			{
				class Chassis
				{
					fatalInjuryCoef = -1;
					memoryPoints[] = {};
					componentNames[] = {"dmgZone_chassis"};
					class Health
					{
						hitpoints = 3000;
						transferToGlobalCoef = 0;
					};
					transferToZonesNames[] = {};
					transferToZonesCoefs[] = {};
					inventorySlots[] = {};
					inventorySlotsCoefs[] = {};
				};
				class Engine
				{
					fatalInjuryCoef = 0.001;
					memoryPoints[] = {"dmgZone_engine"};
					componentNames[] = {"dmgZone_engine"};
					class Health
					{
						hitpoints = 1500;
						transferToGlobalCoef = 1;
						healthLevels[] = {{1.0,{}},{0.7,{}},{0.5,{}},{0.3,{}},{0.0,{}}};
					};
					transferToZonesNames[] = {};
					transferToZonesCoefs[] = {};
					inventorySlots[] = {};
					inventorySlotsCoefs[] = {};
				};
				class Front
				{
					fatalInjuryCoef = -1;
					memoryPoints[] = {"dmgZone_front"};
					componentNames[] = {"dmgZone_front"};
					class Health
					{
						hitpoints = 2500;
						transferToGlobalCoef = 0;
						healthLevels[] = {{1.0,{"dz\vehicles\wheeled\Truck_01\data\Truck_01_cab.rvmat"}},{0.7,{"dz\vehicles\wheeled\Truck_01\data\Truck_01_cab.rvmat"}},{0.5,{"dz\vehicles\wheeled\Truck_01\data\Truck_01_cab_damage.rvmat"}},{0.3,{"dz\vehicles\wheeled\Truck_01\data\Truck_01_cab_damage.rvmat"}},{0.0,{"dz\vehicles\wheeled\Truck_01\data\Truck_01_cab_destruct.rvmat"}}};
					};
					transferToZonesNames[] = {"Fender_1_1","Fender_2_1","Engine"};
					transferToZonesCoefs[] = {0.2,0.2,0.1};
					inventorySlots[] = {"Truck_01_Hood"};
					inventorySlotsCoefs[] = {0.1};
				};
				class BackWood
				{
					fatalInjuryCoef = -1;
					memoryPoints[] = {"dmgZone_wood"};
					componentNames[] = {"dmgZone_wood"};
					class Health
					{
						hitpoints = 3000;
						transferToGlobalCoef = 0;
						healthLevels[] = {{1.0,{"dz\vehicles\wheeled\Truck_01\data\truck_01_cargo_super.rvmat"}},{0.7,{"dz\vehicles\wheeled\Truck_01\data\truck_01_cargo_super.rvmat"}},{0.5,{"dz\vehicles\wheeled\Truck_01\data\truck_01_cargo_super_damage.rvmat"}},{0.3,{"dz\vehicles\wheeled\Truck_01\data\truck_01_cargo_super_damage.rvmat"}},{0.0,{"dz\vehicles\wheeled\Truck_01\data\truck_01_cargo_super_destruct.rvmat"}}};
					};
					transferToZonesNames[] = {};
					transferToZonesCoefs[] = {};
					inventorySlots[] = {};
					inventorySlotsCoefs[] = {};
				};
				class BackTarp
				{
					fatalInjuryCoef = -1;
					memoryPoints[] = {"dmgZone_tarp"};
					componentNames[] = {"dmgZone_tarp"};
					class Health
					{
						hitpoints = 1000;
						transferToGlobalCoef = 0;
						healthLevels[] = {{1.0,{"dz\gear\camping\Data\tent_pristine_co.paa"}},{0.7,{"dz\gear\camping\data\tent_worn_co.paa"}},{0.5,{"dz\gear\camping\data\tent_damage_co.paa"}},{0.3,{"dz\gear\camping\data\tent_destruct_co.paa"}},{0.0,{"dz\gear\camping\data\tent_destruct_co.paa"}}};
					};
					transferToZonesNames[] = {};
					transferToZonesCoefs[] = {};
					inventorySlots[] = {};
					inventorySlotsCoefs[] = {};
				};
				class Roof
				{
					fatalInjuryCoef = -1;
					memoryPoints[] = {"dmgZone_roof"};
					componentNames[] = {"dmgZone_roof"};
					class Health
					{
						hitpoints = 500;
						transferToGlobalCoef = 0;
						healthLevels[] = {{1.0,{"dz\vehicles\wheeled\Truck_01\data\truck_01_cab.rvmat","dz\vehicles\wheeled\Truck_01\data\truck_01_cargo.rvmat"}},{0.7,{"dz\vehicles\wheeled\Truck_01\data\truck_01_cab.rvmat","dz\vehicles\wheeled\Truck_01\data\truck_01_cargo.rvmat"}},{0.5,{"dz\vehicles\wheeled\Truck_01\data\truck_01_cab_damage.rvmat","dz\vehicles\wheeled\Truck_01\data\truck_01_cargo_damage.rvmat"}},{0.3,{"dz\vehicles\wheeled\Truck_01\data\truck_01_cab_damage.rvmat","dz\vehicles\wheeled\Truck_01\data\truck_01_cargo_damage.rvmat"}},{0.0,{"dz\vehicles\wheeled\Truck_01\data\truck_01_cab_destruct.rvmat","dz\vehicles\wheeled\Truck_01\data\truck_01_cargo_destruct.rvmat"}}};
					};
					transferToZonesNames[] = {};
					transferToZonesCoefs[] = {};
					inventorySlots[] = {};
					inventorySlotsCoefs[] = {};
				};
				class Fender_1_1
				{
					fatalInjuryCoef = -1;
					memoryPoints[] = {"dmgZone_fender_1_1"};
					componentNames[] = {"dmgZone_fender_1_1"};
					class Health
					{
						hitpoints = 800;
						transferToGlobalCoef = 0;
						healthLevels[] = {{1.0,{"dz\vehicles\wheeled\Truck_01\data\Truck_01_cab.rvmat"}},{0.7,{"dz\vehicles\wheeled\Truck_01\data\Truck_01_cab.rvmat"}},{0.5,{"dz\vehicles\wheeled\Truck_01\data\Truck_01_cab_damage.rvmat"}},{0.3,{"dz\vehicles\wheeled\Truck_01\data\Truck_01_cab_damage.rvmat"}},{0.0,{"dz\vehicles\wheeled\Truck_01\data\Truck_01_cab_destruct.rvmat"}}};
					};
					transferToZonesNames[] = {"Front","Engine","Roof"};
					transferToZonesCoefs[] = {0.1,0.05,0.15};
					inventorySlots[] = {"Truck_01_Hood"};
					inventorySlotsCoefs[] = {0.1};
				};
				class Fender_2_1: Fender_1_1
				{
					memoryPoints[] = {"dmgZone_fender_2_1"};
					componentNames[] = {"dmgZone_fender_2_1"};
				};
				class WindowLeft
				{
					fatalInjuryCoef = -1;
					memoryPoints[] = {"dmgZone_windowLeft"};
					componentNames[] = {"dmgZone_windowLeft"};
					class Health
					{
						hitpoints = 50;
						transferToGlobalCoef = 0;
						healthLevels[] = {{1.0,{"dz\vehicles\wheeled\Truck_01\data\glass_i.rvmat"}},{0.7,{}},{0.5,{"dz\vehicles\wheeled\Truck_01\data\glass_i_damage.rvmat"}},{0.3,{"dz\vehicles\wheeled\Truck_01\data\glass_i_destruct.rvmat"}},{0.0,"hidden"}};
					};
					transferToZonesNames[] = {};
					transferToZonesCoefs[] = {};
					inventorySlots[] = {};
					inventorySlotsCoefs[] = {};
				};
				class WindowRight: WindowLeft
				{
					fatalInjuryCoef = -1;
					memoryPoints[] = {"dmgZone_windowRight"};
					componentNames[] = {"dmgZone_windowRight"};
					inventorySlots[] = {};
					inventorySlotsCoefs[] = {};
				};
				class WindowBack: WindowLeft
				{
					fatalInjuryCoef = -1;
					memoryPoints[] = {"dmgZone_windowBack"};
					componentNames[] = {"dmgZone_windowBack"};
					inventorySlots[] = {};
				};
				class FuelTank
				{
					fatalInjuryCoef = -1;
					memoryPoints[] = {"dmgZone_fuelTank"};
					componentNames[] = {"dmgZone_fuelTank"};
					class Health
					{
						hitpoints = 300;
						transferToGlobalCoef = 0;
						healthLevels[] = {{1.0,{"dz\vehicles\wheeled\Truck_01\data\truck_01_chassis.rvmat"}},{0.7,{"dz\vehicles\wheeled\Truck_01\data\truck_01_chassis.rvmat"}},{0.5,{"dz\vehicles\wheeled\Truck_01\data\truck_01_chassis_damage.rvmat"}},{0.3,{"dz\vehicles\wheeled\Truck_01\data\truck_01_chassis_damage.rvmat"}},{0.0,{"dz\vehicles\wheeled\Truck_01\data\truck_01_chassis_destruct.rvmat"}}};
					};
					transferToZonesNames[] = {};
					transferToZonesCoefs[] = {};
					inventorySlots[] = {};
					inventorySlotsCoefs[] = {};
				};
				class Reflector_1_1
				{
					fatalInjuryCoef = -1;
					memoryPoints[] = {"dmgZone_lights_1_1"};
					componentNames[] = {"dmgZone_lights_1_1"};
					class Health
					{
						hitpoints = 50;
						transferToGlobalCoef = 0;
						healthLevels[] = {{1.0,{"dz\vehicles\wheeled\Truck_01\data\truck_01_glass.rvmat"}},{0.7,{}},{0.5,{"dz\vehicles\wheeled\Truck_01\data\truck_01_glass_destruct.rvmat"}},{0.3,{}},{0.0,{"dz\vehicles\wheeled\Truck_01\data\truck_01_glass_destruct.rvmat"}}};
					};
					transferToZonesNames[] = {"Front","Fender_1_1"};
					transferToZonesCoefs[] = {1.0,1.0};
					inventorySlots[] = {"Reflector_1_1"};
					inventorySlotsCoefs[] = {1.0};
				};
				class Reflector_2_1: Reflector_1_1
				{
					memoryPoints[] = {"dmgZone_lights_2_1"};
					componentNames[] = {"dmgZone_lights_2_1"};
					transferToZonesNames[] = {"Front","Fender_2_1"};
					inventorySlots[] = {"Reflector_2_1"};
				};
			};
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
						center[] = {0,0,1.5};
					};
					class Cyl2
					{
						radius = 1;
						height = 1.5;
						center[] = {0,0,0};
					};
					class Cyl3
					{
						radius = 1;
						height = 1.5;
						center[] = {0,0,-1.5};
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
				attachmentSlots[] = {"TruckBattery"};
			};
			class Body
			{
				name = "$STR_attachment_Body0";
				description = "";
				icon = "cat_vehicle_body";
				attachmentSlots[] = {"Reflector_1_1","Reflector_2_1","Truck_01_Door_1_1","Truck_01_Door_2_1","Truck_01_Hood"};
			};
			class Chassis
			{
				name = "$STR_attachment_Chassis0";
				description = "";
				icon = "cat_vehicle_chassis";
				attachmentSlots[] = {"Truck_01_Wheel_1_1","Truck_01_Wheel_1_2","Truck_01_Wheel_1_3","Truck_01_Wheel_2_1","Truck_01_Wheel_2_2","Truck_01_Wheel_2_3","Truck_01_Wheel_Spare_1","Truck_01_Wheel_Spare_2"};
			};
		};
	};
	class Vehicle_Truck_01_Covered: Vehicle_Truck_01_Base
	{
		scope = 2;
		displayName = "$STR_v3s_covered0";
		model = "\dz\vehicles\wheeled\Truck_01\Truck_01_Covered.p3d";
		attachments[] = {"TruckBattery","Reflector_1_1","Reflector_2_1","Truck_01_Door_1_1","Truck_01_Door_2_1","Truck_01_Hood","Truck_01_Wheel_1_1","Truck_01_Wheel_1_2","Truck_01_Wheel_1_3","Truck_01_Wheel_2_1","Truck_01_Wheel_2_2","Truck_01_Wheel_2_3","Truck_01_Wheel_Spare_1","Truck_01_Wheel_Spare_2","Truck_01_WoodenLogs","Truck_01_WoodenPlanks","Truck_01_MetalSheets","Truck_01_Barrel1","Truck_01_Barrel2","Truck_01_Barrel3","Truck_01_Barrel4","Truck_01_WoodenCrate1","Truck_01_WoodenCrate2","Truck_01_WoodenCrate3","Truck_01_WoodenCrate4"};
		class Cargo
		{
			itemsCargoSize[] = {10,60};
			allowOwnedCargoManipulation = 1;
			openable = 0;
		};
		class GUIInventoryAttachmentsProps
		{
			class Engine
			{
				name = "$STR_attachment_Engine0";
				description = "";
				icon = "cat_vehicle_engine";
				attachmentSlots[] = {"TruckBattery"};
			};
			class Body
			{
				name = "$STR_attachment_Body0";
				description = "";
				icon = "cat_vehicle_body";
				attachmentSlots[] = {"Reflector_1_1","Reflector_2_1","Truck_01_Door_1_1","Truck_01_Door_2_1","Truck_01_Hood"};
			};
			class Chassis
			{
				name = "$STR_attachment_Chassis0";
				description = "";
				icon = "cat_vehicle_chassis";
				attachmentSlots[] = {"Truck_01_Wheel_1_1","Truck_01_Wheel_1_2","Truck_01_Wheel_1_3","Truck_01_Wheel_2_1","Truck_01_Wheel_2_2","Truck_01_Wheel_2_3","Truck_01_Wheel_Spare_1","Truck_01_Wheel_Spare_2"};
			};
			class BaseBuildingAttachments
			{
				name = "$STR_cfgvehicles_fence_att_category_materials";
				description = "";
				icon = "cat_common_cargo";
				attachmentSlots[] = {"Truck_01_WoodenLogs","Truck_01_WoodenPlanks","Truck_01_MetalSheets"};
			};
			class BaseBuildingContainers
			{
				name = "$STR_attachment_containers0";
				description = "";
				icon = "cat_common_cargo";
				attachmentSlots[] = {"Truck_01_Barrel1","Truck_01_Barrel2","Truck_01_Barrel3","Truck_01_Barrel4","Truck_01_WoodenCrate1","Truck_01_WoodenCrate2","Truck_01_WoodenCrate3","Truck_01_WoodenCrate4"};
			};
		};
	};
	class Vehicle_Truck_01_Wheel: ExpansionWheelBase
	{
		scope = 2;
		displayName = "$STR_V3SWheel0";
		descriptionShort = "$STR_V3SWheel1";
		model = "\DZ\vehicles\wheeled\Truck_01\proxy\Truck_01_Wheel.p3d";
		weight = 30000;
		inventorySlot[] = {"Truck_01_Wheel_1_1","Truck_01_Wheel_2_1","Truck_01_Wheel_Spare_1","Truck_01_Wheel_Spare_2"};
		rotationFlags = 12;
		radiusByDamage[] = {0,0.465,0.3,0.38,0.9998,0.35,0.9999,0.28};
		radius = 0.465;
		width = 0.2;
		tyreRollResistance = 0.07;
		tyreTread = 0.95;
		class DamageSystem
		{
			class GlobalHealth
			{
				class Health
				{
					hitpoints = 200;
					healthLevels[] = {{1.0,{"DZ\vehicles\wheeled\Truck_01\data\Truck_01_tire.rvmat","DZ\vehicles\wheeled\Truck_01\data\Truck_01_rim.rvmat"}},{0.7,{"DZ\vehicles\wheeled\Truck_01\data\Truck_01_tire.rvmat","DZ\vehicles\wheeled\Truck_01\data\Truck_01_rim.rvmat"}},{0.5,{"DZ\vehicles\wheeled\Truck_01\data\Truck_01_tire_damage.rvmat","DZ\vehicles\wheeled\Truck_01\data\Truck_01_rim_damage.rvmat"}},{0.3,{"DZ\vehicles\wheeled\Truck_01\data\Truck_01_tire_damage.rvmat","DZ\vehicles\wheeled\Truck_01\data\Truck_01_rim_damage.rvmat"}},{0,{"DZ\vehicles\wheeled\Truck_01\data\Truck_01_tire_destruct.rvmat","DZ\vehicles\wheeled\Truck_01\data\Truck_01_rim_destruct.rvmat"}}};
				};
			};
		};
	};
	class Vehicle_Truck_01_Wheel_Ruined: Vehicle_Truck_01_Wheel
	{
		scope = 2;
		model = "\DZ\vehicles\wheeled\Truck_01\proxy\Truck_01_Wheel_ruined.p3d";
		weight = 25000;
		tyreRollResistance = 1;
		tyreRollDrag = 80;
		tyreRoughness = 1.35;
		tyreTread = 0.45;
		radius = 0.3;
		friction = -1;
		width = 0.2;
	};
	class Vehicle_Truck_01_WheelDouble: Vehicle_Truck_01_Wheel
	{
		scope = 2;
		displayName = "$STR_V3SWheelDouble0";
		descriptionShort = "$STR_V3SWheelDouble1";
		model = "\DZ\vehicles\wheeled\Truck_01\proxy\Truck_01_WheelDouble.p3d";
		weight = 50000;
		inventorySlot[] = {"Truck_01_Wheel_1_2","Truck_01_Wheel_1_3","Truck_01_Wheel_2_2","Truck_01_Wheel_2_3"};
		width = 0.4;
		class DamageSystem
		{
			class GlobalHealth
			{
				class Health
				{
					hitpoints = 200;
					healthLevels[] = {{1.0,{"DZ\vehicles\wheeled\Truck_01\data\Truck_01_tire.rvmat","DZ\vehicles\wheeled\Truck_01\data\Truck_01_rim.rvmat"}},{0.7,{"DZ\vehicles\wheeled\Truck_01\data\Truck_01_tire.rvmat","DZ\vehicles\wheeled\Truck_01\data\Truck_01_rim.rvmat"}},{0.5,{"DZ\vehicles\wheeled\Truck_01\data\Truck_01_tire_damage.rvmat","DZ\vehicles\wheeled\Truck_01\data\Truck_01_rim_damage.rvmat"}},{0.3,{"DZ\vehicles\wheeled\Truck_01\data\Truck_01_tire_damage.rvmat","DZ\vehicles\wheeled\Truck_01\data\Truck_01_rim_damage.rvmat"}},{0,{"DZ\vehicles\wheeled\Truck_01\data\Truck_01_tire_destruct.rvmat","DZ\vehicles\wheeled\Truck_01\data\Truck_01_rim_destruct.rvmat"}}};
				};
			};
		};
	};
	class Vehicle_Truck_01_WheelDouble_Ruined: Vehicle_Truck_01_WheelDouble
	{
		scope = 2;
		model = "\DZ\vehicles\wheeled\Truck_01\proxy\Truck_01_WheelDouble_ruined.p3d";
		weight = 40000;
		tyreRollResistance = 1;
		tyreRollDrag = 80;
		tyreRoughness = 1.35;
		tyreTread = 0.45;
		radius = 0.3;
		friction = -1;
	};
};
