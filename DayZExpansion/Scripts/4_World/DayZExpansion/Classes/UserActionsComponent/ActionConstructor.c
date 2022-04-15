/**
 * ActionConstructor.c
 *
 * DayZ Expansion Mod
 * www.dayzexpansion.com
 * © 2022 DayZ Expansion Mod Team
 *
 * This work is licensed under the Creative Commons Attribution-NonCommercial-NoDerivatives 4.0 International License. 
 * To view a copy of this license, visit http://creativecommons.org/licenses/by-nc-nd/4.0/.
 *
*/

/**@class		ActionConstructor
 * @brief		
 **/
modded class ActionConstructor
{
	// ------------------------------------------------------------
	// RegisterActions
	// ------------------------------------------------------------
	override void RegisterActions( TTypenameArray actions )
	{
		super.RegisterActions( actions );
		
		//! Generator
		actions.Insert( ExpansionActionTurnOnGeneratorOnGround );
		actions.Insert( ExpansionActionTurnOffGeneratorOnGround );

		//! Music
		//#ifdef EXPANSION_GUITAR_ENABLE
		//actions.Insert( ExpansionActionStartPlayingGuitar );
		//actions.Insert( ExpansionActionStopPlayingGuitar );
		//#endif
		
		//! Removed because it wont be in first release
		//actions.Insert( ExpansionActionTakeChicken );
		//actions.Insert( ExpansionActionGetQuest );
		//actions.Insert( ExpansionActionTakeInQuest );

		//! Misc
		actions.Insert( ActionLickBattery );
		actions.Insert( ExpansionActionWritePaper );
	}
}