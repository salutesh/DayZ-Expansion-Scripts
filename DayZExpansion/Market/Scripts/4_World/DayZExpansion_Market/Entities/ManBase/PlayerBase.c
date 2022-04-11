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

	// ------------------------------------------------------------
	// PlayerBase Destructor
	// ------------------------------------------------------------
	void ~PlayerBase()
	{
		delete m_MarketReserve; 
		delete m_MarketSell;
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
			Print("PlayerBase::IsMarketItemReserved - 1");
			if (m_MarketReserve.RootItem.ClassName == className)
			{
				Print("PlayerBase::IsMarketItemReserved - 2");
				Print("PlayerBase::IsMarketItemReserved - m_MarketReserve.Time: " + m_MarketReserve.Time);
				Print("PlayerBase::IsMarketItemReserved - GetGame().GetTime() - 30000: " + (GetGame().GetTime() - 30000));
				if (m_MarketReserve.Time >= GetGame().GetTime() - 30000)
				{
					//! Still within 30 second timelimit until item reservation expires
					Print("PlayerBase::IsMarketItemReserved - End and return true!");
					return true;
				}
			}
		}

		//! After 30 seconds, reservation is no longer valid
		Print("PlayerBase::IsMarketItemReserved - End and return false!");
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
	
	override void OnVehicleSeatDriverEnter()
	{
		super.OnVehicleSeatDriverEnter();

		HumanCommandVehicle hcv = GetCommand_Vehicle();
		if (hcv && hcv.GetTransport())
		{
			CarScript car = CarScript.Cast(hcv.GetTransport());
			if (car)
			{
				car.ExpansionSetLastDriverUID(this);
				if (IsMissionHost())
					GetGame().GetCallQueue(CALL_CATEGORY_SYSTEM).CallLater(car.ExpansionResetLastDriverUIDSynch, 1000, false);
			}
		}

		#ifdef EXPANSIONMODVEHICLE;
		ExpansionHumanCommandVehicle exhcv = GetCommand_ExpansionVehicle();
		if (exhcv && exhcv.GetObject())
		{
			ExpansionVehicleBase vehicle = ExpansionVehicleBase.Cast(exhcv.GetObject());
			if (vehicle)
			{
				vehicle.ExpansionSetLastDriverUID(this);
				if (IsMissionHost())
					GetGame().GetCallQueue(CALL_CATEGORY_SYSTEM).CallLater(vehicle.ExpansionResetLastDriverUIDSynch, 1000, false);
			}
		}
		#endif
	}
}