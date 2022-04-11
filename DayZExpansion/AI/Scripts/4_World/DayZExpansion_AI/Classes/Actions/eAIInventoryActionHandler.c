// Copyright 2021 William Bowers
//
// Licensed under the Apache License, Version 2.0 (the "License");
// you may not use this file except in compliance with the License.
// You may obtain a copy of the License at
//
//    http://www.apache.org/licenses/LICENSE-2.0
//
// Unless required by applicable law or agreed to in writing, software
// distributed under the License is distributed on an "AS IS" BASIS,
// WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
// See the License for the specific language governing permissions and
// limitations under the License.

//! Client only - manage set up crafting on client 
class eAIInventoryActionHandler
{
	ActionBase m_action;
	ActionTarget m_target;
	ItemBase m_mainItem;
	bool 	m_useItemInHands;

	PlayerBase m_player;
	
	bool m_isActive;
	vector m_actionStartPos;
	
	const float MIN_DISTANCE_TO_INTERRUPT = 1.0;	
	const int IAH_SINGLE_USE = 1;
	const int IAH_CONTINUOUS = 2;


	void eAIInventoryActionHandler(PlayerBase player)
	{
		m_player = player;
		m_isActive = false;
		m_action = null;
		m_target = null;
		m_mainItem = null;
		m_useItemInHands = false;

	}
	
	void SetAction(ActionBase action, ItemBase target_item, ItemBase main_item )
	{
		Object target_parent = null;
		if(target_item)
		{
			target_parent = target_item.GetHierarchyParent();
		}
		ActionTarget at = new ActionTarget(target_item, target_parent, -1, vector.Zero, -1);
		
		SetAction(action, at, main_item );
	}
	
	void SetAction(ActionBase action, ActionTarget target, ItemBase main_item )
	{
		eAIActionManager mngr;
		Class.CastTo(mngr, m_player.GetActionManager());
		
		m_action = action;
		m_target = target;
		m_mainItem = main_item;
		
		ItemBase itemInHand = m_player.GetItemInHands();
		m_useItemInHands = main_item == itemInHand;
		
		m_actionStartPos = m_player.GetPosition();
		m_isActive = true;
		
		mngr.PerformActionStart( action, target, main_item );
		//mngr.ForceTarget(m_target.GetObject());
		
		//GetGame().GetMission().HideInventory();
	}	
	
	bool IsActiveAction()
	{
		return m_isActive;
	}
	
	void OnUpdate()
	{
		
		if( !m_isActive ) return;
		
		if( m_player.IsRaised() || m_player.GetCommand_Melee() )
		{
			DeactiveAction();
			return;			
		}
		
		/*if( GetGame().IsInventoryOpen() )
		{
			DeactiveAction();
			return;				
		}*/

		if (m_useItemInHands)
		{
			ItemBase handItem = m_player.GetItemInHands();
		
			if( handItem != m_mainItem )
			{
				DeactiveAction();
				return;
			}
		}
			
		if( Math.AbsFloat( vector.Distance(m_actionStartPos, m_player.GetPosition())) > MIN_DISTANCE_TO_INTERRUPT )
		{
			DeactiveAction();
			return;
		}
		
		
	}

	void DeactiveAction()
	{
		if( !m_isActive ) return;
		
		m_isActive = false;
		
		eAIActionManager mngr;
		Class.CastTo(mngr, m_player.GetActionManager());
		
		// Since this unit is managed by the server we shouldn't actually ever see an action interrupted (perhaps by death, maybe?)
		// regardless, I think this code will work. But it is untested.
		mngr.Interrupt();
	
		//mngr.EjectAction(m_action);
		//mngr.ClearForceTarget();
		
		//m_player.GetCraftingManager().ResetInventoryCraft();
		
		m_action = null;
		m_target = null;
		m_mainItem = null;
		m_useItemInHands = false;	
	}
}
