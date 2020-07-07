#include "$CurrentDir:\\Missions\\ExpansionCOM.Enoch\\core\\BaseModuleInclude.c"

#include "$CurrentDir:\\Missions\\ExpansionCOM.Enoch\\expansion\\ExpansionObjectSpawnTools.c"
#include "$CurrentDir:\\Missions\\ExpansionCOM.Enoch\\expansion\\missions\\MissionConstructor.c"

Mission CreateCustomMission(string path)
{
	return new CommunityOfflineClient();
}

void main()
{
	bool loadTraderObjects = false;
	bool loadTraderNPCs = false;

	string MissionWorldName = "empty";
	GetGame().GetWorldName(MissionWorldName);
	
	if (MissionWorldName != "empty")
	{
		//! Spawn mission objects and traders
		FindMissionFiles(MissionWorldName, loadTraderObjects, loadTraderNPCs);
	}
	
	//! Init server central economy
	// Hive ce = CreateHive();
	// if ( ce )
	// 	ce.InitOffline();
}