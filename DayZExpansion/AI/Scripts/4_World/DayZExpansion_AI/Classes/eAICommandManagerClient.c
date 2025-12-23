// This class handles the inputs from the eAICommandMenu locally and shoots RPCs to the server.
class eAICommandManagerClient : eAICommandManager
{
	ref ExpansionRPCManager m_Expansion_RPCManager;

	int m_MovementSpeedLimit = eAIMovementSpeed.SPRINT;  //! Client
	int m_UnlimitedReload;  //! Client
	int m_LootingBehavior = eAILootingBehavior.DEFAULT;  //! Client
	int m_DamageIn = eAICommands.DEB_DAMAGE;  //! Client
	int m_DamageOut = eAICommands.DEB_DAMAGE;  //! Client
	int m_HeadshotResistance = eAICommands.DEB_DAMAGE;  //! Client

	void eAICommandManagerClient()
	{
		m_Expansion_RPCManager = new ExpansionRPCManager(this);

		m_Expansion_RPCManager.RegisterServer("RPC_SpawnAI");
		m_Expansion_RPCManager.RegisterServer("RPC_SpawnZombie");
		m_Expansion_RPCManager.RegisterServer("RPC_SpawnWolf");
		m_Expansion_RPCManager.RegisterServer("RPC_SpawnBear");
		m_Expansion_RPCManager.RegisterServer("RPC_ClearAllAI");

		m_Expansion_RPCManager.RegisterServer("RPC_SetFaction");

		m_Expansion_RPCManager.RegisterServer("RPC_ReqFormationChange");
		m_Expansion_RPCManager.RegisterServer("RPC_ReqFormRejoin");
		m_Expansion_RPCManager.RegisterServer("RPC_ReqFormStop");
		m_Expansion_RPCManager.RegisterServer("RPC_ReqFormFlank");
		m_Expansion_RPCManager.RegisterServer("RPC_ReqFormRoam");

		m_Expansion_RPCManager.RegisterServer("RPC_SetFormationScale");
		m_Expansion_RPCManager.RegisterServer("RPC_SetFormationLooseness");

		m_Expansion_RPCManager.RegisterServer("RPC_SetWaypoint");
		m_Expansion_RPCManager.RegisterServer("RPC_ExportPatrol");
		m_Expansion_RPCManager.RegisterServer("RPC_ClearWaypoints");

		m_Expansion_RPCManager.RegisterServer("RPC_SetMovementSpeed");
		m_Expansion_RPCManager.RegisterServer("RPC_SetStance");

		m_Expansion_RPCManager.RegisterServer("RPC_DumpState");
		m_Expansion_RPCManager.RegisterServer("RPC_UnlimitedReload");
		m_Expansion_RPCManager.RegisterServer("RPC_ResetPathfinding");
		m_Expansion_RPCManager.RegisterServer("RPC_ForceWeaponResync");
		m_Expansion_RPCManager.RegisterServer("RPC_DebugObjects");
		m_Expansion_RPCManager.RegisterServer("RPC_DebugDamage");
		m_Expansion_RPCManager.RegisterServer("RPC_SetDamageInOut");

		m_Expansion_RPCManager.RegisterServer("RPC_SitRep");

		m_Expansion_RPCManager.RegisterServer("RPC_SetLootingBehavior");
	}

