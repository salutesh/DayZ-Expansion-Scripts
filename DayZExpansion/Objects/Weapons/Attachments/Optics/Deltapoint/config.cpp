class CfgPatches
{
	class DayZExpansion_Weapons_Optics_DeltaPoint
	{
		units[]={};
		weapons[]={};
		requiredVersion=0.1;
		requiredAddons[]=
		{
			"DZ_Weapons_Optics"
		};
	};
};
class cfgVehicles
{
	class ItemOptics_Base;
	class ExpansionDeltapointOptic: ItemOptics_Base
	{
		scope=2;
		displayName="$STR_EXPANSION_DELTAPOINT";
		descriptionShort="$STR_EXPANSION_DELTAPOINT_DESC";
		model="\DayZExpansion\Objects\Weapons\Attachments\Optics\Deltapoint\Deltapoint.p3d";
		animClass="Binoculars";
		rotationFlags=17;
		reversed=0;
		weight=150;
		itemSize[]={1,1};
		inventorySlot="pistolOptics";
		simulation="itemoptics";
		selectionFireAnim="zasleh";
		memoryPointCamera="eyeScope";
		cameraDir="cameraDir";
		recoilModifier[]={1,1,1};
		class DamageSystem
		{
			class GlobalHealth
			{
				class Health
				{
					hitpoints=30;
					healthLevels[]=
					{
						
						{
							1,
							
							{
								"DZ\weapons\attachments\optics\data\lensglass_ca.paa",
								"DZ\weapons\attachments\optics\data\lensglass_clearer.rvmat",
								"DZ\weapons\attachments\data\mrds.rvmat"
							}
						},
						
						{
							0.69999999,
							{}
						},
						
						{
							0.5,
							
							{
								"DZ\weapons\attachments\optics\data\lensglass_damage_ca.paa",
								"DZ\weapons\attachments\optics\data\lensglass_clearer_damage.rvmat",
								"DZ\weapons\attachments\data\mrds_damage.rvmat"
							}
						},
						
						{
							0.30000001,
							{}
						},
						
						{
							0,
							
							{
								"DZ\weapons\attachments\optics\data\lensglass_destruct_ca.paa",
								"DZ\weapons\attachments\optics\data\lensglass_clearer_destruct.rvmat",
								"DZ\weapons\attachments\data\mrds_destruct.rvmat"
							}
						}
					};
				};
			};
		};
		class OpticsInfo
		{
			memoryPointCamera="eyeScope";
			cameraDir="cameraDir";
			modelOptics="-";
			opticsDisablePeripherialVision=0.67000002;
			opticsFlare=1;
			opticsPPEffects[]={};
			opticsZoomMin=0.52359998;
			opticsZoomMax=0.52359998;
			opticsZoomInit=0.52359998;
			distanceZoomMin=50;
			distanceZoomMax=50;
			discreteDistance[]={50};
			discreteDistanceInitIndex=0;
			PPMaskProperties[]={0.5,0.5,0.40000001,0.050000001};
			PPLensProperties[]={1,0.15000001,0,0};
			PPBlurProperties=0.2;
			PPDOFProperties[]={1,0.2,20,185,4,10};
			opticSightTexture="dz\weapons\attachments\optics\data\collimdot_red_ca.paa";
		};
		repairableWithKits[]={5,7};
		repairCosts[]={30,25};
		class EnergyManager
		{
			hasIcon=1;
			energyUsagePerSecond=0.02;
			plugType=1;
			attachmentAction=1;
		};
	};
};
