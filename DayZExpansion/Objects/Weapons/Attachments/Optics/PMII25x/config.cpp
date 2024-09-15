class CfgPatches
{
	class DayZExpansion_PMII25Optic
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
	class Expansion_PMII25Optic: ItemOptics_Base
	{
		scope=2;
		displayName="$STR_EXPANSION_PMII25";
		descriptionShort="$STR_EXPANSION_PMII25_DESC";
		model="DayZExpansion\Objects\Weapons\Attachments\Optics\PMII25x\SniperOptic.p3d";
		animClass="Binoculars";
		rotationFlags=4;
		reversed=0;
		ContinuousActions[]={236};
		weight=700;
		itemSize[]={4,1};
		inventorySlot="ExpansionSniperOptics";
		simulation="itemoptics";
		dispersionModifier=-0.001;
		dispersionCondition="true";
		recoilModifier[]={1,1,1};
		memoryPointCamera="eyeScope";
		cameraDir="cameraDir";
		s_showEnterMisalignment=1;
		s_isFullscreen=1;
		s_pipRadius=1;
		s_pipMagnification=-0.2;
		s_pipBlur=0.0099999998;
		s_pipChromAber=0.5;
		class DamageSystem
		{
			class GlobalHealth
			{
				class Health
				{
					hitpoints=100;
					healthLevels[]=
					{
						
						{
							1,
							
							{
								"DZ\gear\optics\data\scope_alpha_ca.paa",
								"DZ\weapons\attachments\data\terra.rvmat"
							}
						},
						
						{
							0.69999999,
							{}
						},
						
						{
							0.5,
							
							{
								"DZ\weapons\attachments\data\scope_alpha_damaged_ca.paa",
								"DZ\weapons\attachments\data\terra_damage.rvmat"
							}
						},
						
						{
							0.30000001,
							{}
						},
						
						{
							0,
							
							{
								"DZ\weapons\attachments\data\scope_alpha_destroyed_ca.paa",
								"DZ\weapons\attachments\data\terra_destruct.rvmat"
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
			useModelOptics=1;
			modelOptics="\DZ\weapons\attachments\optics\opticview_longrange.p3d";
			opticsDisablePeripherialVision=0.67000002;
			opticsFlare=1;
			opticsPPEffects[]=
			{
				"OpticsCHAbera3",
				"OpticsBlur1"
			};
			opticsZoomMin="0.3926/12";
			opticsZoomMax="0.3926/25";
			opticsZoomInit="0.3926/12";
			discretefov[]=
			{
				"0.3926/12",
				"0.3926/25"
			};
			discreteInitIndex=0;
			distanceZoomMin=100;
			distanceZoomMax=1000;
			discreteDistance[]={200,300,400,500,600,700,800,900,1000,1100,1200};
			discreteDistanceInitIndex=0;
			PPMaskProperties[]={0.5,0.5,0.50000012,0.050000001};
			PPLensProperties[]={0.2,0.15000001,0,0};
			PPBlurProperties=0.30000001;
		};
	};
};
class CfgSlots
{
	class Slot_ExpansionSniperOptics
	{
		name="ExpansionSniperOptics";
		displayName="$STR_EXPANSION_PMII25";
		ghostIcon="weaponoptics";
	};
};
class CfgNonAIVehicles
{
	class ProxyAttachment;
	class ProxySniperOptic: ProxyAttachment
	{
		scope=2;
		inventorySlot="ExpansionSniperOptics";
		model="DayZExpansion\Objects\Weapons\Attachments\Optics\PMII25x\SniperOptic.p3d";
	};
};
