#define _ARMA_

class CfgPatches
{
	class DayZExpansion_M203_Shells
	{
		weapons[] = {};
		requiredVersion = 0.1;
		requiredAddons[] = {"DZ_Data"};
		units[] = {"Expansion_M203Round_Smoke_White","Expansion_M203Round_Smoke_Teargas","Expansion_M203Round_Smoke_Red","Expansion_M203Round_Smoke_Green","Expansion_M203Round_Smoke_Yellow","Expansion_M203Round_Smoke_Purple"};
	};
};
class CfgVehicles
{
	class Grenade_Base;
	class Expansion_M203Round_Smoke_Colorbase: Grenade_Base
	{
		displayName = "";
		descriptionShort = "";
		model = "DayZExpansion\Objects\Weapons\Firearms\M79\m79\round\m79_projectile.p3d";
		rotationFlags = 17;
		weight = 500;
		class EnergyManager
		{
			energyAtSpawn = 90;
			energyUsagePerSecond = 1;
			wetnessExposure = 0.1;
		};
		class AnimEvents
		{
			class SoundWeapon
			{
				class Unpin
				{
					soundSet = "SmokegGrenades_M18_oneshot_ignite_SoundSet";
					id = 201;
				};
			};
		};
	};
	class Expansion_M203Round_Smoke_White: Expansion_M203Round_Smoke_Colorbase
	{
		scope = 2;
	};
	class Expansion_M203Round_Smoke_Teargas: Expansion_M203Round_Smoke_Colorbase
	{
		scope = 2;
	};
	class Expansion_M203Round_Smoke_Red: Expansion_M203Round_Smoke_Colorbase
	{
		scope = 2;
	};
	class Expansion_M203Round_Smoke_Green: Expansion_M203Round_Smoke_Colorbase
	{
		scope = 2;
	};
	class Expansion_M203Round_Smoke_Yellow: Expansion_M203Round_Smoke_Colorbase
	{
		scope = 2;
	};
	class Expansion_M203Round_Smoke_Purple: Expansion_M203Round_Smoke_Colorbase
	{
		scope = 2;
	};
};
