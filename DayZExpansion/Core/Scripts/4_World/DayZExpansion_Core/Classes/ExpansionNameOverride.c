/**
 * ExpansionNameOverride.c
 *
 * DayZ Expansion Mod
 * www.dayzexpansion.com
 * © 2022 DayZ Expansion Mod Team
 *
 * This work is licensed under the Creative Commons Attribution-NonCommercial-NoDerivatives 4.0 International License. 
 * To view a copy of this license, visit http://creativecommons.org/licenses/by-nc-nd/4.0/.
 *
*/

class ExpansionNameOverride
{
	Object m_Object;
	string m_NameOverride;

	void ExpansionNameOverride(Object object)
	{
		m_Object = object;

		if (GetGame().IsClient())
			GetGame().GetCallQueue(CALL_CATEGORY_SYSTEM).CallLater(Request, 250);  //! Has to be delayed, else won't work!
	}

	void Set(string name)
	{
		m_NameOverride = name;
	}

	bool Get(out string output)
	{
		if (m_NameOverride)
		{
			output = m_NameOverride;
			return true;
		}

		return false;
	}

	//! Request name override from server
	void Request()
	{
		EXTrace.Print(EXTrace.ENABLE, m_Object, "Requesting name override");
		ScriptRPC rpc = new ScriptRPC();
		rpc.Send(m_Object, ExpansionEntityRPC.NameOverride, true, null);
	}

	//! Send name override to client
	void Send(PlayerIdentity recipient)
	{
		if (!m_NameOverride)
			return;

		EXTrace.Print(EXTrace.ENABLE, m_Object, "Sending name override '" + m_NameOverride + "' to " + recipient.GetId());

		ScriptRPC rpc = new ScriptRPC();
		rpc.Write(m_NameOverride);
		rpc.Send(m_Object, ExpansionEntityRPC.NameOverride, true, recipient);
	}

	void Receive(ParamsReadContext ctx)
	{
		ctx.Read(m_NameOverride);

		EXTrace.Print(EXTrace.ENABLE, m_Object, "Received name override '" + m_NameOverride + "'");
	}

	void OnRPC(PlayerIdentity sender, int rpc_type, ParamsReadContext ctx)
	{
		switch (rpc_type)
		{
			case ExpansionEntityRPC.NameOverride:
				if (GetGame().IsClient())
					Receive(ctx);
				else
					Send(sender);
				break;
		}
	}
}
