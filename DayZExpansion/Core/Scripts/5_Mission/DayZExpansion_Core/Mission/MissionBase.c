modded class MissionBase
{
	void MissionBase()
	{
#ifdef EXTRACE_DIAG
		auto trace = EXTrace.Start(EXTrace.MISC, this);
#endif

		GetDayZGame().Expansion_SetIsMissionMainMenu(IsInherited(MissionMainMenu));

		CreateDayZExpansion();
	}

	void ~MissionBase()
	{
		if (!GetGame())
			return;

		DestroyDayZExpansion();

#ifdef DIAG_DEVELOPER
		Print("[DayZ Expansion] ~MissionBase");
#endif
	}
};
