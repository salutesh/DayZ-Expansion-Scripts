#define _ARMA_

class CfgPatches
{
	class DayZExpansion_Objects_BaseBuilding_Vanilla_Tents
	{
		requiredVersion = 0.1;
		requiredAddons[] = {"DZ_Data","DZ_Gear_Camping"};
		units[] = {};
		weapons[] = {};
	};
};
class CfgVehicles
{
	class Container_Base;
	class GUIInventoryAttachmentsProps;
	class Attachments;
	class TentBase: Container_Base
	{
		expansionIsOpenable = 1;
	};
	class MediumTent: TentBase
	{
		attachments[] = {"CamoNet","Lights","Att_ExpansionCodeLock","Att_CombinationLock"};
		class GUIInventoryAttachmentsProps: GUIInventoryAttachmentsProps
		{
			class Attachments: Attachments
			{
				attachmentSlots[] = {"CamoNet","Lights","Att_ExpansionCodeLock"};
			};
		};
	};
	class LargeTent: TentBase
	{
		attachments[] = {"CamoNet","Lights","Att_ExpansionCodeLock","Att_CombinationLock"};
		class GUIInventoryAttachmentsProps: GUIInventoryAttachmentsProps
		{
			class Attachments: Attachments
			{
				attachmentSlots[] = {"CamoNet","Lights","Att_ExpansionCodeLock"};
			};
		};
	};
	class CarTent: TentBase
	{
		attachments[] = {"CamoNet","Lights","Att_ExpansionCodeLock","Att_CombinationLock"};
		class GUIInventoryAttachmentsProps: GUIInventoryAttachmentsProps
		{
			class Attachments: Attachments
			{
				attachmentSlots[] = {"CamoNet","Lights","Att_ExpansionCodeLock"};
			};
		};
	};
	class PartyTent: TentBase
	{
		attachments[] = {"CamoNet","Lights","Att_ExpansionCodeLock","Att_CombinationLock"};
		class GUIInventoryAttachmentsProps: GUIInventoryAttachmentsProps
		{
			class Attachments: Attachments
			{
				attachmentSlots[] = {"CamoNet","Lights","Att_ExpansionCodeLock"};
			};
		};
	};
};
