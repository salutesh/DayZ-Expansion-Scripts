class CfgPatches
{
	class DayZExpansion_Vehicles_Water_Boat_01
	{
		requiredVersion=0.1;
		requiredAddons[]=
		{
			"DZ_Vehicles_Water"
		};
	};
};
class CfgVehicles
{
	class BoatScript;
	class GUIInventoryAttachmentsProps;
	class Engine;
	class Boat_01_ColorBase: BoatScript
	{
		attachments[]+=
		{
			"CamoNet",
			"KeyChain"
		};
		class GUIInventoryAttachmentsProps: GUIInventoryAttachmentsProps
		{
			class Engine: Engine
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
