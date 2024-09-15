class CfgPatches
{
	class DayZExpansion_Vehicles_Ground_Sedan_02
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
	class Vehicle_Sedan_02_Wheel: ExpansionWheelBase
	{
		scope=0;
		displayName="$STR_Sedan_02_Wheel0";
		descriptionShort="$STR_Sedan_02_Wheel1";
		model="\DZ\vehicles\wheeled\Sedan_02\proxy\Sedan_02_Wheel.p3d";
		inventorySlot[]=
		{
			"Sedan_02_Wheel_1_1",
			"Sedan_02_Wheel_1_2",
			"Sedan_02_Wheel_2_1",
			"Sedan_02_Wheel_2_2"
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
								"DZ\vehicles\wheeled\Sedan_02\data\Sedan_02_wheel.rvmat"
							}
						},
						
						{
							0.69999999,
							
							{
								"DZ\vehicles\wheeled\Sedan_02\data\Sedan_02_wheel.rvmat"
							}
						},
						
						{
							0.5,
							
							{
								"DZ\vehicles\wheeled\Sedan_02\data\Sedan_02_wheel_damage.rvmat"
							}
						},
						
						{
							0.30000001,
							
							{
								"DZ\vehicles\wheeled\Sedan_02\data\Sedan_02_wheel_damage.rvmat"
							}
						},
						
						{
							0,
							
							{
								"DZ\vehicles\wheeled\Sedan_02\data\Sedan_02_wheel_damage.rvmat"
							}
						}
					};
				};
			};
		};
	};
	class Vehicle_Sedan_02_Wheel_Ruined: Vehicle_Sedan_02_Wheel
	{
		model="\DZ\vehicles\wheeled\Sedan_02\proxy\Sedan_02_Wheel_ruined.p3d";
		radius=0.22;
		friction=-1;
		width=0.2;
		tyreRollResistance=1;
		tyreRollDrag=80;
		tyreRoughness=1.2;
		tyreTread=0.40000001;
	};
	class Vehicle_Sedan_02: ExpansionVehicleCarBase
	{
		scope=0;
		displayname="$STR_Sedan_020";
		model="\DZ\vehicles\wheeled\Sedan_02\Sedan_02.p3d";
		attachments[]=
		{
			"CarBattery",
			"Reflector_1_1",
			"Reflector_2_1",
			"CarRadiator",
			"EngineBelt",
			"SparkPlug",
			"Sedan_02_Hood",
			"Sedan_02_Trunk",
			"Sedan_02_Door_1_1",
			"Sedan_02_Door_2_1",
			"Sedan_02_Door_1_2",
			"Sedan_02_Door_2_2",
			"Sedan_02_Wheel_1_1",
			"Sedan_02_Wheel_1_2",
			"Sedan_02_Wheel_2_1",
			"Sedan_02_Wheel_2_2",
			"Sedan_02_Wheel_Spare_1"
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
			"dmgZone_chassis",
			"dmgZone_front",
			"dmgZone_back",
			"dmgZone_roof",
			"dmgZone_fender_1_1",
			"dmgZone_fender_1_2",
			"dmgZone_fender_2_1",
			"dmgZone_fender_2_2"
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
			"dz\vehicles\wheeled\sedan_02\data\yellow\sedan_02_body.rvmat",
			"dz\vehicles\wheeled\sedan_02\data\yellow\sedan_02_body.rvmat",
			"dz\vehicles\wheeled\sedan_02\data\yellow\sedan_02_body.rvmat",
			"dz\vehicles\wheeled\sedan_02\data\yellow\sedan_02_body.rvmat",
			"dz\vehicles\wheeled\sedan_02\data\yellow\sedan_02_body.rvmat",
			"dz\vehicles\wheeled\sedan_02\data\yellow\sedan_02_body.rvmat",
			"dz\vehicles\wheeled\sedan_02\data\yellow\sedan_02_body.rvmat",
			"dz\vehicles\wheeled\sedan_02\data\yellow\sedan_02_body.rvmat"
		};
		dashboardMatOn="dz\vehicles\wheeled\sedan_02\data\sedan_02_int2e.rvmat";
		dashboardMatOff="dz\vehicles\wheeled\sedan_02\data\sedan_02_int2.rvmat";
		frontReflectorMatOn="dz\vehicles\wheeled\sedan_02\data\sedan_02_chrome_e.rvmat";
		frontReflectorMatOff="dz\vehicles\wheeled\sedan_02\data\sedan_02_chrome.rvmat";
		brakeReflectorMatOn="dz\vehicles\wheeled\sedan_02\data\sedan_02_lights_e.rvmat";
		brakeReflectorMatOff="dz\vehicles\wheeled\sedan_02\data\sedan_02_lights.rvmat";
		ReverseReflectorMatOn="dz\vehicles\wheeled\sedan_02\data\sedan_02_lights_e.rvmat";
		ReverseReflectorMatOff="dz\vehicles\wheeled\sedan_02\data\sedan_02_lights.rvmat";
		TailReflectorMatOn="dz\vehicles\wheeled\sedan_02\data\sedan_02_lights_e.rvmat";
		TailReflectorMatOff="dz\vehicles\wheeled\sedan_02\data\sedan_02_lights.rvmat";
		fuelCapacity=38;
		fuelConsumption=7;
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
				getInPos="pos_cargo1";
				getInDir="pos_cargo1_dir";
			};
			class Cargo2
			{
				actionSel="seat_cargo2";
				proxyPos="crewCargo2";
				getInPos="pos_cargo2";
				getInDir="pos_cargo2_dir";
			};
		};
		class SimulationModule: SimulationModule
		{
			class Steering
			{
				maxSteeringAngle=30;
				increaseSpeed[]={0,40,30,20,60,10,100,5};
				decreaseSpeed[]={0,90,60,70,100,50};
				centeringSpeed[]={0,0,15,35,60,65,100,90};
			};
			class Throttle
			{
				reactionTime=0.80000001;
				defaultThrust=0.75;
				gentleThrust=0.5;
				turboCoef=3.8;
				gentleCoef=0.5;
			};
			class Brake
			{
				pressureBySpeed[]={0,1,10,0.94999999,20,0.5,40,0.40000001,80,0.55000001,100,0.60000002,120,0.80000001};
				reactionTime=0.34999999;
				driverless=0.1;
			};
			class Aerodynamics
			{
				frontalArea=2;
				dragCoefficient=0.34999999;
			};
			drive="DRIVE_RWD";
			class Engine
			{
				torqueCurve[]={600,0,990,50,1800,75,3000,82,5000,71,7000,0};
				inertia=0.2;
				frictionTorque=80;
				rollingFriction=0.5;
				viscousFriction=0;
				rpmIdle=900;
				rpmMin=1000;
				rpmClutch=1500;
				rpmRedline=5750;
			};
			class Clutch
			{
				maxTorqueTransfer=165;
				uncoupleTime=0.1;
				coupleTime=0.30000001;
			};
			class Gearbox
			{
				type="GEARBOX_MANUAL";
				reverse=3.27;
				ratios[]={3.8,2.1199999,1.41,0.95999998};
			};
			class Axles: Axles
			{
				class Front: Front
				{
					maxBrakeTorque=800;
					wheelHubMass=5;
					wheelHubRadius=0.17;
					class Suspension
					{
						stiffness=25000;
						compression=1600;
						damping=5200;
						travelMaxUp=0.07;
						travelMaxDown=0.079999998;
					};
					class Wheels: Wheels
					{
						class Left: Left
						{
							animDamper="damper_1_1";
							inventorySlot="Sedan_02_Wheel_1_1";
						};
						class Right: Right
						{
							animDamper="damper_2_1";
							inventorySlot="Sedan_02_Wheel_2_1";
						};
					};
				};
				class Rear: Rear
				{
					maxBrakeTorque=600;
					maxHandbrakeTorque=1200;
					wheelHubMass=5;
					wheelHubRadius=0.17;
					class Differential
					{
						ratio=4.2199998;
						type="DIFFERENTIAL_OPEN";
					};
					class Suspension
					{
						stiffness=21500;
						compression=1800;
						damping=5600;
						travelMaxUp=0.1;
						travelMaxDown=0.1;
					};
					class Wheels: Wheels
					{
						class Left: Left
						{
							animDamper="damper_1_2";
							inventorySlot="Sedan_02_Wheel_1_2";
						};
						class Right: Right
						{
							animDamper="damper_2_2";
							inventorySlot="Sedan_02_Wheel_2_2";
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
						hitpoints=1000;
						transferToGlobalCoef=0;
						healthLevels[]=
						{
							
							{
								1,
								
								{
									"dz\vehicles\wheeled\sedan_02\data\yellow\sedan_02_body.rvmat"
								}
							},
							
							{
								0.69999999,
								
								{
									"dz\vehicles\wheeled\sedan_02\data\yellow\sedan_02_body.rvmat"
								}
							},
							
							{
								0.5,
								
								{
									"dz\vehicles\wheeled\sedan_02\data\yellow\sedan_02_body_damage.rvmat"
								}
							},
							
							{
								0.30000001,
								
								{
									"dz\vehicles\wheeled\sedan_02\data\yellow\sedan_02_body_damage.rvmat"
								}
							},
							
							{
								0,
								
								{
									"dz\vehicles\wheeled\sedan_02\data\yellow\sedan_02_body_destruct.rvmat"
								}
							}
						};
					};
					transferToZonesNames[]=
					{
						"Fender_1_1",
						"Fender_2_1"
					};
					transferToZonesCoefs[]={0.69999999,0.69999999};
					inventorySlots[]=
					{
						"Sedan_02_Hood",
						"CarRadiator",
						"Sedan_02_Wheel_1_1",
						"Sedan_02_Wheel_2_1"
					};
					inventorySlotsCoefs[]={0.69999999,0.5,0.80000001,0.80000001};
				};
				class Reflector_1_1
				{
					fatalInjuryCoef=-1;
					memoryPoints[]=
					{
						"dmgZone_lights_1_1"
					};
					componentNames[]=
					{
						"dmgZone_lights_1_1"
					};
					class Health
					{
						hitpoints=10;
						transferToGlobalCoef=0;
						healthLevels[]=
						{
							
							{
								1,
								
								{
									"dz\vehicles\wheeled\sedan_02\data\glass_i.rvmat"
								}
							},
							
							{
								0.69999999,
								{}
							},
							
							{
								0.5,
								
								{
									"dz\vehicles\wheeled\sedan_02\data\glass_i_damage.rvmat"
								}
							},
							
							{
								0.30000001,
								{}
							},
							
							{
								0,
								
								{
									"dz\vehicles\wheeled\sedan_02\data\glass_i_destruct.rvmat"
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
						"Sedan_02_Wheel_1_1"
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
						"Sedan_02_Wheel_2_1"
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
						"dmgZone_back"
					};
					class Health
					{
						hitpoints=1000;
						transferToGlobalCoef=0;
						healthLevels[]=
						{
							
							{
								1,
								
								{
									"dz\vehicles\wheeled\sedan_02\data\yellow\sedan_02_body.rvmat"
								}
							},
							
							{
								0.69999999,
								
								{
									"dz\vehicles\wheeled\sedan_02\data\yellow\sedan_02_body.rvmat"
								}
							},
							
							{
								0.5,
								
								{
									"dz\vehicles\wheeled\sedan_02\data\yellow\sedan_02_body_damage.rvmat"
								}
							},
							
							{
								0.30000001,
								
								{
									"dz\vehicles\wheeled\sedan_02\data\yellow\sedan_02_body_damage.rvmat"
								}
							},
							
							{
								0,
								
								{
									"dz\vehicles\wheeled\sedan_02\data\yellow\sedan_02_body_destruct.rvmat"
								}
							}
						};
					};
					transferToZonesNames[]=
					{
						"Fender_1_2",
						"Fender_2_2",
						"Engine"
					};
					transferToZonesCoefs[]={0.69999999,0.69999999,0.80000001};
					inventorySlots[]=
					{
						"Sedan_02_Trunk",
						"Sedan_02_Wheel_1_2",
						"Sedan_02_Wheel_2_2"
					};
					inventorySlotsCoefs[]={1,0.89999998,0.89999998};
				};
				class Roof
				{
					fatalInjuryCoef=-1;
					memoryPoints[]=
					{
						"dmgZone_roof"
					};
					componentNames[]=
					{
						"dmgZone_roof"
					};
					class Health
					{
						hitpoints=600;
						transferToGlobalCoef=0;
						healthLevels[]=
						{
							
							{
								1,
								
								{
									"dz\vehicles\wheeled\sedan_02\data\yellow\sedan_02_body.rvmat"
								}
							},
							
							{
								0.69999999,
								
								{
									"dz\vehicles\wheeled\sedan_02\data\yellow\sedan_02_body.rvmat"
								}
							},
							
							{
								0.5,
								
								{
									"dz\vehicles\wheeled\sedan_02\data\yellow\sedan_02_body_damage.rvmat"
								}
							},
							
							{
								0.30000001,
								
								{
									"dz\vehicles\wheeled\sedan_02\data\yellow\sedan_02_body_damage.rvmat"
								}
							},
							
							{
								0,
								
								{
									"dz\vehicles\wheeled\sedan_02\data\yellow\sedan_02_body_destruct.rvmat"
								}
							}
						};
					};
					inventorySlots[]={};
				};
				class Fender_1_1
				{
					fatalInjuryCoef=-1;
					memoryPoints[]=
					{
						"dmgZone_fender_1_1"
					};
					componentNames[]=
					{
						"dmgZone_fender_1_1"
					};
					class Health
					{
						hitpoints=800;
						transferToGlobalCoef=0;
						healthLevels[]=
						{
							
							{
								1,
								
								{
									"dz\vehicles\wheeled\sedan_02\data\yellow\sedan_02_body.rvmat"
								}
							},
							
							{
								0.69999999,
								
								{
									"dz\vehicles\wheeled\sedan_02\data\yellow\sedan_02_body.rvmat"
								}
							},
							
							{
								0.5,
								
								{
									"dz\vehicles\wheeled\sedan_02\data\yellow\sedan_02_body_damage.rvmat"
								}
							},
							
							{
								0.30000001,
								
								{
									"dz\vehicles\wheeled\sedan_02\data\yellow\sedan_02_body_damage.rvmat"
								}
							},
							
							{
								0,
								
								{
									"dz\vehicles\wheeled\sedan_02\data\yellow\sedan_02_body_destruct.rvmat"
								}
							}
						};
					};
					transferToZonesNames[]=
					{
						"Front",
						"Reflector_1_1"
					};
					transferToZonesCoefs[]={0.30000001,0.60000002};
					inventorySlots[]=
					{
						"Sedan_02_Hood",
						"Sedan_02_Wheel_1_1",
						"Sedan_02_Door_1_1"
					};
					inventorySlotsCoefs[]={0.60000002,0.89999998,0.80000001};
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
						"Reflector_2_1"
					};
					transferToZonesCoefs[]={0.30000001,0.60000002};
					inventorySlots[]=
					{
						"Sedan_02_Hood",
						"Sedan_02_Wheel_2_1",
						"Sedan_02_Door_2_1"
					};
					inventorySlotsCoefs[]={0.60000002,0.89999998,0.80000001};
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
						"Engine",
						"FuelTank"
					};
					transferToZonesCoefs[]={0.69999999,0.40000001,0.69999999};
					inventorySlots[]=
					{
						"Sedan_02_Trunk",
						"Sedan_02_Wheel_1_2",
						"Sedan_02_Door_1_2"
					};
					inventorySlotsCoefs[]={0.69999999,0.89999998,0.80000001};
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
						"Engine",
						"FuelTank"
					};
					transferToZonesCoefs[]={0.69999999,0.40000001,0.69999999};
					inventorySlots[]=
					{
						"Sedan_02_Trunk",
						"Sedan_02_Wheel_2_2",
						"Sedan_02_Door_2_2"
					};
					inventorySlotsCoefs[]={0.69999999,0.89999998,0.80000001};
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
						hitpoints=90;
						transferToGlobalCoef=0;
						healthLevels[]=
						{
							
							{
								1,
								
								{
									"dz\vehicles\wheeled\sedan_02\data\glass_interior.rvmat"
								}
							},
							
							{
								0.69999999,
								{}
							},
							
							{
								0.5,
								
								{
									"dz\vehicles\wheeled\sedan_02\data\glass_i_damage.rvmat"
								}
							},
							
							{
								0.30000001,
								
								{
									"dz\vehicles\wheeled\sedan_02\data\glass_i_destruct.rvmat"
								}
							},
							
							{
								0,
								"hidden"
							}
						};
					};
					inventorySlots[]={};
					inventorySlotsCoefs[]={};
				};
				class WindowBack: WindowFront
				{
					componentNames[]=
					{
						"dmgZone_windowBack"
					};
					memoryPoints[]=
					{
						"dmgZone_windowBack"
					};
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
						healthLevels[]=
						{
							
							{
								1,
								
								{
									"dz\vehicles\wheeled\sedan_02\data\sedan_02_engine.rvmat"
								}
							},
							
							{
								0.69999999,
								
								{
									"dz\vehicles\wheeled\sedan_02\data\sedan_02_engine.rvmat"
								}
							},
							
							{
								0.5,
								
								{
									"dz\vehicles\wheeled\sedan_02\data\sedan_02_RUST_engine.rvmat"
								}
							},
							
							{
								0.30000001,
								
								{
									"dz\vehicles\wheeled\sedan_02\data\sedan_02_RUST_engine.rvmat"
								}
							},
							
							{
								0,
								
								{
									"dz\vehicles\wheeled\sedan_02\data\sedan_02_RUST_engine.rvmat"
								}
							}
						};
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
						hitpoints=400;
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
						center[]={0,0,0.80000001};
					};
					class Cyl3
					{
						radius=1;
						height=1.5;
						center[]={0,0,-0.80000001};
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
					"Sedan_02_Door_1_1",
					"Sedan_02_Door_2_1",
					"Sedan_02_Door_1_2",
					"Sedan_02_Door_2_2",
					"Sedan_02_Hood",
					"Sedan_02_Trunk"
				};
			};
			class Chassis
			{
				name="$STR_attachment_Chassis0";
				description="";
				icon="cat_vehicle_chassis";
				attachmentSlots[]=
				{
					"Sedan_02_Wheel_1_1",
					"Sedan_02_Wheel_1_2",
					"Sedan_02_Wheel_2_1",
					"Sedan_02_Wheel_2_2"
				};
			};
		};
	};
	class Sedan_02: CarScript
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
			"camo",
			"dmgZone_chassis",
			"dmgZone_front",
			"dmgZone_back",
			"dmgZone_roof",
			"dmgZone_fender_1_1",
			"dmgZone_fender_1_2",
			"dmgZone_fender_2_1",
			"dmgZone_fender_2_2"
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
			"dz\vehicles\wheeled\sedan_02\data\yellow\sedan_02_body.rvmat",
			"dz\vehicles\wheeled\sedan_02\data\yellow\sedan_02_body.rvmat",
			"dz\vehicles\wheeled\sedan_02\data\yellow\sedan_02_body.rvmat",
			"dz\vehicles\wheeled\sedan_02\data\yellow\sedan_02_body.rvmat",
			"dz\vehicles\wheeled\sedan_02\data\yellow\sedan_02_body.rvmat",
			"dz\vehicles\wheeled\sedan_02\data\yellow\sedan_02_body.rvmat",
			"dz\vehicles\wheeled\sedan_02\data\yellow\sedan_02_body.rvmat",
			"dz\vehicles\wheeled\sedan_02\data\yellow\sedan_02_body.rvmat",
			"dz\vehicles\wheeled\sedan_02\data\yellow\sedan_02_body.rvmat"
		};
		doors[]=
		{
			"Sedan_02_Door_1_1",
			"Sedan_02_Door_1_2",
			"Sedan_02_Door_2_1",
			"Sedan_02_Door_2_2",
			"Sedan_02_Trunk",
			"Sedan_02_Hood"
		};
		defaultSkin="Yellow";
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
					displayName="$STR_CfgVehicleDmg_Chassis0";
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
						hitpoints=1000;
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
					displayName="$STR_CfgVehicleDmg_Bumper0";
					transferToZonesNames[]=
					{
						"Fender_1_1",
						"Fender_2_1"
					};
					transferToZonesCoefs[]={0.40000001,0.40000001};
					inventorySlots[]=
					{
						"Sedan_02_Hood",
						"CarRadiator",
						"Sedan_02_Wheel_1_1",
						"Sedan_02_Wheel_2_1"
					};
					inventorySlotsCoefs[]={0.60000002,0.60000002,0.40000001,0.40000001};
				};
				class Reflector_1_1
				{
					fatalInjuryCoef=-1;
					memoryPoints[]=
					{
						"dmgZone_lights_1_1"
					};
					componentNames[]=
					{
						"dmgZone_lights_1_1"
					};
					class Health
					{
						hitpoints=10;
						transferToGlobalCoef=0;
						healthLevels[]=
						{
							
							{
								1,
								
								{
									"dz\vehicles\wheeled\sedan_02\data\glass_frontlights.rvmat"
								}
							},
							
							{
								0.69999999,
								
								{
									"dz\vehicles\wheeled\sedan_02\data\glass_frontlights_damaged.rvmat"
								}
							},
							
							{
								0.5,
								
								{
									"dz\vehicles\wheeled\sedan_02\data\glass_frontlights_damaged.rvmat"
								}
							},
							
							{
								0.30000001,
								
								{
									"dz\vehicles\wheeled\sedan_02\data\glass_frontlights_ruined.rvmat"
								}
							},
							
							{
								0,
								"Hidden"
							}
						};
					};
					displayName="$STR_CfgVehicleDmg_Reflector0";
					transferToZonesNames[]=
					{
						"Front",
						"Fender_1_1"
					};
					transferToZonesCoefs[]={0.60000002,0.30000001};
					inventorySlots[]=
					{
						"Reflector_1_1",
						"Sedan_02_Wheel_1_1"
					};
					inventorySlotsCoefs[]={1,0.1};
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
						"Sedan_02_Wheel_2_1"
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
						"dmgZone_back"
					};
					class Health
					{
						hitpoints=1000;
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
					displayName="$STR_CfgVehicleDmg_Bumper0";
					transferToZonesNames[]=
					{
						"Fender_1_2",
						"Fender_2_2",
						"Engine"
					};
					transferToZonesCoefs[]={0.40000001,0.40000001,0.60000002};
					inventorySlots[]=
					{
						"Sedan_02_Trunk",
						"Sedan_02_Wheel_1_2",
						"Sedan_02_Wheel_2_2"
					};
					inventorySlotsCoefs[]={0.69999999,0.40000001,0.40000001};
				};
				class Roof
				{
					fatalInjuryCoef=-1;
					memoryPoints[]=
					{
						"dmgZone_roof"
					};
					componentNames[]=
					{
						"dmgZone_roof"
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
					displayName="$STR_CfgVehicleDmg_Roof0";
					inventorySlots[]={};
				};
				class Fender_1_1
				{
					fatalInjuryCoef=-1;
					memoryPoints[]=
					{
						"dmgZone_fender_1_1"
					};
					componentNames[]=
					{
						"dmgZone_fender_1_1"
					};
					class Health
					{
						hitpoints=800;
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
					displayName="$STR_CfgVehicleDmg_Fender0";
					transferToZonesNames[]=
					{
						"Front",
						"Reflector_1_1"
					};
					transferToZonesCoefs[]={0.30000001,0.60000002};
					inventorySlots[]=
					{
						"Sedan_02_Hood",
						"Sedan_02_Wheel_1_1",
						"Sedan_02_Door_1_1"
					};
					inventorySlotsCoefs[]={0.60000002,0.89999998,0.80000001};
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
						"Reflector_2_1"
					};
					transferToZonesCoefs[]={0.30000001,0.60000002};
					inventorySlots[]=
					{
						"Sedan_02_Hood",
						"Sedan_02_Wheel_2_1",
						"Sedan_02_Door_2_1"
					};
					inventorySlotsCoefs[]={0.60000002,0.89999998,0.80000001};
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
						"Engine",
						"FuelTank"
					};
					transferToZonesCoefs[]={0.69999999,0.2,0.69999999};
					inventorySlots[]=
					{
						"Sedan_02_Trunk",
						"Sedan_02_Wheel_1_2",
						"Sedan_02_Door_1_2"
					};
					inventorySlotsCoefs[]={0.69999999,0.89999998,0.80000001};
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
						"Engine",
						"FuelTank"
					};
					transferToZonesCoefs[]={0.69999999,0.2,0.69999999};
					inventorySlots[]=
					{
						"Sedan_02_Trunk",
						"Sedan_02_Wheel_2_2",
						"Sedan_02_Door_2_2"
					};
					inventorySlotsCoefs[]={0.69999999,0.89999998,0.80000001};
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
						hitpoints=90;
						transferToGlobalCoef=0;
						healthLevels[]=
						{
							
							{
								1,
								
								{
									"dz\vehicles\wheeled\sedan_02\data\glass_interior.rvmat"
								}
							},
							
							{
								0.69999999,
								{}
							},
							
							{
								0.5,
								
								{
									"dz\vehicles\wheeled\sedan_02\data\glass_i_damage.rvmat"
								}
							},
							
							{
								0.30000001,
								
								{
									"dz\vehicles\wheeled\sedan_02\data\glass_i_destruct.rvmat"
								}
							},
							
							{
								0,
								"hidden"
							}
						};
					};
					displayName="$STR_CfgVehicleDmg_Window0";
					inventorySlots[]={};
					inventorySlotsCoefs[]={};
				};
				class WindowBack: WindowFront
				{
					componentNames[]=
					{
						"dmgZone_windowBack"
					};
					memoryPoints[]=
					{
						"dmgZone_windowBack"
					};
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
					displayName="$STR_CfgVehicleDmg_Engine0";
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
						hitpoints=400;
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
					displayName="$STR_CfgVehicleDmg_FuelTank0";
					inventorySlots[]={};
					inventorySlotsCoefs[]={};
				};
			};
		};
	};
	class Sedan_02_Door_1_1: CarDoor
	{
		applySkinsTo[]=
		{
			"Sedan_02_Door_2_1",
			"Expansion_Sedan_02_Door_2_1"
		};
		defaultSkin="Yellow";
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
									"dz\vehicles\wheeled\sedan_02\data\glass_interior.rvmat"
								}
							},
							
							{
								0.69999999,
								{}
							},
							
							{
								0.5,
								
								{
									"dz\vehicles\wheeled\Sedan_02\data\glass_i_damage.rvmat"
								}
							},
							
							{
								0.30000001,
								
								{
									"dz\vehicles\wheeled\Sedan_02\data\glass_i_destruct.rvmat"
								}
							},
							
							{
								0,
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
	class Sedan_02_Door_2_1: Sedan_02_Door_1_1
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
									"dz\vehicles\wheeled\sedan_02\data\glass_interior.rvmat"
								}
							},
							
							{
								0.69999999,
								{}
							},
							
							{
								0.5,
								
								{
									"dz\vehicles\wheeled\Sedan_02\data\glass_i_damage.rvmat"
								}
							},
							
							{
								0.30000001,
								
								{
									"dz\vehicles\wheeled\Sedan_02\data\glass_i_destruct.rvmat"
								}
							},
							
							{
								0,
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
	class Sedan_02_Door_1_2: Sedan_02_Door_1_1
	{
		applySkinsTo[]=
		{
			"Sedan_02_Door_2_2",
			"Expansion_Sedan_02_Door_2_2"
		};
		defaultSkin="Yellow";
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
									"dz\vehicles\wheeled\sedan_02\data\glass_interior.rvmat"
								}
							},
							
							{
								0.69999999,
								{}
							},
							
							{
								0.5,
								
								{
									"dz\vehicles\wheeled\Sedan_02\data\glass_i_damage.rvmat"
								}
							},
							
							{
								0.30000001,
								
								{
									"dz\vehicles\wheeled\Sedan_02\data\glass_i_destruct.rvmat"
								}
							},
							
							{
								0,
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
	class Sedan_02_Door_2_2: Sedan_02_Door_1_1
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
									"dz\vehicles\wheeled\sedan_02\data\glass_interior.rvmat"
								}
							},
							
							{
								0.69999999,
								{}
							},
							
							{
								0.5,
								
								{
									"dz\vehicles\wheeled\Sedan_02\data\glass_i_damage.rvmat"
								}
							},
							
							{
								0.30000001,
								
								{
									"dz\vehicles\wheeled\Sedan_02\data\glass_i_destruct.rvmat"
								}
							},
							
							{
								0,
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
	class Sedan_02_Hood: CarDoor
	{
		defaultSkin="Yellow";
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
	class Sedan_02_Trunk: CarDoor
	{
		defaultSkin="Yellow";
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
	class Sedan_02_Red: Sedan_02
	{
		skinBase="Sedan_02";
		skinName="Red";
	};
	class Vehicle_Sedan_02_Red: Vehicle_Sedan_02
	{
		skinBase="Vehicle_Sedan_02";
		skinName="Red";
	};
	class Sedan_02_Door_1_1_Red: Sedan_02_Door_1_1
	{
		skinBase="Sedan_02_Door_1_1";
		skinName="Red";
	};
	class Sedan_02_Door_1_2_Red: Sedan_02_Door_1_2
	{
		skinBase="Sedan_02_Door_1_2";
		skinName="Red";
	};
	class Sedan_02_Door_2_1_Red: Sedan_02_Door_2_1
	{
		skinBase="Sedan_02_Door_2_1";
		skinName="Red";
	};
	class Sedan_02_Door_2_2_Red: Sedan_02_Door_2_2
	{
		skinBase="Sedan_02_Door_2_2";
		skinName="Red";
	};
	class Sedan_02_Hood_Red: Sedan_02_Hood
	{
		skinBase="Sedan_02_Hood";
		skinName="Red";
	};
	class Sedan_02_Trunk_Red: Sedan_02_Trunk
	{
		skinBase="Sedan_02_Trunk";
		skinName="Red";
	};
	class Sedan_02_Yellow: Sedan_02
	{
		skinBase="Sedan_02";
		skinName="Yellow";
	};
	class Vehicle_Sedan_02_Yellow: Vehicle_Sedan_02
	{
		skinBase="Vehicle_Sedan_02";
		skinName="Yellow";
	};
	class Sedan_02_Door_1_1_Yellow: Sedan_02_Door_1_1
	{
		skinBase="Sedan_02_Door_1_1";
		skinName="Yellow";
	};
	class Sedan_02_Door_1_2_Yellow: Sedan_02_Door_1_2
	{
		skinBase="Sedan_02_Door_1_2";
		skinName="Yellow";
	};
	class Sedan_02_Door_2_1_Yellow: Sedan_02_Door_2_1
	{
		skinBase="Sedan_02_Door_2_1";
		skinName="Yellow";
	};
	class Sedan_02_Door_2_2_Yellow: Sedan_02_Door_2_2
	{
		skinBase="Sedan_02_Door_2_2";
		skinName="Yellow";
	};
	class Sedan_02_Hood_Yellow: Sedan_02_Hood
	{
		skinBase="Sedan_02_Hood";
		skinName="Yellow";
	};
	class Sedan_02_Trunk_Yellow: Sedan_02_Trunk
	{
		skinBase="Sedan_02_Trunk";
		skinName="Yellow";
	};
	class Sedan_02_Grey: Sedan_02
	{
		skinBase="Sedan_02";
		skinName="Grey";
	};
	class Vehicle_Sedan_02_Grey: Vehicle_Sedan_02
	{
		skinBase="Vehicle_Sedan_02";
		skinName="Grey";
	};
	class Sedan_02_Door_1_1_Grey: Sedan_02_Door_1_1
	{
		skinBase="Sedan_02_Door_1_1";
		skinName="Grey";
	};
	class Sedan_02_Door_1_2_Grey: Sedan_02_Door_1_2
	{
		skinBase="Sedan_02_Door_1_2";
		skinName="Grey";
	};
	class Sedan_02_Door_2_1_Grey: Sedan_02_Door_2_1
	{
		skinBase="Sedan_02_Door_2_1";
		skinName="Grey";
	};
	class Sedan_02_Door_2_2_Grey: Sedan_02_Door_2_2
	{
		skinBase="Sedan_02_Door_2_2";
		skinName="Grey";
	};
	class Sedan_02_Hood_Grey: Sedan_02_Hood
	{
		skinBase="Sedan_02_Hood";
		skinName="Grey";
	};
	class Sedan_02_Trunk_Grey: Sedan_02_Trunk
	{
		skinBase="Sedan_02_Trunk";
		skinName="Grey";
	};
	class Sedan_02_YellowBright: Sedan_02
	{
		skinBase="Sedan_02";
		skinName="YellowBright";
	};
	class Vehicle_Sedan_02_YellowBright: Vehicle_Sedan_02
	{
		skinBase="Vehicle_Sedan_02";
		skinName="YellowBright";
	};
	class Sedan_02_Door_1_1_YellowBright: Sedan_02_Door_1_1
	{
		skinBase="Sedan_02_Door_1_1";
		skinName="YellowBright";
	};
	class Sedan_02_Door_1_2_YellowBright: Sedan_02_Door_1_2
	{
		skinBase="Sedan_02_Door_1_2";
		skinName="YellowBright";
	};
	class Sedan_02_Door_2_1_YellowBright: Sedan_02_Door_2_1
	{
		skinBase="Sedan_02_Door_2_1";
		skinName="YellowBright";
	};
	class Sedan_02_Door_2_2_YellowBright: Sedan_02_Door_2_2
	{
		skinBase="Sedan_02_Door_2_2";
		skinName="YellowBright";
	};
	class Sedan_02_Hood_YellowBright: Sedan_02_Hood
	{
		skinBase="Sedan_02_Hood";
		skinName="YellowBright";
	};
	class Sedan_02_Trunk_YellowBright: Sedan_02_Trunk
	{
		skinBase="Sedan_02_Trunk";
		skinName="YellowBright";
	};
	class Sedan_02_Blue: Sedan_02
	{
		skinBase="Sedan_02";
		skinName="Blue";
	};
	class Vehicle_Sedan_02_Blue: Vehicle_Sedan_02
	{
		skinBase="Vehicle_Sedan_02";
		skinName="Blue";
	};
	class Sedan_02_Door_1_1_Blue: Sedan_02_Door_1_1
	{
		skinBase="Sedan_02_Door_1_1";
		skinName="Blue";
	};
	class Sedan_02_Door_1_2_Blue: Sedan_02_Door_1_2
	{
		skinBase="Sedan_02_Door_1_2";
		skinName="Blue";
	};
	class Sedan_02_Door_2_1_Blue: Sedan_02_Door_2_1
	{
		skinBase="Sedan_02_Door_2_1";
		skinName="Blue";
	};
	class Sedan_02_Door_2_2_Blue: Sedan_02_Door_2_2
	{
		skinBase="Sedan_02_Door_2_2";
		skinName="Blue";
	};
	class Sedan_02_Hood_Blue: Sedan_02_Hood
	{
		skinBase="Sedan_02_Hood";
		skinName="Blue";
	};
	class Sedan_02_Trunk_Blue: Sedan_02_Trunk
	{
		skinBase="Sedan_02_Trunk";
		skinName="Blue";
	};
	class Sedan_02_RedRust: Sedan_02
	{
		skinBase="Sedan_02";
		skinName="RedRust";
	};
	class Vehicle_Sedan_02_RedRust: Vehicle_Sedan_02
	{
		skinBase="Vehicle_Sedan_02";
		skinName="RedRust";
	};
	class Sedan_02_Door_1_1_RedRust: Sedan_02_Door_1_1
	{
		skinBase="Sedan_02_Door_1_1";
		skinName="RedRust";
	};
	class Sedan_02_Door_1_2_RedRust: Sedan_02_Door_1_2
	{
		skinBase="Sedan_02_Door_1_2";
		skinName="RedRust";
	};
	class Sedan_02_Door_2_1_RedRust: Sedan_02_Door_2_1
	{
		skinBase="Sedan_02_Door_2_1";
		skinName="RedRust";
	};
	class Sedan_02_Door_2_2_RedRust: Sedan_02_Door_2_2
	{
		skinBase="Sedan_02_Door_2_2";
		skinName="RedRust";
	};
	class Sedan_02_Hood_RedRust: Sedan_02_Hood
	{
		skinBase="Sedan_02_Hood";
		skinName="RedRust";
	};
	class Sedan_02_Trunk_RedRust: Sedan_02_Trunk
	{
		skinBase="Sedan_02_Trunk";
		skinName="RedRust";
	};
	class Sedan_02_YellowRust: Sedan_02
	{
		skinBase="Sedan_02";
		skinName="YellowRust";
	};
	class Vehicle_Sedan_02_YellowRust: Vehicle_Sedan_02
	{
		skinBase="Vehicle_Sedan_02";
		skinName="YellowRust";
	};
	class Sedan_02_Door_1_1_YellowRust: Sedan_02_Door_1_1
	{
		skinBase="Sedan_02_Door_1_1";
		skinName="YellowRust";
	};
	class Sedan_02_Door_1_2_YellowRust: Sedan_02_Door_1_2
	{
		skinBase="Sedan_02_Door_1_2";
		skinName="YellowRust";
	};
	class Sedan_02_Door_2_1_YellowRust: Sedan_02_Door_2_1
	{
		skinBase="Sedan_02_Door_2_1";
		skinName="YellowRust";
	};
	class Sedan_02_Door_2_2_YellowRust: Sedan_02_Door_2_2
	{
		skinBase="Sedan_02_Door_2_2";
		skinName="YellowRust";
	};
	class Sedan_02_Hood_YellowRust: Sedan_02_Hood
	{
		skinBase="Sedan_02_Hood";
		skinName="YellowRust";
	};
	class Sedan_02_Trunk_YellowRust: Sedan_02_Trunk
	{
		skinBase="Sedan_02_Trunk";
		skinName="YellowRust";
	};
	class Sedan_02_GreyRust: Sedan_02
	{
		skinBase="Sedan_02";
		skinName="GreyRust";
	};
	class Vehicle_Sedan_02_GreyRust: Vehicle_Sedan_02
	{
		skinBase="Vehicle_Sedan_02";
		skinName="GreyRust";
	};
	class Sedan_02_Door_1_1_GreyRust: Sedan_02_Door_1_1
	{
		skinBase="Sedan_02_Door_1_1";
		skinName="GreyRust";
	};
	class Sedan_02_Door_1_2_GreyRust: Sedan_02_Door_1_2
	{
		skinBase="Sedan_02_Door_1_2";
		skinName="GreyRust";
	};
	class Sedan_02_Door_2_1_GreyRust: Sedan_02_Door_2_1
	{
		skinBase="Sedan_02_Door_2_1";
		skinName="GreyRust";
	};
	class Sedan_02_Door_2_2_GreyRust: Sedan_02_Door_2_2
	{
		skinBase="Sedan_02_Door_2_2";
		skinName="GreyRust";
	};
	class Sedan_02_Hood_GreyRust: Sedan_02_Hood
	{
		skinBase="Sedan_02_Hood";
		skinName="GreyRust";
	};
	class Sedan_02_Trunk_GreyRust: Sedan_02_Trunk
	{
		skinBase="Sedan_02_Trunk";
		skinName="GreyRust";
	};
	class Sedan_02_YellowBrightRust: Sedan_02
	{
		skinBase="Sedan_02";
		skinName="YellowBrightRust";
	};
	class Vehicle_Sedan_02_YellowBrightRust: Vehicle_Sedan_02
	{
		skinBase="Vehicle_Sedan_02";
		skinName="YellowBrightRust";
	};
	class Sedan_02_Door_1_1_YellowBrightRust: Sedan_02_Door_1_1
	{
		skinBase="Sedan_02_Door_1_1";
		skinName="YellowBrightRust";
	};
	class Sedan_02_Door_1_2_YellowBrightRust: Sedan_02_Door_1_2
	{
		skinBase="Sedan_02_Door_1_2";
		skinName="YellowBrightRust";
	};
	class Sedan_02_Door_2_1_YellowBrightRust: Sedan_02_Door_2_1
	{
		skinBase="Sedan_02_Door_2_1";
		skinName="YellowBrightRust";
	};
	class Sedan_02_Door_2_2_YellowBrightRust: Sedan_02_Door_2_2
	{
		skinBase="Sedan_02_Door_2_2";
		skinName="YellowBrightRust";
	};
	class Sedan_02_Hood_YellowBrightRust: Sedan_02_Hood
	{
		skinBase="Sedan_02_Hood";
		skinName="YellowBrightRust";
	};
	class Sedan_02_Trunk_YellowBrightRust: Sedan_02_Trunk
	{
		skinBase="Sedan_02_Trunk";
		skinName="YellowBrightRust";
	};
	class Sedan_02_BlueRust: Sedan_02
	{
		skinBase="Sedan_02";
		skinName="BlueRust";
	};
	class Vehicle_Sedan_02_BlueRust: Vehicle_Sedan_02
	{
		skinBase="Vehicle_Sedan_02";
		skinName="BlueRust";
	};
	class Sedan_02_Door_1_1_BlueRust: Sedan_02_Door_1_1
	{
		skinBase="Sedan_02_Door_1_1";
		skinName="BlueRust";
	};
	class Sedan_02_Door_1_2_BlueRust: Sedan_02_Door_1_2
	{
		skinBase="Sedan_02_Door_1_2";
		skinName="BlueRust";
	};
	class Sedan_02_Door_2_1_BlueRust: Sedan_02_Door_2_1
	{
		skinBase="Sedan_02_Door_2_1";
		skinName="BlueRust";
	};
	class Sedan_02_Door_2_2_BlueRust: Sedan_02_Door_2_2
	{
		skinBase="Sedan_02_Door_2_2";
		skinName="BlueRust";
	};
	class Sedan_02_Hood_BlueRust: Sedan_02_Hood
	{
		skinBase="Sedan_02_Hood";
		skinName="BlueRust";
	};
	class Sedan_02_Trunk_BlueRust: Sedan_02_Trunk
	{
		skinBase="Sedan_02_Trunk";
		skinName="BlueRust";
	};
};
