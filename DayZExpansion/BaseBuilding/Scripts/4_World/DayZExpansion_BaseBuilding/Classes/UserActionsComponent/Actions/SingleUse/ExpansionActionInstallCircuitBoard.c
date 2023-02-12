/**
 * ExpansionActionInstallCircuitBoard.c
 *
 * DayZ Expansion Mod
 * www.dayzexpansion.com
 * © 2022 DayZ Expansion Mod Team
 *
 * This work is licensed under the Creative Commons Attribution-NonCommercial-NoDerivatives 4.0 International License.
 * To view a copy of this license, visit http://creativecommons.org/licenses/by-nc-nd/4.0/.
 *
*/

class ExpansionActionInstallCircuitBoard: ActionSingleUseBase
{
	protected int m_Expansion_CircuitBoardSlotId;

	void ExpansionActionInstallCircuitBoard()
	{
		m_StanceMask = DayZPlayerConstants.STANCEMASK_CROUCH | DayZPlayerConstants.STANCEMASK_ERECT;
		m_CommandUID = DayZPlayerConstants.CMD_ACTIONMOD_ATTACHITEM;
		m_Expansion_CircuitBoardSlotId = InventorySlots.GetSlotIdFromString("Att_ExpansionCircuitBoard");
	}

	override void CreateConditionComponents() 
	{
		m_ConditionItem = new CCINonRuined;
		m_ConditionTarget = new CCTNonRuined( UAMaxDistances.DEFAULT );
	}

	override bool ActionCondition(PlayerBase player, ActionTarget target, ItemBase item)
	{
		EntityAI targetEntity;
		if (!Class.CastTo(targetEntity, target.GetParentOrObject()))
			return false;

		//! Get board from player hands.
		auto boardInHands = ExpansionCircuitBoardBase.Cast(item);
		if (!boardInHands)
			return false;
		
		//! Check if target object has required attachment slot or slot is not free.
		//! If this check fails that means the object cant be attached because the slot is already blocked by an other attachment or the object does not have a slot for the item.
		if (targetEntity.GetInventory() && !targetEntity.GetInventory().CanAddAttachment(item))
		{
			auto attachedBoard = ExpansionCircuitBoardBase.Cast(targetEntity.GetInventory().FindAttachment(m_Expansion_CircuitBoardSlotId));
			if (attachedBoard)
			{
				//! If there is already a board attached only allow action if the tier of the board in the players hands is higher then the attached one.
				if (boardInHands.GetTier() <= attachedBoard.GetTier())
					return false;
			}
		}
		
		if (!GetGame().IsDedicatedServer())
		{
			//! TODO: Localization
			m_Text = "Install Upgrade - Tier " + boardInHands.GetTier().ToString();
		}

		return true;
	}
	
	protected void OnExecuteImpl(ActionData action_data)
	{
		EntityAI targetEntity = EntityAI.Cast(action_data.m_Target.GetObject());

		if (targetEntity && action_data.m_MainItem)
		{
			string attachedType;
			float boardHealth;
			EntityAI attachedBoard = targetEntity.GetInventory().FindAttachment(m_Expansion_CircuitBoardSlotId);
			if (attachedBoard)
			{
				attachedType = attachedBoard.GetType();
				boardHealth = attachedBoard.GetHealth();
				targetEntity.GetInventory().SetSlotLock(m_Expansion_CircuitBoardSlotId, false);
				//! Delete the attached board.
				GetGame().ObjectDelete(attachedBoard);
			}

			//! Attach the new board to the target object.
			attachedBoard = targetEntity.GetInventory().CreateInInventory(action_data.m_MainItem.GetType());
			if (attachedBoard)
			{
				attachedBoard.SetHealth(action_data.m_MainItem.GetHealth());
				targetEntity.GetInventory().SetSlotLock(m_Expansion_CircuitBoardSlotId, true);
				//! Delete the board in the players hands on board installation.
				GetGame().ObjectDelete(action_data.m_MainItem);
			}

			if (attachedType)
			{
				//! Recreate the attached board in the players hands.
				EntityAI boardInHands = action_data.m_Player.GetHumanInventory().CreateInHands(attachedType);
				if (boardInHands)
					boardInHands.SetHealth(boardHealth);
			}
		}
	}

	override void OnExecuteServer(ActionData action_data)
	{
		if (GetGame().IsMultiplayer())
			OnExecuteImpl(action_data);
	}
		
	override void OnExecuteClient(ActionData action_data)
	{
		if (!GetGame().IsMultiplayer())
			OnExecuteImpl(action_data);
	}
};
