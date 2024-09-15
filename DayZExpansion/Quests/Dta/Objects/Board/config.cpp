class CfgPatches
{
	class DayZExpansion_Quests_Objects_Boards
	{
		units[]={};
		weapons[]={};
		requiredVersion=0.1;
		requiredAddons[]=
		{
			"DZ_Data"
		};
	};
};
class CfgVehicles
{
	class HouseNoDestruct;
	class ExpansionQuestBoardSmall: HouseNoDestruct
	{
		scope=1;
		model="\DayZExpansion\Quests\Dta\Objects\Board\ExpansionQuestBoardSmall.p3d";
		forceFarBubble="true";
	};
	class ExpansionQuestBoardLarge: HouseNoDestruct
	{
		scope=1;
		model="\DayZExpansion\Quests\Dta\Objects\Board\ExpansionQuestBoardLarge.p3d";
		forceFarBubble="true";
	};
};
