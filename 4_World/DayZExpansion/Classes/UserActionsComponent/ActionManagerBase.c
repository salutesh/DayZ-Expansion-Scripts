/**
 * ActionManagerBase.c
 *
 * DayZ Expansion Mod
 * www.dayzexpansion.com
 * © 2020 DayZ Expansion Mod Team
 *
 * This work is licensed under the Creative Commons Attribution-NonCommercial-NoDerivatives 4.0 International License. 
 * To view a copy of this license, visit http://creativecommons.org/licenses/by-nc-nd/4.0/.
 *
*/

/**@class		ActionManagerBase
 * @brief		
 **/
modded class ActionManagerBase
{
	override bool ActionPossibilityCheck( int pCurrentCommandID )
	{
		//! Print( "IsSprinting: " + m_Player.IsSprinting() );
		//! Print( "IsUnconscious: " + m_Player.IsUnconscious() );
		//! Print( "GetCommandModifier_Action: " + m_Player.GetCommandModifier_Action() );
		//! Print( "GetCommand_Action: " + m_Player.GetCommand_Action() );
		//! Print( "IsEmotePlaying: " + m_Player.IsEmotePlaying() );
		//! Print( "Throwing ANim: " + m_Player.GetThrowing().IsThrowingAnimationPlaying() );
		//! Print( "Inv Processing: " + m_Player.GetDayZPlayerInventory().IsProcessing() );

		if ( pCurrentCommandID == DayZPlayerConstants.COMMANDID_SCRIPT )
		{
			if ( m_Player.GetCommand_ExpansionFall() )
				return true;

			if ( m_Player.GetCommand_ExpansionVehicle() )
				return true;
		}

		return super.ActionPossibilityCheck( pCurrentCommandID );
	}
}