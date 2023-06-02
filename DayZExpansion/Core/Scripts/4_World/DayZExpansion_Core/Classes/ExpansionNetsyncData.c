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

	void ExpansionNetsyncData(Object object)
	{
		m_Object = object;

		if (GetGame().IsClient())
		{
			SI_Receive = new ScriptInvoker();
			GetGame().GetCallQueue(CALL_CATEGORY_SYSTEM).CallLater(Request, 250);  //! Has to be delayed, else won't work!
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
		auto rpc = ExpansionScriptRPC.Create();
		rpc.Send(m_Object, ExpansionEntityRPC.NetsyncData, true, null);
	}

	//! Send name override to client
	void Send(PlayerIdentity recipient)
	{
		if (!m_Data || !m_Data.Count())
			return;

		if (recipient)
			EXTrace.Print(EXTrace.MISC, m_Object, "Sending " + m_Data.Count() + " netsync data entries to " + recipient.GetId());
		else
			EXTrace.Print(EXTrace.MISC, m_Object, "Sending " + m_Data.Count() + " netsync data entries to all players");

		auto rpc = ExpansionScriptRPC.Create();
		rpc.Write(m_Data.Count());
		foreach (string value: m_Data)
		{
			rpc.Write(value);
		}
		rpc.Send(m_Object, ExpansionEntityRPC.NetsyncData, true, recipient);
	}

	void Receive(ParamsReadContext ctx)
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

	void OnRPC(PlayerIdentity sender, int rpc_type, ParamsReadContext ctx)
	{
		switch (rpc_type)
		{
			case ExpansionEntityRPC.NetsyncData:
				if (!ExpansionScriptRPC.CheckMagicNumber(ctx))
					return;
				if (GetGame().IsClient())
					Receive(ctx);
				else if (sender)
					Send(sender);
				break;
		}
	}
}
