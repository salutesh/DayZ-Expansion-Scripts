/**
 * DayZPlayerImplementSwimming.c
 *
 * DayZ Expansion Mod
 * www.dayzexpansion.com
 * © 2020 DayZ Expansion Mod Team
 *
 * This work is licensed under the Creative Commons Attribution-NonCommercial-NoDerivatives 4.0 International License. 
 * To view a copy of this license, visit http://creativecommons.org/licenses/by-nc-nd/4.0/.
 *
*/

modded class DayZPlayerImplementSwimming
{
	private bool m_WasCalledCurrentCommand;
	private bool m_LastReturn;

	void ResetCall()
	{
		m_WasCalledCurrentCommand = false;
	}

	override bool HandleSwimming( int pCurrentCommandID, HumanCommandMove pCMove, HumanMovementState pState )
	{
		return super.HandleSwimming( pCurrentCommandID, pCMove, pState );
/*
		if ( m_WasCalledCurrentCommand )
			return m_LastReturn;

		m_LastReturn = super.HandleSwimming( pCurrentCommandID, pCMove, pState );
		m_WasCalledCurrentCommand = true;
		return m_LastReturn;
*/
	}
}