	override bool Send(int cmd, int category)
	{
		ExpansionScriptRPC rpc;
		Object target;
		eAIBase ai;

		switch (cmd)
		{
			case eAICommands.DEB_SPAWNALLY:
				SpawnAI(cmd);
				return true;
			
			case eAICommands.DEB_CLEARALL:
				m_Expansion_RPCManager.SendRPC("RPC_ClearAllAI");
				return true;
			
			case eAICommands.DEB_SPAWNZOM:
				m_Expansion_RPCManager.SendRPC("RPC_SpawnZombie", new Param1<vector>(ExpansionStatic.GetCursorHitPos()));
				return true;
			case eAICommands.DEB_SPAWNWOLF:
				m_Expansion_RPCManager.SendRPC("RPC_SpawnWolf", new Param1<vector>(ExpansionStatic.GetCursorHitPos()));
				return true;
			case eAICommands.DEB_SPAWNBEAR:
				m_Expansion_RPCManager.SendRPC("RPC_SpawnBear", new Param1<vector>(ExpansionStatic.GetCursorHitPos()));
				return true;

			case eAICommands.DEB_UNLIMITEDRELOAD_ALL:
			case eAICommands.DEB_UNLIMITEDRELOAD_ANIMALS:
			case eAICommands.DEB_UNLIMITEDRELOAD_INFECTED:
			case eAICommands.DEB_UNLIMITEDRELOAD_PLAYERS:
			case eAICommands.DEB_UNLIMITEDRELOAD_VEHICLES:
				int unlimitedReload = cmd - eAICommands.DEB_UNLIMITEDRELOAD_ALL;

				if (!unlimitedReload)
					unlimitedReload = eAITargetType.ALL_OR;
				else
					unlimitedReload = Math.Pow(2, unlimitedReload);

				if ((m_UnlimitedReload & unlimitedReload) == unlimitedReload)
					m_UnlimitedReload &= ~unlimitedReload;
				else
					m_UnlimitedReload |= unlimitedReload;

				m_Expansion_RPCManager.SendRPC("RPC_UnlimitedReload", new Param1<int>(m_UnlimitedReload));
				return true;

			case eAICommands.DEB_RESET_PATHFINDING:
				rpc = m_Expansion_RPCManager.CreateRPC("RPC_ResetPathfinding");
				rpc.Expansion_Send(GetAIAtCursorOrNearest(), true);
				return true;

			case eAICommands.DEB_FORCEWEAPONRESYNC:
				rpc = m_Expansion_RPCManager.CreateRPC("RPC_ForceWeaponResync");
				rpc.Expansion_Send(GetAIAtCursorOrNearest(), true);
				return true;

			case eAICommands.DEB_DBGOBJECTS:
				m_Expansion_RPCManager.SendRPC("RPC_DebugObjects");
				return true;

			case eAICommands.DEB_DBGDAMAGE:
				m_Expansion_RPCManager.SendRPC("RPC_DebugDamage");
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
				m_Expansion_RPCManager.SendRPC("RPC_ReqFormationChange", new Param1<int>(cmd));
				return true;

			case eAICommands.FOR_SCALE_1X:
			case eAICommands.FOR_SCALE_2X:
			case eAICommands.FOR_SCALE_3X:
			case eAICommands.FOR_SCALE_4X:
			case eAICommands.FOR_SCALE_5X:
				m_Expansion_RPCManager.SendRPC("RPC_SetFormationScale", new Param1<int>(cmd));
				return true;

			case eAICommands.FOR_LOOSENESS_00:
			case eAICommands.FOR_LOOSENESS_05:
			case eAICommands.FOR_LOOSENESS_10:
			case eAICommands.FOR_LOOSENESS_15:
			case eAICommands.FOR_LOOSENESS_20:
			case eAICommands.FOR_LOOSENESS_30:
			case eAICommands.FOR_LOOSENESS_40:
			case eAICommands.FOR_LOOSENESS_50:
				m_Expansion_RPCManager.SendRPC("RPC_SetFormationLooseness", new Param1<int>(cmd));
				return true;
			
			case eAICommands.MOV_STOP:
				m_Expansion_RPCManager.SendRPC("RPC_ReqFormStop");
				return true;
			
			case eAICommands.MOV_FLANK:
				m_Expansion_RPCManager.SendRPC("RPC_ReqFormFlank", new Param1<int>(m_MovementSpeedLimit));
				return true;
			
			case eAICommands.MOV_ROAM:
				m_Expansion_RPCManager.SendRPC("RPC_ReqFormRoam", new Param1<int>(m_MovementSpeedLimit));
				return true;
			
			case eAICommands.MOV_RTF:
				m_Expansion_RPCManager.SendRPC("RPC_ReqFormRejoin", new Param1<int>(m_MovementSpeedLimit));
				return true;
			
			case eAICommands.MOV_SETWP:
				vector position = ExpansionStatic.GetCursorHitPos();
				if (position != vector.Zero)
					m_Expansion_RPCManager.SendRPC("RPC_SetWaypoint", new Param1<vector>(position));
				return true;
			
			case eAICommands.DEB_EXPORTPATROL:
				m_Expansion_RPCManager.SendRPC("RPC_ExportPatrol");
				return true;
			
			case eAICommands.MOV_CLEARWP:
				m_Expansion_RPCManager.SendRPC("RPC_ClearWaypoints");
				return true;
			
			case eAICommands.MOV_WALK:
			case eAICommands.MOV_JOG:
			case eAICommands.MOV_SPRINT:
				m_MovementSpeedLimit = cmd - eAICommands.MOV_WALK + 1;
				m_Expansion_RPCManager.SendRPC("RPC_SetMovementSpeed", new Param1<int>(m_MovementSpeedLimit));
				return true;
			
			case eAICommands.MOV_ERECT:
			case eAICommands.MOV_CROUCH:
			case eAICommands.MOV_PRONE:
				m_Expansion_RPCManager.SendRPC("RPC_SetStance", new Param1<int>(cmd - eAICommands.MOV_ERECT));
				return true;
			
			case eAICommands.DEB_TARGET_CREATE:
				eAIBase.Get(0).CreateDebugApple();
				return true;
			
			case eAICommands.DEB_TARGET_DESTROY:
				eAIBase.Get(0).DestroyDebugApple();
				return true;

			case eAICommands.STA_DUMP:
				rpc = m_Expansion_RPCManager.CreateRPC("RPC_DumpState");
				rpc.Expansion_Send(GetAIAtCursorOrNearest(), true);
				return true;

			case eAICommands.STA_SITREP:
				//! @note sitrep should not disclose information that a player couldn't have by looking at his own status
				PlayerBase player;
				if (Class.CastTo(player, g_Game.GetPlayer()))
				{
					bool isAdmin = GetExpansionSettings().GetAI().IsAdmin();

					ai = GetAIAtCursorOrNearest();

					//! If not admin, only allow sitrep from AI in group
					if (ai && !isAdmin && ai.GetGroup() != player.GetGroup())
						ai = null;

					if (!ai && player.GetGroup())
					{
						//! Select 1st AI in group
						for (int i = 0; i < player.GetGroup().Count(); ++i)
						{
							if (Class.CastTo(ai, player.GetGroup().GetMember(i)))
								break;
						}
					}

					if (ai)
					{
					#ifdef DIAG_DEVELOPER
						SitRep_Client(player, ai);
					#endif

						rpc = m_Expansion_RPCManager.CreateRPC("RPC_SitRep");
						rpc.Expansion_Send(ai, true);
					}
				}
				return true;

			case eAICommands.DEB_SPECTATE:
				GetDayZGame().GetExpansionGame().SpectateAI(null, GetAIAtCursorOrNearest(), null);
				return true;

			default:
				if (eAIRegisterFaction.s_FactionTypes.Contains(cmd))
				{
					if (category == eAICommandCategories.CAT_FACTION)
						m_Expansion_RPCManager.SendRPC("RPC_SetFaction", new Param1<int>(cmd));
					else
						SpawnAI(cmd);
					return true;
				}
				else if (cmd >= eAICommands.DEB_DAMAGE && cmd < eAICommands.DEB_DAMAGE_COUNT)
				{
					ai = GetAIAtCursorOrNearest();
					if (ai)
					{
						if (category == eAICommandCategories.CAT_DAMAGE_IN)
							m_DamageIn = cmd;
						else if (category == eAICommandCategories.CAT_DAMAGE_OUT)
							m_DamageOut = cmd;
						else
							m_HeadshotResistance = cmd;

						rpc = m_Expansion_RPCManager.CreateRPC("RPC_SetDamageInOut");
						rpc.Write(cmd);
						rpc.Write(category);
						rpc.Expansion_Send(ai, true);
					}
					return true;
				}
				else if (cmd - eAICommands.BEH_LOOT >= 0)
				{
					int lootingBehavior = cmd - eAICommands.BEH_LOOT;

					if ((m_LootingBehavior & lootingBehavior) == lootingBehavior)
						m_LootingBehavior &= ~lootingBehavior;
					else
						m_LootingBehavior |= lootingBehavior;

					rpc = m_Expansion_RPCManager.SendRPC("RPC_SetLootingBehavior", new Param1<int>(m_LootingBehavior));
					return true;
				}
				break;
		}
		
		return false;
	}

	//! @note returns only alive AI
	eAIBase GetAIAtCursorOrNearest()
	{
		Object target;
		ExpansionStatic.GetCursorHitPos(target);

		eAIBase targetAI;
		if (!Class.CastTo(targetAI, target))
		{
			//! Get nearest AI
			vector cameraPosition = g_Game.GetCurrentCameraPosition();
			float minDistSq = float.MAX;
			array<eAIBase> allAI = eAIBase.eAI_GetAll();
			foreach (eAIBase ai: allAI)
			{
				if (!ai.IsAlive())
					continue;

				float distSq = vector.DistanceSq(cameraPosition, ai.GetPosition());
				if (distSq < minDistSq)
				{
					minDistSq = distSq;
					targetAI = ai;
				}
			}
		}

		return targetAI;
	}

