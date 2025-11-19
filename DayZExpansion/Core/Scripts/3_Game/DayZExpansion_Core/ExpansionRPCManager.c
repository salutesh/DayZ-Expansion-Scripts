/**
 * ExpansionRPCManager.c
 *
 * DayZ Expansion Mod
 * www.dayzexpansion.com
 * © 2023 DayZ Expansion Mod Team
 *
 * This work is licensed under the Creative Commons Attribution-NonCommercial-NoDerivatives 4.0 International License. 
 * To view a copy of this license, visit http://creativecommons.org/licenses/by-nc-nd/4.0/.
 *
*/

class ExpansionRPCTypeMapping
{
	//! @note server RPCs are received and executed on server, client RPCs are received and executed on client
	ref map<string, int> m_RegisteredServerRPCIDs = new map<string, int>;
	ref map<int, string> m_RegisteredServerRPCs = new map<int, string>;
	ref map<string, int> m_RegisteredClientRPCIDs = new map<string, int>;
	ref map<int, string> m_RegisteredClientRPCs = new map<int, string>;

	void DebugIDs()
	{
		foreach (string serverRPC, int serverRPCID: m_RegisteredServerRPCIDs)
		{
			PrintFormat("SERVER: %1 %2 %3", ToString(), serverRPC, serverRPCID);
		}

		foreach (string clientRPC, int clientRPCID: m_RegisteredClientRPCIDs)
		{
			PrintFormat("CLIENT: %1 %2 %3", ToString(), clientRPC, clientRPCID);
		}
	}
}

class ExpansionRPCManager
{
	static const typename NULLTYPE;

	static ref map<typename, ExpansionRPCManager> s_TargetlessManagersByType = new map<typename, ExpansionRPCManager>;
	static ref map<int, ExpansionRPCManager> s_RegisteredTargetlessManagers = new map<int, ExpansionRPCManager>;
	static ref map<typename, ref ExpansionRPCTypeMapping> s_TypeMappings = new map<typename, ref ExpansionRPCTypeMapping>;

	protected Managed m_Owner;
	protected bool m_OwnerIsObject;
	protected typename m_Type;
	protected ref ExpansionRPCTypeMapping m_TypeMapping;

	ref map<int, Managed> m_RegisteredInstances = new map<int, Managed>;

	ref ExpansionBitStreamSerializerReader m_Reader;

	void ExpansionRPCManager(Managed owner, typename type = NULLTYPE)
	{
		SetOwner(owner);

		if (type)
			SetType(type);
		else if (owner)
			SetType(owner.Type());
	}

	void ~ExpansionRPCManager()
	{
		if (!g_Game)
			return;

		if (!m_OwnerIsObject)
		{
#ifdef EXTRACE
			auto trace = EXTrace.Start(EXTrace.PROFILING, this, "" + m_Owner, "" + m_Type, "" + s_RegisteredTargetlessManagers.Count());
#endif
			
			if (s_TargetlessManagersByType) {
				s_TargetlessManagersByType.Remove(m_Type);
			}

			foreach (int serverRPCID: m_TypeMapping.m_RegisteredServerRPCIDs)
			{
				s_RegisteredTargetlessManagers.Remove(serverRPCID);
			}

			foreach (int clientRPCID: m_TypeMapping.m_RegisteredClientRPCIDs)
			{
				s_RegisteredTargetlessManagers.Remove(clientRPCID);
			}

#ifdef EXTRACE
			EXTrace.Add(trace, "" + s_RegisteredTargetlessManagers.Count());
#endif
		}
	}

	/**
	 * @brief Get new or existing RPC manager instance
	 * 
	 * @note Use this instead of calling `new ExpansionRPCManager` directly if owner is effectively a singleton,
	 * but is recreated before the existing instance is destroyed.
	 */
	static ExpansionRPCManager Get(Managed owner, typename type = NULLTYPE)
	{
	#ifdef EXTRACE_DIAG
		auto trace = EXTrace.Start(EXTrace.RPC, ExpansionRPCManager, "" + owner, "" + type);
	#endif

		bool ownerIsObject;
		if (owner)
		{
			if (owner.IsInherited(Object))
				ownerIsObject = true;
			else if (!type)
				type = owner.Type();
		}

		if (!s_TargetlessManagersByType) {
			s_TargetlessManagersByType = new map<typename, ExpansionRPCManager>();
		}
		
		ExpansionRPCManager manager;
		if (ownerIsObject || !type || !s_TargetlessManagersByType.Find(type, manager) || !manager)
		{
			manager = new ExpansionRPCManager(owner, type);

			if (!ownerIsObject && type)
				s_TargetlessManagersByType[type] = manager;
		}
		else if (manager.GetOwner() != owner)
		{
			manager.SetOwner(owner);
		}

		return manager;
	}

