/**
 * ActionCloseCarDoors.c
 *
 * DayZ Expansion Mod
 * www.dayzexpansion.com
 * © 2020 DayZ Expansion Mod Team
 *
 * This work is licensed under the Creative Commons Attribution-NonCommercial-NoDerivatives 4.0 International License. 
 * To view a copy of this license, visit http://creativecommons.org/licenses/by-nc-nd/4.0/.
 *
*/

modded class ActionCloseCarDoors
{
	CarScript m_CarScript;

	override void OnStartClient( ActionData action_data )
	{
		super.OnStartClient( action_data );

		if ( IsMissionHost() && Class.CastTo( m_CarScript, action_data.m_Target.GetParent() ) )
		{
			m_CarScript.OnCarDoorClosed( m_AnimSource );
		}
	}

	override void OnStartServer( ActionData action_data )
	{
		super.OnStartServer( action_data );

		if ( Class.CastTo( m_CarScript, action_data.m_Target.GetParent() ) )
		{
			m_CarScript.OnCarDoorClosed( m_AnimSource );
		}
	}
}