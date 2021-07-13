#define _ARMA_

class CfgPatches
{
	class DayZExpansion_Kar98ScopeOptic
	{
		units[] = {"ExpansionKar98ScopeOptic"};
		weapons[] = {};
		requiredVersion = 0.1;
		requiredAddons[] = {"DZ_Weapons_Optics"};
	};
};
class cfgVehicles
{
	class ItemOptics;
	class ExpansionKar98ScopeOptic: ItemOptics
	{
		scope = 2;
		displayName = "$STR_KAR98_SCOPE";
		descriptionShort = "$STR_KAR98_SCOPE_DESC";
		model = "\DayZExpansion\Objects\Weapons\Attachments\Optics\Kar98Scope\karscope.p3d";
		animClass = "Binoculars";
		rotationFlags = 16;
		reversed = 0;
		weight = 800;
		itemSize[] = {3,1};
		inventorySlot[] = {"ExpansionKar98Optics"};
		simulation = "itemoptics";
		dispersionModifier = -0.0001;
		dispersionCondition = "true";
		recoilModifier[] = {1,1,1};
		swayModifier[] = {1,1,1};
		memoryPointCamera = "eyeScope";
		cameraDir = "cameraDir";
		class DamageSystem
		{
			class GlobalHealth
			{
				class Health
				{
					hitpoints = 50;
					healthLevels[] = {{1.0,{}},{0.7,{}},{0.5,{}},{0.3,{}},{0.0,{}}};
				};
			};
		};
		class OpticsInfo
		{
			memoryPointCamera = "eyeScope";
			cameraDir = "cameraDir";
			modelOptics = "-";
			opticsDisablePeripherialVision = 0.67;
			opticsFlare = 1;
			opticsPPEffects[] = {"OpticsCHAbera3","OpticsBlur1"};
			opticsZoomMin = "0.3926/6";
			opticsZoomMax = "0.3926/6";
			opticsZoomInit = "0.3926/6";
			distanceZoomMin = 100;
			distanceZoomMax = 1300;
			discreteDistance[] = {100,200,300,400,500,600,700,800,900,1000,1100,1200,1300};
			discreteDistanceInitIndex = 0;
			PPMaskProperties[] = {0.5,0.5,0.35,0.05};
			PPLensProperties[] = {0.5,0.15,0,0};
			PPBlurProperties = 0.6;
		};
	};
};
class CfgSlots
{
	class Slot_ExpansionKar98Optics
	{
		name = "ExpansionKar98Optics";
		displayName = "$STR_KAR98_SCOPE";
		ghostIcon = "weaponoptics";
	};
};
class CfgNonAIVehicles
{
	class ProxyAttachment;
	class Proxykarscope: ProxyAttachment
	{
		scope = 2;
		inventorySlot = "ExpansionKar98Optics";
		model = "DayZExpansion\Objects\Weapons\Attachments\Optics\Kar98Scope\karscope.p3d";
	};
};
