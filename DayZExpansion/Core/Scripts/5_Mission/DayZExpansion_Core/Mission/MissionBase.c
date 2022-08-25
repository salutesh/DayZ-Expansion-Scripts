modded class MissionBase
{
	void MissionBase()
	{
		GetDayZGame().Expansion_SetIsMissionMainMenu(IsInherited(MissionMainMenu));

		CreateDayZExpansion();
	}
};
