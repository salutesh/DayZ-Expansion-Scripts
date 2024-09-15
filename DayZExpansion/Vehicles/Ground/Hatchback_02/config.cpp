class CfgPatches
{
	class DayZExpansion_Vehicles_Ground_Hatchback_02
	{
		units[]={};
		weapons[]={};
		requiredVersion=0.1;
		requiredAddons[]=
		{
			"DayZExpansion_Vehicles_Data"
		};
	};
};
class CfgVehicles
{
	class ExpansionWheelBase;
	class ExpansionVehicleCarBase;
	class CarDoor;
	class CarScript;
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
	class GUIInventoryAttachmentsProps;
	class Body;
	class Vehicle_Hatchback_02_Wheel: ExpansionWheelBase
	{
		scope=0;
		displayName="$STR_Hatchback_02_Wheel0";
		descriptionShort="$STR_Hatchback_02_Wheel1";
		model="\DZ\vehicles\wheeled\Hatchback_02\proxy\Hatchback_02_Wheel.p3d";
		inventorySlot[]=
		{
			"Hatchback_02_Wheel_1_1",
			"Hatchback_02_Wheel_1_2",
			"Hatchback_02_Wheel_2_1",
			"Hatchback_02_Wheel_2_2"
		};
		rotationFlags=8;
		radiusByDamage[]={0,0.33000001,0.30000001,0.28999999,0.99980003,0.25,0.99989998,0.20999999};
		radius=0.33000001;
		friction=0.95999998;
		width=0.16;
		tyreRollResistance=0.0099999998;
		tyreTread=0.94999999;
		class DamageSystem
		{
			class GlobalHealth
			{
				class Health
				{
					hitpoints=200;
					healthLevels[]=
					{
						
						{
							1,
							
							{
								"DZ\vehicles\wheeled\Hatchback_02\data\Hatchback_02_wheel.rvmat"
							}
						},
						
						{
							0.69999999,
							
							{
								"DZ\vehicles\wheeled\Hatchback_02\data\Hatchback_02_wheel.rvmat"
							}
						},
						
						{
							0.5,
							
							{
								"DZ\vehicles\wheeled\Hatchback_02\data\Hatchback_02_wheel_damage.rvmat"
							}
						},
						
						{
							0.30000001,
							
							{
								"DZ\vehicles\wheeled\Hatchback_02\data\Hatchback_02_wheel_damage.rvmat"
							}
						},
						
						{
							0,
							
							{
								"DZ\vehicles\wheeled\Hatchback_02\data\Hatchback_02_wheel_destruct.rvmat"
							}
						}
					};
				};
			};
		};
	};
	class Vehicle_Hatchback_02_Wheel_Ruined: Vehicle_Hatchback_02_Wheel
	{
		model="\DZ\vehicles\wheeled\Hatchback_02\proxy\Hatchback_02_Wheel_ruined.p3d";
		radius=0.22;
		friction=-1;
		width=0.2;
		tyreRollResistance=1;
		tyreRollDrag=80;
		tyreRoughness=1.2;
		tyreTread=0.40000001;
	};
	class Vehicle_Hatchback_02: ExpansionVehicleCarBase
	{
		scope=0;
		displayname="$STR_CivHatchback0";
		descriptionShort="$STR_CivHatchback1";
		model="\dz\vehicles\wheeled\Hatchback_02\Hatchback_02.p3d";
		frontReflectorMatDamaged="";
		frontReflectorMatRuined="";
		fuelCapacity=55;
		fuelConsumption=6.5;
		attachments[]=
		{
			"CarBattery",
			"Reflector_1_1",
			"Reflector_2_1",
			"CarRadiator",
			"SparkPlug",
			"Hatchback_02_Door_1_1",
			"Hatchback_02_Door_1_2",
			"Hatchback_02_Door_2_1",
			"Hatchback_02_Door_2_2",
			"Hatchback_02_Trunk",
			"Hatchback_02_Hood",
			"Hatchback_02_Wheel_1_1",
			"Hatchback_02_Wheel_1_2",
			"Hatchback_02_Wheel_2_1",
			"Hatchback_02_Wheel_2_2",
			"Hatchback_02_Wheel_Spare_1"
		};
		hiddenSelections[]=
		{
			"light_1_1",
			"light_2_1",
			"light_brake_1_2",
			"light_brake_2_2",
			"light_reverse_1_2",
			"light_reverse_2_2",
			"light_1_2",
			"light_2_2",
			"light_dashboard",
			"camo_chassi"
		};
		hiddenSelectionsTextures[]=
		{
			"",
			"",
			"",
			"",
			"",
			"",
			"",
			"",
			"",
			""
		};
		hiddenSelectionsMaterials[]=
		{
			"dz\vehicles\wheeled\offroadhatchback\data\offroadhatchback_lights.rvmat",
			"dz\vehicles\wheeled\offroadhatchback\data\offroadhatchback_lights.rvmat",
			"dz\vehicles\wheeled\offroadhatchback\data\offroadhatchback_lights.rvmat",
			"dz\vehicles\wheeled\offroadhatchback\data\offroadhatchback_lights.rvmat",
			"dz\vehicles\wheeled\offroadhatchback\data\offroadhatchback_lights.rvmat",
			"dz\vehicles\wheeled\offroadhatchback\data\offroadhatchback_lights.rvmat",
			"dz\vehicles\wheeled\offroadhatchback\data\offroadhatchback_lights.rvmat",
			"dz\vehicles\wheeled\offroadhatchback\data\offroadhatchback_lights.rvmat",
			"dz\vehicles\wheeled\offroadhatchback\data\offroadhatchback_lights.rvmat",
			"dz\vehicles\wheeled\hatchback_02\data\red\hatchback_02_body.rvmat"
		};
		dashboardMatOn="dz\vehicles\wheeled\Hatchback_02\data\Hatchback_02_gauges_e.rvmat";
		dashboardMatOff="dz\vehicles\wheeled\Hatchback_02\data\Hatchback_02_gauges.rvmat";
		frontReflectorMatOn="dz\vehicles\wheeled\Hatchback_02\data\hatchback_02_lights.rvmat";
		frontReflectorMatOff="dz\vehicles\wheeled\Hatchback_02\data\Hatchback_02_Body.rvmat";
		brakeReflectorMatOn="dz\vehicles\wheeled\Hatchback_02\data\Hatchback_02_TailLights_e.rvmat";
		brakeReflectorMatOff="dz\vehicles\wheeled\Hatchback_02\data\Hatchback_02_TailLights.rvmat";
		tailReflectorMatOn="dz\vehicles\wheeled\Hatchback_02\data\Hatchback_02_TailLights_e.rvmat";
		tailReflectorMatOff="dz\vehicles\wheeled\Hatchback_02\data\Hatchback_02_TailLights.rvmat";
		reverseReflectorMatOn="dz\vehicles\wheeled\Hatchback_02\data\Hatchback_02_TailLights_e.rvmat";
		reverseReflectorMatOff="dz\vehicles\wheeled\Hatchback_02\data\Hatchback_02_TailLights.rvmat";
		doors[]=
		{
			"Hatchback_02_Door_1_1",
			"Hatchback_02_Door_1_2",
			"Hatchback_02_Door_2_1",
			"Hatchback_02_Door_2_2",
			"Hatchback_02_Trunk",
			"Hatchback_02_Hood"
		};
		defaultSkin="Red";
		class Crew: Crew
		{
			class Driver: Driver
			{
			};
			class CoDriver: CoDriver
			{
			};
			class Cargo1
			{
				actionSel="seat_cargo1";
				proxyPos="crewCargo1";
				getInPos="pos_driver";
				getInDir="pos_driver_dir";
			};
			class Cargo2
			{
				actionSel="seat_cargo2";
				proxyPos="crewCargo2";
				getInPos="pos_coDriver";
				getInDir="pos_coDriver_dir";
			};
		};
		class SimulationModule: SimulationModule
		{
			class Steering
			{
				maxSteeringAngle=35;
				increaseSpeed[]={0,50,60,30,100,15};
				decreaseSpeed[]={0,90,60,45,100,20};
				centeringSpeed[]={0,0,15,27,60,45,100,63};
			};
			class Throttle
			{
				reactionTime=0.85000002;
				defaultThrust=0.69999999;
				gentleThrust=0.40000001;
				turboCoef=2.2;
				gentleCoef=0.5;
			};
			class Brake
			{
				pressureBySpeed[]={0,0.5,10,0.40000001,20,0.30000001,40,0.28,60,0.30000001,80,0.38,100,0.41999999,120,0.47999999,150,0.60000002};
				reactionTime=0.2;
				driverless=0.1;
			};
			class Aerodynamics
			{
				frontalArea=2.1500001;
				dragCoefficient=0.51999998;
				downforceCoefficient=0.0099999998;
				downforceOffset[]={0,0.80000001,-0.69999999};
			};
			drive="DRIVE_FWD";
			class Engine
			{
				torqueCurve[]={850,0,1150,90,2000,130,4400,186,5800,165,7500,0};
				inertia=0.20999999;
				frictionTorque=150;
				rollingFriction=0.2;
				viscousFriction=0.5;
				rpmIdle=1250;
				rpmMin=1500;
				rpmClutch=2000;
				rpmRedline=6250;
			};
			class Clutch
			{
				maxTorqueTransfer=360;
				uncoupleTime=0.25;
				coupleTime=0.34999999;
			};
			class Gearbox
			{
				type="GEARBOX_MANUAL";
				reverse=3.1670001;
				ratios[]={3.4549999,2.118,1.444,1.1289999,0.912};
			};
			class Axles: Axles
			{
				class Front: Front
				{
					maxBrakeTorque=1800;
					wheelHubMass=5;
					wheelHubRadius=0.125;
					class Differential
					{
						ratio=3.6670001;
						type="DIFFERENTIAL_OPEN";
					};
					class Suspension
					{
						stiffness=27000;
						compression=2000;
						damping=7500;
						travelMaxUp=0.16;
						travelMaxDown=0.16;
					};
					class Wheels: Wheels
					{
						class Left: Left
						{
							animDamper="damper_1_1";
							inventorySlot="Hatchback_02_Wheel_1_1";
						};
						class Right: Right
						{
							animDamper="damper_2_1";
							inventorySlot="Hatchback_02_Wheel_2_1";
						};
					};
				};
				class Rear: Rear
				{
					maxBrakeTorque=800;
					maxHandbrakeTorque=2000;
					wheelHubMass=5;
					wheelHubRadius=0.125;
					class Suspension
					{
						stiffness=27500;
						compression=2100;
						damping=8000;
						travelMaxUp=0.16;
						travelMaxDown=0.16;
					};
					class Wheels: Wheels
					{
						class Left: Left
						{
							animDamper="damper_1_2";
							inventorySlot="Hatchback_02_Wheel_1_2";
						};
						class Right: Right
						{
							animDamper="damper_2_2";
							inventorySlot="Hatchback_02_Wheel_2_2";
						};
					};
				};
			};
		};
		class Cargo
		{
			itemsCargoSize[]={10,30};
			allowOwnedCargoManipulation=1;
			openable=0;
		};
		class AnimationSources: AnimationSources
		{
			class DoorsCargo1
			{
				source="user";
				initPhase=0;
				animPeriod=0.5;
			};
			class DoorsCargo2
			{
				source="user";
				initPhase=0;
				animPeriod=0.5;
			};
			class damper_1_1
			{
				source="user";
				initPhase=0.5;
				animPeriod=9.9999997e-06;
			};
			class damper_2_1: damper_1_1
			{
			};
			class damper_1_2: damper_1_1
			{
			};
			class damper_2_2: damper_1_2
			{
			};
		};
		class DamageSystem
		{
			class GlobalHealth
			{
				class Health
				{
					hitpoints=1000;
					healthLevels[]=
					{
						
						{
							1,
							{}
						},
						
						{
							0.69999999,
							{}
						},
						
						{
							0.5,
							{}
						},
						
						{
							0.30000001,
							{}
						},
						
						{
							0,
							{}
						}
					};
				};
			};
			class DamageZones
			{
				class Chassis
				{
					fatalInjuryCoef=-1;
					componentNames[]=
					{
						"dmgZone_chassis"
					};
					class Health
					{
						hitpoints=3000;
						transferToGlobalCoef=0;
					};
					inventorySlots[]={};
				};
				class Front
				{
					fatalInjuryCoef=-1;
					memoryPoints[]=
					{
						"dmgZone_front"
					};
					componentNames[]=
					{
						"dmgZone_front",
						"dmgZone_bumper_1"
					};
					class Health
					{
						hitpoints=700;
						transferToGlobalCoef=0;
						healthLevels[]=
						{
							
							{
								1,
								
								{
									"dz\vehicles\wheeled\hatchback_02\data\red\hatchback_02_body.rvmat"
								}
							},
							
							{
								0.69999999,
								
								{
									"dz\vehicles\wheeled\hatchback_02\data\red\hatchback_02_body.rvmat"
								}
							},
							
							{
								0.5,
								
								{
									"dz\vehicles\wheeled\hatchback_02\data\red\hatchback_02_body_damage.rvmat"
								}
							},
							
							{
								0.30000001,
								
								{
									"dz\vehicles\wheeled\hatchback_02\data\red\hatchback_02_body_damage.rvmat"
								}
							},
							
							{
								0,
								
								{
									"dz\vehicles\wheeled\hatchback_02\data\red\hatchback_02_body_destruct.rvmat"
								}
							}
						};
					};
					transferToZonesNames[]=
					{
						"Fender_1_1",
						"Fender_2_1",
						"Engine"
					};
					transferToZonesCoefs[]={0.69999999,0.69999999,0.80000001};
					inventorySlots[]=
					{
						"Hatchback_02_Hood",
						"CarRadiator",
						"Hatchback_02_Wheel_1_1",
						"Hatchback_02_Wheel_2_1"
					};
					inventorySlotsCoefs[]={0.69999999,0.5,0.80000001,0.80000001};
				};
				class Reflector_1_1
				{
					fatalInjuryCoef=-1;
					componentNames[]=
					{
						"dmgZone_lights_1_1"
					};
					memoryPoints[]=
					{
						"dmgZone_lights_1_1"
					};
					class Health
					{
						hitpoints=20;
						transferToGlobalCoef=0;
						healthLevels[]=
						{
							
							{
								1,
								
								{
									"dz\vehicles\wheeled\hatchback_02\data\hatchback_02_windows.rvmat"
								}
							},
							
							{
								0.69999999,
								{}
							},
							
							{
								0.5,
								
								{
									"dz\vehicles\wheeled\hatchback_02\data\glass_i_damage.rvmat"
								}
							},
							
							{
								0.30000001,
								{}
							},
							
							{
								0,
								
								{
									"dz\vehicles\wheeled\hatchback_02\data\glass_i_destruct.rvmat"
								}
							}
						};
					};
					transferToZonesNames[]=
					{
						"Front",
						"Fender_1_1"
					};
					transferToZonesCoefs[]={1,1};
					inventorySlots[]=
					{
						"Reflector_1_1",
						"Hatchback_02_Wheel_1_1"
					};
					inventorySlotsCoefs[]={1,0.89999998};
				};
				class Reflector_2_1: Reflector_1_1
				{
					memoryPoints[]=
					{
						"dmgZone_lights_2_1"
					};
					componentNames[]=
					{
						"dmgZone_lights_2_1"
					};
					transferToZonesNames[]=
					{
						"Front",
						"Fender_2_1"
					};
					inventorySlots[]=
					{
						"Reflector_2_1",
						"Hatchback_02_Wheel_2_1"
					};
				};
				class Back
				{
					fatalInjuryCoef=-1;
					memoryPoints[]=
					{
						"dmgZone_back"
					};
					componentNames[]=
					{
						"dmgZone_back",
						"dmgZone_bumper_2"
					};
					class Health
					{
						hitpoints=500;
						transferToGlobalCoef=0;
						healthLevels[]=
						{
							
							{
								1,
								
								{
									"dz\vehicles\wheeled\hatchback_02\data\hatchback_02_body.rvmat"
								}
							},
							
							{
								0.69999999,
								
								{
									"dz\vehicles\wheeled\hatchback_02\data\hatchback_02_body.rvmat"
								}
							},
							
							{
								0.5,
								
								{
									"dz\vehicles\wheeled\hatchback_02\data\hatchback_02_body_damage.rvmat"
								}
							},
							
							{
								0.30000001,
								
								{
									"dz\vehicles\wheeled\hatchback_02\data\hatchback_02_body_damage.rvmat"
								}
							},
							
							{
								0,
								
								{
									"dz\vehicles\wheeled\hatchback_02\data\hatchback_02_body_destruct.rvmat"
								}
							}
						};
					};
					transferToZonesNames[]=
					{
						"Fender_1_2",
						"Fender_2_2"
					};
					transferToZonesCoefs[]={0.69999999,0.69999999};
					inventorySlots[]=
					{
						"Hatchback_02_Trunk",
						"Hatchback_02_Wheel_1_2",
						"Hatchback_02_Wheel_2_2"
					};
					inventorySlotsCoefs[]={0.89999998,0.89999998,0.89999998};
				};
				class Roof
				{
					fatalInjuryCoef=-1;
					componentNames[]=
					{
						"dmgZone_roof"
					};
					memoryPoints[]=
					{
						"dmgZone_roof"
					};
					class Health
					{
						hitpoints=400;
						transferToGlobalCoef=0;
						healthLevels[]=
						{
							
							{
								1,
								
								{
									"dz\vehicles\wheeled\hatchback_02\data\red\hatchback_02_body.rvmat"
								}
							},
							
							{
								0.69999999,
								
								{
									"dz\vehicles\wheeled\hatchback_02\data\red\hatchback_02_body.rvmat"
								}
							},
							
							{
								0.5,
								
								{
									"dz\vehicles\wheeled\hatchback_02\data\red\hatchback_02_body_damage.rvmat"
								}
							},
							
							{
								0.30000001,
								
								{
									"dz\vehicles\wheeled\hatchback_02\data\red\hatchback_02_body_damage.rvmat"
								}
							},
							
							{
								0,
								
								{
									"dz\vehicles\wheeled\hatchback_02\data\red\hatchback_02_body_destruct.rvmat"
								}
							}
						};
					};
					inventorySlots[]={};
				};
				class Fender_1_1
				{
					fatalInjuryCoef=-1;
					componentNames[]=
					{
						"dmgZone_fender_1_1"
					};
					memoryPoints[]=
					{
						"dmgZone_fender_1_1"
					};
					class Health
					{
						hitpoints=700;
						transferToGlobalCoef=0;
						healthLevels[]=
						{
							
							{
								1,
								
								{
									"dz\vehicles\wheeled\hatchback_02\data\red\hatchback_02_body.rvmat"
								}
							},
							
							{
								0.69999999,
								
								{
									"dz\vehicles\wheeled\hatchback_02\data\red\hatchback_02_body.rvmat"
								}
							},
							
							{
								0.5,
								
								{
									"dz\vehicles\wheeled\hatchback_02\data\red\hatchback_02_body_damage.rvmat"
								}
							},
							
							{
								0.30000001,
								
								{
									"dz\vehicles\wheeled\hatchback_02\data\red\hatchback_02_body_damage.rvmat"
								}
							},
							
							{
								0,
								
								{
									"dz\vehicles\wheeled\hatchback_02\data\red\hatchback_02_body_destruct.rvmat"
								}
							}
						};
					};
					transferToZonesNames[]=
					{
						"Front",
						"Reflector_1_1",
						"Engine"
					};
					transferToZonesCoefs[]={0.30000001,0.60000002,0.40000001};
					inventorySlots[]=
					{
						"Hatchback_02_Hood",
						"Hatchback_02_Wheel_1_1",
						"Hatchback_02_Door_1_1"
					};
					inventorySlotsCoefs[]={0.60000002,0.89999998,0.30000001};
				};
				class Fender_2_1: Fender_1_1
				{
					memoryPoints[]=
					{
						"dmgZone_fender_2_1"
					};
					componentNames[]=
					{
						"dmgZone_fender_2_1"
					};
					transferToZonesNames[]=
					{
						"Front",
						"Reflector_2_1",
						"Engine"
					};
					transferToZonesCoefs[]={0.30000001,0.60000002,0.40000001};
					inventorySlots[]=
					{
						"Hatchback_02_Hood",
						"Hatchback_02_Wheel_2_1",
						"Hatchback_02_Door_2_1"
					};
					inventorySlotsCoefs[]={0.60000002,0.89999998,0.30000001};
				};
				class Fender_1_2: Fender_1_1
				{
					memoryPoints[]=
					{
						"dmgZone_fender_1_2"
					};
					componentNames[]=
					{
						"dmgZone_fender_1_2"
					};
					transferToZonesNames[]=
					{
						"Back",
						"FuelTank"
					};
					transferToZonesCoefs[]={0.69999999,0.69999999};
					inventorySlots[]=
					{
						"Hatchback_02_Trunk",
						"Hatchback_02_Wheel_1_2",
						"Hatchback_02_Door_1_2"
					};
					inventorySlotsCoefs[]={0.69999999,0.89999998,0.30000001};
				};
				class Fender_2_2: Fender_1_1
				{
					memoryPoints[]=
					{
						"dmgZone_fender_2_2"
					};
					componentNames[]=
					{
						"dmgZone_fender_2_2"
					};
					transferToZonesNames[]=
					{
						"Back",
						"FuelTank"
					};
					transferToZonesCoefs[]={0.69999999,0.69999999};
					inventorySlots[]=
					{
						"Hatchback_02_Trunk",
						"Hatchback_02_Wheel_2_2",
						"Hatchback_02_Door_2_2"
					};
					inventorySlotsCoefs[]={0.69999999,0.89999998,0.30000001};
				};
				class WindowFront
				{
					fatalInjuryCoef=-1;
					memoryPoints[]=
					{
						"dmgZone_windowFront"
					};
					componentNames[]=
					{
						"dmgZone_windowFront"
					};
					class Health
					{
						hitpoints=100;
						transferToGlobalCoef=0;
						healthLevels[]=
						{
							
							{
								1,
								
								{
									"dz\vehicles\wheeled\hatchback_02\data\Hatchback_02_Windows.rvmat",
									"dz\vehicles\wheeled\hatchback_02\data\glass_i.rvmat"
								}
							},
							
							{
								0.69999999,
								
								{
									"dz\vehicles\wheeled\hatchback_02\data\Hatchback_02_Windows.rvmat",
									"dz\vehicles\wheeled\hatchback_02\data\glass_i_damage.rvmat"
								}
							},
							
							{
								0.5,
								
								{
									"hidden",
									"dz\vehicles\wheeled\hatchback_02\data\glass_i_damage.rvmat"
								}
							},
							
							{
								0.30000001,
								
								{
									"hidden",
									"dz\vehicles\wheeled\hatchback_02\data\glass_i_destruct.rvmat"
								}
							},
							
							{
								0,
								"hidden",
								"hidden"
							}
						};
					};
					inventorySlots[]={};
					inventorySlotsCoefs[]={};
				};
				class Engine
				{
					fatalInjuryCoef=0.001;
					memoryPoints[]=
					{
						"dmgZone_engine"
					};
					componentNames[]=
					{
						"dmgZone_engine"
					};
					class Health
					{
						hitpoints=1000;
						transferToGlobalCoef=1;
					};
					inventorySlots[]=
					{
						"CarBattery",
						"SparkPlug",
						"CarRadiator"
					};
					inventorySlotsCoefs[]={0.2,0.2,0.40000001};
				};
				class FuelTank
				{
					fatalInjuryCoef=-1;
					componentNames[]=
					{
						"dmgZone_fuelTank"
					};
					class Health
					{
						hitpoints=600;
						transferToGlobalCoef=0;
						healthLevels[]=
						{
							
							{
								1,
								{}
							},
							
							{
								0.69999999,
								{}
							},
							
							{
								0.5,
								{}
							},
							
							{
								0.30000001,
								{}
							},
							
							{
								0,
								{}
							}
						};
					};
					inventorySlots[]={};
					inventorySlotsCoefs[]={};
				};
			};
		};
		class ObstacleGenerator
		{
			carve=1;
			timeToStationary=5;
			moveThreshold=0.5;
			class Shapes
			{
				class Cylindric
				{
					class Cyl1
					{
						radius=1;
						height=1.5;
						center[]={0,0,0.69999999};
					};
					class Cyl3
					{
						radius=1;
						height=1.5;
						center[]={0,0,-0.69999999};
					};
				};
			};
		};
		class GUIInventoryAttachmentsProps
		{
			class Engine
			{
				name="$STR_attachment_Engine0";
				description="";
				icon="cat_vehicle_engine";
				attachmentSlots[]=
				{
					"CarBattery",
					"CarRadiator",
					"SparkPlug"
				};
			};
			class Body
			{
				name="$STR_attachment_Body0";
				description="";
				icon="cat_vehicle_body";
				attachmentSlots[]=
				{
					"Reflector_1_1",
					"Reflector_2_1",
					"Hatchback_02_Door_1_1",
					"Hatchback_02_Door_1_2",
					"Hatchback_02_Door_2_1",
					"Hatchback_02_Door_2_2",
					"Hatchback_02_Hood",
					"Hatchback_02_Trunk"
				};
			};
			class Chassis
			{
				name="$STR_attachment_Chassis0";
				description="";
				icon="cat_vehicle_chassis";
				attachmentSlots[]=
				{
					"Hatchback_02_Wheel_1_1",
					"Hatchback_02_Wheel_1_2",
					"Hatchback_02_Wheel_2_1",
					"Hatchback_02_Wheel_2_2"
				};
			};
		};
	};
	class Hatchback_02: CarScript
	{
		hiddenSelections[]=
		{
			"light_1_1",
			"light_2_1",
			"light_brake_1_2",
			"light_brake_2_2",
			"light_reverse_1_2",
			"light_reverse_2_2",
			"light_1_2",
			"light_2_2",
			"light_dashboard",
			"camo_chassi"
		};
		hiddenSelectionsTextures[]=
		{
			"",
			"",
			"",
			"",
			"",
			"",
			"",
			"",
			"",
			"",
			""
		};
		hiddenSelectionsMaterials[]=
		{
			"dz\vehicles\wheeled\offroadhatchback\data\offroadhatchback_lights.rvmat",
			"dz\vehicles\wheeled\offroadhatchback\data\offroadhatchback_lights.rvmat",
			"dz\vehicles\wheeled\offroadhatchback\data\offroadhatchback_lights.rvmat",
			"dz\vehicles\wheeled\offroadhatchback\data\offroadhatchback_lights.rvmat",
			"dz\vehicles\wheeled\offroadhatchback\data\offroadhatchback_lights.rvmat",
			"dz\vehicles\wheeled\offroadhatchback\data\offroadhatchback_lights.rvmat",
			"dz\vehicles\wheeled\offroadhatchback\data\offroadhatchback_lights.rvmat",
			"dz\vehicles\wheeled\offroadhatchback\data\offroadhatchback_lights.rvmat",
			"dz\vehicles\wheeled\offroadhatchback\data\offroadhatchback_lights.rvmat",
			"dz\vehicles\wheeled\hatchback_02\data\red\hatchback_02_body.rvmat"
		};
		doors[]=
		{
			"Hatchback_02_Door_1_1",
			"Hatchback_02_Door_1_2",
			"Hatchback_02_Door_2_1",
			"Hatchback_02_Door_2_2",
			"Hatchback_02_Trunk",
			"Hatchback_02_Hood"
		};
		defaultSkin="Red";
		attachments[]+=
		{
			"CamoNet",
			"KeyChain"
		};
		class GUIInventoryAttachmentsProps: GUIInventoryAttachmentsProps
		{
			class Body: Body
			{
				attachmentSlots[]+=
				{
					"CamoNet",
					"KeyChain"
				};
			};
		};
		class DamageSystem
		{
			class GlobalHealth
			{
				class Health
				{
					hitpoints=1000;
				};
			};
			class DamageZones
			{
				class Chassis
				{
					class Health
					{
						hitpoints=3000;
						transferToGlobalCoef=0;
					};
					componentNames[]=
					{
						"dmgZone_chassis"
					};
					fatalInjuryCoef=-1;
					inventorySlots[]={};
				};
				class Front
				{
					class Health
					{
						hitpoints=500;
						transferToGlobalCoef=0;
						healthLevels[]=
						{
							
							{
								1,
								{}
							},
							
							{
								0.69999999,
								{}
							},
							
							{
								0.5,
								{}
							},
							
							{
								0.30000001,
								{}
							},
							
							{
								0,
								{}
							}
						};
					};
					componentNames[]=
					{
						"dmgZone_front"
					};
					memoryPoints[]=
					{
						"dmgZone_front",
						"dmgZone_bumper_1"
					};
					fatalInjuryCoef=-1;
					transferToZonesNames[]=
					{
						"Fender_1_1",
						"Fender_2_1",
						"Engine",
						"WindowFront"
					};
					transferToZonesThreshold[]={0.5,0.5,0.5,0.1};
					transferToZonesCoefs[]={0.69999999,0.69999999,0.5,0.050000001};
					inventorySlots[]=
					{
						"Hatchback_02_Hood",
						"CarRadiator"
					};
					inventorySlotsCoefs[]={0.30000001,0.30000001};
				};
				class Back
				{
					class Health
					{
						hitpoints=100;
						transferToGlobalCoef=0;
						healthLevels[]=
						{
							
							{
								1,
								{}
							},
							
							{
								0.69999999,
								{}
							},
							
							{
								0.5,
								{}
							},
							
							{
								0.30000001,
								{}
							},
							
							{
								0,
								{}
							}
						};
					};
					componentNames[]=
					{
						"dmgZone_back"
					};
					memoryPoints[]=
					{
						"dmgZone_back",
						"dmgZone_bumper_1"
					};
					fatalInjuryCoef=-1;
					inventorySlots[]={};
					inventorySlotsCoefs[]={};
				};
				class WindowFront
				{
					class Health
					{
						hitpoints=100;
						transferToGlobalCoef=0;
						healthLevels[]=
						{
							
							{
								1,
								
								{
									"dz\vehicles\wheeled\hatchback_02\data\Hatchback_02_Windows.rvmat",
									"dz\vehicles\wheeled\hatchback_02\data\glass_i.rvmat"
								}
							},
							
							{
								0.69999999,
								
								{
									"dz\vehicles\wheeled\hatchback_02\data\Hatchback_02_Windows.rvmat",
									"dz\vehicles\wheeled\hatchback_02\data\glass_i_damage.rvmat"
								}
							},
							
							{
								0.5,
								
								{
									"hidden",
									"dz\vehicles\wheeled\hatchback_02\data\glass_i_damage.rvmat"
								}
							},
							
							{
								0.30000001,
								
								{
									"hidden",
									"dz\vehicles\wheeled\hatchback_02\data\glass_i_destruct.rvmat"
								}
							},
							
							{
								0,
								"hidden",
								"hidden"
							}
						};
					};
					componentNames[]=
					{
						"dmgZone_windowFront"
					};
					memoryPoints[]=
					{
						"dmgZone_windowFront"
					};
					fatalInjuryCoef=-1;
					inventorySlots[]={};
					inventorySlotsCoefs[]={};
				};
				class Reflector_1_1
				{
					class Health
					{
						hitpoints=10;
						transferToGlobalCoef=0;
						healthLevels[]=
						{
							
							{
								1,
								
								{
									"dz\vehicles\wheeled\hatchback_02\data\hatchback_02_windows.rvmat"
								}
							},
							
							{
								0.69999999,
								{}
							},
							
							{
								0.5,
								
								{
									"dz\vehicles\wheeled\hatchback_02\data\glass_i_damage.rvmat"
								}
							},
							
							{
								0.30000001,
								{}
							},
							
							{
								0,
								
								{
									"dz\vehicles\wheeled\hatchback_02\data\glass_i_destruct.rvmat"
								}
							}
						};
					};
					componentNames[]=
					{
						"dmgZone_lights_1_1"
					};
					memoryPoints[]=
					{
						"dmgZone_lights_1_1"
					};
					fatalInjuryCoef=-1;
					transferToZonesNames[]=
					{
						"Front",
						"Fender_1_1",
						"Engine"
					};
					transferToZonesCoefs[]={0.1,0.15000001,0.050000001};
					inventorySlots[]=
					{
						"Reflector_1_1"
					};
					inventorySlotsCoefs[]={1};
				};
				class Reflector_2_1: Reflector_1_1
				{
					componentNames[]=
					{
						"dmgZone_lights_2_1"
					};
					memoryPoints[]=
					{
						"dmgZone_lights_2_1"
					};
					transferToZonesNames[]=
					{
						"Front",
						"Fender_2_1",
						"Engine"
					};
					transferToZonesCoefs[]={0.1,0.15000001,0.050000001};
					inventorySlots[]=
					{
						"Reflector_2_1"
					};
					inventorySlotsCoefs[]={1};
				};
				class Roof
				{
					class Health
					{
						hitpoints=500;
						transferToGlobalCoef=0;
						healthLevels[]=
						{
							
							{
								1,
								{}
							},
							
							{
								0.69999999,
								{}
							},
							
							{
								0.5,
								{}
							},
							
							{
								0.30000001,
								{}
							},
							
							{
								0,
								{}
							}
						};
					};
					componentNames[]=
					{
						"dmgZone_roof"
					};
					memoryPoints[]=
					{
						"dmgZone_roof"
					};
					fatalInjuryCoef=-1;
					inventorySlots[]={};
				};
				class Fender_1_1
				{
					class Health
					{
						hitpoints=500;
						transferToGlobalCoef=0;
						healthLevels[]=
						{
							
							{
								1,
								{}
							},
							
							{
								0.69999999,
								{}
							},
							
							{
								0.5,
								{}
							},
							
							{
								0.30000001,
								{}
							},
							
							{
								0,
								{}
							}
						};
					};
					componentNames[]=
					{
						"dmgZone_fender_1_1"
					};
					memoryPoints[]=
					{
						"dmgZone_fender_1_1"
					};
					fatalInjuryCoef=-1;
					inventorySlots[]={};
				};
				class Fender_2_1: Fender_1_1
				{
					componentNames[]=
					{
						"dmgZone_fender_2_1"
					};
					memoryPoints[]=
					{
						"dmgZone_fender_2_1"
					};
				};
				class Fender_1_2: Fender_1_1
				{
					componentNames[]=
					{
						"dmgZone_fender_1_2"
					};
					memoryPoints[]=
					{
						"dmgZone_fender_1_2"
					};
				};
				class Fender_2_2: Fender_1_1
				{
					componentNames[]=
					{
						"dmgZone_fender_2_2"
					};
					memoryPoints[]=
					{
						"dmgZone_fender_2_2"
					};
				};
				class Engine
				{
					class Health
					{
						hitpoints=1000;
						transferToGlobalCoef=1;
					};
					componentNames[]=
					{
						"dmgZone_engine"
					};
					memoryPoints[]=
					{
						"dmgZone_engine"
					};
					fatalInjuryCoef=0.001;
					inventorySlots[]={};
				};
				class FuelTank
				{
					class Health
					{
						hitpoints=1000;
						transferToGlobalCoef=-1;
					};
					componentNames[]=
					{
						"dmgZone_fuelTank"
					};
					memoryPoints[]=
					{
						"dmgZone_fuelTank"
					};
					fatalInjuryCoef=-1;
					inventorySlots[]={};
				};
			};
		};
	};
	class Hatchback_02_Door_1_1: CarDoor
	{
		applySkinsTo[]=
		{
			"Hatchback_02_Door_2_1",
			"Expansion_Hatchback_02_Door_2_1"
		};
		defaultSkin="Red";
		class DamageSystem
		{
			class GlobalHealth
			{
				class Health
				{
					hitpoints=500;
					healthLevels[]=
					{
						
						{
							1,
							{}
						},
						
						{
							0.69999999,
							{}
						},
						
						{
							0.5,
							{}
						},
						
						{
							0.30000001,
							{}
						},
						
						{
							0,
							{}
						}
					};
				};
			};
			class DamageZones
			{
				class Window
				{
					class Health
					{
						hitpoints=70;
						transferToGlobalCoef=0;
						healthLevels[]=
						{
							
							{
								1,
								
								{
									"dz\vehicles\wheeled\hatchback_02\data\Hatchback_02_Windows.rvmat",
									"dz\vehicles\wheeled\hatchback_02\data\glass_i.rvmat"
								}
							},
							
							{
								0.69999999,
								
								{
									"dz\vehicles\wheeled\hatchback_02\data\Hatchback_02_Windows.rvmat",
									"dz\vehicles\wheeled\hatchback_02\data\glass_i_damage.rvmat"
								}
							},
							
							{
								0.5,
								
								{
									"hidden",
									"dz\vehicles\wheeled\hatchback_02\data\glass_i_damage.rvmat"
								}
							},
							
							{
								0.30000001,
								
								{
									"hidden",
									"dz\vehicles\wheeled\hatchback_02\data\glass_i_destruct.rvmat"
								}
							},
							
							{
								0,
								"hidden",
								"hidden"
							}
						};
					};
					componentNames[]=
					{
						"dmgZone_window"
					};
					fatalInjuryCoef=-1;
					inventorySlots[]={};
				};
				class Doors
				{
					class Health
					{
						hitpoints=500;
						transferToGlobalCoef=1;
						healthLevels[]=
						{
							
							{
								1,
								{}
							},
							
							{
								0.69999999,
								{}
							},
							
							{
								0.5,
								{}
							},
							
							{
								0.30000001,
								{}
							},
							
							{
								0,
								
								{
									""
								}
							}
						};
					};
					componentNames[]=
					{
						"dmgZone_doors"
					};
					fatalInjuryCoef=-1;
					inventorySlots[]={};
				};
			};
		};
	};
	class Hatchback_02_Door_2_1: Hatchback_02_Door_1_1
	{
		class DamageSystem
		{
			class GlobalHealth
			{
				class Health
				{
					hitpoints=500;
					healthLevels[]=
					{
						
						{
							1,
							{}
						},
						
						{
							0.69999999,
							{}
						},
						
						{
							0.5,
							{}
						},
						
						{
							0.30000001,
							{}
						},
						
						{
							0,
							{}
						}
					};
				};
			};
			class DamageZones
			{
				class Window
				{
					class Health
					{
						hitpoints=70;
						transferToGlobalCoef=0;
						healthLevels[]=
						{
							
							{
								1,
								
								{
									"dz\vehicles\wheeled\hatchback_02\data\Hatchback_02_Windows.rvmat",
									"dz\vehicles\wheeled\hatchback_02\data\glass_i.rvmat"
								}
							},
							
							{
								0.69999999,
								
								{
									"dz\vehicles\wheeled\hatchback_02\data\Hatchback_02_Windows.rvmat",
									"dz\vehicles\wheeled\hatchback_02\data\glass_i_damage.rvmat"
								}
							},
							
							{
								0.5,
								
								{
									"hidden",
									"dz\vehicles\wheeled\hatchback_02\data\glass_i_damage.rvmat"
								}
							},
							
							{
								0.30000001,
								
								{
									"hidden",
									"dz\vehicles\wheeled\hatchback_02\data\glass_i_destruct.rvmat"
								}
							},
							
							{
								0,
								"hidden",
								"hidden"
							}
						};
					};
					componentNames[]=
					{
						"dmgZone_window"
					};
					fatalInjuryCoef=-1;
					inventorySlots[]={};
				};
				class Doors
				{
					class Health
					{
						hitpoints=500;
						transferToGlobalCoef=1;
						healthLevels[]=
						{
							
							{
								1,
								{}
							},
							
							{
								0.69999999,
								{}
							},
							
							{
								0.5,
								{}
							},
							
							{
								0.30000001,
								{}
							},
							
							{
								0,
								
								{
									""
								}
							}
						};
					};
					componentNames[]=
					{
						"dmgZone_doors"
					};
					fatalInjuryCoef=-1;
					inventorySlots[]={};
				};
			};
		};
	};
	class Hatchback_02_Door_1_2: Hatchback_02_Door_1_1
	{
		applySkinsTo[]=
		{
			"Hatchback_02_Door_2_2",
			"Expansion_Hatchback_02_Door_2_2"
		};
		defaultSkin="Red";
		class DamageSystem
		{
			class GlobalHealth
			{
				class Health
				{
					hitpoints=500;
					healthLevels[]=
					{
						
						{
							1,
							{}
						},
						
						{
							0.69999999,
							{}
						},
						
						{
							0.5,
							{}
						},
						
						{
							0.30000001,
							{}
						},
						
						{
							0,
							{}
						}
					};
				};
			};
			class DamageZones
			{
				class Window
				{
					class Health
					{
						hitpoints=70;
						transferToGlobalCoef=0;
						healthLevels[]=
						{
							
							{
								1,
								
								{
									"dz\vehicles\wheeled\hatchback_02\data\Hatchback_02_Windows.rvmat",
									"dz\vehicles\wheeled\hatchback_02\data\glass_i.rvmat"
								}
							},
							
							{
								0.69999999,
								
								{
									"dz\vehicles\wheeled\hatchback_02\data\Hatchback_02_Windows.rvmat",
									"dz\vehicles\wheeled\hatchback_02\data\glass_i_damage.rvmat"
								}
							},
							
							{
								0.5,
								
								{
									"hidden",
									"dz\vehicles\wheeled\hatchback_02\data\glass_i_damage.rvmat"
								}
							},
							
							{
								0.30000001,
								
								{
									"hidden",
									"dz\vehicles\wheeled\hatchback_02\data\glass_i_destruct.rvmat"
								}
							},
							
							{
								0,
								"hidden",
								"hidden"
							}
						};
					};
					componentNames[]=
					{
						"dmgZone_window"
					};
					fatalInjuryCoef=-1;
					inventorySlots[]={};
				};
				class Doors
				{
					class Health
					{
						hitpoints=500;
						transferToGlobalCoef=1;
						healthLevels[]=
						{
							
							{
								1,
								{}
							},
							
							{
								0.69999999,
								{}
							},
							
							{
								0.5,
								{}
							},
							
							{
								0.30000001,
								{}
							},
							
							{
								0,
								
								{
									""
								}
							}
						};
					};
					componentNames[]=
					{
						"dmgZone_doors"
					};
					fatalInjuryCoef=-1;
					inventorySlots[]={};
				};
			};
		};
	};
	class Hatchback_02_Door_2_2: Hatchback_02_Door_1_2
	{
		class DamageSystem
		{
			class GlobalHealth
			{
				class Health
				{
					hitpoints=500;
					healthLevels[]=
					{
						
						{
							1,
							{}
						},
						
						{
							0.69999999,
							{}
						},
						
						{
							0.5,
							{}
						},
						
						{
							0.30000001,
							{}
						},
						
						{
							0,
							{}
						}
					};
				};
			};
			class DamageZones
			{
				class Window
				{
					class Health
					{
						hitpoints=70;
						transferToGlobalCoef=0;
						healthLevels[]=
						{
							
							{
								1,
								
								{
									"dz\vehicles\wheeled\hatchback_02\data\Hatchback_02_Windows.rvmat",
									"dz\vehicles\wheeled\hatchback_02\data\glass_i.rvmat"
								}
							},
							
							{
								0.69999999,
								
								{
									"dz\vehicles\wheeled\hatchback_02\data\Hatchback_02_Windows.rvmat",
									"dz\vehicles\wheeled\hatchback_02\data\glass_i_damage.rvmat"
								}
							},
							
							{
								0.5,
								
								{
									"hidden",
									"dz\vehicles\wheeled\hatchback_02\data\glass_i_damage.rvmat"
								}
							},
							
							{
								0.30000001,
								
								{
									"hidden",
									"dz\vehicles\wheeled\hatchback_02\data\glass_i_destruct.rvmat"
								}
							},
							
							{
								0,
								"hidden",
								"hidden"
							}
						};
					};
					componentNames[]=
					{
						"dmgZone_window"
					};
					fatalInjuryCoef=-1;
					inventorySlots[]={};
				};
				class Doors
				{
					class Health
					{
						hitpoints=500;
						transferToGlobalCoef=1;
						healthLevels[]=
						{
							
							{
								1,
								{}
							},
							
							{
								0.69999999,
								{}
							},
							
							{
								0.5,
								{}
							},
							
							{
								0.30000001,
								{}
							},
							
							{
								0,
								{}
							}
						};
					};
					componentNames[]=
					{
						"dmgZone_doors"
					};
					fatalInjuryCoef=-1;
					inventorySlots[]={};
				};
			};
		};
	};
	class Hatchback_02_Hood: CarDoor
	{
		scope=2;
		defaultSkin="Red";
		class DamageSystem
		{
			class GlobalHealth
			{
				class Health
				{
					hitpoints=500;
					healthLevels[]=
					{
						
						{
							1,
							{}
						},
						
						{
							0.69999999,
							{}
						},
						
						{
							0.5,
							{}
						},
						
						{
							0.30000001,
							{}
						},
						
						{
							0,
							{}
						}
					};
				};
			};
			class DamageZones
			{
				class Doors
				{
					class Health
					{
						hitpoints=500;
						transferToGlobalCoef=1;
						healthLevels[]=
						{
							
							{
								1,
								{}
							},
							
							{
								0.69999999,
								{}
							},
							
							{
								0.5,
								{}
							},
							
							{
								0.30000001,
								{}
							},
							
							{
								0,
								{}
							}
						};
					};
					componentNames[]=
					{
						"dmgZone_doors"
					};
					fatalInjuryCoef=-1;
					inventorySlots[]={};
				};
			};
		};
	};
	class Hatchback_02_Trunk: CarDoor
	{
		scope=2;
		defaultSkin="Red";
		class DamageSystem
		{
			class GlobalHealth
			{
				class Health
				{
					hitpoints=500;
					healthLevels[]=
					{
						
						{
							1,
							{}
						},
						
						{
							0.69999999,
							{}
						},
						
						{
							0.5,
							{}
						},
						
						{
							0.30000001,
							{}
						},
						
						{
							0,
							{}
						}
					};
				};
			};
		};
	};
	class Hatchback_02_Red: Hatchback_02
	{
		skinBase="Hatchback_02";
		skinName="Red";
	};
	class Vehicle_Hatchback_02_Red: Vehicle_Hatchback_02
	{
		skinBase="Vehicle_Hatchback_02";
		skinName="Red";
	};
	class Hatchback_02_Door_1_1_Red: Hatchback_02_Door_1_1
	{
		skinBase="Hatchback_02_Door_1_1";
		skinName="Red";
	};
	class Hatchback_02_Door_1_2_Red: Hatchback_02_Door_1_2
	{
		skinBase="Hatchback_02_Door_1_2";
		skinName="Red";
	};
	class Hatchback_02_Door_2_1_Red: Hatchback_02_Door_2_1
	{
		skinBase="Hatchback_02_Door_2_1";
		skinName="Red";
	};
	class Hatchback_02_Door_2_2_Red: Hatchback_02_Door_2_2
	{
		skinBase="Hatchback_02_Door_2_2";
		skinName="Red";
	};
	class Hatchback_02_Hood_Red: Hatchback_02_Hood
	{
		skinBase="Hatchback_02_Hood";
		skinName="Red";
	};
	class Hatchback_02_Trunk_Red: Hatchback_02_Trunk
	{
		skinBase="Hatchback_02_Trunk";
		skinName="Red";
	};
	class Hatchback_02_Black: Hatchback_02
	{
		skinBase="Hatchback_02";
		skinName="Black";
	};
	class Vehicle_Hatchback_02_Black: Vehicle_Hatchback_02
	{
		skinBase="Vehicle_Hatchback_02";
		skinName="Black";
	};
	class Hatchback_02_Door_1_1_Black: Hatchback_02_Door_1_1
	{
		skinBase="Hatchback_02_Door_1_1";
		skinName="Black";
	};
	class Hatchback_02_Door_1_2_Black: Hatchback_02_Door_1_2
	{
		skinBase="Hatchback_02_Door_1_2";
		skinName="Black";
	};
	class Hatchback_02_Door_2_1_Black: Hatchback_02_Door_2_1
	{
		skinBase="Hatchback_02_Door_2_1";
		skinName="Black";
	};
	class Hatchback_02_Door_2_2_Black: Hatchback_02_Door_2_2
	{
		skinBase="Hatchback_02_Door_2_2";
		skinName="Black";
	};
	class Hatchback_02_Hood_Black: Hatchback_02_Hood
	{
		skinBase="Hatchback_02_Hood";
		skinName="Black";
	};
	class Hatchback_02_Trunk_Black: Hatchback_02_Trunk
	{
		skinBase="Hatchback_02_Trunk";
		skinName="Black";
	};
	class Hatchback_02_Blue: Hatchback_02
	{
		skinBase="Hatchback_02";
		skinName="Blue";
	};
	class Vehicle_Hatchback_02_Blue: Vehicle_Hatchback_02
	{
		skinBase="Vehicle_Hatchback_02";
		skinName="Blue";
	};
	class Hatchback_02_Door_1_1_Blue: Hatchback_02_Door_1_1
	{
		skinBase="Hatchback_02_Door_1_1";
		skinName="Blue";
	};
	class Hatchback_02_Door_1_2_Blue: Hatchback_02_Door_1_2
	{
		skinBase="Hatchback_02_Door_1_2";
		skinName="Blue";
	};
	class Hatchback_02_Door_2_1_Blue: Hatchback_02_Door_2_1
	{
		skinBase="Hatchback_02_Door_2_1";
		skinName="Blue";
	};
	class Hatchback_02_Door_2_2_Blue: Hatchback_02_Door_2_2
	{
		skinBase="Hatchback_02_Door_2_2";
		skinName="Blue";
	};
	class Hatchback_02_Hood_Blue: Hatchback_02_Hood
	{
		skinBase="Hatchback_02_Hood";
		skinName="Blue";
	};
	class Hatchback_02_Trunk_Blue: Hatchback_02_Trunk
	{
		skinBase="Hatchback_02_Trunk";
		skinName="Blue";
	};
	class Hatchback_02_RedRust: Hatchback_02
	{
		skinBase="Hatchback_02";
		skinName="RedRust";
	};
	class Vehicle_Hatchback_02_RedRust: Vehicle_Hatchback_02
	{
		skinBase="Vehicle_Hatchback_02";
		skinName="RedRust";
	};
	class Hatchback_02_Door_1_1_RedRust: Hatchback_02_Door_1_1
	{
		skinBase="Hatchback_02_Door_1_1";
		skinName="RedRust";
	};
	class Hatchback_02_Door_1_2_RedRust: Hatchback_02_Door_1_2
	{
		skinBase="Hatchback_02_Door_1_2";
		skinName="RedRust";
	};
	class Hatchback_02_Door_2_1_RedRust: Hatchback_02_Door_2_1
	{
		skinBase="Hatchback_02_Door_2_1";
		skinName="RedRust";
	};
	class Hatchback_02_Door_2_2_RedRust: Hatchback_02_Door_2_2
	{
		skinBase="Hatchback_02_Door_2_2";
		skinName="RedRust";
	};
	class Hatchback_02_Hood_RedRust: Hatchback_02_Hood
	{
		skinBase="Hatchback_02_Hood";
		skinName="RedRust";
	};
	class Hatchback_02_Trunk_RedRust: Hatchback_02_Trunk
	{
		skinBase="Hatchback_02_Trunk";
		skinName="RedRust";
	};
	class Hatchback_02_BlackRust: Hatchback_02
	{
		skinBase="Hatchback_02";
		skinName="BlackRust";
	};
	class Vehicle_Hatchback_02_BlackRust: Vehicle_Hatchback_02
	{
		skinBase="Vehicle_Hatchback_02";
		skinName="BlackRust";
	};
	class Hatchback_02_Door_1_1_BlackRust: Hatchback_02_Door_1_1
	{
		skinBase="Hatchback_02_Door_1_1";
		skinName="BlackRust";
	};
	class Hatchback_02_Door_1_2_BlackRust: Hatchback_02_Door_1_2
	{
		skinBase="Hatchback_02_Door_1_2";
		skinName="BlackRust";
	};
	class Hatchback_02_Door_2_1_BlackRust: Hatchback_02_Door_2_1
	{
		skinBase="Hatchback_02_Door_2_1";
		skinName="BlackRust";
	};
	class Hatchback_02_Door_2_2_BlackRust: Hatchback_02_Door_2_2
	{
		skinBase="Hatchback_02_Door_2_2";
		skinName="BlackRust";
	};
	class Hatchback_02_Hood_BlackRust: Hatchback_02_Hood
	{
		skinBase="Hatchback_02_Hood";
		skinName="BlackRust";
	};
	class Hatchback_02_Trunk_BlackRust: Hatchback_02_Trunk
	{
		skinBase="Hatchback_02_Trunk";
		skinName="BlackRust";
	};
	class Hatchback_02_BlueRust: Hatchback_02
	{
		skinBase="Hatchback_02";
		skinName="BlueRust";
	};
	class Vehicle_Hatchback_02_BlueRust: Vehicle_Hatchback_02
	{
		skinBase="Vehicle_Hatchback_02";
		skinName="BlueRust";
	};
	class Hatchback_02_Door_1_1_BlueRust: Hatchback_02_Door_1_1
	{
		skinBase="Hatchback_02_Door_1_1";
		skinName="BlueRust";
	};
	class Hatchback_02_Door_1_2_BlueRust: Hatchback_02_Door_1_2
	{
		skinBase="Hatchback_02_Door_1_2";
		skinName="BlueRust";
	};
	class Hatchback_02_Door_2_1_BlueRust: Hatchback_02_Door_2_1
	{
		skinBase="Hatchback_02_Door_2_1";
		skinName="BlueRust";
	};
	class Hatchback_02_Door_2_2_BlueRust: Hatchback_02_Door_2_2
	{
		skinBase="Hatchback_02_Door_2_2";
		skinName="BlueRust";
	};
	class Hatchback_02_Hood_BlueRust: Hatchback_02_Hood
	{
		skinBase="Hatchback_02_Hood";
		skinName="BlueRust";
	};
	class Hatchback_02_Trunk_BlueRust: Hatchback_02_Trunk
	{
		skinBase="Hatchback_02_Trunk";
		skinName="BlueRust";
	};
	class Hatchback_02_BanditKitty: Hatchback_02
	{
		skinBase="Hatchback_02";
		skinName="BanditKitty";
	};
	class Vehicle_Hatchback_02_BanditKitty: Vehicle_Hatchback_02
	{
		skinBase="Vehicle_Hatchback_02";
		skinName="BanditKitty";
	};
	class Hatchback_02_Door_1_1_BanditKitty: Hatchback_02_Door_1_1
	{
		skinBase="Hatchback_02_Door_1_1";
		skinName="BanditKitty";
	};
	class Hatchback_02_Door_1_2_BanditKitty: Hatchback_02_Door_1_2
	{
		skinBase="Hatchback_02_Door_1_2";
		skinName="BanditKitty";
	};
	class Hatchback_02_Door_2_1_BanditKitty: Hatchback_02_Door_2_1
	{
		skinBase="Hatchback_02_Door_2_1";
		skinName="BanditKitty";
	};
	class Hatchback_02_Door_2_2_BanditKitty: Hatchback_02_Door_2_2
	{
		skinBase="Hatchback_02_Door_2_2";
		skinName="BanditKitty";
	};
	class Hatchback_02_Hood_BanditKitty: Hatchback_02_Hood
	{
		skinBase="Hatchback_02_Hood";
		skinName="BanditKitty";
	};
	class Hatchback_02_Trunk_BanditKitty: Hatchback_02_Trunk
	{
		skinBase="Hatchback_02_Trunk";
		skinName="BanditKitty";
	};
};
