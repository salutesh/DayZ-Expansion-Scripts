class CfgPatches
{
	class DayZExpansion_NamalskAdventure_Weapons_Projectiles
	{
		units[]={};
		weapons[]={};
		requiredVersion=0.1;
		requiredAddons[]=
		{
			"DZ_Data",
			"DZ_Weapons_Projectiles"
		};
	};
};
class CfgAmmo
{
	class Bolt_Base;
	class Expansion_Bullet_HuntingBolt_Anomaly: Bolt_Base
	{
		scope=1;
		model="\dz\weapons\projectiles\bolt_flying.p3d";
		proxyShape="\dz\weapons\projectiles\bolt_biggame.p3d";
		simulation="shotArrow";
		hit=12;
		indirectHit=0;
		indirectHitRange=0;
		casing="";
		round="FxRound_HuntingBolt";
		deflecting=25;
		dispersion=0.0015;
		initSpeed=105;
		typicalSpeed=105;
		caliber=3.0999999;
		airFriction=-0.0013;
		weight=0.029999999;
		impactBehaviour=1;
		hitAnimation=1;
		unconRefillModifier=4;
		spawnPileType="Expansion_Ammo_HuntingBolt_Anomaly";
		dmgPerUse=0.090000004;
		class DamageApplied
		{
			type="Projectile";
			dispersion=0;
			bleedThreshold=1;
			defaultDamageOverride[]=
			{
				{0.98000002,0.95999998},
				{0.96100003,0.95999998},
				{0.935,0.92500001},
				{0.90499997,0.70999998},
				{0.87,0.56}
			};
			class Health
			{
				damage=125;
			};
			class Blood
			{
				damage=100;
			};
			class Shock
			{
				damage=95;
			};
		};
	};
};
