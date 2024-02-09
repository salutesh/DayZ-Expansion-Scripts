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
	// ActionConstructor RegisterActions
	// ------------------------------------------------------------
	override void RegisterActions( TTypenameArray actions )
	{
		super.RegisterActions( actions );	

		actions.Insert( ExpansionActionPaint );

		actions.Insert( ExpansionActionDebugStoreEntity );

		actions.Insert( ExpansionActionRestoreEntity );
		actions.Insert( ExpansionActionPlaceDebugGoat );

		actions.Insert( ExpansionActionDebugLobotomize );
	}
};