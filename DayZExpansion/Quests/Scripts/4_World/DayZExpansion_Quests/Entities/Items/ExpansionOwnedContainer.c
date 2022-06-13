/**
 * SeaChest.c
 *
 * DayZ Expansion Mod
 * www.dayzexpansion.com
 * © 2022 DayZ Expansion Mod Team
 *
 * This work is licensed under the Creative Commons Attribution-NonCommercial-NoDerivatives 4.0 International License.
 * To view a copy of this license, visit http://creativecommons.org/licenses/by-nc-nd/4.0/.
 *
*/

class ExpansionTemporaryOwnedQuestContainer: ExpansionOwnedContainer
{
	protected bool m_ExpansionCanReceiveItems;
	protected bool m_ExpansionStashDelete;
	
	void ExpansionTemporaryOwnedQuestContainer()
	{
		if (IsMissionHost())  //! Server or COM
		{
			SetAllowDamage(false);
			
			//! Check if empty every 5 seconds
			GetGame().GetCallQueue(CALL_CATEGORY_SYSTEM).CallLater(ExpansionCheckStorage, 5000, true );
		}
	}
	
	void ~ExpansionTemporaryOwnedQuestContainer()
	{
		GetGame().GetCallQueue(CALL_CATEGORY_SYSTEM).Remove(ExpansionCheckStorage);
		
		if (m_ExpansionStashDelete)
		{
			GetGame().GetCallQueue(CALL_CATEGORY_SYSTEM).Remove(ExpansionDeleteStorage);
			GetGame().GetCallQueue(CALL_CATEGORY_SYSTEM).Remove(ExpansionStorageNotification);
		}
	}

	/*override bool CanPutIntoHands(EntityAI parent)
	{
		return false;
	}*/
	
	override bool CanPutInCargo(EntityAI parent)
	{
		if (parent.IsKindOf("UndergroundStash"))
			return true;
		
		if (parent.IsInherited(UndergroundStash))
			return true;

		return super.CanPutInCargo(parent);
	}
	
	override bool CanCombineAttachment(notnull EntityAI e, int slot, bool stack_max_limit = false)
	{
		return false;
	}

	override bool CanReceiveAttachment(EntityAI attachment, int slotId)
	{
		return m_ExpansionCanReceiveItems;
	}

	override bool CanReceiveItemIntoCargo(EntityAI item)
	{
		return m_ExpansionCanReceiveItems;
	}

	override bool CanSwapItemInCargo(EntityAI child_entity, EntityAI new_entity)
	{
		return false;
	}

	void ExpansionSetCanReceiveItems(bool state)
	{
		m_ExpansionCanReceiveItems = state;
	}

	void ExpansionCheckStorage()
	{
		if (!IsInStash() && !m_ExpansionStashDelete)
			ExpansionDeleteStashAfterCooldown();
		
		if (IsEmpty())
			ExpansionDeleteStorage();
	}
	
	void ExpansionDeleteStashAfterCooldown()
	{
		m_ExpansionStashDelete = true;
		if (IsMissionHost())  //! Server or COM
		{
			//! Delete after 20 minutes
			GetGame().GetCallQueue(CALL_CATEGORY_SYSTEM).CallLater(ExpansionDeleteStorage, 1000 * 60 * 20, false);
		}

		//! Warn about pending deletion after 15 minutes
		if (IsMissionClient())  //! Client or COM
			GetGame().GetCallQueue(CALL_CATEGORY_SYSTEM).CallLater(ExpansionStorageNotification, 1000 * 60 * 15, false, "STR_EXPANSION_TEMPORARY_STORAGE_EXPIRATION_WARNING");
	}
	
	bool IsInStash()
	{
		UndergroundStash stash;
		if (Class.CastTo(stash, GetParent()))
			return true;
		
		return false;
	}
	
	void ExpansionStorageNotification(string msg)
	{
		if (IsEmpty())
			return;

		PlayerBase player = PlayerBase.GetPlayerByUID(m_ExpansionContainerUID);
		if (!player || !player.GetIdentity())
			return;

		StringLocaliser title = new StringLocaliser("STR_EXPANSION_TEMPORARY_STORAGE");
		StringLocaliser text = new StringLocaliser(msg);

		ExpansionNotification(title, text, EXPANSION_NOTIFICATION_ICON_TRADER, COLOR_EXPANSION_NOTIFICATION_ORANGE).Create(player.GetIdentity());
	}

	void ExpansionDeleteStorage()
	{
		ExpansionStorageNotification("STR_EXPANSION_TEMPORARY_STORAGE_EXPIRED");
		
		GetGame().ObjectDelete(this);
	}
};

class ExpansionQuestSeaChest: ExpansionTemporaryOwnedQuestContainer {};