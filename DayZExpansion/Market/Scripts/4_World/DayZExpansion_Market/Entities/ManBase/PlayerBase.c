/**
 * PlayerBase.c
 *
 * DayZ Expansion Mod
 * www.dayzexpansion.com
 * © 2022 DayZ Expansion Mod Team
 *
 * This work is licensed under the Creative Commons Attribution-NonCommercial-NoDerivatives 4.0 International License.
 * To view a copy of this license, visit http://creativecommons.org/licenses/by-nc-nd/4.0/.
 *
*/

modded class PlayerBase
{
	protected ref ExpansionMarketReserve m_MarketReserve;
	protected ref ExpansionMarketSell m_MarketSell;

	// ------------------------------------------------------------
	// PlayerBase Constructor
	// ------------------------------------------------------------
	void PlayerBase()
	{
		m_MarketReserve = new ExpansionMarketReserve; 
		m_MarketSell = new ExpansionMarketSell;
	}

	override void SetActions( out TInputActionMap InputActionMap )
	{
		super.SetActions(InputActionMap);

		AddAction(ExpansionActionOpenTraderMenu, InputActionMap);
	}

	// ------------------------------------------------------------
	// PlayerBase IsMarketItemReserved
	// ------------------------------------------------------------
	bool IsMarketItemReserved(string className)
	{
		if (m_MarketReserve != NULL && m_MarketReserve.Valid)
		{
#ifdef EXPANSIONMODMARKET_DEBUG
			EXPrint("PlayerBase::IsMarketItemReserved - 1");
#endif
			if (m_MarketReserve.RootItem.ClassName == className)
			{
#ifdef EXPANSIONMODMARKET_DEBUG
				EXPrint("PlayerBase::IsMarketItemReserved - 2");
				EXPrint("PlayerBase::IsMarketItemReserved - m_MarketReserve.Time: " + m_MarketReserve.Time);
				EXPrint("PlayerBase::IsMarketItemReserved - GetGame().GetTime() - 30000: " + (GetGame().GetTime() - 30000));
#endif
				if (m_MarketReserve.Time >= GetGame().GetTime() - 30000)
				{
					//! Still within 30 second timelimit until item reservation expires
#ifdef EXPANSIONMODMARKET_DEBUG
					EXPrint("PlayerBase::IsMarketItemReserved - End and return true!");
#endif
					return true;
				}
			}
		}

		//! After 30 seconds, reservation is no longer valid
#ifdef EXPANSIONMODMARKET_DEBUG
		EXPrint("PlayerBase::IsMarketItemReserved - End and return false!");
#endif
		return false;
	}
	
	// ------------------------------------------------------------
	// PlayerBase GetMarketReserve
	// ------------------------------------------------------------
	ExpansionMarketReserve GetMarketReserve()
	{
		return m_MarketReserve;
	}
	
	// ------------------------------------------------------------
	// PlayerBase GetMarketSell
	// ------------------------------------------------------------
	ExpansionMarketSell GetMarketSell()
	{
		return m_MarketSell;
	}
	
	// ------------------------------------------------------------
	// PlayerBase ClearMarketReserve
	// ------------------------------------------------------------
	void ClearMarketReserve()
	{
		m_MarketReserve.Valid = false;
		m_MarketReserve.Reserved.Clear();
	}
	
	// ------------------------------------------------------------
	// PlayerBase ClearMarketSell
	// ------------------------------------------------------------
	void ClearMarketSell()
	{
		m_MarketSell.Valid = false;
		m_MarketSell.Sell.Clear();
	}
}