	eAIBase SpawnAI_Helper(PlayerBase leader, string loadout = "HumanLoadout.json")
	{
	#ifdef EXTRACE
		auto trace = EXTrace.Start(EXTrace.AI, this);
	#endif

		auto group = leader.GetGroup();
		if (group)
		{
			leader.Expansion_SetFormerGroup(group);
			loadout = group.GetFaction().GetDefaultLoadout();
		}
		else
		{
			group = eAIGroup.GetGroupByLeader(leader);
		}

		auto form = group.GetFormation();
		vector pos = ExpansionAISpawnBase.GetPlacementPosition(form.ToWorld(form.GetPosition(group.Count())));
		auto ai = SpawnAIEx(pos, loadout);

		if (ai)
			ai.SetGroup(group);

		return ai;
	}
	
	void SpawnAI(int cmd)
	{
		auto rpc = m_Expansion_RPCManager.CreateRPC("RPC_SpawnAI");
		rpc.Write(cmd);
		rpc.Write(ExpansionStatic.GetCursorHitPos());
		rpc.Write(m_MovementSpeedLimit);
		rpc.Write(m_UnlimitedReload);
		rpc.Write(m_LootingBehavior);
		rpc.Write(m_DamageIn);
		rpc.Write(m_DamageOut);
		rpc.Write(m_HeadshotResistance);
		rpc.Expansion_Send(null, true);
	}

	eAIBase SpawnAIEx(vector pos, string loadout = "HumanLoadout.json")
	{
		eAIBase ai;
		if (Class.CastTo(ai, g_Game.CreateObject(GetRandomAI(), pos)))
			ExpansionHumanLoadout.Apply(ai, loadout, true);

		EXTrace.Print(EXTrace.AI, null, "Spawned AI at " + pos);

		return ai;
	}

	eAIBase SpawnAI_Sentry(vector pos, string loadout = "WestLoadout.json")
	{
	#ifdef EXTRACE
		auto trace = EXTrace.Start(EXTrace.AI, this);
	#endif

		return SpawnAIEx(pos, loadout);
	}
	
	eAIBase SpawnAI_Patrol(vector pos, string loadout = "HumanLoadout.json")
	{
	#ifdef EXTRACE
		auto trace = EXTrace.Start(EXTrace.AI, this);
	#endif

		return SpawnAIEx(pos, loadout);
	}
	
	vector GetEntitySpawnPosition(DayZPlayer player)
	{
		vector position, direction, rotation;
		player.GetCurrentCameraTransform(position, direction, rotation);
		position = position + direction * 20;
		float surfaceY = g_Game.SurfaceY(position[0], position[2]);
		if (surfaceY > position[1])
			position[1] = surfaceY;
		return ExpansionAISpawnBase.GetPlacementPosition(position);
	}

	// Server Side: This RPC spawns a helper AI next to the player, and tells them to join the player's formation.
	void RPC_SpawnAI(PlayerIdentity sender, Object target, ParamsReadContext ctx)
	{
	#ifdef EXTRACE
		auto trace = EXTrace.Start(EXTrace.AI, this);
	#endif

		int command;
		if (!ctx.Read(command)) return;

		vector pos;
		if (!ctx.Read(pos)) return;

		int speed;
		if (!ctx.Read(speed)) return;

		int unlimitedReload;
		if (!ctx.Read(unlimitedReload)) return;

		int lootingBehavior;
		if (!ctx.Read(lootingBehavior)) return;

		int damageIn;
		if (!ctx.Read(damageIn)) return;

		int damageOut;
		if (!ctx.Read(damageOut)) return;

		int headshotResistance;
		if (!ctx.Read(headshotResistance)) return;

		if (g_Game.IsMultiplayer())
		{
			if (!GetExpansionSettings().GetAI().IsAdmin(sender))
				return;
		}

		auto player = PlayerBase.ExpansionGetPlayerByIdentity(sender);

		if (pos == vector.Zero)
			pos = GetEntitySpawnPosition(player);
		else
			pos = ExpansionAISpawnBase.GetPlacementPosition(pos);

		eAIBase ai;
		switch (command)
		{
			case eAICommands.DEB_SPAWNALLY:
				ai = SpawnAI_Helper(player);
				eAIGroup group = ai.GetGroup();
				SetMovementSpeed(group, speed);
				ai.eAI_SetUnlimitedReload(unlimitedReload);
				ai.eAI_SetLootingBehavior(lootingBehavior);
				ai.eAI_SetDamageReceivedMultiplier(1.0 - damageIn * (1.0 / eAICommands.DEB_DAMAGE_COUNT));
				ai.eAI_SetDamageMultiplier(1.0 - damageOut * (1.0 / eAICommands.DEB_DAMAGE_COUNT));
				ai.m_eAI_HeadshotResistance = headshotResistance * (1.0 / eAICommands.DEB_DAMAGE_COUNT);
				break;
			default:
				eAIFaction faction = eAIFaction.CreateByID(command);
				ai = SpawnAI_Sentry(pos, faction.GetDefaultLoadout());
				ai.GetGroup().SetFaction(faction);
				break;
		}
	}
	
	void RPC_SpawnZombie(PlayerIdentity sender, Object target, ParamsReadContext ctx)
	{
	#ifdef EXTRACE
		auto trace = EXTrace.Start(EXTrace.AI, this);
	#endif

		vector pos;
		if (!ctx.Read(pos)) return;

		if (g_Game.IsMultiplayer())
		{
			if (!GetExpansionSettings().GetAI().IsAdmin(sender))
				return;
		}

		auto player = PlayerBase.ExpansionGetPlayerByIdentity(sender);

		if (pos == vector.Zero)
			pos = GetEntitySpawnPosition(player);
		else
			pos = ExpansionAISpawnBase.GetPlacementPosition(pos);

		g_Game.CreateObject(ExpansionStatic.GetWorkingZombieClasses().GetRandomElement(), pos, false, true, true);
	}

	void RPC_SpawnWolf(PlayerIdentity sender, Object target, ParamsReadContext ctx)
	{
	#ifdef EXTRACE
		auto trace = EXTrace.Start(EXTrace.AI, this);
	#endif

		vector pos;
		if (!ctx.Read(pos)) return;

		if (g_Game.IsMultiplayer())
		{
			if (!GetExpansionSettings().GetAI().IsAdmin(sender))
				return;
		}

		auto player = PlayerBase.ExpansionGetPlayerByIdentity(sender);

		if (pos == vector.Zero)
			pos = GetEntitySpawnPosition(player);
		else
			pos = ExpansionAISpawnBase.GetPlacementPosition(pos);
		
		g_Game.CreateObject("Animal_CanisLupus_Grey", pos, false, true, true);
	}

