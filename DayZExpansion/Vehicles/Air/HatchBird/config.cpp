#define _ARMA_

class CfgPatches
{
	class DayZExpansion_Vehicles_Air_HatchBird
	{
		units[] = {"ExpansionHatchBirdWreck"};
		weapons[] = {};
		requiredVersion = 0.1;
		requiredAddons[] = {"DayZExpansion_Vehicles_Ground_Hatchback"};
	};
};
class CfgVehicles
{
	class Inventory_Base;
	class ExpansionWreck;
	class SimulationModule;
	class Axles;
	class Suspension;
	class Front;
	class Rear;
	class AnimationSources;
	class Crew;
	class Driver;
	class CoDriver;
	class ExpansionHelicopterScript;
	class GUIInventoryAttachmentsProps;
	class Engine;
	class Body;
	class DamageSystem;
	class DamageZones;
	class GlobalHealth;
	class HatchbackWheel;
	class OffroadHatchback;
	class ExpansionHatchBirdWreck: ExpansionWreck
	{
		scope = 2;
		displayName = "If it quacks like a duck...";
		model = "DZ\structures\Wrecks\Vehicles\Wreck_hb01_aban1_green";
		class Cargo
		{
			itemsCargoSize[] = {10,30};
			allowOwnedCargoManipulation = 1;
			openable = 0;
		};
	};
	class ExpansionHatchBird: OffroadHatchback
	{
		displayName = "If it quacks like a duck...";
		descriptionShort = "...";
		modelZeroPointDistanceFromGround = 0.0;
		vehicleClass = "Expansion_Helicopter";
		wreck = "ExpansionHatchBirdWreck";
		fuelCapacity = 192;
		fuelConsumption = 91;
		attachments[] = {"ExpansionHelicopterBattery","Reflector_1_1","Reflector_2_1","ExpansionIgniterPlug","ExpansionHydraulicHoses","NivaDriverDoors","NivaCoDriverDoors","NivaHood","NivaTrunk","NivaWheel_1_1","NivaWheel_1_2","NivaWheel_2_1","NivaWheel_2_2","NivaWheel_Spare_1","CamoNet"};
		wheels[] = {"HatchbackWheel","HatchbackWheel","HatchbackWheel","HatchbackWheel"};
		class GUIInventoryAttachmentsProps: GUIInventoryAttachmentsProps
		{
			class Engine: Engine
			{
				attachmentSlots[] = {"ExpansionHelicopterBattery","ExpansionIgniterPlug","ExpansionHydraulicHoses"};
			};
			class Body
			{
				attachmentSlots[] = {"Reflector_1_1","Reflector_2_1","NivaHood","NivaTrunk","NivaDriverDoors","NivaCoDriverDoors","NivaWheel_Spare_1","CamoNet"};
			};
			class Chassis
			{
				attachmentSlots[] = {"NivaWheel_1_1","NivaWheel_1_2","NivaWheel_2_1","NivaWheel_2_2"};
			};
		};
		class Sounds
		{
			thrust = 0.6;
			thrustTurbo = 1;
			thrustGentle = 0.3;
			thrustSmoothCoef = 0.1;
			camposSmoothCoef = 0.03;
			soundSetsFilter[] = {"Expansion_Mh6_Engine_Ext_SoundSet","Expansion_Mh6_Rotor_Ext_SoundSet"};
			soundSetsInt[] = {"Expansion_Mh6_Engine_Int_SoundSet","Expansion_Mh6_Rotor_Int_SoundSet"};
		};
		class SimulationModule: SimulationModule
		{
			maxSpeed = 220;
			altitudeFullForce = 1000;
			altitudeNoForce = 2000;
			bodyFrictionCoef = 1.1;
			liftForceCoef = 1.3;
			bankForceCoef = 0.05;
			tailForceCoef = 2.0;
			linearFrictionCoef[] = {16.0,0.04,0.04};
			angularFrictionCoef = 1.5;
			class Rotor
			{
				minAutoRotateSpeed = 2.0;
				maxAutoRotateSpeed = 10.0;
				startUpTime = 15;
			};
			class AntiTorque
			{
				speed = 1.5;
				max = 0.16;
			};
			class Cyclic
			{
				forceCoefficient = 1.3;
				class Forward
				{
					speed = 10.0;
					max = 0.7;
					coefficient = 1.0;
					animation = "cyclicForward";
				};
				class Side
				{
					speed = 10.0;
					max = 0.7;
					coefficient = 1.0;
					animation = "cyclicAside";
				};
			};
		};
	};
};