	void SetOwner(Managed owner)
	{
		m_Owner = owner;
		if (m_Owner && m_Owner.IsInherited(Object))
			m_OwnerIsObject = true;
		else
			m_OwnerIsObject = false;
	}

	void SetType(typename type)
	{
		m_Type = type;

		if (!s_TypeMappings) {
			s_TypeMappings = new map<typename, ref ExpansionRPCTypeMapping>();
		}
		
		if (!s_TypeMappings.Find(m_Type, m_TypeMapping))
		{
			m_TypeMapping = new ExpansionRPCTypeMapping();
			s_TypeMappings[m_Type] = m_TypeMapping;
		}
	}

	Managed GetOwner()
	{
		return m_Owner;
	}

	ExpansionBitStreamReader GetReader(ParamsReadContext ctx)
	{
		if (m_Reader)
		{
			m_Reader.Reset();
			m_Reader.m_Serializer = ctx;
		}
		else
		{
			m_Reader = new ExpansionBitStreamSerializerReader(ctx);
		}

		return m_Reader;
	}

	protected int CreateRPCID(string fn, Managed instance = null)
	{
#ifdef EXTRACE
		auto trace = EXTrace.Start(EXTrace.PROFILING, this);
#endif
		string name;

		if (instance)
		{
			name = instance.ClassName() + "::";
		}
		else if (!m_Owner)
		{
			name = m_Type.ToString() + "::";
		}
		else if (!m_Owner.IsInherited(Object))
		{
			name = m_Owner.ClassName() + "::";
		}

		name += fn;

#ifdef EXTRACE
		EXTrace.Add(trace, name);
#endif

		//int rpcID = ExpansionString.BetterHash(name);
		int rpcID = name.Hash();

#ifdef EXTRACE
		EXTrace.Add(trace, rpcID);
#endif

		return rpcID;
	}

	protected void Register(map<string, int> registeredRPCIDs, map<int, string> registeredRPCs, string fn, int rpcID)
	{
		string registeredFuncName;

		if (!s_RegisteredTargetlessManagers) {
			s_RegisteredTargetlessManagers = new map<int, ExpansionRPCManager>();
		}
		
		if (!m_OwnerIsObject)
		{
			ExpansionRPCManager manager;
			if (s_RegisteredTargetlessManagers.Find(rpcID, manager) && manager)
			{
				if (manager != this || (registeredRPCs.Find(rpcID, registeredFuncName) && registeredFuncName != fn))
				{
					Error("Hash collision: "  + ToString() + " " + fn + " " + manager + " " + registeredFuncName);
					return;
				}
			}
			else
			{
				s_RegisteredTargetlessManagers[rpcID] = this;
			}
		}
		else
		{
			if (registeredRPCs.Find(rpcID, registeredFuncName))
			{
				if (registeredFuncName != fn)
					Error("Hash collision: "  + ToString() + " " + fn + " " + registeredFuncName);
				return;
			}
		}

		registeredRPCIDs[fn] = rpcID;
		registeredRPCs[rpcID] = fn;
	}

	ExpansionRPCTypeMapping GetTypeMapping()
	{
		return m_TypeMapping;
	}

	/**
	 * @brief register RPC
	 * 
	 * @param fn Name of function to register
	 * 
	 * @return non-zero RPC ID
	 */
	int RegisterServer(string fn, Managed instance = null)
	{
#ifdef EXTRACE
		auto trace = EXTrace.Start(EXTrace.PROFILING, this, "" + m_Owner, "" + m_Type, fn, "" + instance);
#endif

		int rpcID = m_TypeMapping.m_RegisteredServerRPCIDs[fn];

		if (!rpcID)
		{
			//! Not yet registered, create RPC ID & register
			rpcID = CreateRPCID(fn, instance);

			if (rpcID)
			{
				Register(m_TypeMapping.m_RegisteredServerRPCIDs, m_TypeMapping.m_RegisteredServerRPCs, fn, rpcID);
			}
		}

		if (instance && rpcID)
			m_RegisteredInstances[rpcID] = instance;

		return rpcID;
	}

	int RegisterClient(string fn, Managed instance = null)
	{
#ifdef EXTRACE
		auto trace = EXTrace.Start(EXTrace.PROFILING, this, "" + m_Owner, "" + m_Type, fn, "" + instance);
#endif

		int rpcID = m_TypeMapping.m_RegisteredClientRPCIDs[fn];

		if (!rpcID)
		{
			//! Not yet registered, create RPC ID & register
			rpcID = CreateRPCID(fn, instance);

			if (rpcID)
			{
				Register(m_TypeMapping.m_RegisteredClientRPCIDs, m_TypeMapping.m_RegisteredClientRPCs, fn, rpcID);
			}
		}

		if (instance && rpcID)
			m_RegisteredInstances[rpcID] = instance;

		return rpcID;
	}