	void RPC_SpawnBear(PlayerIdentity sender, Object target, ParamsReadContext ctx)
	{
	#ifdef EXTRACE
		auto trace = EXTrace.Start(EXTrace.AI, this);
	#endif

		vector pos;
		if (!ctx.Read(pos)) return;

		if (g_Game.IsMultiplayer())
		{
			if (!GetExpansionSettings().GetAI().IsAdmin(sender))
				return;
		}

		auto player = PlayerBase.ExpansionGetPlayerByIdentity(sender);

		if (pos == vector.Zero)
			pos = GetEntitySpawnPosition(player);
		else
			pos = ExpansionAISpawnBase.GetPlacementPosition(pos);
	
		g_Game.CreateObject("Animal_UrsusArctos", pos, false, true, true);
	}
	
	// Server Side: Delete AI.
	void RPC_ClearAllAI(PlayerIdentity sender, Object target, ParamsReadContext ctx)
	{
	#ifdef EXTRACE
		auto trace = EXTrace.Start(EXTrace.AI, this);
	#endif

		if (g_Game.IsMultiplayer())
		{
			if (!GetExpansionSettings().GetAI().IsAdmin(sender))
				return;
		}
	
		eAIGroup.Admin_ClearAllAI();
	}
	
	void RPC_SetFaction(PlayerIdentity sender, Object target, ParamsReadContext ctx)
	{
	#ifdef EXTRACE
		auto trace = EXTrace.Start(EXTrace.AI, this);
	#endif

		int factionID;
		if (!ctx.Read(factionID)) return;

		if (g_Game.IsMultiplayer())
		{
			if (!GetExpansionSettings().GetAI().IsAdmin(sender))
				return;
		}

		typename factionType;
		if (!eAIRegisterFaction.s_FactionTypes.Find(factionID, factionType))
			return;

		eAIFaction faction = eAIFaction.Cast(factionType.Spawn());

		PlayerBase player = PlayerBase.ExpansionGetPlayerByIdentity(sender);

		eAIGroup g = player.GetGroup();

		if (g)
			g.SetFaction(faction);
		else
			player.SetGroup(eAIGroup.CreateGroup(faction));
	}

	void RPC_UnlimitedReload(PlayerIdentity sender, Object target, ParamsReadContext ctx)
	{
	#ifdef EXTRACE
		auto trace = EXTrace.Start(EXTrace.AI, this);
	#endif

		int unlimitedReload;
		if (!ctx.Read(unlimitedReload)) return;

		if (g_Game.IsMultiplayer())
		{
			if (!GetExpansionSettings().GetAI().IsAdmin(sender))
				return;
		}

		PlayerBase player = PlayerBase.ExpansionGetPlayerByIdentity(sender);

		eAIGroup g = player.GetGroup();

		if (g)
		{
			for (int i = 0; i < g.Count(); ++i)
			{
				eAIBase ai;
				if (Class.CastTo(ai, g.GetMember(i)))
					ai.eAI_SetUnlimitedReload(unlimitedReload);
			}
		}
		else
		{
			eAIBase.s_eAI_UnlimitedReload = unlimitedReload;

			if (unlimitedReload == eAITargetType.ALL || unlimitedReload == eAITargetType.ALL_OR)
				eAIBase.s_eAI_UnlimitedReloadAll = true;
			else
				eAIBase.s_eAI_UnlimitedReloadAll = false;
		}

		string msg;

		if (unlimitedReload)
		{
			string targetTypes = ExpansionStatic.BitmaskEnumToString(eAITargetType, unlimitedReload);
			targetTypes.Replace("|ALL_OR", "");
			targetTypes.Replace("|", ", ");
			msg = "for targets of type " + targetTypes;
		}
		else
		{
			msg = "OFF";
		}

		if (g)
			msg += " (your group)";
		else
			msg += " (global)";

		ExpansionNotification("EXPANSION AI", "Unlimited reload " + msg).Info(sender);
	}
	
	void RPC_ResetPathfinding(PlayerIdentity sender, Object target, ParamsReadContext ctx)
	{
	#ifdef EXTRACE
		auto trace = EXTrace.Start(EXTrace.AI, this);
	#endif

		if (g_Game.IsMultiplayer())
		{
			if (!GetExpansionSettings().GetAI().IsAdmin(sender))
				return;
		}

		eAIBase ai;
		if (Class.CastTo(ai, target))
			ai.eAI_ResetPathfinding();
	}
	
	void RPC_DebugObjects(PlayerIdentity sender, Object target, ParamsReadContext ctx)
	{
	#ifdef EXTRACE
		auto trace = EXTrace.Start(EXTrace.AI, this);
	#endif

		if (g_Game.IsMultiplayer())
		{
			if (!GetExpansionSettings().GetAI().IsAdmin(sender))
				return;
		}
	
		DayZPlayerImplement.s_Expansion_DebugObjects_Enabled = !DayZPlayerImplement.s_Expansion_DebugObjects_Enabled;

		string onOff;
		if (DayZPlayerImplement.s_Expansion_DebugObjects_Enabled)
		{
			onOff = "ON";

			if (g_Game.IsMultiplayer())
				onOff += "<br/>\nWARNING: Debug objects are server-side and thus visible to anyone!";
		}
		else
		{
			onOff = "OFF";

			CF_DoublyLinkedNode_WeakRef<PlayerBase> node = PlayerBase.s_Expansion_AllPlayers.m_Head;
			while (node)
			{
				node.m_Value.Expansion_DeleteDebugObjects();
				node = node.m_Next;
			}
		}

		ExpansionNotification("EXPANSION AI", "Debug objects " + onOff).Info(sender);
	}
	
	void RPC_DebugDamage(PlayerIdentity sender, Object target, ParamsReadContext ctx)
	{
	#ifdef EXTRACE
		auto trace = EXTrace.Start(EXTrace.AI, this);
	#endif

		if (g_Game.IsMultiplayer())
		{
			if (!GetExpansionSettings().GetAI().IsAdmin(sender))
				return;
		}
	
		DayZPlayerImplement.s_eAI_DebugDamage = !DayZPlayerImplement.s_eAI_DebugDamage;

		string onOff;
		if (DayZPlayerImplement.s_eAI_DebugDamage)
			onOff = "ON";
		else
			onOff = "OFF";

		ExpansionNotification("EXPANSION AI", "Debug damage " + onOff).Info(sender);
	}

