class CfgPatches
{
	class DayZExpansion_Sounds_Weapons_Crossbow
	{
		units[]={};
		weapons[]={};
		requiredVersion=0.1;
		requiredAddons[]=
		{
			"DZ_Sounds_Effects"
		};
	};
};
class CfgSoundShaders
{
	class reloadWeapon_SoundShader;
	class ExpansionCrossbow_Charging_SoundShader: reloadWeapon_SoundShader
	{
		samples[]=
		{
			
			{
				"DayZExpansion\Sounds\Weapons\Crossbow\crossbow_drawing",
				1
			}
		};
		volume=0.79432821;
	};
	class ExpansionCrossbow_Firing_SoundShader: reloadWeapon_SoundShader
	{
		samples[]=
		{
			
			{
				"DayZExpansion\Sounds\Weapons\Crossbow\crossbow_firing1",
				1
			}
		};
		volume=0.79432821;
	};
};
class CfgSoundSets
{
	class baseCharacter_SoundSet;
	class ExpansionCrossbow_Charging_SoundSet: baseCharacter_SoundSet
	{
		soundShaders[]=
		{
			"ExpansionCrossbow_Charging_SoundShader"
		};
	};
	class ExpansionCrossbow_Firing_SoundSet: baseCharacter_SoundSet
	{
		soundShaders[]=
		{
			"ExpansionCrossbow_Firing_SoundShader"
		};
	};
};
class CfgVehicles
{
	class Man;
	class SurvivorBase: Man
	{
		class AnimEvents
		{
			class Sounds
			{
				class ExpansionCrossbow_Charging
				{
					soundSet="ExpansionCrossbow_Charging_SoundSet";
					id=69691;
				};
				class ExpansionCrossbow_Firing
				{
					soundSet="ExpansionCrossbow_Firing_SoundSet";
					id=69692;
				};
			};
		};
	};
};