	int RegisterBoth(string fn, Managed instance = null)
	{
#ifdef EXTRACE
		auto trace = EXTrace.Start(EXTrace.PROFILING, this, "" + m_Owner, "" + m_Type, fn, "" + instance);
#endif
		
		int rpcID;
		int serverRPCID = m_TypeMapping.m_RegisteredServerRPCIDs[fn];
		int clientRPCID = m_TypeMapping.m_RegisteredClientRPCIDs[fn];

		//! Not yet registered, create RPC ID & register
		if (!serverRPCID && !clientRPCID)
			rpcID = CreateRPCID(fn, instance);
		else if (serverRPCID)
			rpcID = serverRPCID;
		else
			rpcID = clientRPCID;

		if (!serverRPCID || !clientRPCID)
		{
			if (rpcID)
			{
				if (!serverRPCID)
					Register(m_TypeMapping.m_RegisteredServerRPCIDs, m_TypeMapping.m_RegisteredServerRPCs, fn, rpcID);
				if (!clientRPCID)
					Register(m_TypeMapping.m_RegisteredClientRPCIDs, m_TypeMapping.m_RegisteredClientRPCs, fn, rpcID);
			}
		}

		if (instance && rpcID)
			m_RegisteredInstances[rpcID] = instance;

		return rpcID;
	}

	/**
	 * @brief return registered RPC ID
	 * 
	 * @param fn Name of registered function
	 * 
	 * @return non-zero RPC ID if registered, else zero.
	 */
	int GetRPCID(string fn)
	{
	#ifdef EXPANSION_RPCMANAGER_TRACE
		auto trace = EXTrace.Start(EXTrace.PROFILING, this, fn);
	#endif

		int rpcID;

	#ifdef SERVER
		//! On server, we want to send RPC to client
		rpcID = m_TypeMapping.m_RegisteredClientRPCIDs[fn];

		if (!rpcID)
		{
			m_TypeMapping.DebugIDs();
			Error("No CLIENT RPC registered for type " + m_Type + " and function " + fn);
		}
	#else
		//! On client, we want to send RPC to server
		rpcID = m_TypeMapping.m_RegisteredServerRPCIDs[fn];

		if (!rpcID)
		{
			if (!g_Game.IsMultiplayer())
				rpcID = m_TypeMapping.m_RegisteredClientRPCIDs[fn];

			if (!rpcID)
			{
				m_TypeMapping.DebugIDs();
				Error("No SERVER RPC registered for type " + m_Type + " and function " + fn);
			}
		}
	#endif

		return rpcID;
	}

	string GetRPCName(int rpcID)
	{
		string fn;

	#ifdef SERVER
		fn = m_TypeMapping.m_RegisteredServerRPCs[rpcID];
	#ifdef EXPANSION_RPCMANAGER_TRACE
		EXTrace.Print(EXTrace.PROFILING, this, "::GetRPCName SERVER " + rpcID + " " + fn);
	#endif
	#else
		fn = m_TypeMapping.m_RegisteredClientRPCs[rpcID];
		if (!fn && !g_Game.IsMultiplayer() && !GetDayZGame().IsLoading() && !GetDayZGame().Expansion_IsMissionMainMenu())
			fn = m_TypeMapping.m_RegisteredServerRPCs[rpcID];
	#ifdef EXPANSION_RPCMANAGER_TRACE
		EXTrace.Print(EXTrace.PROFILING, this, "::GetRPCName CLIENT " + rpcID + " " + fn);
	#endif
	#endif

		return fn;
	}

	bool IsRegistered(string fn)
	{
	#ifdef SERVER
		//! On server, we want to send RPC to client
		return m_TypeMapping.m_RegisteredClientRPCIDs.Contains(fn);
	#else
		//! On client, we want to send RPC to server
		return m_TypeMapping.m_RegisteredServerRPCIDs.Contains(fn);
	#endif
	}

	bool IsRegistered(int rpcID)
	{
	#ifdef SERVER
		//! On server, we want to send RPC to client
		return m_TypeMapping.m_RegisteredClientRPCs.Contains(rpcID);
	#else
		//! On client, we want to send RPC to server
		return m_TypeMapping.m_RegisteredServerRPCs.Contains(rpcID);
	#endif
	}

