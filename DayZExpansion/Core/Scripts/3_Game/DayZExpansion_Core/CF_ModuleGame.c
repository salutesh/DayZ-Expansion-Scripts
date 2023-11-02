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
	ref ExpansionRPCManager m_Expansion_RPCManager;

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
}
