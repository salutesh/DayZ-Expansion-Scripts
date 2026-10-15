class CfgPatches
{
	class DayZExpansion_Vehicles_Singletrack
	{
		requiredVersion=0.1;
		requiredAddons[]=
		{
			"DZ_Vehicles_Singletrack"
		};
	};
};
class CfgVehicles
{
	class MotorbikeScript;
	class DamageSystem;
	class GUIInventoryAttachmentsProps;
	class Body;
	class Motorbike_01_ColorBase: MotorbikeScript
	{
		attachments[]+=
		{
			"CamoNet",
			"KeyChain"
		};
		class DamageSystem: DamageSystem
		{
			class GUIInventoryAttachmentsProps: GUIInventoryAttachmentsProps
			{
				class Body: Body
				{
					attachmentSlots[]+=
					{
						"CamoNet",
						"KeyChain"
					};
				};
			};
		};
	};
	class Motorbike_02_ColorBase: MotorbikeScript
	{
		attachments[]+=
		{
			"CamoNet",
			"KeyChain"
		};
		class GUIInventoryAttachmentsProps: GUIInventoryAttachmentsProps
		{
			class Body: Body
			{
				attachmentSlots[]+=
				{
					"CamoNet",
					"KeyChain"
				};
			};
		};
	};
};