	void RPC_SetDamageInOut(PlayerIdentity sender, Object target, ParamsReadContext ctx)
	{
	#ifdef EXTRACE
		auto trace = EXTrace.Start(EXTrace.AI, this);
	#endif

		int cmd;
		if (!ctx.Read(cmd)) return;

		int category;
		if (!ctx.Read(category)) return;

		if (g_Game.IsMultiplayer())
		{
			if (!GetExpansionSettings().GetAI().IsAdmin(sender))
				return;
		}

		eAIBase ai;
		if (Class.CastTo(ai, target))
		{
			eAIGroup group = ai.GetGroup();

			int percent = cmd * (100 / eAICommands.DEB_DAMAGE_COUNT);
			if (category != eAICommandCategories.CAT_HEADSHOTRESISTANCE)
				percent = 100 - percent;
			float multiplier = percent * 0.01;

			for (int i = 0; i < group.Count(); ++i)
			{
				if (Class.CastTo(ai, group.GetMember(i)))
				{
					if (category == eAICommandCategories.CAT_DAMAGE_IN)
						ai.eAI_SetDamageReceivedMultiplier(multiplier);
					else if (category == eAICommandCategories.CAT_DAMAGE_OUT)
						ai.eAI_SetDamageMultiplier(multiplier);
					else
						ai.m_eAI_HeadshotResistance = multiplier;
				}
			}

			string groupdesc;

			if (group.GetName())
				groupdesc = string.Format("\"%1\"", group.GetName());
			else
				groupdesc = string.Format("ID %1", group.GetID());

			PlayerBase leader;
			if (Class.CastTo(leader, group.GetLeader()))
			{
				groupdesc += " of ";

				if (leader.Expansion_IsAI())
					groupdesc += "AI ";
				else
					groupdesc += "player ";

				groupdesc += leader.GetCachedName();
			}

			if (category == eAICommandCategories.CAT_DAMAGE_IN)
				ExpansionNotification("EXPANSION AI", "Damage In " + percent.ToString() + "%% for group " + groupdesc).Info(sender);
			else if (category == eAICommandCategories.CAT_DAMAGE_OUT)
				ExpansionNotification("EXPANSION AI", "Damage Out " + percent.ToString() + "%% for group " + groupdesc).Info(sender);
			else
				ExpansionNotification("EXPANSION AI", "Headshot Resistance " + percent.ToString() + "%% for group " + groupdesc).Info(sender);
		}
	}
	
	void RPC_ReqFormRejoin(PlayerIdentity sender, Object target, ParamsReadContext ctx)
	{
	#ifdef EXTRACE
		auto trace = EXTrace.Start(EXTrace.AI, this);
	#endif

		int speed;
		if (!ctx.Read(speed)) return;

		auto player = PlayerBase.ExpansionGetPlayerByIdentity(sender);

		eAIGroup g = eAIGroup.GetGroupByLeader(player, false);
		g.SetFormationState(eAIGroupFormationState.IN);
		g.SetWaypointBehaviourAuto(eAIWaypointBehavior.ONCE);

		SetMovementSpeed(g, speed);
	}
	
	void RPC_ReqFormStop(PlayerIdentity sender, Object target, ParamsReadContext ctx)
	{
	#ifdef EXTRACE
		auto trace = EXTrace.Start(EXTrace.AI, this);
	#endif

		auto player = PlayerBase.ExpansionGetPlayerByIdentity(sender);

		eAIGroup g = eAIGroup.GetGroupByLeader(player, false);
		g.SetFormationState(eAIGroupFormationState.NONE);
		g.SetWaypointBehaviour(eAIWaypointBehavior.HALT);
	}
	
	void RPC_ReqFormFlank(PlayerIdentity sender, Object target, ParamsReadContext ctx)
	{
	#ifdef EXTRACE
		auto trace = EXTrace.Start(EXTrace.AI, this);
	#endif

		int speed;
		if (!ctx.Read(speed)) return;

		auto player = PlayerBase.ExpansionGetPlayerByIdentity(sender);

		eAIGroup g = eAIGroup.GetGroupByLeader(player, false);

		eAIBase ai;
		for (int i = 0; i < g.Count(); i++)
		{
			eAITarget currentTarget = ai.GetTarget();
			if (Class.CastTo(ai, g.GetMember(i)) && currentTarget && currentTarget.m_SearchOnLOSLost && ai.eAI_IsInFlankRange(currentTarget))
			{
				g.SetFormationState(eAIGroupFormationState.FLANK);
				SetMovementSpeed(g, speed);
				return;
			}
		}

		ExpansionNotification("EXPANSION AI", "No valid targets to flank").Info(sender);
	}
	
	void RPC_ReqFormRoam(PlayerIdentity sender, Object target, ParamsReadContext ctx)
	{
	#ifdef EXTRACE
		auto trace = EXTrace.Start(EXTrace.AI, this);
	#endif

		int speed;
		if (!ctx.Read(speed)) return;

		auto player = PlayerBase.ExpansionGetPlayerByIdentity(sender);

		eAIGroup g = eAIGroup.GetGroupByLeader(player, false);
		g.SetFormationState(eAIGroupFormationState.IN);
		g.SetWaypointBehaviour(eAIWaypointBehavior.ROAMING);

		SetMovementSpeed(g, speed);
	}
	
	void RPC_ReqFormationChange(PlayerIdentity sender, Object target, ParamsReadContext ctx)
	{
	#ifdef EXTRACE
		auto trace = EXTrace.Start(EXTrace.AI, this);
	#endif

		int command;
        if (!ctx.Read(command)) return;

		auto player = PlayerBase.ExpansionGetPlayerByIdentity(sender);

		eAIGroup g = eAIGroup.GetGroupByLeader(player, false);
		eAIFormation newForm;
		switch (command)
		{
			case eAICommands.FOR_INVVEE:
				newForm = new eAIFormationInvVee(g);
				break;
			case eAICommands.FOR_FILE:
				newForm = new eAIFormationFile(g);
				break;
			case eAICommands.FOR_INVFILE:
				newForm = new eAIFormationInvFile(g);
				break;
			case eAICommands.FOR_WALL:
				newForm = new eAIFormationWall(g);
				break;
			case eAICommands.FOR_COL:
				newForm = new eAIFormationColumn(g);
				break;
			case eAICommands.FOR_INVCOL:
				newForm = new eAIFormationInvColumn(g);
				break;
			case eAICommands.FOR_CIRCLE:
				newForm = new eAIFormationCircle(g);
				break;
			case eAICommands.FOR_CIRCLEDOT:
				newForm = new eAIFormationCircleDot(g);
				break;
			case eAICommands.FOR_STAR:
				newForm = new eAIFormationStar(g);
				break;
			case eAICommands.FOR_STARDOT:
				newForm = new eAIFormationStarDot(g);
				break;
			case eAICommands.FOR_VEE:
			default:
				newForm = new eAIFormationVee(g);
				break;
		}

		newForm.SetScale(-1);

		g.SetFormation(newForm);
	}

