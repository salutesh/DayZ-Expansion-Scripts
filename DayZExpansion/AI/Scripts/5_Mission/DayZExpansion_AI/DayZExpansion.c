/**
 * DayZExpansion.c
 * 
 * Partly based on Enfusion AI Project Copyright 2021 William Bowers
 *
 * DayZ Expansion Mod
 * www.dayzexpansion.com
 * © 2022 DayZ Expansion Mod Team
 *
 * This work is licensed under the Creative Commons Attribution-NonCommercial-NoDerivatives 4.0 International License. 
 * To view a copy of this license, visit http://creativecommons.org/licenses/by-nc-nd/4.0/.
 *
*/

modded class DayZExpansion
{
	private static DayZExpansion m_Instance_5; //! weak ref
	
    void DayZExpansion()
	{
		#ifdef EAI_TRACE
		auto trace = CF_Trace_0(this, "DayZExpansion");
		#endif

		m_Instance_5 = this;
		
		GetRPCManager().AddRPC("eAI", "SpawnAI", this, SingeplayerExecutionType.Server);
		GetRPCManager().AddRPC("eAI", "SpawnZombie", this, SingeplayerExecutionType.Server);
		GetRPCManager().AddRPC("eAI", "SpawnWolf", this, SingeplayerExecutionType.Server);
		GetRPCManager().AddRPC("eAI", "SpawnBear", this, SingeplayerExecutionType.Server);
		GetRPCManager().AddRPC("eAI", "ClearAllAI", this, SingeplayerExecutionType.Server);

		GetRPCManager().AddRPC("eAI", "ReqFormationChange", this, SingeplayerExecutionType.Server);
		GetRPCManager().AddRPC("eAI", "ReqFormRejoin", this, SingeplayerExecutionType.Server);
		GetRPCManager().AddRPC("eAI", "ReqFormStop", this, SingeplayerExecutionType.Server);
    }

	static DayZExpansion Get5()
	{
		#ifdef EAI_TRACE
		auto trace = CF_Trace_0("DayZExpansion", "Get5");
		#endif
		
		return m_Instance_5;
	}
	
	//! @param owner Who is the manager of this AI
	//! @param formOffset Where should this AI follow relative to the formation?
	eAIBase SpawnAI_Helper(PlayerBase owner, string loadout = "HumanLoadout.json")
	{
		#ifdef EAI_TRACE
		auto trace = CF_Trace_0(this, "SpawnAI_Helper");
		#endif

		auto group = eAIGroup.GetGroupByLeader(owner);
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
	void SpawnAI(CallType type, ParamsReadContext ctx, PlayerIdentity sender, Object target)
	{
		#ifdef EAI_TRACE
		auto trace = CF_Trace_0(this, "SpawnAI");
		#endif

		Param2<DayZPlayer, int> data;
        if (!ctx.Read(data)) return;

		if (GetGame().IsMultiplayer())
		{
			if (!GetExpansionSettings().GetAI().IsAdmin(sender))
				return;
		}
		
		if(type == CallType.Server )
		{
			if (!GetGame().IsMultiplayer()) data.param1 = GetGame().GetPlayer();
			
            CF_Log.Debug("eAI: spawn entity RPC called.");
            vector pos = GetEntitySpawnPosition(data.param1);
            eAIBase sentry;
            switch (data.param2)
            {
				case eAICommands.DEB_SPAWNALLY:
					SpawnAI_Helper(PlayerBase.Cast(data.param1));
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
	}
	
	// Server Side: This RPC spawns a zombie. It's actually not the right way to do it. But it's only for testing.
	// BUG: this has sometimes crashed us before. Not sure why yet.
	void SpawnZombie(CallType type, ParamsReadContext ctx, PlayerIdentity sender, Object target)
	{
		#ifdef EAI_TRACE
		auto trace = CF_Trace_0(this, "SpawnZombie");
		#endif

		Param1<DayZPlayer> data;
        if (!ctx.Read(data)) return;

		if (GetGame().IsMultiplayer())
		{
			if (!GetExpansionSettings().GetAI().IsAdmin(sender))
				return;
		}

		if(type == CallType.Server) {
			if (!GetGame().IsMultiplayer()) Class.CastTo(data.param1, GetGame().GetPlayer());
			
            CF_Log.Debug("eAI: SpawnZombie RPC called.");
			GetGame().CreateObject(ExpansionStatic.GetWorkingZombieClasses().GetRandomElement(), GetEntitySpawnPosition(data.param1), false, true, true);
        }
	}

	void SpawnWolf(CallType type, ParamsReadContext ctx, PlayerIdentity sender, Object target)
	{
		#ifdef EAI_TRACE
		auto trace = CF_Trace_0(this, "SpawnWolf");
		#endif

		Param1<DayZPlayer> data;
        if (!ctx.Read(data)) return;

		if (GetGame().IsMultiplayer())
		{
			if (!GetExpansionSettings().GetAI().IsAdmin(sender))
				return;
		}

		if(type == CallType.Server) {
			if (!GetGame().IsMultiplayer()) Class.CastTo(data.param1, GetGame().GetPlayer());
			
            CF_Log.Debug("eAI: SpawnWolf RPC called.");
			GetGame().CreateObject("Animal_CanisLupus_Grey", GetEntitySpawnPosition(data.param1), false, true, true);
        }
	}

	void SpawnBear(CallType type, ParamsReadContext ctx, PlayerIdentity sender, Object target)
	{
		#ifdef EAI_TRACE
		auto trace = CF_Trace_0(this, "SpawnBear");
		#endif

		Param1<DayZPlayer> data;
        if (!ctx.Read(data)) return;

		if (GetGame().IsMultiplayer())
		{
			if (!GetExpansionSettings().GetAI().IsAdmin(sender))
				return;
		}

		if(type == CallType.Server) {
			if (!GetGame().IsMultiplayer()) Class.CastTo(data.param1, GetGame().GetPlayer());
			
            CF_Log.Debug("eAI: SpawnBear RPC called.");
			GetGame().CreateObject("Animal_UrsusArctos", GetEntitySpawnPosition(data.param1), false, true, true);
        }
	}
	
	// Server Side: Delete AI.
	void ClearAllAI(CallType type, ParamsReadContext ctx, PlayerIdentity sender, Object target)
	{
		#ifdef EAI_TRACE
		auto trace = CF_Trace_0(this, "ClearAllAI");
		#endif

		Param1<PlayerBase> data;
        if (!ctx.Read(data)) return;
		
		if (GetGame().IsMultiplayer())
		{
			if (!GetExpansionSettings().GetAI().IsAdmin(sender))
				return;
		}
		
		if (type == CallType.Server)
		{
			if (!GetGame().IsMultiplayer()) Class.CastTo(data.param1, GetGame().GetPlayer());
			
            CF_Log.Debug("eAI: ClearAllAI called.");
			eAIGroup.Admin_ClearAllAI();
		}
	}
	
	void ReqFormRejoin(CallType type, ParamsReadContext ctx, PlayerIdentity sender, Object target)
	{
		#ifdef EAI_TRACE
		auto trace = CF_Trace_0(this, "ReqFormRejoin");
		#endif

		Param1<DayZPlayer> data;
        if (!ctx.Read(data)) return;
		if(type == CallType.Server)
		{
			if (!GetGame().IsMultiplayer()) Class.CastTo(data.param1, GetGame().GetPlayer());
			
			CF_Log.Debug("eAI: ReqFormRejoin called.");
			eAIGroup g = eAIGroup.GetGroupByLeader(DayZPlayerImplement.Cast(data.param1), false);
			g.SetFormationState(eAIGroupFormationState.IN);
		}
	}
	
	void ReqFormStop(CallType type, ParamsReadContext ctx, PlayerIdentity sender, Object target)
	{
		#ifdef EAI_TRACE
		auto trace = CF_Trace_0(this, "ReqFormStop");
		#endif

		Param1<DayZPlayer> data;
        if (!ctx.Read(data)) return;
		if(type == CallType.Server)
		{
			if (!GetGame().IsMultiplayer()) Class.CastTo(data.param1, GetGame().GetPlayer());
			
			CF_Log.Debug("eAI: ReqFormStop called.");
			eAIGroup g = eAIGroup.GetGroupByLeader(DayZPlayerImplement.Cast(data.param1), false);
			g.SetFormationState(eAIGroupFormationState.NONE);
		}
	}
	
	void ReqFormationChange(CallType type, ParamsReadContext ctx, PlayerIdentity sender, Object target)
	{
		#ifdef EAI_TRACE
		auto trace = CF_Trace_0(this, "ReqFormationChange");
		#endif

		Param2<DayZPlayer, int> data;
        if (!ctx.Read(data)) return;
		if(type == CallType.Server)
		{
			if (!GetGame().IsMultiplayer()) Class.CastTo(data.param1, GetGame().GetPlayer());
			
			CF_Log.Debug("eAI: ReqFormationChange called.");
			eAIGroup g = eAIGroup.GetGroupByLeader(DayZPlayerImplement.Cast(data.param1), false);
			eAIFormation newForm;
			switch (data.param2)
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
	}
};
