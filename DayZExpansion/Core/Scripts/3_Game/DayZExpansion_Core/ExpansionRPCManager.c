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

	static ref map<int, ExpansionRPCManager> s_RegisteredTargetlessManagers = new map<int, ExpansionRPCManager>;
	static ref map<typename, ref ExpansionRPCTypeMapping> s_TypeMappings = new map<typename, ref ExpansionRPCTypeMapping>;

	protected Class m_Owner;
	protected typename m_Type;
	protected ref ExpansionRPCTypeMapping m_TypeMapping;

	ref map<int, Class> m_RegisteredInstances = new map<int, Class>;

	void ExpansionRPCManager(Class owner, typename type = NULLTYPE)
	{
		SetOwner(owner);

		if (type)
			SetType(type);
		else if (owner)
			SetType(owner.Type());
	}

	void ~ExpansionRPCManager()
	{
		if (!GetGame())
			return;

		if (!m_Owner || !m_Owner.IsInherited(Object))
		{
			auto trace = EXTrace.Start(EXTrace.PROFILING, this, "" + m_Owner, "" + s_RegisteredTargetlessManagers.Count());

			foreach (int serverRPCID: m_TypeMapping.m_RegisteredServerRPCIDs)
			{
				s_RegisteredTargetlessManagers.Remove(serverRPCID);
			}

			foreach (int clientRPCID: m_TypeMapping.m_RegisteredClientRPCIDs)
			{
				s_RegisteredTargetlessManagers.Remove(clientRPCID);
			}

			EXTrace.Add(trace, "" + s_RegisteredTargetlessManagers.Count());
		}
	}

	void SetOwner(Class owner)
	{
		m_Owner = owner;
	}

	void SetType(typename type)
	{
		m_Type = type;

		if (!s_TypeMappings.Find(m_Type, m_TypeMapping))
		{
			m_TypeMapping = new ExpansionRPCTypeMapping();
			s_TypeMappings[m_Type] = m_TypeMapping;
		}
	}

	protected int CreateRPCID(string fn, Class instance = null)
	{
		auto trace = EXTrace.Start(EXTrace.PROFILING, this);

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

		EXTrace.Add(trace, name);

		//int rpcID = ExpansionString.BetterHash(name);
		int rpcID = name.Hash();

		EXTrace.Add(trace, rpcID);

		return rpcID;
	}

	protected void Register(map<string, int> registeredRPCIDs, map<int, string> registeredRPCs, string fn, int rpcID)
	{
		if (!m_Owner || !m_Owner.IsInherited(Object))
		{
			ExpansionRPCManager manager;
			if (s_RegisteredTargetlessManagers.Find(rpcID, manager))
			{
				if (manager != this)
				{
					Error("Hash collision: "  + ToString() + " " + fn + " " + manager);
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
			string registeredFuncName;
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
	int RegisterServer(string fn, Class instance = null)
	{
		auto trace = EXTrace.Start(EXTrace.PROFILING, this, "" + m_Owner, "" + m_Type, fn, "" + instance);

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

	int RegisterClient(string fn, Class instance = null)
	{
		auto trace = EXTrace.Start(EXTrace.PROFILING, this, "" + m_Owner, "" + m_Type, fn, "" + instance);

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

	int RegisterBoth(string fn, Class instance = null)
	{
		auto trace = EXTrace.Start(EXTrace.PROFILING, this, "" + m_Owner, "" + m_Type, fn, "" + instance);

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
			if (!GetGame().IsMultiplayer())
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
		if (!fn && !GetGame().IsMultiplayer())
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

		if (m_Owner && m_Owner.IsInherited(Object))
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

		Class instance;
		if (!m_RegisteredInstances.Find(rpcID, instance))
			instance = m_Owner;

		if (!instance)
		{
			Error("Expansion RPC " + rpcID + " " + fn + " has no instance!");
			return false;
		}

		auto params = new Param3<PlayerIdentity, Object, ParamsReadContext>(sender, target, ctx);
		GetGame().GameScript.CallFunctionParams(instance, fn, null, params);

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

		Class instance;
		if (!m_RegisteredInstances.Find(rpcID, instance))
			instance = m_Owner;

		if (!instance)
		{
			Error("Expansion RPC " + rpcID + " " + fn + " has no instance!");
			return false;
		}

		auto params = new Param2<PlayerIdentity, ParamsReadContext>(sender, ctx);
		GetGame().GameScript.CallFunctionParams(instance, fn, null, params);

		return true;
	}
}
