class CfgPatches
{
	class DayZExpansion_Kar98Bayonet
	{
		units[]={};
		weapons[]={};
		requiredVersion=0.1;
		requiredAddons[]=
		{
			"DZ_Weapons_Muzzles"
		};
	};
};
class cfgVehicles
{
	class AK_Bayonet;
	class Expansion_Kar98_Bayonet: AK_Bayonet
	{
		scope=2;
		displayName="$STR_EXPANSION_KAR98_BAYONET";
		descriptionShort="$STR_EXPANSION_KAR98_BAYONET_DESC";
		model="DayZExpansion\Objects\Weapons\Attachments\Support\kar98bayonet\k98bayonet.p3d";
		weight=426;
		itemModelLength=0.38800001;
		inventorySlot="expansionWeaponBayonetKar";
		canSkinBodies=1;
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
								"DZ\weapons\attachments\data\Russian_AKM_Bayonet.rvmat"
							}
						},
						
						{
							0.69999999,
							
							{
								"DZ\weapons\attachments\data\Russian_AKM_Bayonet.rvmat"
							}
						},
						
						{
							0.5,
							
							{
								"DZ\weapons\attachments\data\Russian_AKM_Bayonet_damage.rvmat"
							}
						},
						
						{
							0.30000001,
							
							{
								"DZ\weapons\attachments\data\Russian_AKM_Bayonet_damage.rvmat"
							}
						},
						
						{
							0,
							
							{
								"DZ\weapons\attachments\data\Russian_AKM_Bayonet_destruct.rvmat"
							}
						}
					};
				};
			};
		};
	};
};
class CfgNonAIVehicles
{
	class ProxyAttachment;
	class Proxyk98bayonet: ProxyAttachment
	{
		scope=2;
		inventorySlot="expansionWeaponBayonetKar";
		model="DayZExpansion\Objects\Weapons\Attachments\Support\kar98bayonet\k98bayonet.p3d";
	};
};
class CfgSlots
{
	class Slot_expansionWeaponBayonetKar
	{
		name="expansionWeaponBayonetKar";
		displayName="$STR_EXPANSION_KAR98_BAYONET";
		ghostIcon="set:dayz_inventory image:bayonet";
	};
};
