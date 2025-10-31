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

class ExpansionMoneyBase: ItemBase
{
	// ------------------------------------------------------------
	// ExpansionMoneyBase CanBeDamaged
	// ------------------------------------------------------------
	override bool CanBeDamaged()
	{
		return false;
	}
}

class ExpansionMoneyBase_InsanityStack: ExpansionMoneyBase
{
	override void InitItemVariables()
	{
		super.InitItemVariables();

		//! The max for a netsynced float with precision = 1 is 214748256
		//! We don't need any places after decimal point for money, so we can set precision to 0
		//! which increases range to float.MAX
		UpdateNetSyncVariableFloat("m_VarQuantity", GetQuantityMin(), m_VarQuantityMax, 0);
	}

	override void OnDebugSpawn()
	{
		g_Game.GetCallQueue(CALL_CATEGORY_SYSTEM).Call(Expansion_SetHalfInsanityQuantity);
	}
	
	override void EEOnCECreate()
	{
		super.EEOnCECreate();

		g_Game.GetCallQueue(CALL_CATEGORY_SYSTEM).Call(Expansion_SetHalfInsanityQuantity);
	}

	void Expansion_SetHalfInsanityQuantity()
	{
		SetQuantity(GetQuantityMax() * 0.5);
	}

	//! Only players with Entity.Spawn permission get quantity other than 1
	override float GetQuantity()
	{
		PlayerBase owner;
		if (!Class.CastTo(owner, GetHierarchyRootPlayer()) || owner.IsPlayerDisconnected() || (owner.GetIdentity() && GetPermissionsManager().HasPermission("Entity.Spawn", owner.GetIdentity())))
			return super.GetQuantity();

		return 1;
	}

	override float GetQuantityNormalized()
	{
		return Math.InverseLerp(GetQuantityMin(), GetQuantityMax(), GetQuantity());
	}

	override bool CanBeCombined(EntityAI other_item, bool reservation_check = true, bool stack_max_limit = false)
	{
		string type = GetType();
		int index = type.IndexOf("_InsanityStack");
		if (index > -1)
			type = type.Substring(0, index);
		
		if (!other_item || (GetType() != other_item.GetType() && type != other_item.GetType()) || (IsFullQuantity() && other_item.GetQuantity() > 0) || other_item == this)
			return false;

		if (GetHealthLevel() == GameConstants.STATE_RUINED || other_item.GetHealthLevel() == GameConstants.STATE_RUINED)	
			return false;

		if (!can_this_be_combined)
			return false;

		if (stack_max_limit && (GetQuantity() + other_item.GetQuantity() > GetQuantityMax()))
			return false;

		PlayerBase player = null;
		if (CastTo(player, GetHierarchyRootPlayer()))
		{
			if (player.GetInventory().HasAttachment(this))
				return false;

			if (player.IsItemsToDelete())
				return false;
		}

		if (reservation_check && (GetInventory().HasInventoryReservation(this, null) || other_item.GetInventory().HasInventoryReservation(other_item, null)))
			return false;
		
		int slotID;
		string slotName;
		if (GetInventory().GetCurrentAttachmentSlotInfo(slotID, slotName) && GetHierarchyParent().GetInventory().GetSlotLock(slotID))
			return false;

		return true;
	}
}

class ExpansionSilverNugget: ExpansionMoneyBase {};
class ExpansionSilverBar: ExpansionMoneyBase {};
class ExpansionGoldNugget: ExpansionMoneyBase {};
class ExpansionGoldBar: ExpansionMoneyBase {};
class ExpansionBanknoteUSD: ExpansionMoneyBase {};
class ExpansionBanknoteEuro: ExpansionMoneyBase {};
class ExpansionBanknoteHryvnia: ExpansionMoneyBase {};

class ExpansionSilverNugget_InsanityStack: ExpansionMoneyBase_InsanityStack {};
class ExpansionSilverBar_InsanityStack: ExpansionMoneyBase_InsanityStack {};
class ExpansionGoldNugget_InsanityStack: ExpansionMoneyBase_InsanityStack {};
class ExpansionGoldBar_InsanityStack: ExpansionMoneyBase_InsanityStack {};
class ExpansionBanknoteUSD_InsanityStack: ExpansionMoneyBase_InsanityStack {};
class ExpansionBanknoteEuro_InsanityStack: ExpansionMoneyBase_InsanityStack {};
class ExpansionBanknoteHryvnia_InsanityStack: ExpansionMoneyBase_InsanityStack {};

class ExpansionSilverNugget_InsanityStack_Billions: ExpansionSilverNugget_InsanityStack {};
class ExpansionSilverBar_InsanityStack_Billions: ExpansionSilverBar_InsanityStack {};
class ExpansionGoldNugget_InsanityStack_Billions: ExpansionGoldNugget_InsanityStack {};
class ExpansionGoldBar_InsanityStack_Billions: ExpansionGoldBar_InsanityStack {};
class ExpansionBanknoteUSD_InsanityStack_Billions: ExpansionBanknoteUSD_InsanityStack {};
class ExpansionBanknoteEuro_InsanityStack_Billions: ExpansionBanknoteEuro_InsanityStack {};
class ExpansionBanknoteHryvnia_InsanityStack_Billions: ExpansionBanknoteHryvnia_InsanityStack {};
