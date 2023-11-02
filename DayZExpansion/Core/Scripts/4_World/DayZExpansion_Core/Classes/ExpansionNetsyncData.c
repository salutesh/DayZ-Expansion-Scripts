/**
 * ExpansionNetsyncData.c
 *
 * DayZ Expansion Mod
 * www.dayzexpansion.com
 * © 2022 DayZ Expansion Mod Team
 *
 * This work is licensed under the Creative Commons Attribution-NonCommercial-NoDerivatives 4.0 International License. 
 * To view a copy of this license, visit http://creativecommons.org/licenses/by-nc-nd/4.0/.
 *
*/

class ExpansionNetsyncData
{
	Object m_Object;
	ref TStringArray m_Data;
	ref ScriptInvoker SI_Receive;
	ref ExpansionRPCManager m_Expansion_RPCManager;
	bool m_WasDataRequested;

	void ExpansionNetsyncData(Object object)
	{
		m_Object = object;

		EnScript.GetClassVar(object, "m_Expansion_RPCManager", 0, m_Expansion_RPCManager);
		if (!m_Expansion_RPCManager)
		{
			EntityAI entity;
			typename type;
			if (Class.CastTo(entity, object))
				type = ExpansionWorld.GetModdableRootType(entity);
			m_Expansion_RPCManager = new ExpansionRPCManager(object, type);
			EnScript.SetClassVar(object, "m_Expansion_RPCManager", 0, m_Expansion_RPCManager);
		}

		m_Expansion_RPCManager.RegisterServer("Send", this);
		m_Expansion_RPCManager.RegisterClient("Receive", this);

		if (GetGame().IsClient())
		{
			SI_Receive = new ScriptInvoker();
			GetGame().GetCallQueue(CALL_CATEGORY_SYSTEM).CallLater(LateClientInit, 250);  //! Has to be delayed, else won't work!
		}
	}

	/**
	 * @brief Late client initialization for entities w/o netsynced vars (others will request netsync data in OnVariablesSynchronized)
	 * 
	 * @note if you take an item to hand, vanilla will create a temporary copy of the item on client only.
	 * We don't want to request netsync data for that item, since the RPC will go into nowhere on server.
	 * We can detect such an item by checking its network ID. If it's client only, net ID will be zero.
	 */
	void LateClientInit()
	{
		if (!m_WasDataRequested && m_Object)
		{
			int low, high;
			m_Object.GetNetworkID(low, high);
			EXTrace.Print(EXTrace.MISC, m_Object, "Netsync data late client init - network ID " + low + " " + high);
			if (low || high)
				Request();
		}
	}

	void Set(int index, string value)
	{
		if (!m_Data)
			m_Data = new TStringArray;
		if (index < m_Data.Count())
			m_Data[index] = value;
		else
			m_Data.Insert(value);
		EXTrace.Print(EXTrace.MISC, m_Object, "Set netsync data " + index + " '" + m_Data[index] + "'");
	}

	bool Get(int index, out string output)
	{
		if (m_Data)
		{
			string value = m_Data[index];
			if (value)
			{
				output = value;
				return true;
			}
		}

		return false;
	}

	//! Request name override from server
	void Request()
	{
		EXTrace.Print(EXTrace.MISC, m_Object, "Requesting netsync data");
		m_Expansion_RPCManager.SendRPC("Send");
		m_WasDataRequested = true;
	}

	//! Send name override to client
	void Send(PlayerIdentity recipient, ParamsReadContext ctx = null)
	{
		if (!m_Data || !m_Data.Count())
			return;

		if (recipient)
			EXTrace.Print(EXTrace.MISC, m_Object, "Sending " + m_Data.Count() + " netsync data entries to " + recipient.GetId());
		else
			EXTrace.Print(EXTrace.MISC, m_Object, "Sending " + m_Data.Count() + " netsync data entries to all players");

		auto rpc = m_Expansion_RPCManager.CreateRPC("Receive");
		rpc.Write(m_Data.Count());
		foreach (string value: m_Data)
		{
			rpc.Write(value);
		}
		rpc.Expansion_Send(true, recipient);
	}

	void Receive(PlayerIdentity sender, ParamsReadContext ctx)
	{
		int count;
		ctx.Read(count);

		string value;
		m_Data = new TStringArray;
		while (count)
		{
			ctx.Read(value);
			m_Data.Insert(value);
			count--;
		}

		if (SI_Receive)
			SI_Receive.Invoke();

		EXTrace.Print(EXTrace.MISC, m_Object, "Received " + m_Data.Count() + " netsync data entries");
	}
}
