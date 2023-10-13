enum ExpansionNamalskModuleRPC
{
	INVALID = 100000,
	SendWorkbenchData,
	SendWorkbenchResult,
	SetWorkbenchState,
	COUNT
};

enum ExpansionCommunityGoalsModuleRPC
{
	INVALID = 200000,
	SendCommunityGoalData,
	RequestCommunityGoalDetails,
	SendCommunityGoalDetails
	COUNT
};

enum Expansion_Satellite_Control_ERPCs
{
	PLAY_BOOT_SFX = 3000101,
	PLAY_RUNNING_SFX = 3000102,
	PLAY_SHUTDOWN_SFX = 3000103
};