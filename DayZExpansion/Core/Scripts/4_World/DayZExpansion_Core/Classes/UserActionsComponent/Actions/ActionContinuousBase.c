/**
 * ActionContinuousBase.c
 *
 * DayZ Expansion Mod
 * www.dayzexpansion.com
 * © 2022 DayZ Expansion Mod Team
 *
 * This work is licensed under the Creative Commons Attribution-NonCommercial-NoDerivatives 4.0 International License. 
 * To view a copy of this license, visit http://creativecommons.org/licenses/by-nc-nd/4.0/.
 *
*/

modded class ActionContinuousBase
{
	override void OnStartAnimationLoop( ActionData action_data )
	{
		super.OnStartAnimationLoop( action_data );

		if ( action_data.m_MainItem )
			action_data.m_MainItem.ExpansionActionOnStartAnimationLoop( action_data );
	}

	override void OnFinishProgress( ActionData action_data )
	{
		super.OnFinishProgress( action_data );

		if ( action_data.m_MainItem )
			action_data.m_MainItem.ExpansionActionOnFinishProgress( action_data );
	}

	override void OnEndAnimationLoop( ActionData action_data  )
	{
		super.OnEndAnimationLoop( action_data );

		if ( action_data.m_MainItem )
			action_data.m_MainItem.ExpansionActionOnEndAnimationLoop( action_data );
	}
}
