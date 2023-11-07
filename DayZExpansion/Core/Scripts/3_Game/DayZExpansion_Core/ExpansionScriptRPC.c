/**
 * ExpansionScriptRPC.c
 *
 * DayZ Expansion Mod
 * www.dayzexpansion.com
 * © 2022 DayZ Expansion Mod Team
 *
 * This work is licensed under the Creative Commons Attribution-NonCommercial-NoDerivatives 4.0 International License. 
 * To view a copy of this license, visit http://creativecommons.org/licenses/by-nc-nd/4.0/.
 *
*/

//! @note do not instantiate directly! Use ExpansionRPCManager::Create
class ExpansionScriptRPC: ScriptRPC
{
	static const int EXPANSION_RPC_UNTARGETED = 1506850293;  //! "DayZ Expansion".Hash()
	static const int EXPANSION_RPC_TARGETED = 1506850294;  //! "DayZ Expansion".Hash() + 1

	Object m_Expansion_Target;

	void Expansion_WriteNetworkID(Object object)
	{
		int low, high;
		object.GetNetworkID(low, high);
		Write(low);
		Write(high);
	}

	void Expansion_Send(bool guaranteed = true, PlayerIdentity recipient = null)
	{
		Expansion_Send(m_Expansion_Target, guaranteed, recipient);
	}

	void Expansion_Send(Object target, bool guaranteed = true, PlayerIdentity recipient = null)
	{
		if (target)
			Send(target, EXPANSION_RPC_TARGETED, guaranteed, recipient);
		else
			Send(null, EXPANSION_RPC_UNTARGETED, guaranteed, recipient);
	}

	/**
	 * @brief create and return ExpansionScriptRPC instance
	 * 
	 * @param rpcID Expansion RPC ID
	 * 
	 * @return ExpansionScriptRPC instance
	 */
	static ExpansionScriptRPC Create(int rpcID)
	{
		ExpansionScriptRPC rpc = new ExpansionScriptRPC();
		rpc.Write(rpcID);
		return rpc;
	}

	//! DEPRECATED, use ExpansionRPCManager::Create
	static ScriptRPC Create()
	{
		ScriptRPC rpc = new ScriptRPC();
		return rpc;
	}

	//! DEPRECATED
	static bool CheckMagicNumber(ParamsReadContext ctx)
	{
		return true;
	}
}