	/**
	 * @brief create RPC
	 * 
	 * @param fn Name of registered function
	 * 
	 * @return ExpansionScriptRPC instance
	 */
	ExpansionScriptRPC CreateRPC(string fn)
	{
	#ifdef EXPANSION_RPCMANAGER_TRACE
		auto trace = EXTrace.Start(EXTrace.PROFILING, this, fn);
	#endif

		return CreateRPC(GetRPCID(fn));
	}

	ExpansionScriptRPC CreateRPC(int rpcID)
	{
	#ifdef EXPANSION_RPCMANAGER_TRACE
		auto trace = EXTrace.Start(EXTrace.PROFILING, this, "" + rpcID);
	#endif

		ExpansionScriptRPC rpc = ExpansionScriptRPC.Create(rpcID);

		if (m_OwnerIsObject)
			rpc.m_Expansion_Target = Object.Cast(m_Owner);

		return rpc;
	}

	ExpansionScriptRPC SendRPC(string fn, Param params = null, bool guaranteed = true, PlayerIdentity recipient = null)
	{
		return SendRPC(GetRPCID(fn), params, guaranteed, recipient);
	}

	ExpansionScriptRPC SendRPC(int rpcID, Param params = null, bool guaranteed = true, PlayerIdentity recipient = null)
	{
		auto rpc = CreateRPC(rpcID);

		if (params)
			params.Serialize(rpc);

		rpc.Expansion_Send(guaranteed, recipient);

		return rpc;
	}

	static bool OnRPC(PlayerIdentity sender, Object target, ParamsReadContext ctx)
	{
	#ifdef EXPANSION_RPCMANAGER_TRACE
		auto trace = EXTrace.Start(EXTrace.PROFILING, ExpansionRPCManager, "" + sender, "" + target);
	#endif

	#ifdef SERVER
		if (!sender)
			return false;

		//if (!sender.GetPlayer())
			//return false;
	#endif

		int rpcID;

		if (!ctx.Read(rpcID))
		{
			Error("Couldn't read Expansion RPC ID");
			return false;
		}

		if (!rpcID)
		{
			Error("Invalid Expansion RPC ID 0");
			return false;
		}

		ExpansionRPCManager manager;

		if (target)
			EnScript.GetClassVar(target, "m_Expansion_RPCManager", 0, manager);

		if (manager && manager.OnRPC(sender, rpcID, ctx))
		{
			return true;
		}
		else
		{
			manager = s_RegisteredTargetlessManagers[rpcID];
			if (manager && manager.OnRPC(sender, target, rpcID, ctx))
				return true;

			EXPrint("Warning: No function registered for target " + target + " manager " + manager + " Expansion RPC ID " + rpcID);
		}

		return false;
	}

	bool OnRPC(PlayerIdentity sender, Object target, int rpcID, ParamsReadContext ctx)
	{
	#ifdef EXPANSION_RPCMANAGER_TRACE
		auto trace = EXTrace.Start(EXTrace.PROFILING, this, "" + sender, "" + target, "" + rpcID);
	#endif

		string fn = GetRPCName(rpcID);

		if (!fn)
			return false;

		Managed instance;
		if (!m_RegisteredInstances.Find(rpcID, instance))
			instance = m_Owner;

		if (!instance)
		{
			Error("Expansion RPC " + rpcID + " " + fn + " has no instance!");
			return false;
		}

	#ifdef EXPANSION_RPCMANAGER_TRACE
		EXTrace.Add(trace, instance);
	#endif

		auto params = new Param3<PlayerIdentity, Object, ParamsReadContext>(sender, target, ctx);
		g_Game.GameScript.CallFunctionParams(instance, fn, null, params);

		return true;
	}

	bool OnRPC(PlayerIdentity sender, int rpcID, ParamsReadContext ctx)
	{
	#ifdef EXPANSION_RPCMANAGER_TRACE
		auto trace = EXTrace.Start(EXTrace.PROFILING, this, "" + sender, "" + rpcID);
	#endif

		string fn = GetRPCName(rpcID);

		if (!fn)
			return false;

		Managed instance;
		if (!m_RegisteredInstances.Find(rpcID, instance))
			instance = m_Owner;

		if (!instance)
		{
			Error("Expansion RPC " + rpcID + " " + fn + " has no instance!");
			return false;
		}

	#ifdef EXPANSION_RPCMANAGER_TRACE
		EXTrace.Add(trace, instance);
	#endif

		auto params = new Param2<PlayerIdentity, ParamsReadContext>(sender, ctx);
		g_Game.GameScript.CallFunctionParams(instance, fn, null, params);

		return true;
	}
}
