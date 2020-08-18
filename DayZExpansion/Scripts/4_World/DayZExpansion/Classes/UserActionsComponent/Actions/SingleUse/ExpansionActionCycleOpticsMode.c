/**
 * ExpansionActionCycleOpticsMode.c
 *
 * DayZ Expansion Mod
 * www.dayzexpansion.com
 * © 2020 DayZ Expansion Mod Team
 *
 * This work is licensed under the Creative Commons Attribution-NonCommercial-NoDerivatives 4.0 International License.
 * To view a copy of this license, visit http://creativecommons.org/licenses/by-nc-nd/4.0/.
 *
*/

//! Todo, once this is working try to make it work like weapon flashlights so it isnt needed to remove item to hands. 
class ExpansionActionCycleOpticsMode: ActionSingleUseBase
{
	ExpansionReflexMRSOptic m_optic;
	
	void ExpansionActionCycleOpticsMode()
	{
		m_CommandUID = DayZPlayerConstants.CMD_ACTIONMOD_ITEM_ON;
	}
	
	override void CreateConditionComponents()  
	{	
		m_ConditionItem = new CCINonRuined;
		m_ConditionTarget = new CCTNone;
	}

	override bool HasTarget()
	{
		return false;
	}

	override string GetText()
	{
		return "#STR_USRACT_CYCLE_RETICLE";
	}

	override bool ActionCondition( PlayerBase player, ActionTarget target, ItemBase item )
	{
		if ( item.IsInherited( Weapon_Base ) )	
		{
			ItemBase optic = ItemBase.Cast( item.FindAttachmentBySlotName( "weaponOptics" ) );
			if (optic != NULL)
			{
				if ( optic.IsInherited(ExpansionReflexMRSOptic) )
				{
					m_optic = ExpansionReflexMRSOptic.Cast( optic );
				}
				else { return false; }
			}
			else { return false; }
		}
		else { return false; }
		
		if ( m_optic && m_optic.HasEnergyManager() && m_optic.GetCompEM().CanWork() && item.FindAttachmentBySlotName("weaponOptics").IsKindOf("ExpansionReflexMRSOptic") ) //TODO review conditions for turning off
		{
			return true;
		}
		
		return false;
	}

	override void OnExecuteServer( ActionData action_data )
	{
		//if ( m_optic.HasEnergyManager() )
		{
			//if ( m_optic.GetCompEM().CanWork() && m_optic.GetCompEM().IsSwitchedOn() )
			{
				ItemBase optic_attachment = ItemBase.Cast( action_data.m_MainItem.FindAttachmentBySlotName( "weaponOptics" ) );
				ExpansionReflexMRSOptic optic = ExpansionReflexMRSOptic.Cast( optic_attachment );
				if ( optic )
					optic.CycleOpticMode();
			}
		}
	}
}
