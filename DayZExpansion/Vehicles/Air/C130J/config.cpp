class CfgPatches
{
	class DayZExpansion_Vehicles_Air_C130J
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
	class Slot_expansion_c130jwheel_1
	{
		name="expansion_c130jwheel_1";
		displayName="$STR_EXPANSION_VEHICLE_C130J_WHEEL";
		descriptionShort="$STR_EXPANSION_VEHICLE_C130J_WHEEL_DESC";
		selection="wheel_1_1";
		ghostIcon="wheel";
	};
	class Slot_expansion_c130jwheel_1_2
	{
		name="expansion_c130jwheel_1_2";
		displayName="$STR_EXPANSION_VEHICLE_C130J_WHEEL";
		descriptionShort="$STR_EXPANSION_VEHICLE_C130J_WHEEL_DESC";
		selection="wheel_1_2";
		ghostIcon="wheel";
	};
	class Slot_expansion_c130jwheel_2_2
	{
		name="expansion_c130jwheel_2_2";
		displayName="$STR_EXPANSION_VEHICLE_C130J_WHEEL";
		descriptionShort="$STR_EXPANSION_VEHICLE_C130J_WHEEL_DESC";
		selection="wheel_2_2";
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
	class Vehicle_ExpansionC130JWheel: ExpansionWheelBase
	{
		scope=0;
		displayName="$STR_EXPANSION_VEHICLE_C130J_WHEEL";
		descriptionShort="$STR_EXPANSION_VEHICLE_C130J_WHEEL_DESC";
		model="\DayZExpansion\Vehicles\Air\C130J\proxy\C130JWheelMain.p3d";
		weight=15000;
		inventorySlot[]=
		{
			"expansion_c130jwheel_1",
			"expansion_c130jwheel_1_2",
			"expansion_c130jwheel_2_2"
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
	class Vehicle_ExpansionC130J: ExpansionVehiclePlaneBase
	{
		scope=0;
		displayname="$STR_EXPANSION_VEHICLE_C130J";
		model="DayZExpansion\Vehicles\Air\C130J\C130J.p3d";
		modelZeroPointDistanceFromGround=5.6700001;
		vehicleClass="Expansion_Plane";
		fuelCapacity=192;
		fuelConsumption=21;
		animPhysDetachSpeed=100;
		attachments[]=
		{
			"ExpansionAircraftBattery",
			"Reflector_1_1",
			"Reflector_2_1",
			"CarRadiator",
			"GlowPlug",
			"expansion_c130jwheel_1",
			"expansion_c130jwheel_1_2",
			"expansion_c130jwheel_2_2"
		};
		hiddenSelections[]={};
		hiddenSelectionsTextures[]={};
		hiddenSelectionsMaterials[]={};
		class SimulationModule: SimulationModule
		{
			drive="";
			airDragFrontTotal=0.98000002;
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
			braking[]={0,0.1,1,0.80000001,2.5,0.89999998,3,1};
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
					maxSteeringAngle=25;
					brakeBias=0.69999999;
					brakeForce=1000;
					wheelHubMass=10;
					wheelHubRadius=0.15000001;
					class Suspension
					{
						swayBar=300;
						stiffness=40000;
						compression=2100;
						damping=6500;
						travelMaxUp=0.30000001;
						travelMaxDown=0.1;
					};
					class Wheels: Wheels
					{
						class Center
						{
							inventorySlot="expansion_c130jwheel_1";
							animTurn="turnfront";
							animRotation="wheelfront";
							animDamper="damper_1";
							wheelHub="wheel_1_damper_land";
						};
					};
				};
				class Rear: Rear
				{
					maxSteeringAngle=0;
					brakeBias=0.30000001;
					brakeForce=1000;
					finalRatio=4.0999999;
					wheelHubMass=10;
					wheelHubRadius=0.15000001;
					class Suspension
					{
						swayBar=300;
						stiffness=40000;
						compression=2100;
						damping=6500;
						travelMaxUp=0.30000001;
						travelMaxDown=0.1;
					};
					class Wheels: Wheels
					{
						class Left: Left
						{
							inventorySlot="expansion_c130jwheel_1_2";
							animTurn="turnbackleft";
							animRotation="wheelbackleft";
							animDamper="damper_1_2";
							wheelHub="wheel_1_2_damper_land";
						};
						class Right: Right
						{
							inventorySlot="expansion_c130jwheel_2_2";
							animTurn="turnbackright";
							animRotation="wheelbackright";
							animDamper="damper_2_2";
							wheelHub="wheel_2_2_damper_land";
						};
					};
				};
			};
			class Engines
			{
				class Engine_0
				{
					type="POINTS";
					points[]={1670,410,1910,615,2030,738,2100,820,2200,1000};
					inertia=1.16;
					rpmMin=1200;
					rpmRedline=2200;
					rpmMax=2300;
					gear=-1;
				};
				class Engine_1: Engine_0
				{
				};
				class Engine_2: Engine_0
				{
				};
				class Engine_3: Engine_0
				{
				};
			};
			class Props: Props
			{
				class Rotor_0
				{
					engine=0;
					start="engine_1_end";
					end="engine_1_start";
				};
				class Rotor_1: Rotor_0
				{
					engine=1;
					start="engine_2_start";
					end="engine_2_end";
				};
				class Rotor_2: Rotor_0
				{
					engine=2;
					start="engine_3_start";
					end="engine_3_end";
				};
				class Rotor_3: Rotor_0
				{
					engine=3;
					start="engine_4_start";
					end="engine_4_end";
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
					maxControlAngle=4;
					stallAngle=16;
				};
				class WingRight
				{
					type="wing";
					up="0 1 0";
					min="af_wing_right_min";
					max="af_wing_right_max";
					camber=3;
					maxControlAngle=4;
					stallAngle=16;
				};
				class ElevatorLeft
				{
					type="elevator";
					up="0 1 0";
					min="af_elevator_left_min";
					max="af_elevator_left_max";
					camber=0.5;
					maxControlAngle=5;
					stallAngle=16;
				};
				class ElevatorRight
				{
					type="elevator";
					up="0 1 0";
					min="af_elevator_right_min";
					max="af_elevator_right_max";
					camber=0.5;
					maxControlAngle=5;
					stallAngle=16;
				};
				class Rudder
				{
					type="rudder";
					up="1 0 0";
					min="af_rudder_min";
					max="af_rudder_max";
					camber=0;
					maxControlAngle=2;
					stallAngle=16;
				};
			};
		};
		class Crew: Crew
		{
			class Driver: Driver
			{
				actionSel="seat_driver";
				proxyPos="crewDriver";
				getInPos="pos driver";
				getInDir="pos driver dir";
			};
			class CoDriver: CoDriver
			{
				actionSel="seat_codriver";
				proxyPos="crewCoDriver";
				getInPos="pos cargo";
				getInDir="pos cargo dir";
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
					"expansion_c130jwheel_1",
					"expansion_c130jwheel_1_2",
					"expansion_c130jwheel_2_2"
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
		class AnimationSources: AnimationSources
		{
			class rotor
			{
				source="user";
				animPeriod=0.00099999993;
				initPhase=0;
			};
			class hiderotor
			{
				source="user";
				animPeriod=0.00099999993;
				initPhase=0;
			};
			class hiderotorblur
			{
				source="user";
				animPeriod=0.00099999993;
				initPhase=0;
			};
			class aileron
			{
				source="user";
				animPeriod=0.025;
				initPhase=0;
			};
			class elevator
			{
				source="user";
				animPeriod=0.025;
				initPhase=0;
			};
			class rudder
			{
				source="user";
				animPeriod=0.025;
				initPhase=0;
			};
			class flap
			{
				source="user";
				animPeriod=0.025;
				initPhase=0;
			};
		};
	};
	class ExpansionSpraycanBase;
	class ExpansionSpraycanGrey: ExpansionSpraycanBase
	{
		scope=2;
		displayName="$STR_EXPANSION_SPRAYCAN_Grey_NAME";
		descriptionShort="$STR_EXPANSION_SPRAYCAN_Grey_DESC";
		hiddenSelectionsTextures[]=
		{
			"\DayZExpansion\Objects\Gear\Spraycans\data\spraycan_grey_co.paa"
		};
		skinName="Grey";
	};
	class Vehicle_ExpansionC130J_Gray: Vehicle_ExpansionC130J
	{
		skinBase="Vehicle_ExpansionC130J";
		skinName="Gray";
	};
};
class CfgNonAIVehicles
{
	class ProxyVehiclePart;
	class ProxyC130JWheelMain: ProxyVehiclePart
	{
		model="\DayZExpansion\Vehicles\Air\C130J\proxy\C130JWheelMain.p3d";
		inventorySlot[]=
		{
			"expansion_c130jwheel_1",
			"expansion_c130jwheel_1_2",
			"expansion_c130jwheel_2_2"
		};
	};
};
