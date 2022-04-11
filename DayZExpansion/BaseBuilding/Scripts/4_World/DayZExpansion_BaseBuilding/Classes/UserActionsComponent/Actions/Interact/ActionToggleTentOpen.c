/**
 * ActionToggleTentOpen.c
 *
 * DayZ Expansion Mod
 * www.dayzexpansion.com
 * © 2022 DayZ Expansion Mod Team
 *
 * This work is licensed under the Creative Commons Attribution-NonCommercial-NoDerivatives 4.0 International License.
 * To view a copy of this license, visit http://creativecommons.org/licenses/by-nc-nd/4.0/.
 *
*/

modded class ActionToggleTentOpen
{
	TentBase m_Tent;
	bool m_IsEntrance;

	override string GetText()
	{
		if ( m_Tent && m_Tent.ExpansionIsLocked() )
		{
			if ( m_IsEntrance )
				return "#STR_EXPANSION_OPEN_LOCKED_TENT";

			return "#STR_EXPANSION_TOGGLE_LOCKED_OPENING";
		}

		return "#toggle_opening";
	}

    override bool ActionCondition(PlayerBase player, ActionTarget target, ItemBase item)
	{
		m_IsEntrance = false;
        m_Tent = TentBase.Cast( target.GetParent() );
        if ( m_Tent )
		{
            if ( m_Tent.ExpansionGetCodeLock() && m_Tent.ExpansionIsLocked() )
			{
				if ( !m_Tent.IsKnownUser( player ) )
					return false;

				Object targetObject = target.GetObject();

				if ( !targetObject )
					return false;

				array< string > selections = new array< string >;
				targetObject.GetActionComponentNameList( target.GetComponentIndex(), selections );

				for ( int s = 0; s < selections.Count(); s++ )
				{
					if ( !selections[s].Contains( "entrance" ) && !selections[s].Contains( "door" ) )
						continue;

					if ( m_Tent.CanToggleAnimations( selections[s] ) )
					{
						m_IsEntrance = true;
						break;
					}
				}
			}
        }

        return super.ActionCondition( player, target, item );
    }

	override void OnExecuteServer( ActionData action_data )
	{
		if ( !m_Tent || ( m_Tent.ExpansionGetCodeLock() && m_Tent.ExpansionIsLocked() && !m_Tent.IsKnownUser( action_data.m_Player ) ) )
			return;

		super.OnExecuteServer( action_data );
	}
};
