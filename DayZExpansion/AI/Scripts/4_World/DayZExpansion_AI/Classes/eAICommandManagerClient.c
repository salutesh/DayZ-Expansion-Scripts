/**
 * This class is a stub and only exists to not break compiling with 3rd-party mods that mod eAICommandManagerClient until those mods update.
 * Use DayZExpansion/AI/Scripts/5_Mission/DayZExpansion_AI/eAICommandManagerImpl instead.
 *
 * TODO: Remove sometime after the next Expansion update.
 */
class eAICommandManagerClient: eAICommandManager
{
	ref ExpansionRPCManager m_Expansion_RPCManager;

	int m_MovementSpeedLimit;
	int m_UnlimitedReload;
	int m_LootingBehavior;
	int m_DamageIn;
	int m_DamageOut;
	int m_HeadshotResistance;

	eAIBase GetAIAtCursorOrNearest();

	eAIBase SpawnAI_Helper(PlayerBase leader, string loadout = "HumanLoadout.json");
	
	void SpawnAI(int cmd);

	eAIBase SpawnAIEx(vector pos, string loadout = "HumanLoadout.json");

	eAIBase SpawnAI_Sentry(vector pos, string loadout = "WestLoadout.json");
	
	eAIBase SpawnAI_Patrol(vector pos, string loadout = "HumanLoadout.json");
	
	vector GetEntitySpawnPosition(DayZPlayer player);

	void RPC_SpawnAI(PlayerIdentity sender, Object target, ParamsReadContext ctx);
	
	void RPC_SpawnZombie(PlayerIdentity sender, Object target, ParamsReadContext ctx);

	void RPC_SpawnWolf(PlayerIdentity sender, Object target, ParamsReadContext ctx);

	void RPC_SpawnBear(PlayerIdentity sender, Object target, ParamsReadContext ctx);

	void RPC_ClearAllAI(PlayerIdentity sender, Object target, ParamsReadContext ctx);
	
	void RPC_SetFaction(PlayerIdentity sender, Object target, ParamsReadContext ctx);

	void RPC_UnlimitedReload(PlayerIdentity sender, Object target, ParamsReadContext ctx);
	
	void RPC_ResetPathfinding(PlayerIdentity sender, Object target, ParamsReadContext ctx);
	
	void RPC_DebugObjects(PlayerIdentity sender, Object target, ParamsReadContext ctx);
	
	void RPC_DebugDamage(PlayerIdentity sender, Object target, ParamsReadContext ctx);

	void RPC_SetDamageInOut(PlayerIdentity sender, Object target, ParamsReadContext ctx);
	
	void RPC_ReqFormRejoin(PlayerIdentity sender, Object target, ParamsReadContext ctx);
	
	void RPC_ReqFormStop(PlayerIdentity sender, Object target, ParamsReadContext ctx);
	
	void RPC_ReqFormFlank(PlayerIdentity sender, Object target, ParamsReadContext ctx);
	
	void RPC_ReqFormRoam(PlayerIdentity sender, Object target, ParamsReadContext ctx);
	
	void RPC_ReqFormationChange(PlayerIdentity sender, Object target, ParamsReadContext ctx);

	void RPC_SetFormationScale(PlayerIdentity sender, Object target, ParamsReadContext ctx);

	void RPC_SetFormationLooseness(PlayerIdentity sender, Object target, ParamsReadContext ctx);

	void RPC_SetWaypoint(PlayerIdentity sender, Object target, ParamsReadContext ctx);

	void RPC_ExportPatrol(PlayerIdentity sender, Object target, ParamsReadContext ctx);

	void RPC_ClearWaypoints(PlayerIdentity sender, Object target, ParamsReadContext ctx);
	
	void RPC_SetMovementSpeed(PlayerIdentity sender, Object target, ParamsReadContext ctx);

	void SetMovementSpeed(eAIGroup g, int speed);
	
	void RPC_SetStance(PlayerIdentity sender, Object target, ParamsReadContext ctx);

	void RPC_DumpState(PlayerIdentity sender, Object target, ParamsReadContext ctx);
	
	void RPC_ForceWeaponResync(PlayerIdentity sender, Object target, ParamsReadContext ctx);

	void SitRep_Client(notnull PlayerBase player, notnull eAIBase ai);

	void PlaySoundOnObject(string soundSet, Object obj);

	void RPC_SitRep(PlayerIdentity sender, Object target, ParamsReadContext ctx);

	void SitRep_Server(notnull PlayerBase player, notnull eAIBase ai);

	void RPC_SetLootingBehavior(PlayerIdentity sender, Object target, ParamsReadContext ctx);
};
