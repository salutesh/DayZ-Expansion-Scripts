class CfgPatches
{
	class DayZExpansion_NamalskAdventure_Objects_Effects
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
	class Expansion_EffectHolder_Base: HouseNoDestruct
	{
		scope=1;
		model="\DayZExpansion\NamalskAdventure\Dta\Objects\Effects\Expansion_Effect.p3d";
		bounding="BSphere";
		forceFarBubble="true";
		itemBehaviour=2;
		allowOwnedCargoManipulation=0;
		openable=0;
		mapSize=1;
		class DamageSystem
		{
			class GlobalHealth
			{
				class Health
				{
					hitpoints=100;
					healthLevels[]={};
				};
			};
			class GlobalArmor
			{
				class Projectile
				{
					class Health
					{
						damage=0;
					};
					class Blood
					{
						damage=0;
					};
					class Shock
					{
						damage=0;
					};
				};
				class FragGrenade
				{
					class Health
					{
						damage=0;
					};
					class Blood
					{
						damage=0;
					};
					class Shock
					{
						damage=0;
					};
				};
			};
		};
	};
	class Expansion_EffectHolder_Teleport: Expansion_EffectHolder_Base
	{
		scope=2;
	};
};
