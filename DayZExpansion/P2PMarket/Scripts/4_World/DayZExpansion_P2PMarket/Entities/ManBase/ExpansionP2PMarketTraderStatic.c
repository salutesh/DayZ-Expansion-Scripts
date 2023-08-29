/**
 * ExpansionP2PMarketTraderStatic.c
 *
 * DayZ Expansion Mod
 * www.dayzexpansion.com
 * © 2022 DayZ Expansion Mod Team
 *
 * This work is licensed under the Creative Commons Attribution-NonCommercial-NoDerivatives 4.0 International License.
 * To view a copy of this license, visit http://creativecommons.org/licenses/by-nc-nd/4.0/.
 *
*/

/**@class		ExpansionP2PMarketTraderStatic
 * @brief
 **/

class ExpansionP2PMarketTraderStatic: ExpansionStaticObjectBase
{
	protected int m_P2PTraderID;	 //! Unique black marker trader id. Used to get and identify the trader in the black marker module.

	void ExpansionP2PMarketTraderStatic()
	{
		m_Expansion_NetsyncData = new ExpansionNetsyncData(this);
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

class ExpansionP2PMarketTraderLockerClosedBlueV1: ExpansionP2PMarketTraderStatic {};