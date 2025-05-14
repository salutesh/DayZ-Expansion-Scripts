/**
 * ExpansionActionPayParkingFine.c
 *
 * DayZ Expansion Mod
 * www.dayzexpansion.com
 * © 2023 DayZ Expansion Mod Team
 *
 * This work is licensed under the Creative Commons Attribution-NonCommercial-NoDerivatives 4.0 International License. 
 * To view a copy of this license, visit http://creativecommons.org/licenses/by-nc-nd/4.0/.
 *
*/

#ifdef EXPANSIONMODVEHICLE
class ExpansionActionPayParkingFineCB: ActionContinuousBaseCB
{
	override void CreateActionComponent()
	{
		m_ActionData.m_ActionComponent = new CAContinuousTime(1.0);
	}
}

class ExpansionActionPayParkingFine: ActionInteractBase
{
	#ifndef SERVER
	ref CF_Localiser m_Expansion_Localiser;  //! client only!
	#endif

	void ExpansionActionPayParkingFine()
	{
		//m_CallbackClass = ExpansionActionPayParkingFineCB;
		m_CommandUID = DayZPlayerConstants.CMD_ACTIONMOD_PICKUP_INVENTORY;
		m_StanceMask = DayZPlayerConstants.STANCEMASK_CROUCH | DayZPlayerConstants.STANCEMASK_ERECT;

		#ifndef SERVER
		m_Expansion_Localiser = new CF_Localiser("STR_EXPANSION_MARKET_PAY_FINE");
		#endif
	}

	override void CreateConditionComponents()  
	{
		m_ConditionItem = new CCINone;
		m_ConditionTarget = new CCTNone;
	}

	override bool UseMainItem()
	{
		return false;
	}
	 
	override bool CanBeUsedInVehicle()
	{
		return true;
	}

	#ifndef SERVER
	override string GetText()
	{
		return m_Expansion_Localiser.Format();
	}
	#endif

	override bool ActionCondition(PlayerBase player, ActionTarget target, ItemBase item)
	{
		ExpansionVehicle vehicle;

		if (!ExpansionVehicle.Get(vehicle, target.GetParentOrObject()) && !ExpansionVehicle.Get(vehicle, player))
			return false;

		if (!vehicle.GetParkingFine())
			return false;
		
		#ifdef SERVER
		if (GetExpansionSettings().GetMarket().SZVehicleParkingFineUseKey)
		{
			if ( vehicle.HasKey() )
			{
				if (!player.HasKeyForCar(vehicle))
				{
					ExpansionNotification("STR_EXPANSION_ERROR_TITLE", "STR_EXPANSION_MARKET_FINE_MISSING_KEY").Error(player.GetIdentity());
					return false;
				}
			}
		}
		#else
		m_Expansion_Localiser.Set(0, vehicle.GetParkingFine());
		#endif

		return true;
	}

	override void OnExecuteServer(ActionData action_data)
	{
		PlayerBase player = action_data.m_Player;
		ExpansionVehicle vehicle;
		if (ExpansionVehicle.Get(vehicle, action_data.m_Target.GetParentOrObject()) || ExpansionVehicle.Get(vehicle, player))
		{
			int amount = vehicle.GetParkingFine();
			ExpansionMarketModule market = ExpansionMarketModule.GetInstance();

			TIntArray monies = {};
			if (market.FindMoneyAndCountTypes(player, amount, monies, true))
			{
				int removed = market.RemoveMoney(player);
				if (removed - amount > 0)
				{
					EntityAI parent = player;
					market.SpawnMoney(player, parent, removed - amount, true);
					market.CheckSpawn(player, parent);
				}

				vehicle.ForceUnlock(ExpansionVehicleLockState.UNLOCKED);
				vehicle.SetParkingFine(0);
				vehicle.ResetSZParkingTime();
			}
			else
			{
				ExpansionNotification("STR_EXPANSION_MARKET_TITLE", "STR_EXPANSION_TRADER_NOT_ENOGH_MONEY").Error(player.GetIdentity());
				market.UnlockMoney(player);
			}
		}
	}
}
#endif
