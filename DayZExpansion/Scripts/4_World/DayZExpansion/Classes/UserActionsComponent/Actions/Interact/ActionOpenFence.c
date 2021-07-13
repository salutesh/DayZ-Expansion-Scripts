/**
 * ActionOpenFence.c
 *
 * DayZ Expansion Mod
 * www.dayzexpansion.com
 * © 2021 DayZ Expansion Mod Team
 *
 * This work is licensed under the Creative Commons Attribution-NonCommercial-NoDerivatives 4.0 International License.
 * To view a copy of this license, visit http://creativecommons.org/licenses/by-nc-nd/4.0/.
 *
*/

modded class ActionOpenFence
{
	Fence m_Fence;

	override string GetText()
	{
		if ( m_Fence && m_Fence.IsLocked() )
		{
			return "#STR_EXPANSION_OPEN_LOCKED_GATE";
		}
		else
			return "#open";
	}

	override bool ActionCondition( PlayerBase player, ActionTarget target, ItemBase item )
	{
		Object targetObject = target.GetObject();
		if ( targetObject && targetObject.CanUseConstruction() )
		{
			m_Fence = Fence.Cast( targetObject );
			
			if ( m_Fence )
			{
				if ( m_Fence.ExpansionGetCodeLock() )
					return m_Fence.ExpansionCanOpen( player, "" );

				return super.ActionCondition( player, target, item );
			}
		}
		
		return false;
	}
	
	override void OnStartServer( ActionData action_data )
	{
		if ( !m_Fence || ( m_Fence.ExpansionGetCodeLock() && !m_Fence.ExpansionCanOpen( action_data.m_Player, "" ) ) )
			return;

		super.OnStartServer( action_data );
	}
}