	void RPC_SetFormationScale(PlayerIdentity sender, Object target, ParamsReadContext ctx)
	{
	#ifdef EXTRACE
		auto trace = EXTrace.Start(EXTrace.AI, this);
	#endif

		int command;
        if (!ctx.Read(command)) return;

		auto player = PlayerBase.ExpansionGetPlayerByIdentity(sender);

		eAIGroup g = eAIGroup.GetGroupByLeader(player, false);

		float scale = command - eAICommands.FOR_SCALE_1X + 1;

		g.GetFormation().SetScale(scale);
	}

	void RPC_SetFormationLooseness(PlayerIdentity sender, Object target, ParamsReadContext ctx)
	{
	#ifdef EXTRACE
		auto trace = EXTrace.Start(EXTrace.AI, this);
	#endif

		int command;
        if (!ctx.Read(command)) return;

		auto player = PlayerBase.ExpansionGetPlayerByIdentity(sender);

		eAIGroup g = eAIGroup.GetGroupByLeader(player, false);

		float looseness;

		switch (command)
		{
			case eAICommands.FOR_LOOSENESS_05:
				looseness = 0.5;
				break;
			case eAICommands.FOR_LOOSENESS_10:
				looseness = 1.0;
				break;
			case eAICommands.FOR_LOOSENESS_15:
				looseness = 1.5;
				break;
			case eAICommands.FOR_LOOSENESS_20:
				looseness = 2.0;
				break;
			case eAICommands.FOR_LOOSENESS_30:
				looseness = 3.0;
				break;
			case eAICommands.FOR_LOOSENESS_40:
				looseness = 4.0;
				break;
			case eAICommands.FOR_LOOSENESS_50:
				looseness = 5.0;
				break;
		}

		g.GetFormation().SetLooseness(looseness);
	}

	void RPC_SetWaypoint(PlayerIdentity sender, Object target, ParamsReadContext ctx)
	{
	#ifdef EXTRACE
		auto trace = EXTrace.Start(EXTrace.AI, this);
	#endif

		vector position;
		if (!ctx.Read(position))
			return;

		position = ExpansionAISpawnBase.GetPlacementPosition(position);

	#ifdef EXTRACE
		EXTrace.Add(trace, position);
	#endif

		auto player = PlayerBase.ExpansionGetPlayerByIdentity(sender);

		eAIGroup g = eAIGroup.GetGroupByLeader(player, false);

		g.AddWaypoint(position);
		g.SetWaypointBehaviourAuto(eAIWaypointBehavior.ONCE);

		eAIBase ai;
		if (g.GetWaypoints().Count() == 1 && Class.CastTo(ai, g.GetFormationLeader()))
			ai.GetPathFinding().ForceRecalculate(true);
	}

	void RPC_ExportPatrol(PlayerIdentity sender, Object target, ParamsReadContext ctx)
	{
	#ifdef EXTRACE
		auto trace = EXTrace.Start(EXTrace.AI, this);
	#endif

		if (g_Game.IsMultiplayer())
		{
			if (!GetExpansionSettings().GetAI().IsAdmin(sender))
				return;
		}

		auto player = PlayerBase.ExpansionGetPlayerByIdentity(sender);

		eAIGroup g = eAIGroup.GetGroupByLeader(player, false);

		eAIWaypointBehavior bhv = g.GetWaypointBehaviour();

		if (bhv == eAIWaypointBehavior.ONCE)
			bhv = eAIWaypointBehavior.ALTERNATE;

		string beh = typename.EnumToString(eAIWaypointBehavior, bhv);

		eAIFaction faction = g.GetFaction();

		ExpansionAIPatrol patrol = new ExpansionAIPatrol(g.Count(), "JOG", "SPRINT", beh, faction.GetName(), faction.GetDefaultLoadout());
		string formationClassName = g.GetFormation().ClassName();
		patrol.Formation = formationClassName.Substring(12, formationClassName.Length() - 12);
		patrol.FormationLooseness = 0.1;
		patrol.Waypoints = g.GetWaypoints();

	#ifdef SERVER
		ExpansionJsonFileParser<ExpansionAIPatrol>.Save("$profile:Expansion_AI_Patrol.json", patrol);
		ExpansionNotification("EXPANSION AI", "Patrol written to \"$profile:Expansion_AI_Patrol.json\".").Info(sender);
	#else
		g_Game.CopyToClipboard(ExpansionJsonFileParser<ExpansionAIPatrol>.AsString(patrol));
		ExpansionNotification("EXPANSION AI", "Patrol copied to clipboard.").Info();
	#endif
	}

	void RPC_ClearWaypoints(PlayerIdentity sender, Object target, ParamsReadContext ctx)
	{
	#ifdef EXTRACE
		auto trace = EXTrace.Start(EXTrace.AI, this);
	#endif

		auto player = PlayerBase.ExpansionGetPlayerByIdentity(sender);

		eAIGroup g = eAIGroup.GetGroupByLeader(player, false);

		g.ClearWaypoints();
	}
	
	void RPC_SetMovementSpeed(PlayerIdentity sender, Object target, ParamsReadContext ctx)
	{
	#ifdef EXTRACE
		auto trace = EXTrace.Start(EXTrace.AI, this);
	#endif

		int speed;
		if (!ctx.Read(speed))
			return;

		auto player = PlayerBase.ExpansionGetPlayerByIdentity(sender);

		eAIGroup g = eAIGroup.GetGroupByLeader(player, false);
		
		SetMovementSpeed(g, speed);
	}

	void SetMovementSpeed(eAIGroup g, int speed)
	{
		eAIBase ai;

		DayZPlayerImplement formationLeader = g.GetFormationLeader();
		if (formationLeader != g.GetLeader())
		{
			if (Class.CastTo(ai, formationLeader))
			{
				if (speed > 0)
				{
					ai.SetMovementSpeedLimits(speed, eAIMovementSpeed.SPRINT);
					if (speed < eAIMovementSpeed.SPRINT)
						speed += 1;  //! Allow other members to catch up with leader
				}
				else
				{
					ai.SetMovementSpeedLimits(0, 0);
				}
			}
		}

		for (int i = 0; i < g.Count(); i++)
		{
			if (Class.CastTo(ai, g.GetMember(i)) && ai != formationLeader)
			{
				if (speed > 0)
					ai.SetMovementSpeedLimits(speed, eAIMovementSpeed.SPRINT);
				else
					ai.SetMovementSpeedLimits(0, 0);
			}
		}
	}
	
	void RPC_SetStance(PlayerIdentity sender, Object target, ParamsReadContext ctx)
	{
	#ifdef EXTRACE
		auto trace = EXTrace.Start(EXTrace.AI, this);
	#endif

		int stance;
		if (!ctx.Read(stance))
			return;

		auto player = PlayerBase.ExpansionGetPlayerByIdentity(sender);

		eAIGroup g = eAIGroup.GetGroupByLeader(player, false);
		
		eAIBase ai;

		for (int i = 0; i < g.Count(); i++)
		{
			if (Class.CastTo(ai, g.GetMember(i)))
				ai.OverrideStance(stance);
		}
	}

