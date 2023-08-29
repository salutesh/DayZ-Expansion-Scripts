/**
 * ExpansionP2PMarketTraderNPC.c
 *
 * DayZ Expansion Mod
 * www.dayzexpansion.com
 * © 2022 DayZ Expansion Mod Team
 *
 * This work is licensed under the Creative Commons Attribution-NonCommercial-NoDerivatives 4.0 International License.
 * To view a copy of this license, visit http://creativecommons.org/licenses/by-nc-nd/4.0/.
 *
*/

/**@class		ExpansionP2PMarketTraderNPC
 * @brief
 **/

class ExpansionP2PMarketTraderNPC: ExpansionNPCBase
{
	protected int m_P2PTraderID;	 //! Unique black marker trader id. Used to get and identify the trader in the black marker module.

	void ExpansionP2PMarketTraderNPC()
	{
		m_P2PTraderID = -1;
		RegisterNetSyncVariableInt("m_P2PTraderID");
	}

	void SetP2PTraderID(int id)
	{
		m_P2PTraderID = id;
		EXPrint(ToString() + "::SetP2PTraderID - ID: " + m_P2PTraderID);
	}

	int GetP2PTraderID()
	{
		return m_P2PTraderID;
	}
};

class ExpansionP2PTraderMirek: ExpansionP2PMarketTraderNPC {};
class ExpansionP2PTraderDenis: ExpansionP2PMarketTraderNPC {};
class ExpansionP2PTraderBoris: ExpansionP2PMarketTraderNPC {};
class ExpansionP2PTraderCyril: ExpansionP2PMarketTraderNPC {};
class ExpansionP2PTraderElias: ExpansionP2PMarketTraderNPC {};
class ExpansionP2PTraderFrancis: ExpansionP2PMarketTraderNPC {};
class ExpansionP2PTraderGuo: ExpansionP2PMarketTraderNPC {};
class ExpansionP2PTraderHassan: ExpansionP2PMarketTraderNPC {};
class ExpansionP2PTraderIndar: ExpansionP2PMarketTraderNPC {};
class ExpansionP2PTraderJose: ExpansionP2PMarketTraderNPC {};
class ExpansionP2PTraderKaito: ExpansionP2PMarketTraderNPC {};
class ExpansionP2PTraderLewis: ExpansionP2PMarketTraderNPC {};
class ExpansionP2PTraderManua: ExpansionP2PMarketTraderNPC {};
class ExpansionP2PTraderNiki: ExpansionP2PMarketTraderNPC {};
class ExpansionP2PTraderOliver: ExpansionP2PMarketTraderNPC {};
class ExpansionP2PTraderPeter: ExpansionP2PMarketTraderNPC {};
class ExpansionP2PTraderQuinn: ExpansionP2PMarketTraderNPC {};
class ExpansionP2PTraderRolf: ExpansionP2PMarketTraderNPC {};
class ExpansionP2PTraderSeth: ExpansionP2PMarketTraderNPC {};
class ExpansionP2PTraderTaiki: ExpansionP2PMarketTraderNPC {};
class ExpansionP2PTraderLinda: ExpansionP2PMarketTraderNPC {};
class ExpansionP2PTraderMaria: ExpansionP2PMarketTraderNPC {};
class ExpansionP2PTraderFrida: ExpansionP2PMarketTraderNPC {};
class ExpansionP2PTraderGabi: ExpansionP2PMarketTraderNPC {};
class ExpansionP2PTraderHelga: ExpansionP2PMarketTraderNPC {};
class ExpansionP2PTraderIrena: ExpansionP2PMarketTraderNPC {};
class ExpansionP2PTraderJudy: ExpansionP2PMarketTraderNPC {};
class ExpansionP2PTraderKeiko: ExpansionP2PMarketTraderNPC {};
class ExpansionP2PTraderEva: ExpansionP2PMarketTraderNPC {};
class ExpansionP2PTraderNaomi: ExpansionP2PMarketTraderNPC {};
class ExpansionP2PTraderBaty: ExpansionP2PMarketTraderNPC {};