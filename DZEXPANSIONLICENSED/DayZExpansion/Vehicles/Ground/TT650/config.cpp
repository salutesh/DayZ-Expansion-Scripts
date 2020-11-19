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
		radius = 0.333;
		width = 0.316;
		tyreRollResistance = 0.015;
		tyreTread = 1.0;
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
			class Steering
			{
				increaseSpeed[] = {8.0};
				decreaseSpeed[] = {70.0};
				centeringSpeed[] = {50.0};
			};
			class Throttle
			{
				reactionTime = 1.0;
				defaultThrust = 0.85;
				gentleThrust = 0.7;
				turboCoef = 4.0;
				gentleCoef = 0.75;
			};
			braking[] = {0.0,0.1,1.0,0.8,2.5,0.9,3.0,1.0};
			class Engine
			{
				inertia = 0.6;
				torqueMax = 210;
				torqueRpm = 2700;
				powerMax = 80;
				powerRpm = 3500;
				rpmIdle = 650;
				rpmMin = 700;
				rpmClutch = 900;
				rpmRedline = 3700;
				rpmMax = 5000;
			};
			class Gearbox
			{
				reverse = 2.7;
				ratios[] = {3.4,2.3,1.2};
				timeToUncoupleClutch = 0.3;
				timeToCoupleClutch = 0.45;
				maxClutchTorque = 260;
			};
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
						stiffness = 15000;
						compression = 3100;
						damping = 5700;
						travelMaxUp = 0.16;
						travelMaxDown = 0.16;
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
							axis_start = "suspension_1_start";
							axis_end = "suspension_1_end";
						};
					};
				};
				class Rear
				{
					maxSteeringAngle = 0;
					finalRatio = 4.1;
					brakeBias = 0.7;
					brakeForce = 4000;
					wheelHubMass = 10;
					wheelHubRadius = 0.15;
					class Suspension
					{
						swayBar = 0;
						stiffness = 15000;
						compression = 3000;
						damping = 5500;
						travelMaxUp = 0.16;
						travelMaxDown = 0.16;
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
				animPeriod = 1e-06;
			};
			class Damper_2: Damper_1{};
			class wheelfront
			{
				source = "user";
				initPhase = 0;
				animPeriod = 1e-06;
			};
			class wheelback: wheelfront{};
			class steeringwheel
			{
				source = "user";
				initPhase = 0;
				animPeriod = 1e-06;
			};
		};
		class Sounds
		{
			thrust = 0.6;
			thrustTurbo = 1;
			thrustGentle = 0.3;
			thrustSmoothCoef = 0.02;
			camposSmoothCoef = 0.03;
			soundSetsFilter[] = {"Expansion_TT650_Offload_Ext_Rpm1_SoundSet","Expansion_TT650_Offload_Ext_Rpm2_SoundSet","Expansion_TT650_Offload_Ext_Rpm3_SoundSet","Expansion_TT650_Offload_Ext_Rpm4_SoundSet","Expansion_TT650_Offload_Ext_Rpm5_SoundSet","Expansion_TT650_Ext_Rpm0_SoundSet","Expansion_TT650_Ext_Rpm1_SoundSet","Expansion_TT650_Ext_Rpm2_SoundSet","Expansion_TT650_Ext_Rpm3_SoundSet","Expansion_TT650_Ext_Rpm4_SoundSet","Expansion_TT650_Ext_Rpm5_SoundSet"};
			soundSetsInt[] = {};
		};
	};
};
class CfgNonAIVehicles
{
	class ProxyAttachment;
	class ProxyVehiclePart: ProxyAttachment
	{
		scope = 2;
		simulation = "ProxyInventory";
		autocenter = 0;
		animated = 0;
		shadow = 1;
		reversed = 0;
	};
	class Proxyexpansion_tt650_wheel: ProxyVehiclePart
	{
		model = "DayZExpansion\Vehicles\Ground\TT650\proxy\expansion_tt650_wheel.p3d";
		inventorySlot[] = {"expansion_tt650_wheel_1","expansion_tt650_wheel_2"};
	};
};
