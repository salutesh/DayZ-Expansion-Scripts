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
	static const int EXPANSION_MAGIC_NUMBER = 1506850293;  //! "DayZ Expansion".Hash()
	static const int EXPANSION_MAGIC_NUMBER_NETIDTARGET = 1506850294;  //! "DayZ Expansion".Hash() + 1

	int m_Expansion_RPCType = EXPANSION_MAGIC_NUMBER;
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
		Send(m_Expansion_Target, m_Expansion_RPCType, guaranteed, recipient);
	}

	void Expansion_Send(Object target, bool guaranteed = true, PlayerIdentity recipient = null)
	{
		Send(target, m_Expansion_RPCType, guaranteed, recipient);
	}

	/**
	 * @brief create and return ExpansionScriptRPC instance
	 * 
	 * @param rpcID Expansion RPC ID
	 * @param target Optional target object
	 * 
	 * @return ExpansionScriptRPC instance
	 * 
	 * @note if target object is given, its network ID will be written to the RPC.
	 * This is useful if the RPC is sent to the client, because it allows ignoring
	 * it completely if the target object does not exist in the client's network bubble.
	 */
	static ExpansionScriptRPC Create(int rpcID, Object target = null)
	{
		ExpansionScriptRPC rpc = new ExpansionScriptRPC();

		if (target)
		{
			rpc.m_Expansion_RPCType = EXPANSION_MAGIC_NUMBER_NETIDTARGET;
			rpc.Expansion_WriteNetworkID(target);
		}

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
