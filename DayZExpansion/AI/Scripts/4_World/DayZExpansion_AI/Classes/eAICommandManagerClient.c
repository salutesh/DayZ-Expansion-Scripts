// This class handles the inputs from the eAICommandMenu locally and shoots RPCs to the server.
class eAICommandManagerClient : eAICommandManager
{
	ref ExpansionRPCManager m_Expansion_RPCManager;

	void eAICommandManagerClient()
	{
		m_Expansion_RPCManager = new ExpansionRPCManager(this);

		m_Expansion_RPCManager.RegisterServer("SpawnAI");
		m_Expansion_RPCManager.RegisterServer("SpawnZombie");
		m_Expansion_RPCManager.RegisterServer("SpawnWolf");
		m_Expansion_RPCManager.RegisterServer("SpawnBear");
		m_Expansion_RPCManager.RegisterServer("ClearAllAI");

		m_Expansion_RPCManager.RegisterServer("ReqFormationChange");
		m_Expansion_RPCManager.RegisterServer("ReqFormRejoin");
		m_Expansion_RPCManager.RegisterServer("ReqFormStop");
	}

	override bool Send(eAICommands cmd)
	{
		switch (cmd)
		{
			case eAICommands.DEB_SPAWNALLY:
			case eAICommands.DEB_SPAWNSENTRY:
			case eAICommands.DEB_SPAWNGUARD:
			case eAICommands.DEB_SPAWNPASSIVE:
			case eAICommands.DEB_SPAWNSHAMAN:
				m_Expansion_RPCManager.SendRPC("SpawnAI", new Param1<int>(cmd));
				return true;
			
			case eAICommands.DEB_CLEARALL:
				m_Expansion_RPCManager.SendRPC("ClearAllAI");
				return true;
			
			case eAICommands.DEB_SPAWNZOM:
				m_Expansion_RPCManager.SendRPC("SpawnZombie");
				return true;
			case eAICommands.DEB_SPAWNWOLF:
				m_Expansion_RPCManager.SendRPC("SpawnWolf");
				return true;
			case eAICommands.DEB_SPAWNBEAR:
				m_Expansion_RPCManager.SendRPC("SpawnBear");
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
				m_Expansion_RPCManager.SendRPC("ReqFormationChange", new Param1<int>(cmd));
				return true;
			
			case eAICommands.MOV_STOP:
				m_Expansion_RPCManager.SendRPC("ReqFormStop");
				return true;
			
			case eAICommands.MOV_RTF:
				m_Expansion_RPCManager.SendRPC("ReqFormRejoin");
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
	
	//! @param owner Who is the manager of this AI
	//! @param formOffset Where should this AI follow relative to the formation?
	eAIBase SpawnAI_Helper(PlayerBase owner, string loadout = "HumanLoadout.json")
	{
		#ifdef EAI_TRACE
		auto trace = CF_Trace_0(this, "SpawnAI_Helper");
		#endif

		auto group = owner.GetGroup();
		if (group)
		{
			owner.Expansion_SetFormerGroup(group);
			loadout = group.GetFaction().GetDefaultLoadout();
		}
		else
		{
			group = eAIGroup.GetGroupByLeader(owner);
		}

		auto form = group.GetFormation();
		vector pos = ExpansionAISpawnBase.GetPlacementPosition(form.ToWorld(form.GetPosition(group.Count())));
		auto ai = SpawnAIEx(pos, loadout);

		if (ai)
			ai.SetGroup(group);

		return ai;
	}
	
	eAIBase SpawnAIEx(vector pos, string loadout = "HumanLoadout.json")
	{
		eAIBase ai;
		if (Class.CastTo(ai, GetGame().CreateObject(GetRandomAI(), pos)))
			ExpansionHumanLoadout.Apply(ai, loadout, true);

		EXTrace.Print(EXTrace.AI, null, "Spawned AI at " + pos);

		return ai;
	}

	eAIBase SpawnAI_Sentry(vector pos, string loadout = "WestLoadout.json")
	{
		#ifdef EAI_TRACE
		auto trace = CF_Trace_0(this, "SpawnAI_Sentry");
		#endif

		return SpawnAIEx(pos, loadout);
	}
	
	eAIBase SpawnAI_Patrol(vector pos, string loadout = "HumanLoadout.json")
	{
		#ifdef EAI_TRACE
		auto trace = CF_Trace_0(this, "SpawnAI_Patrol");
		#endif

		return SpawnAIEx(pos, loadout);
	}
	
	vector GetEntitySpawnPosition(DayZPlayer player)
	{
		vector position, direction, rotation;
		player.GetCurrentCameraTransform(position, direction, rotation);
		position = position + direction * 20;
		float surfaceY = GetGame().SurfaceY(position[0], position[2]);
		if (surfaceY > position[1])
			position[1] = surfaceY;
		return ExpansionAISpawnBase.GetPlacementPosition(position);
	}

	// Server Side: This RPC spawns a helper AI next to the player, and tells them to join the player's formation.
	void SpawnAI(PlayerIdentity sender, Object target, ParamsReadContext ctx)
	{
		#ifdef EAI_TRACE
		auto trace = CF_Trace_0(this, "SpawnAI");
		#endif

		DayZPlayer player = DayZPlayer.Cast(sender.GetPlayer());

		int command;
        if (!ctx.Read(command)) return;

		if (GetGame().IsMultiplayer())
		{
			if (!GetExpansionSettings().GetAI().IsAdmin(sender))
				return;
		}
		
			if (!GetGame().IsMultiplayer()) player = GetGame().GetPlayer();
			
            CF_Log.Debug("eAI: spawn entity RPC called.");
            vector pos = GetEntitySpawnPosition(player);
            eAIBase sentry;
            switch (command)
            {
				case eAICommands.DEB_SPAWNALLY:
					SpawnAI_Helper(PlayerBase.Cast(player));
					break;
				case eAICommands.DEB_SPAWNSENTRY:
					sentry = SpawnAI_Sentry(pos);
					sentry.GetGroup().SetFaction(new eAIFactionMercenaries());
					break;
				case eAICommands.DEB_SPAWNGUARD:
					sentry = SpawnAI_Sentry(pos);
					sentry.GetGroup().SetFaction(new eAIFactionGuards());
					break;
				case eAICommands.DEB_SPAWNPASSIVE:
					sentry = SpawnAI_Sentry(pos);
					sentry.GetGroup().SetFaction(new eAIFactionPassive());
					break;
				case eAICommands.DEB_SPAWNSHAMAN:
					sentry = SpawnAI_Sentry(pos);
					sentry.GetGroup().SetFaction(new eAIFactionShamans());
					break;
			}
	}
	
	// Server Side: This RPC spawns a zombie. It's actually not the right way to do it. But it's only for testing.
	// BUG: this has sometimes crashed us before. Not sure why yet.
	void SpawnZombie(PlayerIdentity sender, Object target, ParamsReadContext ctx)
	{
		#ifdef EAI_TRACE
		auto trace = CF_Trace_0(this, "SpawnZombie");
		#endif

		DayZPlayer player = DayZPlayer.Cast(sender.GetPlayer());

		if (GetGame().IsMultiplayer())
		{
			if (!GetExpansionSettings().GetAI().IsAdmin(sender))
				return;
		}

		if (!GetGame().IsMultiplayer()) Class.CastTo(player, GetGame().GetPlayer());
		
		CF_Log.Debug("eAI: SpawnZombie RPC called.");
		GetGame().CreateObject(ExpansionStatic.GetWorkingZombieClasses().GetRandomElement(), GetEntitySpawnPosition(player), false, true, true);
	}

	void SpawnWolf(PlayerIdentity sender, Object target, ParamsReadContext ctx)
	{
		#ifdef EAI_TRACE
		auto trace = CF_Trace_0(this, "SpawnWolf");
		#endif

		DayZPlayer player = DayZPlayer.Cast(sender.GetPlayer());

		if (GetGame().IsMultiplayer())
		{
			if (!GetExpansionSettings().GetAI().IsAdmin(sender))
				return;
		}

		if (!GetGame().IsMultiplayer()) Class.CastTo(player, GetGame().GetPlayer());
		
		CF_Log.Debug("eAI: SpawnWolf RPC called.");
		GetGame().CreateObject("Animal_CanisLupus_Grey", GetEntitySpawnPosition(player), false, true, true);
	}

	void SpawnBear(PlayerIdentity sender, Object target, ParamsReadContext ctx)
	{
		#ifdef EAI_TRACE
		auto trace = CF_Trace_0(this, "SpawnBear");
		#endif

		DayZPlayer player = DayZPlayer.Cast(sender.GetPlayer());

		if (GetGame().IsMultiplayer())
		{
			if (!GetExpansionSettings().GetAI().IsAdmin(sender))
				return;
		}

		if (!GetGame().IsMultiplayer()) Class.CastTo(player, GetGame().GetPlayer());
		
		CF_Log.Debug("eAI: SpawnBear RPC called.");
		GetGame().CreateObject("Animal_UrsusArctos", GetEntitySpawnPosition(player), false, true, true);
	}
	
	// Server Side: Delete AI.
	void ClearAllAI(PlayerIdentity sender, Object target, ParamsReadContext ctx)
	{
		#ifdef EAI_TRACE
		auto trace = CF_Trace_0(this, "ClearAllAI");
		#endif

		DayZPlayer player = DayZPlayer.Cast(sender.GetPlayer());
		
		if (GetGame().IsMultiplayer())
		{
			if (!GetExpansionSettings().GetAI().IsAdmin(sender))
				return;
		}
	
		if (!GetGame().IsMultiplayer()) Class.CastTo(player, GetGame().GetPlayer());
		
		CF_Log.Debug("eAI: ClearAllAI called.");
		eAIGroup.Admin_ClearAllAI();
	}
	
	void ReqFormRejoin(PlayerIdentity sender, Object target, ParamsReadContext ctx)
	{
		#ifdef EAI_TRACE
		auto trace = CF_Trace_0(this, "ReqFormRejoin");
		#endif

		DayZPlayer player = DayZPlayer.Cast(sender.GetPlayer());

		if (!GetGame().IsMultiplayer()) Class.CastTo(player, GetGame().GetPlayer());
		
		CF_Log.Debug("eAI: ReqFormRejoin called.");
		eAIGroup g = eAIGroup.GetGroupByLeader(DayZPlayerImplement.Cast(player), false);
		g.SetFormationState(eAIGroupFormationState.IN);
	}
	
	void ReqFormStop(PlayerIdentity sender, Object target, ParamsReadContext ctx)
	{
		#ifdef EAI_TRACE
		auto trace = CF_Trace_0(this, "ReqFormStop");
		#endif

		DayZPlayer player = DayZPlayer.Cast(sender.GetPlayer());

		if (!GetGame().IsMultiplayer()) Class.CastTo(player, GetGame().GetPlayer());
		
		CF_Log.Debug("eAI: ReqFormStop called.");
		eAIGroup g = eAIGroup.GetGroupByLeader(DayZPlayerImplement.Cast(player), false);
		g.SetFormationState(eAIGroupFormationState.NONE);
	}
	
	void ReqFormationChange(PlayerIdentity sender, Object target, ParamsReadContext ctx)
	{
		#ifdef EAI_TRACE
		auto trace = CF_Trace_0(this, "ReqFormationChange");
		#endif

		DayZPlayer player = DayZPlayer.Cast(sender.GetPlayer());

		int command;
        if (!ctx.Read(command)) return;

		if (!GetGame().IsMultiplayer()) Class.CastTo(player, GetGame().GetPlayer());
		
		CF_Log.Debug("eAI: ReqFormationChange called.");
		eAIGroup g = eAIGroup.GetGroupByLeader(DayZPlayerImplement.Cast(player), false);
		eAIFormation newForm;
		switch (command)
		{
			case eAICommands.FOR_VEE:
				newForm = new eAIFormationVee(g);
				break;
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
			// no default needed here
		}
		g.SetFormation(newForm);
	}
};