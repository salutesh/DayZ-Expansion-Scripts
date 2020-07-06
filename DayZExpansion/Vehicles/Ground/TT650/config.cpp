#define _ARMA_

class CfgPatches
{
	class DayZExpansion_Vehicles_Ground_TT650
	{
		units[] = {"ExpansionTT650Wheel","ExpansionTT650"};
		weapons[] = {};
		requiredVersion = 0.1;
		requiredAddons[] = {"DZ_Vehicles_Wheeled"};
	};
};
class CfgSlots
{
	class Slot_expansion_tt650_wheel_1
	{
		name = "expansion_tt650_wheel_1";
		displayName = "Front Wheel";
		selection = "wheel_1";
		ghostIcon = "wheel";
	};
	class Slot_expansion_tt650_wheel_2
	{
		name = "expansion_tt650_wheel_2";
		displayName = "Back Wheel";
		selection = "wheel_2";
		ghostIcon = "wheel";
	};
};
class CfgVehicles
{
	class ExpansionWheel;
	class ExpansionTT650Wheel: ExpansionWheel
	{
		scope = 2;
		model = "\DayZExpansion\Vehicles\Ground\TT650\proxy\expansion_tt650_wheel.p3d";
		weight = 500;
		inventorySlot[] = {"expansion_tt650_wheel_1","expansion_tt650_wheel_2"};
		mass = 80.0;
		radius = 0.536;
		width = 0.316;
	};
	class ExpansionBikeScript;
	class ExpansionTT650: ExpansionBikeScript
	{
		scope = 2;
		displayname = "TT650";
		model = "\DayZExpansion\Vehicles\Ground\TT650\tt650.p3d";
		attachments[] = {"CarBattery","Reflector_1_1","Reflector_2_1","CarRadiator","GlowPlug","expansion_tt650_wheel_1","expansion_tt650_wheel_2"};
		soundController = "";
		class GUIInventoryAttachmentsProps
		{
			class Engine
			{
				name = "Engine";
				description = "";
				attachmentSlots[] = {"CarBattery","GlowPlug"};
				icon = "missing";
			};
			class Body
			{
				name = "Body";
				description = "";
				attachmentSlots[] = {"Reflector_1_1","Reflector_2_1"};
				icon = "missing";
			};
			class Chassis
			{
				name = "Chassis";
				description = "";
				attachmentSlots[] = {"expansion_tt650_wheel_1","expansion_tt650_wheel_2"};
				icon = "missing";
			};
		};
		class Crew
		{
			class Driver
			{
				isDriver = 1;
				actionSel = "seat_driver";
				proxyPos = "crewDriver";
				getInPos = "pos_driver";
				getInDir = "pos_driver_dir";
			};
		};
		class VehicleSimulation
		{
			class Axles
			{
				class Front
				{
					maxSteeringAngle = 45;
					finalRatio = 0.0;
					brakeBias = 0.7;
					brakeForce = 4000;
					wheelHubMass = 10;
					wheelHubRadius = 0.15;
					class Suspension
					{
						swayBar = 0;
						stiffness = 30000;
						compression = 1200;
						damping = 2750;
						travelMaxUp = 0.0482;
						travelMaxDown = 0.0433;
					};
					class Wheels
					{
						class Center
						{
							inventorySlot = "expansion_tt650_wheel_1";
							animTurn = "turnfront";
							animRotation = "wheelfront";
							animDamper = "damper_1";
							wheelHub = "wheel_1_damper_land";
						};
					};
				};
				class Rear
				{
					maxSteeringAngle = 0;
					finalRatio = 3.0;
					brakeBias = 0.7;
					brakeForce = 4000;
					wheelHubMass = 10;
					wheelHubRadius = 0.15;
					class Suspension
					{
						swayBar = 0;
						stiffness = 17000;
						compression = 1000;
						damping = 2550;
						travelMaxUp = 0.0482;
						travelMaxDown = 0.0433;
					};
					class Wheels
					{
						class Center
						{
							inventorySlot = "expansion_tt650_wheel_2";
							animTurn = "";
							animRotation = "wheelback";
							animDamper = "damper_2";
							wheelHub = "wheel_2_damper_land";
						};
					};
				};
			};
		};
		class AnimationSources
		{
			class Damper_1
			{
				source = "user";
				initPhase = 0;
				animPeriod = 1;
			};
			class Damper_2: Damper_1{};
		};
	};
};
