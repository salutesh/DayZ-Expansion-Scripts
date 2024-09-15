class CfgPatches
{
	class DayZExpansion_Vehicles_Air_An2
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
class CfgSlots
{
	class Slot_expansion_anwheel_1_1
	{
		name="expansion_anwheel_1_1";
		displayName="$STR_EXPANSION_VEHICLE_AN2_WHEEL_FRONT";
		descriptionShort="$STR_EXPANSION_VEHICLE_AN2_WHEEL_FRONT_DESC";
		selection="wheel_1_1";
		ghostIcon="wheel";
	};
	class Slot_expansion_anwheel_2_1
	{
		name="expansion_anwheel_2_1";
		displayName="$STR_EXPANSION_VEHICLE_AN2_WHEEL_FRONT";
		descriptionShort="$STR_EXPANSION_VEHICLE_AN2_WHEEL_FRONT_DESC";
		selection="wheel_2_1";
		ghostIcon="wheel";
	};
	class Slot_expansion_ansmallwheel_2
	{
		name="expansion_ansmallwheel_2";
		displayName="$STR_EXPANSION_VEHICLE_AN2_WHEEL_BACK";
		descriptionShort="$STR_EXPANSION_VEHICLE_AN2_WHEEL_BACK_DESC";
		selection="wheel_2";
		ghostIcon="wheel";
	};
};
class CfgVehicles
{
	class SimulationModule;
	class Axles;
	class Wheels;
	class Engines;
	class Engine;
	class Props;
	class Prop;
	class Front;
	class Rear;
	class Left;
	class Right;
	class AnimationSources;
	class Crew;
	class Driver;
	class CoDriver;
	class ExpansionVehiclePlaneBase;
	class ExpansionWheelBase;
	class Vehicle_ExpansionAnWheel: ExpansionWheelBase
	{
		scope=0;
		displayName="$STR_EXPANSION_VEHICLE_AN2_WHEEL_FRONT";
		descriptionShort="$STR_EXPANSION_VEHICLE_AN2_WHEEL_FRONT_DESC";
		model="\DayZExpansion\Vehicles\Air\An2\proxy\AnWheelMain.p3d";
		weight=15000;
		inventorySlot[]=
		{
			"expansion_anwheel_1_1",
			"expansion_anwheel_2_1"
		};
		rotationFlags=4;
		physLayer="item_large";
		radiusByDamage[]={0,0.34299999,0.30000001,0.40000001,0.99980003,0.25,0.99989998,0.2};
		radius=0.53600001;
		width=0.31600001;
		tyreRollResistance=0.015;
		tyreTread=0.69999999;
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
								"DZ\vehicles\wheeled\OffroadHatchback\data\niva_wheel.rvmat"
							}
						},
						
						{
							0.69999999,
							
							{
								"DZ\vehicles\wheeled\OffroadHatchback\data\niva_wheel.rvmat"
							}
						},
						
						{
							0.5,
							
							{
								"DZ\vehicles\wheeled\OffroadHatchback\data\niva_wheel_damage.rvmat"
							}
						},
						
						{
							0.30000001,
							
							{
								"DZ\vehicles\wheeled\OffroadHatchback\data\niva_wheel_damage.rvmat"
							}
						},
						
						{
							0,
							
							{
								"DZ\vehicles\wheeled\OffroadHatchback\data\niva_wheel_damage.rvmat"
							}
						}
					};
				};
			};
		};
	};
	class Vehicle_ExpansionAnWheelSmall: Vehicle_ExpansionAnWheel
	{
		scope=0;
		displayName="$STR_EXPANSION_VEHICLE_AN2_WHEEL_BACK";
		descriptionShort="$STR_EXPANSION_VEHICLE_AN2_WHEEL_BACK_DESC";
		model="\DayZExpansion\Vehicles\Air\An2\proxy\AnWheelSmall.p3d";
		weight=15000;
		inventorySlot[]=
		{
			"expansion_ansmallwheel_2"
		};
		rotationFlags=4;
		physLayer="item_large";
		radiusByDamage[]={0,0.34299999,0.30000001,0.40000001,0.99980003,0.25,0.99989998,0.2};
		radius=0.336;
		width=0.31600001;
		tyreRollResistance=0.015;
		tyreTread=0.69999999;
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
								"DZ\vehicles\wheeled\OffroadHatchback\data\niva_wheel.rvmat"
							}
						},
						
						{
							0.69999999,
							
							{
								"DZ\vehicles\wheeled\OffroadHatchback\data\niva_wheel.rvmat"
							}
						},
						
						{
							0.5,
							
							{
								"DZ\vehicles\wheeled\OffroadHatchback\data\niva_wheel_damage.rvmat"
							}
						},
						
						{
							0.30000001,
							
							{
								"DZ\vehicles\wheeled\OffroadHatchback\data\niva_wheel_damage.rvmat"
							}
						},
						
						{
							0,
							
							{
								"DZ\vehicles\wheeled\OffroadHatchback\data\niva_wheel_damage.rvmat"
							}
						}
					};
				};
			};
		};
	};
	class Vehicle_ExpansionAn2: ExpansionVehiclePlaneBase
	{
		scope=0;
		displayname="$STR_EXPANSION_VEHICLE_AN2";
		model="DayZExpansion\Vehicles\Air\An2\an2.p3d";
		modelZeroPointDistanceFromGround=0;
		vehicleClass="Expansion_Plane";
		fuelCapacity=192;
		fuelConsumption=12;
		animPhysDetachSpeed=100;
		attachments[]=
		{
			"ExpansionAircraftBattery",
			"Reflector_1_1",
			"Reflector_2_1",
			"CarRadiator",
			"GlowPlug",
			"expansion_anwheel_1_1",
			"expansion_anwheel_2_1",
			"expansion_ansmallwheel_2"
		};
		hiddenSelections[]={};
		hiddenSelectionsTextures[]={};
		hiddenSelectionsMaterials[]={};
		class SimulationModule: SimulationModule
		{
			drive="DRIVE_RWD";
			airDragFrontTotal=1e-06;
			airDragSizeMin="drag_max";
			airDragSizeMax="drag_min";
			class Steering
			{
				increaseSpeed[]={0,50,30,40,60,25,120,5};
				decreaseSpeed[]={0,50,60,30,120,10};
				centeringSpeed[]={0,0,15,25,60,40,100,60};
			};
			class Throttle
			{
				reactionTime=0.25;
				defaultThrust=0.80000001;
				gentleThrust=0.60000002;
				turboCoef=5;
				gentleCoef=0.69999999;
			};
			class Gearbox
			{
				reverse=1;
				ratios[]={1};
				timeToUncoupleClutch=0.30000001;
				timeToCoupleClutch=0.30000001;
				maxClutchTorque=180;
			};
			class Axles: Axles
			{
				class Front: Front
				{
					engine=-1;
					maxSteeringAngle=0;
					brakeBias=0.69999999;
					brakeForce=1000;
					wheelHubMass=10;
					wheelHubRadius=0.15000001;
					class Suspension
					{
						swayBar=300;
						stiffness=60000;
						compression=2100;
						damping=5500;
						travelMaxUp=0;
						travelMaxDown=1.1;
					};
					class Wheels: Wheels
					{
						class Left: Left
						{
							inventorySlot="expansion_anwheel_1_1";
							animTurn="turnfrontleft";
							animRotation="wheelfrontleft";
							animDamper="damper_1_1";
							wheelHub="wheel_1_1_damper_land";
						};
						class Right: Right
						{
							inventorySlot="expansion_anwheel_2_1";
							animTurn="turnfrontright";
							animRotation="wheelfrontright";
							animDamper="damper_2_1";
							wheelHub="wheel_2_1_damper_land";
						};
					};
				};
				class Rear: Rear
				{
					engine=-1;
					maxSteeringAngle=-45;
					brakeBias=0.30000001;
					brakeForce=1000;
					wheelHubMass=10;
					wheelHubRadius=0.15000001;
					class Suspension
					{
						swayBar=275;
						stiffness=34000;
						compression=2000;
						damping=5100;
						travelMaxUp=0;
						travelMaxDown=0.40000001;
					};
					class Wheels: Wheels
					{
						class Center
						{
							inventorySlot="expansion_ansmallwheel_2";
							animTurn="turnbackright";
							animRotation="wheelbackright";
							animDamper="damper_2";
							wheelHub="wheel_2_damper_land";
						};
					};
				};
			};
			class Engines: Engines
			{
				class Engine: Engine
				{
					type="POINTS";
					points[]={1670,410,1910,615,2030,738,2100,820,2200,1000};
					inertia=1;
					rpmMin=200;
					rpmRedline=2200;
					rpmMax=2300;
					gear=-1;
				};
			};
			class Props: Props
			{
				class Rotor
				{
					engine=0;
					start="engine_1_end";
					end="engine_1_start";
					type="thruster";
					animation="rotor";
					hiderotor="hiderotor";
					hiderotorblur="hiderotorblur";
				};
			};
			class Aerofoils
			{
				class WingLeft
				{
					type="wing";
					up="0 1 0";
					min="af_wing_left_min";
					max="af_wing_left_max";
					camber=3;
					maxControlAngle=10;
					stallAngle=16;
					animation="aileronr";
				};
				class WingRight
				{
					type="wing";
					up="0 1 0";
					min="af_wing_right_min";
					max="af_wing_right_max";
					camber=3;
					maxControlAngle=10;
					stallAngle=16;
					animation="aileronr";
				};
				class ElevatorLeft
				{
					type="elevator";
					up="0 1 0";
					min="af_elevator_left_min";
					max="af_elevator_left_max";
					camber=3;
					maxControlAngle=10;
					stallAngle=16;
					animation="elevatorl";
				};
				class ElevatorRight
				{
					type="elevator";
					up="0 1 0";
					min="af_elevator_right_min";
					max="af_elevator_right_max";
					camber=3;
					maxControlAngle=10;
					stallAngle=16;
					animation="elevatorr";
				};
				class Rudder
				{
					type="rudder";
					up="1 0 0";
					min="af_rudder_min";
					max="af_rudder_max";
					camber=0;
					maxControlAngle=5;
					stallAngle=16;
					animation="rudder";
				};
			};
		};
		class Crew: Crew
		{
			class Driver: Driver
			{
				actionSel="seat_driver";
				proxyPos="crewDriver";
				getInPos="pos_driver";
				getInDir="pos_driver_dir";
			};
			class CoDriver: CoDriver
			{
				actionSel="seat_codriver";
				proxyPos="crewCoDriver";
				getInPos="pos_codriver";
				getInDir="pos_codriver_dir";
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
			class Cargo3
			{
				actionSel="seat_cargo2";
				proxyPos="crewCargo2";
				getInPos="pos_cargo2";
				getInDir="pos_cargo2_dir";
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
					"ExpansionAircraftBattery",
					"GlowPlug"
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
					"Reflector_2_1"
				};
			};
			class Chassis
			{
				name="$STR_attachment_Chassis0";
				description="";
				icon="cat_vehicle_chassis";
				attachmentSlots[]=
				{
					"expansion_anwheel_1_1",
					"expansion_anwheel_2_1",
					"expansion_ansmallwheel_2"
				};
			};
		};
		class ExpansionSounds
		{
			thrust=0.60000002;
			thrustTurbo=1;
			thrustGentle=0.30000001;
			thrustSmoothCoef=0.1;
			camposSmoothCoef=0.029999999;
			soundSetsFilter[]=
			{
				"Expansion_An2_Engine_Ext_SoundSet"
			};
			soundSetsInt[]=
			{
				"Expansion_An2_Engine_Int_SoundSet"
			};
		};
		class AnimationSources: AnimationSources
		{
			class damper_1_1
			{
				source="user";
				initPhase=0.48570001;
				animPeriod=1e-06;
			};
			class damper_2_1: damper_1_1
			{
			};
			class rotor
			{
				source="user";
				animPeriod=0.001;
				initPhase=0;
			};
			class hiderotor
			{
				source="user";
				animPeriod=0.001;
				initPhase=0;
			};
			class hiderotorblur: hiderotor
			{
			};
			class aileronl
			{
				source="user";
				animPeriod=0.1;
				initPhase=0;
			};
			class aileronr
			{
				source="user";
				animPeriod=0.1;
				initPhase=0;
			};
			class flapl
			{
				source="user";
				animPeriod=0.1;
				initPhase=0;
			};
			class flapr
			{
				source="user";
				animPeriod=0.1;
				initPhase=0;
			};
			class elevatorl
			{
				source="user";
				animPeriod=0.1;
				initPhase=0;
			};
			class elevatorr
			{
				source="user";
				animPeriod=0.1;
				initPhase=0;
			};
			class rudder
			{
				source="user";
				animPeriod=0.1;
				initPhase=0;
			};
		};
		class ObstacleGenerator
		{
			carve=1;
			timeToStationary=9.9999997e-06;
			moveThreshold=9.9999997e-06;
			class Shapes
			{
				class Cylindric
				{
					class Cyl1
					{
						radius=1.5;
						height=2.5;
						center[]={0,0,3};
					};
					class Cyl2
					{
						radius=1.5;
						height=2.5;
						center[]={0,0,1.5};
					};
					class Cyl3
					{
						radius=1.5;
						height=2.5;
						center[]={0,0,0};
					};
					class Cyl4
					{
						radius=1.5;
						height=2.5;
						center[]={0,0,-1.5};
					};
					class Cyl5
					{
						radius=1.5;
						height=2.5;
						center[]={0,0,-3};
					};
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
									"dz\vehicles\wheeled\offroadhatchback\data\engine_niva.rvmat"
								}
							},
							
							{
								0.69999999,
								
								{
									"dz\vehicles\wheeled\offroadhatchback\data\engine_niva.rvmat"
								}
							},
							
							{
								0.5,
								
								{
									"dz\vehicles\wheeled\offroadhatchback\data\engine_niva_destruct.rvmat"
								}
							},
							
							{
								0.30000001,
								
								{
									"dz\vehicles\wheeled\offroadhatchback\data\engine_niva_destruct.rvmat"
								}
							},
							
							{
								0,
								
								{
									"dz\vehicles\wheeled\offroadhatchback\data\engine_niva_destruct.rvmat"
								}
							}
						};
					};
					inventorySlots[]=
					{
						"ExpansionAircraftBattery",
						"GlowPlug",
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
	};
	class ExpansionSpraycanBase;
	class ExpansionSpraycanGreen: ExpansionSpraycanBase
	{
		scope=2;
		displayName="$STR_EXPANSION_SPRAYCAN_Green_NAME";
		descriptionShort="$STR_EXPANSION_SPRAYCAN_Green_DESC";
		hiddenSelectionsTextures[]=
		{
			"DZ\gear\consumables\data\spraycan_black_co.paa"
		};
		skinName="Green";
	};
	class Vehicle_ExpansionAn2_Green: Vehicle_ExpansionAn2
	{
		skinBase="Vehicle_ExpansionAn2";
		skinName="Green";
	};
	class ExpansionSpraycanWhite: ExpansionSpraycanBase
	{
		scope=2;
		displayName="$STR_EXPANSION_SPRAYCAN_White_NAME";
		descriptionShort="$STR_EXPANSION_SPRAYCAN_White_DESC";
		hiddenSelectionsTextures[]=
		{
			"\DayZExpansion\Objects\Gear\Spraycans\data\spraycan_white_co.paa"
		};
		skinName="White";
	};
	class Vehicle_ExpansionAn2_White: Vehicle_ExpansionAn2
	{
		skinBase="Vehicle_ExpansionAn2";
		skinName="White";
	};
	class ExpansionSpraycanOrange: ExpansionSpraycanBase
	{
		scope=2;
		displayName="$STR_EXPANSION_SPRAYCAN_Orange_NAME";
		descriptionShort="$STR_EXPANSION_SPRAYCAN_Orange_DESC";
		hiddenSelectionsTextures[]=
		{
			"\DayZExpansion\Objects\Gear\Spraycans\data\spraycan_Orange_co.paa"
		};
		skinName="Orange";
	};
	class Vehicle_ExpansionAn2_Orange: Vehicle_ExpansionAn2
	{
		skinBase="Vehicle_ExpansionAn2";
		skinName="Orange";
	};
};
class CfgNonAIVehicles
{
	class ProxyVehiclePart;
	class ProxyAnWheelMain: ProxyVehiclePart
	{
		model="\DayZExpansion\Vehicles\Air\An2\proxy\AnWheelMain.p3d";
		inventorySlot[]=
		{
			"expansion_anwheel_1_1",
			"expansion_anwheel_2_1"
		};
	};
	class ProxyAnWheelSmall: ProxyVehiclePart
	{
		model="\DayZExpansion\Vehicles\Air\An2\proxy\AnWheelSmall.p3d";
		inventorySlot[]=
		{
			"expansion_ansmallwheel_2"
		};
	};
};