	void RPC_DumpState(PlayerIdentity sender, Object target, ParamsReadContext ctx)
	{
	#ifdef EXTRACE
		auto trace = EXTrace.Start(EXTrace.AI, this, "" + target);
	#endif

		auto player = PlayerBase.ExpansionGetPlayerByIdentity(sender);

		TStringArray report;

		eAIBase ai;
		if (Class.CastTo(ai, target))
		{
			if (GetExpansionSettings().GetAI().IsAdmin(sender) || player.GetGroup() == ai.GetGroup())
				report = ai.eAI_DumpState(sender);
		}

		if (!report)
		{
			eAIGroup g = player.GetGroup();

			if (!g)
			{
				ExpansionNotification("EXPANSION AI", "No group and no AI selected, nothing to dump").Error(sender);
				return;
			}

			report = g.DumpState(sender);
		}

		string fileName = string.Format("$profile:Expansion_AI_State_%1.txt", ExpansionStatic.GetISODate(false));
		bool fileExisted = FileExist(fileName);

		FileHandle file = OpenFile(fileName, FileMode.APPEND);
		if (file)
		{
			if (fileExisted)
				FPrintln(file, "");

			FPrintln(file, string.Format("State dump started at %1", ExpansionStatic.GetISODateTime(false, " ", ":", true)));
		}

		int chatLineLimit = 100;
		if (report.Count() > chatLineLimit)
			chatLineLimit = 99;

		foreach (int i, string msg: report)
		{
			if (file)
				FPrintln(file, msg);

			if (i < chatLineLimit)
				g_Game.ChatMP(player, msg, "colorDefault");
		}

		if (report.Count() > chatLineLimit)
			g_Game.ChatMP(player, "WARNING: State dump is too large for chat", "colorAction");

		if (file)
		{
			CloseFile(file);
			ExpansionNotification("EXPANSION AI", string.Format("State dumped to %1", fileName)).Info(sender);
		}
	}
	
	void RPC_ForceWeaponResync(PlayerIdentity sender, Object target, ParamsReadContext ctx)
	{
	#ifdef EXTRACE
		auto trace = EXTrace.Start(EXTrace.AI, this);
	#endif

		if (g_Game.IsMultiplayer())
		{
			if (!GetExpansionSettings().GetAI().IsAdmin(sender))
				return;
		}

		eAIBase ai;
		if (Class.CastTo(ai, target))
		{
			Weapon_Base weapon;
			if (Class.CastTo(weapon, ai.GetHumanInventory().GetEntityInHands()) && !ai.GetWeaponManager().IsRunning())
			{
				weapon.eAI_RemoteRecreate();
				ExpansionNotification("EXPANSION AI", string.Format("Recreated network representation of %1 in hands of %2", weapon, ai)).Info(sender);
			}
		}
	}

	void SitRep_Client(notnull PlayerBase player, notnull eAIBase ai)
	{
		if (GetExpansionSettings().GetAI().IsAdmin(player.GetIdentity()) || player == ai.GetGroup().GetLeader())
		{
			float fVisibility = 1 + Environment.Expansion_GetDynVolFogVisibilityEx(ai.GetPosition()[1]) * 999;
			int iVisibility = Math.Round(fVisibility);
			string sVisibility = iVisibility.ToString();

			vector dir = g_Game.GetCurrentCameraDirection();
			vector ori = dir.VectorToAngles();
			ori[1] = 0;
			ori[2] = 0;
			dir = ori.AnglesToVector();
			vector pos = ai.GetPosition() + dir * fVisibility;
			ai.Expansion_DebugObject(30261796, pos, "ExpansionDebugRodBig_Red");

			//! The following is just for the memes
			string soundSetPrefix = "Expansion_AI_";

			if (ai.ConfigGetBool("woman"))
				soundSetPrefix += "Female_";
			else
				return;  //! Didn't bother to create male soundsets for the meme

			string soundSet;

			PlaySoundOnObject(soundSetPrefix + "i_can_see", ai);
			int delay = 817;

			if (iVisibility < 3)
			{
				soundSet = soundSetPrefix + "jack_shit";
				g_Game.GetCallQueue(CALL_CATEGORY_SYSTEM).CallLater(PlaySoundOnObject, delay, false, soundSet, ai);
			}
			else
			{
				soundSet = string.Format(soundSetPrefix + "%1", sVisibility[0]);
				g_Game.GetCallQueue(CALL_CATEGORY_SYSTEM).CallLater(PlaySoundOnObject, delay, false, soundSet, ai);
				delay += 730;

				if (iVisibility % 1000 == 0)
				{
					soundSet = soundSetPrefix + "thousand";
					g_Game.GetCallQueue(CALL_CATEGORY_SYSTEM).CallLater(PlaySoundOnObject, delay, false, soundSet, ai);
					delay += 943;
				}
				else if (iVisibility % 100 == 0)
				{
					soundSet = soundSetPrefix + "hundred";
					g_Game.GetCallQueue(CALL_CATEGORY_SYSTEM).CallLater(PlaySoundOnObject, delay, false, soundSet, ai);
					delay += 887;
				}
				else
				{
					for (int i = 1; i < sVisibility.Length(); i++)
					{
						soundSet = string.Format(soundSetPrefix + "%1", sVisibility[i]);
						g_Game.GetCallQueue(CALL_CATEGORY_SYSTEM).CallLater(PlaySoundOnObject, delay, false, soundSet, ai);
						delay += 730;
					}
				}

				soundSet = soundSetPrefix + "meters";
				g_Game.GetCallQueue(CALL_CATEGORY_SYSTEM).CallLater(PlaySoundOnObject, delay, false, soundSet, ai);
			}
		}
	}

	void PlaySoundOnObject(string soundSet, Object obj)
	{
		SEffectManager.Expansion_PlaySoundOnObject(soundSet + "_SoundSet", obj);
	}

	void RPC_SitRep(PlayerIdentity sender, Object target, ParamsReadContext ctx)
	{
		PlayerBase player;
		eAIBase ai;
		if (Class.CastTo(player, sender.GetPlayer()) && Class.CastTo(ai, target))
		{
			if (GetExpansionSettings().GetAI().IsAdmin(sender) || player == ai.GetGroup().GetLeader())
			{
				eAIGroup group = ai.GetGroup();

				for (int i = 0; i < group.Count(); ++i)
				{
					if (Class.CastTo(ai, group.GetMember(i)))
						SitRep_Server(player, ai);
				}
			}
		}
	}

