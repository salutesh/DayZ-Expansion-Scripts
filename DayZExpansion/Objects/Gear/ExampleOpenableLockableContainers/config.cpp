class CfgPatches
{
	class DayZExpansion_Objects_Gear_ExampleOpenableLockableContainers
	{
		units[]={};
		weapons[]={};
		requiredVersion=0.1;
		requiredAddons[]=
		{
			"DZ_Gear_Camping"
		};
	};
};
class CfgVehicles
{
	class SeaChest;
	class ExpansionLockableChest: SeaChest
	{
		attachments[]=
		{
			"Att_ExpansionCodeLock"
		};
	};
	class ExpansionOpenableChest: SeaChest
	{
		expansionIsOpenable=1;
	};
	class ExpansionOpenableLockableChest: ExpansionLockableChest
	{
		expansionIsOpenable=1;
	};
};
