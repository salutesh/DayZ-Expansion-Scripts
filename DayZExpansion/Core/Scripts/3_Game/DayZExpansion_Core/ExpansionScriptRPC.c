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
	static const int EXPANSION_RPC_SJ = 1506850295;  //! "DayZ Expansion".Hash() + 2

	int m_Expansion_RPCID;
	Object m_Expansion_Target;

	/* Packed write -------------------------------------------------------- */

	int m_Packed;
	int m_PackedBits;

	void Write(int value, int bits)
	{
		ExpansionBitStreamSerializerWriter.Write(this, value, bits, m_Packed, m_PackedBits);
	}
	
	void WriteUInt(int value, int bits)
	{
		ExpansionBitStreamSerializerWriter.WriteUInt(this, value, bits, m_Packed, m_PackedBits);
	}

	/**
	 * @brief Optimized writing of integer in range [min..max]
	 */
	void Write(int value, int min, int max)
	{
		ExpansionBitStreamSerializerWriter.Write(this, value, min, max, m_Packed, m_PackedBits);
	}

	void WriteBool(bool value)
	{
		ExpansionBitStreamSerializerWriter.WriteBool(this, value, m_Packed, m_PackedBits);
	}

	void WriteChar(int value)
	{
		ExpansionBitStreamSerializerWriter.WriteChar(this, value, m_Packed, m_PackedBits);
	}

	void WriteUChar(int value)
	{
		ExpansionBitStreamSerializerWriter.WriteUChar(this, value, m_Packed, m_PackedBits);
	}

	void WriteShort(int value)
	{
		ExpansionBitStreamSerializerWriter.WriteShort(this, value, m_Packed, m_PackedBits);
	}

	void WriteUShort(int value)
	{
		ExpansionBitStreamSerializerWriter.WriteUShort(this, value, m_Packed, m_PackedBits);
	}

	void Write(float value, int bits)
	{
		ExpansionBitStreamSerializerWriter.Write(this, value, bits, m_Packed, m_PackedBits);
	}

	//! @note converts float to fp16
	void WriteHalf(float value)
	{
		ExpansionBitStreamSerializerWriter.WriteHalf(this, value, m_Packed, m_PackedBits);
	}

	void Write(vector value, int bits)
	{
		ExpansionBitStreamSerializerWriter.Write(this, value, bits, m_Packed, m_PackedBits);
	}

	//! @note converts float to fp16
	void WriteHalf(vector value)
	{
		ExpansionBitStreamSerializerWriter.WriteHalf(this, value, m_Packed, m_PackedBits);
	}

	void Write(string value, int bits)
	{
		ExpansionBitStreamSerializerWriter.Write(this, value, bits, m_Packed, m_PackedBits);
	}

	void Write(string value, int bits, string alphabet)
	{
		ExpansionBitStreamSerializerWriter.Write(this, value, bits, alphabet, m_Packed, m_PackedBits);
	}

	void WriteClassName(string value)
	{
		ExpansionBitStreamSerializerWriter.WriteClassName(this, value, m_Packed, m_PackedBits);
	}

	void WriteClassNameLower(string value)
	{
		ExpansionBitStreamSerializerWriter.WriteClassNameLower(this, value, m_Packed, m_PackedBits);
	}

	void Flush()
	{
		ExpansionBitStreamSerializerWriter.Flush(this, m_Packed, m_PackedBits);
	}

	/* --------------------------------------------------------------------- */

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
		if (!m_Expansion_RPCID)
			return;

		Flush();

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
		if (!rpcID)
			Error("Invalid Expansion RPC ID 0");
		else
			rpc.m_Expansion_RPCID = rpcID;
		rpc.Write(rpcID);
		return rpc;
	}
}
