class CfgPatches
{
	class DayZExpansion_Objects_BaseBuilding_Vanilla
	{
		requiredVersion=0.1;
		requiredAddons[]=
		{
			"DZ_Gear_Camping",
			"DZ_Gear_Nasdara_Camping"
		};
	};
};
class CfgVehicles
{
	class Inventory_Base;
	class TentBase;
	class GUIInventoryAttachmentsProps;
	class MediumTent: TentBase
	{
		attachments[]+=
		{
			"Att_ExpansionCodeLock"
		};
		class GUIInventoryAttachmentsProps: GUIInventoryAttachmentsProps
		{
			class Attachments
			{
				attachmentSlots[]+=
				{
					"Att_ExpansionCodeLock"
				};
			};
		};
	};
	class LargeTent: TentBase
	{
		attachments[]+=
		{
			"Att_ExpansionCodeLock"
		};
		class GUIInventoryAttachmentsProps: GUIInventoryAttachmentsProps
		{
			class Attachments
			{
				attachmentSlots[]+=
				{
					"Att_ExpansionCodeLock"
				};
			};
		};
	};
	class CarTent: TentBase
	{
		attachments[]+=
		{
			"Att_ExpansionCodeLock"
		};
		class GUIInventoryAttachmentsProps: GUIInventoryAttachmentsProps
		{
			class Attachments
			{
				attachmentSlots[]+=
				{
					"Att_ExpansionCodeLock"
				};
			};
		};
	};
	class PartyTent: TentBase
	{
		attachments[]+=
		{
			"Att_ExpansionCodeLock"
		};
		class GUIInventoryAttachmentsProps: GUIInventoryAttachmentsProps
		{
			class Attachments
			{
				attachmentSlots[]+=
				{
					"Att_ExpansionCodeLock"
				};
			};
		};
	};
	class DigitalCodeLock: Inventory_Base
	{
		inventorySlot[]+=
		{
			"Att_ExpansionCodeLock",
			"Att_ExpansionCodeLock_1",
			"Att_ExpansionCodeLock_2"
		};
	};
};
