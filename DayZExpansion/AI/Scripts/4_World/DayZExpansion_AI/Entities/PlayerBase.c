enum ExpansionPositionKnowledgeType
{
	NONE,
	COORDS_GRIDSECTOR,
	COORDS_METERS
}

modded class PlayerBase
{
	static ref array<PlayerBase> s_eAI_TickSchedulerPlayers = {};

	ref TIntArray m_eAI_FactionModifiers;

	bool m_eAI_IsLit;

#ifndef SERVER
	//! Client only!

	//! Lights the player is in radius of
	ref array<ScriptedLightBase> m_eAI_Lights;

	ref ScriptCaller m_eAI_AddLightIfPlayerInLight = ScriptCaller.Create(eAI_AddLightIfPlayerInLight);

	int m_eAI_ProcessedLights;

	override void CommandHandler(float pDt, int pCurrentCommandID, bool pCurrentCommandFinished)	
	{
		super.CommandHandler(pDt, pCurrentCommandID, pCurrentCommandFinished);

		eAI_ProcessLights();
	}

	void eAI_AddLightIfPlayerInLight(ScriptedLightBase light)
	{
		if (light.Type() != PersonalLight && light.eAI_IsVisible(true) && (light.eAI_IsLightOnPlayer(this) || light.eAI_IsPlayerInLight(this)))
		{
			if (!m_eAI_Lights || m_eAI_Lights.Find(light) == -1)
				eAI_AddLight(light);
		}
	}

	void eAI_RemoveLightsIfPlayerNotInLight(bool isNight = false)
	{
		for (int i = m_eAI_Lights.Count() - 1; i >= 0; --i)
		{
			ScriptedLightBase light = m_eAI_Lights[i];

			if (!light.eAI_IsVisible(isNight) || (!light.eAI_IsLightOnPlayer(this) && !light.eAI_IsPlayerInLight(this)))
				eAI_RemoveLightAtIndex(i);
		}

		if (m_eAI_IsLit && m_eAI_Lights.Count() == 0)
			m_eAI_IsLit = false;
	}

	void eAI_AddLight(ScriptedLightBase light)
	{
		if (!m_eAI_Lights)
			m_eAI_Lights = {};

		int index = m_eAI_Lights.Insert(light);

		light.eAI_AddPlayer(this);

	#ifdef DIAG_DEVELOPER
		EXTrace.Print(EXTrace.AI, this, string.Format("[Lights] added %1 at index %2", light, index));
	#endif

		m_eAI_IsLit = true;
	}

	void eAI_RemoveLight(ScriptedLightBase light, bool removePlayerFromLight = true)
	{
		if (m_eAI_Lights)
		{
			int index = m_eAI_Lights.Find(light);

			eAI_RemoveLightAtIndex(index, removePlayerFromLight);
		}
	}

	void eAI_RemoveLightAtIndex(int index, bool removePlayerFromLight = true)
	{
		ScriptedLightBase light = m_eAI_Lights[index];

		m_eAI_Lights.Remove(index);

		if (removePlayerFromLight)
			light.eAI_RemovePlayer(this);

	#ifdef DIAG_DEVELOPER
		EXTrace.Print(EXTrace.AI, this, string.Format("[Lights] removed %1 at index %2", light, index));
	#endif
	}

	void eAI_ProcessLights()
	{
		bool isLit = m_eAI_IsLit;
		bool isNight = g_Game.GetWorld().IsNight();

		if (isLit)
			eAI_RemoveLightsIfPlayerNotInLight(isNight);

		if (!ScriptedLightBase.s_eAI_LightNodes.m_Current)
		{
		//#ifdef DIAG_DEVELOPER
			//EXTrace.Print(EXTrace.AI, this, string.Format("[Lights] processed %1 lights", m_eAI_ProcessedLights));
		//#endif
			m_eAI_ProcessedLights = 0;
		}

	#ifdef EXTRACE
		auto trace = EXTrace.Profile(EXTrace.AI_PROFILE, ScriptedLightBase, "s_eAI_LightNodes::Each");
	#endif

		if (isNight && !m_eAI_IsLit)
			m_eAI_ProcessedLights += ScriptedLightBase.s_eAI_LightNodes.Each(m_eAI_AddLightIfPlayerInLight, 30);

	#ifdef EXTRACE
		trace = null;
	#endif

		if (m_eAI_IsLit != isLit)
		{
		#ifdef DIAG_DEVELOPER
			MessageStatus(string.Format("[CLIENT] %1 lit %2", GetIdentityName(), m_eAI_IsLit.ToString()));
			EXTrace.Print(EXTrace.AI, this, string.Format("[Lights] %1 lit %2", GetIdentityName(), m_eAI_IsLit.ToString()));
		#endif

			auto rpc = m_Expansion_RPCManager.CreateRPC("RPC_eAI_SetLit");
			rpc.Write(m_eAI_IsLit);
			rpc.Expansion_Send(true);
		}
	}
#endif

	void ~PlayerBase()
	{
		if (!g_Game)
			return;

	#ifndef SERVER
		if (m_eAI_Lights)
		{
			foreach (ScriptedLightBase light: m_eAI_Lights)
			{
				light.eAI_RemovePlayer(this);
			}
		}
	#endif

		if (g_Game.IsServer())
		{
			//! Get players. Vanilla does this in TickScheduler (which we override),
			//! but it's more efficient to do it only when player count actually changes.
			//! We don't use this array, this is just to avoid potential compat issues
			//! with 3rd party mods that may expect it to be filled
			MissionBaseWorld mission;
			if (Class.CastTo(mission, g_Game.GetMission()))
				g_Game.GetCallQueue(CALL_CATEGORY_SYSTEM).Call(mission.eAI_GetPlayers);

			eAI_RemoveFromTickScheduler();
		}
	}

	override void EEKilled(Object killer)
	{
		super.EEKilled(killer);

		eAI_RemoveFromTickScheduler();
	}

	void eAI_RemoveFromTickScheduler()
	{
		if (g_Game.IsServer())
		{
			if (s_eAI_TickSchedulerPlayers)
			{
				int index = s_eAI_TickSchedulerPlayers.Find(this);  //! AI and non-AI players
				if (index > -1)
				{
					s_eAI_TickSchedulerPlayers.Remove(index);
				#ifdef DIAG_DEVELOPER
					EXPrint(this, "TickScheduler players -1 count=" + s_eAI_TickSchedulerPlayers.Count());
				#endif
				}
			}
		}
	}

	override void OnSelectPlayer()
	{
		super.OnSelectPlayer();

		if (g_Game.IsServer())
		{
			s_eAI_TickSchedulerPlayers.Insert(this);  //! non-AI player
		#ifdef DIAG_DEVELOPER
			EXPrint(this, "TickScheduler players +1 count=" + s_eAI_TickSchedulerPlayers.Count());
		#endif
		}
	}

#ifdef EXTRACE_DIAG
	override void OnScheduledTick(float deltaTime)
	{
		auto trace = EXTrace.Profile(EXTrace.AI_PROFILE, this, "OnScheduledTick");

		super.OnScheduledTick(deltaTime);
	}
#endif

	override void Init()
	{
		super.Init();

		if (!IsAI())
			RegisterNetSyncVariableInt("m_eAI_LastAggressionTimeout");

		if (!m_Expansion_RPCManager)
			m_Expansion_RPCManager = new ExpansionRPCManager(this, ExpansionWorld.GetModdableRootType(this));

		m_Expansion_RPCManager.RegisterServer("RPC_eAI_SetLit");
	}

	override void SetActions(out TInputActionMap InputActionMap)
	{
		super.SetActions(InputActionMap);

		AddAction(ExpansionActionRecruitAI, InputActionMap);
		AddAction(ExpansionActionDismissAI, InputActionMap);
		AddAction(ExpansionActionOpenAIInventory, InputActionMap);
	}

	//! @note Only called for players not eAIBase since the latter overrides the commandhandler completely,
	//! formation update if AI is leader is dealt with in eAICommandMove
	override bool ModCommandHandlerInside(float pDt, int pCurrentCommandID, bool pCurrentCommandFinished)
	{
		if (pCurrentCommandID == DayZPlayerConstants.COMMANDID_MOVE && !pCurrentCommandFinished)
		{
			eAIGroup group = GetGroup();
			if (group && group.GetFormationLeader() == this && group.Count() > 1)
				group.GetFormation().Update(pDt);
		}

		return super.ModCommandHandlerInside(pDt, pCurrentCommandID, pCurrentCommandFinished);
	}

	override void OnCommandSwimStart()
	{
#ifdef EXTRACE
		auto trace = EXTrace.Start(EXTrace.AI, this);
#endif 

		super.OnCommandSwimStart();

		if (g_Game.IsServer() && GetGroup())
			GetGroup().EnableSwimming(true);  //! Enable swimming for the whole group so they can follow the leader
	}

	override void OnCommandSwimFinish()
	{
#ifdef EXTRACE
		auto trace = EXTrace.Start(EXTrace.AI, this);
#endif 

		super.OnCommandSwimFinish();

		//if (g_Game.IsServer() && GetGroup())
			//GetGroup().EnableSwimming(false);  //! Disable swimming for the whole group (unless they are currently swimming)
	}

	override void eAI_SetFactionTypeID(int id)
	{
		super.eAI_SetFactionTypeID(id);

		if (g_Game.IsServer())
		{
			eAI_RemoveFactionModifiers();
			if (GetGroup())
				eAI_ApplyFactionModifiers(GetGroup().GetFaction());
		}
	}

	/**
	 * @brief Apply faction modifiers (if any) e.g. brain disease, flies
	 */
	void eAI_ApplyFactionModifiers(eAIFaction faction)
	{
#ifdef EXTRACE
		auto trace = EXTrace.Start(EXTrace.AI,this);
#endif 

		TIntArray modifiers = faction.GetModifiers();

		if (!modifiers || !modifiers.Count())
			return;

#ifdef EXTRACE
		EXTrace.Add(trace, modifiers.Count());
#endif 

		m_eAI_FactionModifiers = {};

		foreach (int modifier: modifiers)
		{
			//! Don't track if already active
			ModifierBase modifierBase = m_ModifiersManager.GetModifier(modifier);
			if (modifierBase && modifierBase.IsActive())
				continue;

			EXTrace.Print(EXTrace.AI, this, typename.EnumToString(eModifiers, modifier));

			switch (modifier)
			{
				case eModifiers.MDF_BRAIN:
					InsertAgent(eAgents.BRAIN, PluginTransmissionAgents.GetAgentMaxCount(eAgents.BRAIN));
					break;
			}

			m_ModifiersManager.ActivateModifier(modifier);
			m_eAI_FactionModifiers.Insert(modifier);
		}
	}

	/**
	 * @brief Remove faction modifiers (if any) e.g. brain disease, flies
	 */
	void eAI_RemoveFactionModifiers()
	{
#ifdef EXTRACE
		auto trace = EXTrace.Start(EXTrace.AI,this);
#endif 

		if (!m_eAI_FactionModifiers)
			return;

#ifdef EXTRACE
		EXTrace.Add(trace, m_eAI_FactionModifiers.Count());
#endif 

		foreach (int modifier: m_eAI_FactionModifiers)
		{
			if (!m_ModifiersManager.GetModifier(modifier).IsActive())
				continue;

			switch (modifier)
			{
				case eModifiers.MDF_BRAIN:
					RemoveAgent(eAgents.BRAIN);
					break;
			}

			m_ModifiersManager.DeactivateModifier(modifier);
		}

		m_eAI_FactionModifiers = null;
	}

	void RPC_eAI_SetLit(PlayerIdentity sender, ParamsReadContext ctx)
	{
		ctx.Read(m_eAI_IsLit);

	#ifdef DIAG_DEVELOPER
		MessageStatus(string.Format("[SERVER] %1 lit %2", GetIdentityName(), m_eAI_IsLit.ToString()));
		EXTrace.Print(EXTrace.AI, this, string.Format("[Lights] %1 lit %2", GetIdentityName(), m_eAI_IsLit.ToString()));
	#endif
	}

	override bool eAI_IsLit()
	{
		return m_eAI_IsLit;
	}

	void Expansion_OnDangerousAreaEnterServer(EffectArea area, Trigger trigger)
	{
	}

	void Expansion_OnDangerousAreaExitServer(EffectArea area, Trigger trigger)
	{
	}

	void Expansion_SynchLifespanVisual()
	{
		if (m_ModuleLifespan)
		{
			m_ModuleLifespan.SynchLifespanVisual(this, m_LifeSpanState, m_HasBloodyHandsVisible, m_HasBloodTypeVisible, m_BloodType);
		}
	}

	static set<PlayerBase> Expansion_GetInCircle(vector center, float radius)
	{
		set<PlayerBase> players = new set<PlayerBase>;

		auto node = s_Expansion_AllPlayers.m_Head;

		while (node)
		{
			if (Math.IsPointInCircle(center, radius, node.m_Value.GetPosition()))
				players.Insert(node.m_Value);

			node = node.m_Next;
		}

		return players;
	}

	bool Expansion_CanAgentGrow(int agentId)
	{
		typename e = EMedicalDrugsType;
		int count = e.GetVariableCount();
		for (int vIdx = 0; vIdx < count; ++vIdx)
		{
			int drugType;
			if (e.GetVariableType(vIdx) == int && e.GetVariableValue(null, vIdx, drugType) && drugType != EMedicalDrugsType.NONE)
			{
				if ((m_MedicalDrugsActive & drugType) == drugType)
				{
					if (!m_AgentPool.m_PluginTransmissionAgents.GrowDuringMedicalDrugsAttack(agentId, drugType, this))
					{
						return false;
					}
				}
			}
		}

		return true;
	}

	bool Expansion_HasEnergy(notnull ExpansionInventoryItemType itemType)
	{
		foreach (ItemBase item: itemType.Items)
		{
			if (item.GetCompEM() && item.GetCompEM().CanWork())
				return true;
		}

		return false;
	}

	ExpansionPositionKnowledgeType Expansion_GetPositionKnowledgeType()
	{
		ExpansionInventoryItemType itemType;

	#ifdef EXPANSIONMODNAVIGATION
		itemType = Expansion_GetInventoryItemType(ExpansionGPS); 
		if (itemType && Expansion_HasEnergy(itemType))
			return ExpansionPositionKnowledgeType.COORDS_METERS;

		auto mapSettings = GetExpansionSettings().GetMap();
		if (mapSettings.EnableMap)
		{
			bool canUseMap = !mapSettings.NeedMapItemForKeyBinding;

			if (!canUseMap && Expansion_GetInventoryCount(ItemMap) > 0)
				canUseMap = true;

			if (canUseMap)
			{
				//! We really should have an enum for ShowPlayerPosition in Navigation mod...
				switch (mapSettings.ShowPlayerPosition)
				{
					case 1:
						//! Player position is visible
						return ExpansionPositionKnowledgeType.COORDS_METERS;

					case 2:
						//! Player position is visible if player has a compass
						if (Expansion_GetInventoryCount(ItemCompass) > 0)
							return ExpansionPositionKnowledgeType.COORDS_METERS;
						break;
				}
			}
		}
	#endif

		itemType = Expansion_GetInventoryItemType(GPSReceiver); 
		if (itemType && Expansion_HasEnergy(itemType))
			return ExpansionPositionKnowledgeType.COORDS_GRIDSECTOR;

		return ExpansionPositionKnowledgeType.NONE;
	}

	bool Expansion_CanKnowGroupMemberDistance()
	{
	#ifdef EXPANSIONMODGROUPS
		auto groupSettings = GetExpansionSettings().GetParty();

		if (groupSettings.ShowPartyMember3DMarkers && groupSettings.ShowDistanceUnderPartyMembersMarkers)
			return true;

		if (groupSettings.ShowPartyMemberHUD && groupSettings.ShowHUDMemberDistance)
			return true;
	#endif

		return false;
	}

	/**
	 * @brief Check if other player is considered a helper.
	 * 
	 * Currently, AI that are friendly towards the player as well as guards not actively threatened by the player are considered helpers.
	 */
	override bool Expansion_IsHelper(PlayerBase other, bool checkIfWeAreHelper = false)
	{
		if (super.Expansion_IsHelper(other, checkIfWeAreHelper))
			return true;

		eAIBase ai;
		if (Class.CastTo(ai, other))
		{
			if (!ai.PlayerIsEnemy(this))
				return true;

			if (ai.GetGroup().GetFaction().IsGuard() && ai.eAI_GetCachedThreat(GetTargetInformation()) < 0.4)
				return true;
		}
		else if (checkIfWeAreHelper && Class.CastTo(ai, this))
		{
			if (!ai.PlayerIsEnemy(other))
				return true;

			if (ai.GetGroup().GetFaction().IsGuard() && ai.eAI_GetCachedThreat(other.GetTargetInformation()) < 0.4)
				return true;
		}

		return false;
	}

	void eAI_Message(eAIBase sender, string fmt, Class p1 = null, Class p2 = null, Class p3 = null, Class p4 = null, Class p5 = null, Class p6 = null, Class p7 = null, Class p8 = null, Class p9 = null)
	{
		string text = ExpansionStatic.FormatString(fmt, p1, p2, p3, p4, p5, p6, p7, p8, p9);

		eAI_Message(sender, "%1", text);
	}

	void eAI_Message(eAIBase sender, string fmt, string p1 = "", string p2 = "", string p3 = "", string p4 = "", string p5 = "", string p6 = "", string p7 = "", string p8 = "", string p9 = "")
	{
		string name = sender.GetCachedName();
		string body = string.Format(fmt, p1, p2, p3, p4, p5, p6, p7, p8, p9);
		string text = string.Format("%1: %2", name, body);

		string style;
		
		if (sender.GetGroup() == GetGroup())
			style = "friendlyAI";
		else
			style = "AI";

		Message(text, style);
	}
};
