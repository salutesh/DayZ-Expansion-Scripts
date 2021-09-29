#define _ARMA_

class CfgPatches
{
	class DayZExpansion_Sounds_Weapons_RPG7
	{
		units[] = {};
		weapons[] = {};
		requiredVersion = 0.1;
		requiredAddons[] = {"DZ_Sounds_Effects"};
	};
};
class CfgSoundShaders
{
	class base_closeShot_SoundShader;
	class base_midShot_SoundShader;
	class base_distShot_SoundShader;
	class reloadWeapon_SoundShader;
	class Expansion_RPG7_Reload1_SoundShader: reloadWeapon_SoundShader
	{
		samples[] = {{"DayZExpansion\Sounds\Weapons\RPG7\Expansion_RPG7_Reload1",1}};
		volume = 1;
	};
	class Expansion_RPG7_Reload2_SoundShader: reloadWeapon_SoundShader
	{
		samples[] = {{"DayZExpansion\Sounds\Weapons\RPG7\Expansion_RPG7_Reload2",1}};
		volume = 1;
	};
	class Expansion_RPG7_Reload3_SoundShader: reloadWeapon_SoundShader
	{
		samples[] = {{"DayZExpansion\Sounds\Weapons\RPG7\Expansion_RPG7_Reload3",1}};
		volume = 1;
	};
	class Expansion_RPG_closeShot_SoundShader: base_closeShot_SoundShader
	{
		samples[] = {{"DayZExpansion\Sounds\Weapons\RPG7\Expansion_RPG7_Rocket",1}};
		volume = 0.9;
	};
	class Expansion_RPG_midShot_SoundShader: base_midShot_SoundShader
	{
		samples[] = {{"DayZExpansion\Sounds\Weapons\RPG7\Expansion_RPG7_Rocket_mid",1}};
		volume = 0.45;
	};
	class Expansion_RPG_distShot_SoundShader: base_distShot_SoundShader
	{
		samples[] = {{"DayZExpansion\Sounds\Weapons\RPG7\Expansion_RPG7_Rocket_long",1}};
		volume = 0.15;
	};
	class Expansion_RPG_BagShuffle_SoundShader: reloadWeapon_SoundShader
	{
		samples[] = {{"DZ\sounds\Characters\movement\attachment\Backpack\military\runErc_1",1},{"DZ\sounds\Characters\movement\attachment\Backpack\military\runErc_2",1},{"DZ\sounds\Characters\movement\attachment\Backpack\military\runErc_3",1},{"DZ\sounds\Characters\movement\attachment\Backpack\military\runErc_4",1},{"DZ\sounds\Characters\movement\attachment\Backpack\military\runErc_5",1},{"DZ\sounds\Characters\movement\attachment\Backpack\military\runErc_6",1},{"DZ\sounds\Characters\movement\attachment\Backpack\military\runErc_7",1},{"DZ\sounds\Characters\movement\attachment\Backpack\military\runErc_8",1},{"DZ\sounds\Characters\movement\attachment\Backpack\military\sprintErc_1",1},{"DZ\sounds\Characters\movement\attachment\Backpack\military\sprintErc_2",1},{"DZ\sounds\Characters\movement\attachment\Backpack\military\sprintErc_3",1},{"DZ\sounds\Characters\movement\attachment\Backpack\military\sprintErc_4",1},{"DZ\sounds\Characters\movement\attachment\Backpack\military\sprintErc_5",1},{"DZ\sounds\Characters\movement\attachment\Backpack\military\sprintErc_6",1},{"DZ\sounds\Characters\movement\attachment\Backpack\military\sprintErc_7",1},{"DZ\sounds\Characters\movement\attachment\Backpack\military\sprintErc_8",1}};
		volume = 1;
	};
	class Expansion_RPG_Shove_SoundShader: reloadWeapon_SoundShader
	{
		samples[] = {{"DayZExpansion\Sounds\Weapons\RPG7\Expansion_RPG7_Shove1.ogg",1},{"DayZExpansion\Sounds\Weapons\RPG7\Expansion_RPG7_Shove2.ogg",1},{"DayZExpansion\Sounds\Weapons\RPG7\Expansion_RPG7_Shove3.ogg",1}};
		volume = 1;
	};
};
class CfgSoundSets
{
	class Rifle_Shot_Base_SoundSet;
	class baseCharacter_SoundSet;
	class Expansion_RPG7_Reload1_SoundSet: baseCharacter_SoundSet
	{
		soundShaders[] = {"Expansion_RPG7_Reload1_SoundShader"};
	};
	class Expansion_RPG7_Reload2_SoundSet: baseCharacter_SoundSet
	{
		soundShaders[] = {"Expansion_RPG7_Reload2_SoundShader"};
	};
	class Expansion_RPG7_Reload3_SoundSet: baseCharacter_SoundSet
	{
		soundShaders[] = {"Expansion_RPG7_Reload3_SoundShader"};
	};
	class Expansion_RPG7_BagShuffle_SoundSet: baseCharacter_SoundSet
	{
		soundShaders[] = {"Expansion_RPG_BagShuffle_SoundShader"};
	};
	class Expansion_RPG7_Shove_SoundSet: baseCharacter_SoundSet
	{
		soundShaders[] = {"Expansion_RPG_Shove_SoundShader"};
	};
	class Expansion_RPG_Shot_SoundSet: Rifle_Shot_Base_SoundSet
	{
		soundShaders[] = {"Expansion_RPG_closeShot_SoundShader","Expansion_RPG_midShot_SoundShader","Expansion_RPG_distShot_SoundShader"};
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
				class Expansion_RPG7_Reload1
				{
					soundSet = "Expansion_RPG7_Reload1_SoundSet";
					id = 115100;
				};
				class Expansion_RPG7_Reload2
				{
					soundSet = "Expansion_RPG7_Reload2_SoundSet";
					id = 115101;
				};
				class Expansion_RPG7_Reload3
				{
					soundSet = "Expansion_RPG7_Reload3_SoundSet";
					id = 115102;
				};
				class Expansion_RPG7_BagShuffle
				{
					soundSet = "Expansion_RPG7_BagShuffle_SoundSet";
					id = 115103;
				};
				class Expansion_RPG7_Shove
				{
					soundSet = "Expansion_RPG7_Shove_SoundSet";
					id = 115104;
				};
			};
		};
	};
};