	//! @note the number of lines is chosen such that it fits in the Expansion Chat box
	void SitRep_Server(notnull PlayerBase player, notnull eAIBase ai)
	{
		//! Position
		ExpansionPositionKnowledgeType pkt = ai.Expansion_GetPositionKnowledgeType();
		if (pkt != ExpansionPositionKnowledgeType.NONE)
		{
			vector position = ai.GetPosition();
			string positionInfo;

			switch (pkt)
			{
				case ExpansionPositionKnowledgeType.COORDS_GRIDSECTOR:
					array<int> coords = MapNavigationBehaviour.OrderedPositionNumbersFromGridCoords(ai);
					string alt;
					if (coords.Count() == MapNavigationBehaviour.DISPLAY_GRID_POS_MAX_CHARS_COUNT * 2 && coords[0] >= 0)
					{
						positionInfo = string.Format("X %1.%2%3 Y %4.%5%6", coords[0], coords[1], coords[2], coords[3], coords[4], coords[5]);
						alt = string.Format("%1", Math.Round(position[1]));
					}
					else
					{
						positionInfo = "X -.-- Y -.--";
						alt = "---";
					}
					positionInfo = string.Format("Grid %1 alt. %2 m", alt);
					break;

				case ExpansionPositionKnowledgeType.COORDS_METERS:
					positionInfo = string.Format("Pos. %1 : %2 alt. %3 m", Math.Round(position[0]), Math.Round(position[2]), Math.Round(position[1]));
					break;
			}

			if (ai.Expansion_CanKnowGroupMemberDistance() || player.Expansion_GetPositionKnowledgeType() != ExpansionPositionKnowledgeType.NONE)
				player.eAI_Message(ai, "%1 dist. %2 m", positionInfo, Math.Round(vector.Distance(player.GetPosition(), position)).ToString());
			else
				player.eAI_Message(ai, "%1", positionInfo);
		}

		//! Visibility
		string visibility = Math.Round(ai.m_Expansion_Visibility * 1000).ToString();
		string volFogVisibility = Math.Round(ai.m_eAI_DynVolFogVisibility * 1000).ToString();
		player.eAI_Message(ai, "Visibility %1 m (volumetric fog %2 m)", visibility, volFogVisibility);

		//! Health
		string health = Math.Round(ai.GetHealth01() * 100).ToString();
		string blood = Math.Round(ai.GetHealth01("", "Blood") * 100).ToString();
		TStringArray statusEffects = {};
		if (ai.IsUnconscious())
			statusEffects.Insert("unconscious");
		if (ai.IsBleeding())
			statusEffects.Insert(string.Format("bleeding %1", ai.GetBleedingSourceCount()));
		if (ai.GetBrokenLegs() == eBrokenLegs.BROKEN_LEGS)
			statusEffects.Insert("broken legs");
		if (ai.HasDisease())
			statusEffects.Insert("sick");
		if (ai.HasHealings())
			statusEffects.Insert("meds");
		if (statusEffects.Count() > 0)
			player.eAI_Message(ai, "Health %1%% blood %2%% (%3)", health, blood, ExpansionString.JoinStrings(statusEffects));
		else
			player.eAI_Message(ai, "Health %1%% blood %2%%", health, blood);

		//! Weapon
		ItemBase itemInHands = ai.GetItemInHands();
		if (itemInHands)
		{
			Weapon_Base weapon;
			if (Class.CastTo(weapon, itemInHands))
			{
				int ammo = weapon.Expansion_GetMagazineAmmoCount();

				if (ammo == 0 && weapon.Expansion_IsChambered())
					ammo = 1;

				player.eAI_Message(ai, "Weapon %1 (%2 rounds)", weapon.GetType(), ammo.ToString());
			}
			else
			{
				player.eAI_Message(ai, "Item in hands %1", itemInHands.GetType());
			}
		}

		//! Threats
		int threats = ai.m_eAI_AcuteDangerTargetCount + ai.m_eAI_AcuteDangerPlayerTargetCount;
		player.eAI_Message(ai, "Threats %1", threats.ToString());

		//! Current focus
		eAITarget target = ai.GetTarget();
		if (target && (target.m_LOS || ai.m_eAI_IsFightingFSM))
		{
			string desc;
			string info;

			EntityAI targetEntity = target.GetEntity();
			if (targetEntity)
			{
				if (target.IsPlayer())
				{
					if (ai.GetGroup() == DayZPlayerImplement.Cast(targetEntity).GetGroup())
						info = "companion";
					else if (ai.PlayerIsEnemy(targetEntity))
						info = "hostile";
					else
						info = "neutral";
				}
				else if (target.GetThreat() >= 0.4)
				{
					if (target.IsItem())
					{
						if (target.IsHazard())
							info = "hazard";
						else
							info = "loot";
					}
					else
					{
						info = "hostile";
					}
				}
				else
				{
					info = "neutral";
				}

				desc = string.Format("%1 (%2)", ExpansionStatic.FormatString("{1:name}", targetEntity), info);
			}
			else if (target.IsNoise())
			{
				switch (eAINoiseTargetInformation.Cast(target.m_Info).GetNoiseType())
				{
					case eAINoiseType.SHOT:
						desc = "shot";
						break;

					case eAINoiseType.EXPLOSION:
						desc = "explosion";
						break;

					default:
						desc = "noise";
						break;
				}
			}

			player.eAI_Message(ai, "Current focus %1", desc);
		}
	}

	void RPC_SetLootingBehavior(PlayerIdentity sender, Object target, ParamsReadContext ctx)
	{
	#ifdef EXTRACE
		auto trace = EXTrace.Start(EXTrace.AI, this);
	#endif

		int lootingBehavior;
        if (!ctx.Read(lootingBehavior)) return;

		auto player = PlayerBase.ExpansionGetPlayerByIdentity(sender);

		eAIGroup g = player.GetGroup();
		if (g)
		{
			int bitmask = -1;

			for (int i = 0; i < g.Count(); ++i)
			{
				eAIBase ai;
				if (Class.CastTo(ai, g.GetMember(i)))
				{
					if (bitmask == -1)
						bitmask = ai.m_eAI_LootingBehavior;  //! Set all AI to the same looting behavior as the 1st AI in group

					ai.eAI_SetLootingBehavior(lootingBehavior);
				}
			}

			if (bitmask > -1)
			{
				int behavior;
				string msg;

				if ((bitmask & lootingBehavior) == bitmask)
				{
					behavior = lootingBehavior & ~bitmask;  //! The added behavior
					msg = "+";
				}
				else
				{
					behavior = bitmask & ~lootingBehavior;  //! The removed behavior
					msg = "-";
				}

				msg += ExpansionStatic.BitmaskEnumToString(eAILootingBehavior, behavior, ", " + msg);

				ExpansionNotification("EXPANSION AI", "Looting behavior " + msg).Info(sender);
			}
		}
	}
};
