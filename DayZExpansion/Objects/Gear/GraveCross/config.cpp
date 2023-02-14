#define _ARMA_

class CfgPatches
{
	class DayZExpansion_Objects_Gravecross
	{
		units[] = {};
		weapons[] = {};
		requiredVersion = 0.1;
		requiredAddons[] = {"DZ_Data"};
	};
};
class CfgWeapons
{
	class RifleCore;
	class Rifle_Base: RifleCore
	{
		inventorySlot[] += {"ExpansionHands"};
	};
	class PistolCore;
	class Pistol_Base: PistolCore
	{
		inventorySlot[] += {"ExpansionHands"};
	};
	class LauncherCore;
	class Launcher_Base: LauncherCore
	{
		inventorySlot[] += {"ExpansionHands"};
	};
};
class CfgSlots
{
	class Slot_ExpansionHands
	{
		name = "ExpansionHands";
		displayName = "$STR_CfgShoulder0";
		ghostIcon = "set:dayz_inventory image:shoulderleft";
	};
};
class CfgNonAIVehicles
{
	class ProxyAttachment;
	class ProxyExpansionHands: ProxyAttachment
	{
		scope = 0;
		inventorySlot = "ExpansionHands";
		model = "";
	};
};
class CfgVehicles
{
	class Inventory_Base;
	class MeleeItemBase: Inventory_Base
	{
		inventorySlot[] += {"ExpansionHands"};
	};
	class Expansion_GraveBase: Inventory_Base
	{
		scope = 0;
		displayName = "$STR_EXPANSION_GRAVECROSS";
		descriptionShort = "$STR_EXPANSION_GRAVECROSS_DESC";
		forceFarBubble = "true";
		attachments[] = {"ExpansionHands","Shoulder","Melee","Headgear","Mask","Eyewear","Gloves","Armband","Vest","Body","Back","Hips","Legs","Feet"};
	};
	class Expansion_Gravecross: Expansion_GraveBase
	{
		scope = 2;
		model = "DZ\structures\specific\cemeteries\cemetery_tombstone10.p3d";
	};
	class Expansion_Gravecross_LowLifetime: Expansion_GraveBase
	{
		scope = 2;
		model = "DZ\structures\specific\cemeteries\cemetery_tombstone2.p3d";
	};
};
