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

#ifndef DAYZ_1_10
modded class ActionCloseCarDoors
{
	override void OnStartClient( ActionData action_data )
	{
		super.OnStartClient( action_data );

		if ( IsMissionHost() && m_Car )
		{
			m_Car.OnCarDoorClosed( m_AnimSource );
		}
	}

	override void OnStartServer( ActionData action_data )
	{
		super.OnStartServer( action_data );

		if ( m_Car )
		{
			m_Car.OnCarDoorClosed( m_AnimSource );
		}
	}
}
#endif