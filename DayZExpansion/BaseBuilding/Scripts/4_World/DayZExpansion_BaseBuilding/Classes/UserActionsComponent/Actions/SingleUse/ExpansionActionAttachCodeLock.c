/**
 * ActionAttachToConstruction.c
 *
 * DayZ Expansion Mod
 * www.dayzexpansion.com
 * © 2022 DayZ Expansion Mod Team
 *
 * This work is licensed under the Creative Commons Attribution-NonCommercial-NoDerivatives 4.0 International License.
 * To view a copy of this license, visit http://creativecommons.org/licenses/by-nc-nd/4.0/.
 *
*/

class ExpansionActionAttachCodeLock: ActionSingleUseBase
{
	override void CreateConditionComponents()
	{
		m_ConditionItem = new CCINonRuined;
		m_ConditionTarget = new CCTCursorNoObject( UAMaxDistances.DEFAULT );
	}

	override string GetText()
	{
		return "#attach";
	}

	int GetCodeLockSlotId( ActionTarget target, out ItemBase targetItem )
	{
		Object targetObject = target.GetObject();
		Object targetParent = target.GetParent();
		TentBase tent;
	
		string slotName;

		if ( Class.CastTo( tent, targetParent ) )
		{
			if ( IsMissionClient() )
			{
				if ( !targetObject )  //! Only on client, NULL on server as we cannot use proxies here
					return InventorySlots.INVALID;

				//! Only allow attaching codelocks to the entrances of tents, as tents may be used to block off areas,
				//! so we don't want players to be able to attach codelocks from anywhere.

				array< string > selections = new array< string >;
				targetObject.GetActionComponentNameList( target.GetComponentIndex(), selections );

				bool isEntrance;
				for ( int s = 0; s < selections.Count(); s++ )
				{
					if ( !selections[s].Contains( "entrance" ) && !selections[s].Contains( "door" ) )
						continue;

					if ( tent.CanToggleAnimations( selections[s] ) )
					{
						isEntrance = true;
						break;
					}
				}

				if ( !isEntrance )
					return InventorySlots.INVALID;
			}

			slotName = "Att_ExpansionCodeLock";

			targetItem = ItemBase.Cast( targetParent );
		} else if ( targetObject )
		{
			ExpansionWallBase wall;
			Fence fence;

			targetItem = ItemBase.Cast( targetObject );

			if ( Class.CastTo( wall, targetObject ) )
			{
				if ( wall.HasDoor() )
					slotName = "Att_ExpansionCodeLock_1";
				else if ( wall.HasGate() )
					slotName = "Att_ExpansionCodeLock_2";
			}
			else if ( Class.CastTo( fence, targetObject ) && fence.HasFullyConstructedGate() )
				slotName = "Att_CombinationLock";
			else if ( targetItem && targetItem.IsOpen() && !targetItem.ExpansionIsLocked() )
				targetItem.ExpansionFindCodeLockSlot( slotName );
		}

		if ( !slotName )
			return InventorySlots.INVALID;

		return InventorySlots.GetSlotIdFromString( slotName );
	}

	override bool ActionCondition( PlayerBase player, ActionTarget target, ItemBase item )
	{
		ItemBase targetItem;

		int slotId = GetCodeLockSlotId( target, targetItem );

		if ( !targetItem || slotId == InventorySlots.INVALID || targetItem.ExpansionGetCodeLock() )
			return false;

		return targetItem.CanReceiveAttachment( item, slotId );
	}

	override void OnExecuteServer( ActionData action_data )
	{
		if ( GetGame().IsMultiplayer() )
			return;

		OnExecuteInternal( action_data );
	}

	override void OnExecuteClient( ActionData action_data )
	{
		ClearInventoryReservationEx( action_data );

		OnExecuteInternal( action_data );
	}

	void OnExecuteInternal( ActionData action_data )
	{
		ItemBase targetItem;

		int slotId = GetCodeLockSlotId( action_data.m_Target, targetItem );

		if ( !targetItem || slotId == InventorySlots.INVALID )
			return;

		action_data.m_Player.PredictiveTakeEntityToTargetAttachmentEx( targetItem, action_data.m_MainItem, slotId );
	}
}
