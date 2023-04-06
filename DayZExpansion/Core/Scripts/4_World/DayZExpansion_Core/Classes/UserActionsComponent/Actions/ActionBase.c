/**
 * ActionBase.c
 *
 * DayZ Expansion Mod
 * www.dayzexpansion.com
 * © 2022 DayZ Expansion Mod Team
 *
 * This work is licensed under the Creative Commons Attribution-NonCommercial-NoDerivatives 4.0 International License. 
 * To view a copy of this license, visit http://creativecommons.org/licenses/by-nc-nd/4.0/.
 *
*/

modded class ActionBase
{
	override void OnStart( ActionData action_data )
	{
		super.OnStart( action_data );

		if ( action_data.m_MainItem )
			action_data.m_MainItem.ExpansionActionOnStart( action_data );
	}

	override void OnUpdate( ActionData action_data )
	{
		super.OnUpdate( action_data );

		if ( action_data.m_MainItem )
			action_data.m_MainItem.ExpansionActionOnUpdate( action_data );
	}

	override void OnEndRequest( ActionData action_data )
	{
		super.OnEndRequest( action_data );

		if ( action_data.m_MainItem )
			action_data.m_MainItem.ExpansionActionOnEndRequest( action_data );
	}

	override void OnEndInput(ActionData action_data)
	{
		super.OnEndInput( action_data );

		if ( action_data.m_MainItem )
			action_data.m_MainItem.ExpansionActionOnEndInput( action_data );
	}

	override void OnEnd( ActionData action_data  )
	{
		super.OnEnd( action_data );

		if ( action_data.m_MainItem )
			action_data.m_MainItem.ExpansionActionOnEnd( action_data );
	}

	bool Expansion_CheckSuccess(ActionData action_data)
	{
		return true;
	}
}
