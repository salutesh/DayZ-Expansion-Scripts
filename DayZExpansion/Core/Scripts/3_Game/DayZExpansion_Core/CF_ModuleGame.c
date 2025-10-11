/**
 * CF_ModuleGame.c
 *
 * DayZ Expansion Mod
 * www.dayzexpansion.com
 * © 2023 DayZ Expansion Mod Team
 *
 * This work is licensed under the Creative Commons Attribution-NonCommercial-NoDerivatives 4.0 International License. 
 * To view a copy of this license, visit http://creativecommons.org/licenses/by-nc-nd/4.0/.
 *
*/

modded class CF_ModuleGame
{
	static ref map<typename, bool> s_Expansion_TracingEnabled = new map<typename, bool>;
	bool m_Expansion_TracingEnabled;

	ref ExpansionRPCManager m_Expansion_RPCManager;

	void EnableTracing(bool enable = true)
	{
		m_Expansion_TracingEnabled = enable;
		typename type = Type();
		s_Expansion_TracingEnabled[type] = enable;
	}

	override void EnableRPC()
	{
		if (m_Expansion_RPCManager)
		{
			Error("Cannot use CF Module RPC system and Expansion RPC manager at the same time!");
			return;
		}

		super.EnableRPC();
	}

	void Expansion_EnableRPCManager()
	{
		if (GetRPCMin() != -1 || GetRPCMax() != -1)
		{
			Error("Cannot use CF Module RPC system and Expansion RPC manager at the same time!");
			return;
		}

		m_Expansion_RPCManager = new ExpansionRPCManager(this);
	}

	int Expansion_RegisterServerRPC(string fn)
	{
		return m_Expansion_RPCManager.RegisterServer(fn);
	}

	int Expansion_RegisterClientRPC(string fn)
	{
		return m_Expansion_RPCManager.RegisterClient(fn);
	}

	int Expansion_RegisterBothRPC(string fn)
	{
		return m_Expansion_RPCManager.RegisterBoth(fn);
	}

	ExpansionScriptRPC Expansion_CreateRPC(string fn)
	{
		return m_Expansion_RPCManager.CreateRPC(fn);
	}

	ExpansionBitStreamReader GetReader(ParamsReadContext ctx)
	{
		return m_Expansion_RPCManager.GetReader(ctx);
	}

	void TracePrint(string msg)
	{
		EXTrace.Print(m_Expansion_TracingEnabled, this, msg);
	}

	void TracePrint(string msg, string p1, string p2 = "", string p3 = "", string p4 = "", string p5 = "", string p6 = "", string p7 = "", string p8 = "", string p9 = "")
	{
		EXTrace.Print(m_Expansion_TracingEnabled, this, string.Format(msg, p1, p2, p3, p4, p5, p6, p7, p8, p9));
	}

	void TracePrint(string msg, Class p1, Class p2 = null, Class p3 = null, Class p4 = null, Class p5 = null, Class p6 = null, Class p7 = null, Class p8 = null, Class p9 = null)
	{
		EXTrace.Print(m_Expansion_TracingEnabled, this, ExpansionStatic.FormatString(msg, p1, p2, p3, p4, p5, p6, p7, p8, p9));
	}

	static void TracePrint(typename type, string msg)
	{
		EXTrace.Print(s_Expansion_TracingEnabled[type], type, msg);
	}

	static void TracePrint(typename type, string msg, string p1, string p2 = "", string p3 = "", string p4 = "", string p5 = "", string p6 = "", string p7 = "", string p8 = "", string p9 = "")
	{
		EXTrace.Print(s_Expansion_TracingEnabled[type], type, string.Format(msg, p1, p2, p3, p4, p5, p6, p7, p8, p9));
	}

	static void TracePrint(typename type, string msg, Class p1, Class p2 = null, Class p3 = null, Class p4 = null, Class p5 = null, Class p6 = null, Class p7 = null, Class p8 = null, Class p9 = null)
	{
		EXTrace.Print(s_Expansion_TracingEnabled[type], type, ExpansionStatic.FormatString(msg, p1, p2, p3, p4, p5, p6, p7, p8, p9));
	}
}
