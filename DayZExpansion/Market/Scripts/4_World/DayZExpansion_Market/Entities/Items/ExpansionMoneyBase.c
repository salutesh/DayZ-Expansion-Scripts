/**
 * ExpansionMoneyBase.c
 *
 * DayZ Expansion Mod
 * www.dayzexpansion.com
 * © 2022 DayZ Expansion Mod Team
 *
 * This work is licensed under the Creative Commons Attribution-NonCommercial-NoDerivatives 4.0 International License. 
 * To view a copy of this license, visit http://creativecommons.org/licenses/by-nc-nd/4.0/.
 *
*/

modded class ItemBase
{
	protected int m_ExpansionReservedMoneyAmount;

	void ItemBase()
	{
		RegisterNetSyncVariableInt("m_ExpansionReservedMoneyAmount");
	}

	// register net var "m_ExpansionReservedMoneyAmount"
	// any methods that need to be overridden, do so to prevent this 
	// item from being moved to any other inventory except the player 
	// while a non-zero amount of money is reserved.
	// ------------------------------------------------------------
	// ItemBase CanRemoveFromCargo
	// ------------------------------------------------------------
	override bool CanRemoveFromCargo(EntityAI parent)
	{
		if ( !super.CanRemoveFromCargo(parent))
			return false;

		return !ExpansionIsMoneyReserved();
	}
	
	// ------------------------------------------------------------
	// ItemBase CanRemoveFromHands
	// ------------------------------------------------------------
	override bool CanRemoveFromHands(EntityAI parent)
	{
		if (!super.CanRemoveFromHands(parent))
			return false;

		return !ExpansionIsMoneyReserved();
	}
	
	// ------------------------------------------------------------
	// ItemBase CanPutIntoHands
	// ------------------------------------------------------------	
	override bool CanPutIntoHands(EntityAI parent)
	{
		if (!super.CanPutIntoHands(parent))
			return false;
		
		return !ExpansionIsMoneyReserved();
	}

	bool ExpansionIsMoney()
	{
		//! Prevent any item without quantity from being used as money
		if (!HasQuantity())
			return false;

		ExpansionMarketModule module;
		if (CF_Modules<ExpansionMarketModule>.Get(module))
			return module.IsMoney(this);

		return false;
	}

	// ------------------------------------------------------------
	// ItemBase ExpansionIsMoneyReserved
	// ------------------------------------------------------------
	bool ExpansionIsMoneyReserved()
	{
		return m_ExpansionReservedMoneyAmount > 0;
	}

	// ------------------------------------------------------------
	// ItemBase ExpansionIsMoneyReserved
	// ------------------------------------------------------------
	int ExpansionGetReservedMoneyAmount()
	{
		return m_ExpansionReservedMoneyAmount;
	}
	
	// ------------------------------------------------------------
	// ItemBase ExpansionReserveMoney
	// ------------------------------------------------------------
	void ExpansionReserveMoney(int amount)
	{
		m_ExpansionReservedMoneyAmount = Math.Min(amount, GetQuantity());
		SetSynchDirty();
	}
}

class ExpansionMoneyBase extends ItemBase
{
	// ------------------------------------------------------------
	// ExpansionMoneyBase CanBeDamaged
	// ------------------------------------------------------------
	override bool CanBeDamaged()
	{
		return false;
	}
}

class ExpansionSilverNugget: ExpansionMoneyBase {}
class ExpansionSilverBar: ExpansionMoneyBase {}
class ExpansionGoldNugget: ExpansionMoneyBase {}
class ExpansionGoldBar: ExpansionMoneyBase {}
 