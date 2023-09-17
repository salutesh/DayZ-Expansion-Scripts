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
	CarScript m_Vehicle;  //! client only!

	void ExpansionActionPayParkingFine()
	{
		//m_CallbackClass = ExpansionActionPayParkingFineCB;
		m_CommandUID = DayZPlayerConstants.CMD_ACTIONMOD_PICKUP_INVENTORY;
		m_StanceMask = DayZPlayerConstants.STANCEMASK_CROUCH | DayZPlayerConstants.STANCEMASK_ERECT;
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

	override string GetText()
	{
		return string.Format("Pay fine (%1)", m_Vehicle.Expansion_GetParkingFine());
	}

	override bool ActionCondition(PlayerBase player, ActionTarget target, ItemBase item)
	{
		if (!Class.CastTo(m_Vehicle, target.GetParentOrObject()) && !Class.CastTo(m_Vehicle, player.GetParent()))
			return false;

		if (!m_Vehicle.Expansion_GetParkingFine())
			return false;

		return true;
	}

	override void OnExecuteServer(ActionData action_data)
	{
		PlayerBase player = action_data.m_Player;
		CarScript vehicle;
		if (Class.CastTo(vehicle, action_data.m_Target.GetParentOrObject()) || Class.CastTo(vehicle, player.GetParent()))
		{
			int amount = vehicle.Expansion_GetParkingFine();
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

				vehicle.UnlockCarWithoutKey(ExpansionVehicleLockState.UNLOCKED);
				vehicle.Expansion_SetParkingFine(0);
				vehicle.Expansion_ResetSZParkingTime();
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
