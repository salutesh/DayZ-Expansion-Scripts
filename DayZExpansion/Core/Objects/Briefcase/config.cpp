class CfgPatches
{
	class DayZExpansion_Objects_Gear_Breifcase
	{
		units[]={};
		weapons[]={};
		requiredVersion=0.1;
		requiredAddons[]=
		{
			"DZ_Data"
		};
	};
};
class CfgVehicles
{
	class HouseNoDestruct;
	class bldr_ExpansionBriefcase: HouseNoDestruct
	{
		scope=1;
		rotationFlags=17;
		model="\DayZExpansion\Core\Objects\Briefcase\briefcase.p3d";
	};
	class bldr_ExpansionBriefcase_Open: bldr_ExpansionBriefcase
	{
		scope=1;
		class AnimationSources
		{
			class upperpart_rotate
			{
				source="user";
				animPeriod=-2.5;
				initPhase=1;
			};
		};
	};
	class bldr_ExpansionBriefcase_MidOpen: bldr_ExpansionBriefcase
	{
		scope=1;
		class AnimationSources
		{
			class upperpart_rotate
			{
				source="user";
				animPeriod=-1.5;
				initPhase=0.40000001;
			};
		};
	};
	class Container_Base;
	class ExpansionBriefcase: Container_Base
	{
		scope=2;
		weight=895;
		itemSize[]={5,3};
		itemsCargoSize[]={5,3};
		rotationFlags=17;
		allowOwnedCargoManipulation=1;
		isMeleeWeapon=1;
		randomQuantity=2;
		model="\DayZExpansion\Core\Objects\Briefcase\briefcase.p3d";
		vehicleClass="Expansion_Trader";
		displayName="Briefcase";
		descriptionShort="briefcase description";
		class DamageSystem
		{
			class GlobalHealth
			{
				class Health
				{
					hitpoints=750;
					healthLevels[]=
					{
						
						{
							1,
							
							{
								""
							}
						},
						
						{
							0.69999999,
							
							{
								""
							}
						},
						
						{
							0.5,
							
							{
								""
							}
						},
						
						{
							0.30000001,
							
							{
								""
							}
						},
						
						{
							0,
							
							{
								""
							}
						}
					};
				};
			};
			class GlobalArmor
			{
				class FragGrenade
				{
					class Health
					{
						damage=8;
					};
					class Blood
					{
						damage=8;
					};
					class Shock
					{
						damage=8;
					};
				};
			};
		};
		class AnimEvents
		{
			class SoundWeapon
			{
				class pickUpItem
				{
					soundSet="smallprotectorcase_pickup_SoundSet";
					id=797;
				};
				class drop
				{
					soundset="smallprotectorcase_drop_SoundSet";
					id=898;
				};
			};
		};
	};
};
