class CfgPatches
{
	class DayZExpansion_Vehicles_Ground_Offroad_02
	{
		units[]={};
		weapons[]={};
		requiredVersion=0.1;
		requiredAddons[]=
		{
			"DZ_Vehicles_Wheeled"
		};
	};
};
class CfgVehicles
{
	class GUIInventoryAttachmentsProps;
	class Body;
	class CarScript;
	class Offroad_02: CarScript
	{
		doors[]=
		{
			"Offroad_02_Door_1_1",
			"Offroad_02_Door_2_1",
			"Offroad_02_Door_1_2",
			"Offroad_02_Door_2_2",
			"Offroad_02_Hood",
			"Offroad_02_Trunk"
		};
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
