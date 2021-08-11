#define _ARMA_

class CfgPatches
{
	class DayZExpansion_Weapons_Optics_DeltaPoint
	{
		units[] = {"ExpansionDeltapointOptic"};
		weapons[] = {};
		requiredVersion = 0.1;
		requiredAddons[] = {"DZ_Data"};
	};
};
class cfgVehicles
{
	class ItemOptics;
	class ExpansionDeltapointOptic: ItemOptics
	{
		scope = 2;
		displayName = "$STR_EXPANSION_DELTAPOINT";
		descriptionShort = "$STR_EXPANSION_DELTAPOINT_DESC";
		model = "\DayZExpansion\Objects\Weapons\Attachments\Optics\Deltapoint\Deltapoint.p3d";
		animClass = "Binoculars";
		rotationFlags = 17;
		reversed = 0;
		weight = 150;
		itemSize[] = {1,1};
		inventorySlot = "pistolOptics";
		simulation = "itemoptics";
		selectionFireAnim = "zasleh";
		memoryPointCamera = "eyeScope";
		cameraDir = "cameraDir";
		recoilModifier[] = {1,1,1};
		class DamageSystem
		{
			class GlobalHealth
			{
				class Health
				{
					hitpoints = 30;
					healthLevels[] = {{1,{"DZ\weapons\attachments\optics\data\lensglass_ca.paa","DZ\weapons\attachments\optics\data\lensglass_clearer.rvmat","DZ\weapons\attachments\data\mrds.rvmat"}},{0.7,{}},{0.5,{"DZ\weapons\attachments\optics\data\lensglass_damage_ca.paa","DZ\weapons\attachments\optics\data\lensglass_clearer_damage.rvmat","DZ\weapons\attachments\data\mrds_damage.rvmat"}},{0.3,{}},{0,{"DZ\weapons\attachments\optics\data\lensglass_destruct_ca.paa","DZ\weapons\attachments\optics\data\lensglass_clearer_destruct.rvmat","DZ\weapons\attachments\data\mrds_destruct.rvmat"}}};
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
			opticsPPEffects[] = {};
			opticsZoomMin = 0.5236;
			opticsZoomMax = 0.5236;
			opticsZoomInit = 0.5236;
			distanceZoomMin = 50;
			distanceZoomMax = 50;
			discreteDistance[] = {50};
			discreteDistanceInitIndex = 0;
			PPMaskProperties[] = {0.5,0.5,0.4,0.05};
			PPLensProperties[] = {1,0.15,0,0};
			PPBlurProperties = 0.2;
			PPDOFProperties[] = {1,0.2,20,185,4,10};
			opticSightTexture = "dz\weapons\attachments\optics\data\collimdot_red_ca.paa";
		};
		repairableWithKits[] = {5,7};
		repairCosts[] = {30,25};
		class EnergyManager
		{
			hasIcon = 1;
			energyUsagePerSecond = 0.02;
			plugType = 1;
			attachmentAction = 1;
		};
	};
};
