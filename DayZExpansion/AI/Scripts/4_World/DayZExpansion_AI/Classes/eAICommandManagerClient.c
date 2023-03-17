// This class handles the inputs from the eAICommandMenu locally and shoots RPCs to the server.
class eAICommandManagerClient : eAICommandManager
{
	override bool Send(eAICommands cmd)
	{
		switch (cmd)
		{
			case eAICommands.DEB_SPAWNALLY:
			case eAICommands.DEB_SPAWNSENTRY:
			case eAICommands.DEB_SPAWNGUARD:
			case eAICommands.DEB_SPAWNPASSIVE:
			case eAICommands.DEB_SPAWNSHAMAN:
				GetRPCManager().SendRPC("eAI", "SpawnAI", new Param2<DayZPlayer, int>(GetGame().GetPlayer(), cmd));
				return true;
			
			case eAICommands.DEB_CLEARALL:
				GetRPCManager().SendRPC("eAI", "ClearAllAI", new Param1<DayZPlayer>(GetGame().GetPlayer()));
				return true;
			
			case eAICommands.DEB_SPAWNZOM:
				GetRPCManager().SendRPC("eAI", "SpawnZombie", new Param1<DayZPlayer>(GetGame().GetPlayer()));
				return true;
			case eAICommands.DEB_SPAWNWOLF:
				GetRPCManager().SendRPC("eAI", "SpawnWolf", new Param1<DayZPlayer>(GetGame().GetPlayer()));
				return true;
			case eAICommands.DEB_SPAWNBEAR:
				GetRPCManager().SendRPC("eAI", "SpawnBear", new Param1<DayZPlayer>(GetGame().GetPlayer()));
				return true;
			case eAICommands.FOR_VEE:
			case eAICommands.FOR_INVVEE:
			case eAICommands.FOR_FILE:
			case eAICommands.FOR_INVFILE:
			case eAICommands.FOR_WALL:
			case eAICommands.FOR_COL:
			case eAICommands.FOR_INVCOL:
			case eAICommands.FOR_CIRCLE:
			case eAICommands.FOR_CIRCLEDOT:
			case eAICommands.FOR_STAR:
			case eAICommands.FOR_STARDOT:
				GetRPCManager().SendRPC("eAI", "ReqFormationChange", new Param2<DayZPlayer, int>(GetGame().GetPlayer(), cmd));
				return true;
			
			case eAICommands.MOV_STOP:
				GetRPCManager().SendRPC("eAI", "ReqFormStop", new Param1<DayZPlayer>(GetGame().GetPlayer()));
				return true;
			
			case eAICommands.MOV_RTF:
				GetRPCManager().SendRPC("eAI", "ReqFormRejoin", new Param1<DayZPlayer>(GetGame().GetPlayer()));
				return true;
			
			case eAICommands.DEB_TARGET_CREATE:
				eAIBase.Get(0).CreateDebugApple();
				return true;
			
			case eAICommands.DEB_TARGET_DESTROY:
				eAIBase.Get(0).DestroyDebugApple();
				return true;
		}
		
		return false;
	}
};