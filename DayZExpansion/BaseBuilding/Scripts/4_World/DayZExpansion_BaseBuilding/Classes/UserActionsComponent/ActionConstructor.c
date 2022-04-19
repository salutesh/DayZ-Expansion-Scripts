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

		//! Basebuilding
		actions.Insert( ExpansionActionSelectNextPlacement );
		actions.Insert( ExpansionActionDamageBaseBuilding );
		actions.Insert( ExpansionActionConvertFloor );

		actions.Insert( ExpansionActionConnectElectricityToSource );
		actions.Insert( ExpansionActionDisconnectElectricityToSource );
		
		actions.Insert( ExpansionActionTogglePowerSwitch );

		actions.Insert( ExpansionActionClose );
		actions.Insert( ExpansionActionOpen );

		//! Locks
		actions.Insert( ExpansionActionEnterCodeLock );
		actions.Insert( ExpansionActionChangeCodeLock );
		actions.Insert( ExpansionActionEnterSafeLock );
		actions.Insert( ExpansionActionChangeSafeLock );
		actions.Insert( ExpansionActionAttachCodeLock );

		//! Safes
		actions.Insert( ExpansionActionCloseAndLockSafe );
		actions.Insert( ExpansionActionCloseSafe ); 

		actions.Insert( ExpansionActionEnterFlagMenu );
		actions.Insert( ExpansionActionDismantleFlag );


		//! Raiding
		actions.Insert( ExpansionActionCrackSafe );
		actions.Insert( ExpansionActionCrackContainer );
		actions.Insert( ExpansionActionDestroyBarbedWire );
		actions.Insert( ExpansionActionDestroyLock );

		actions.Insert( ExpansionActionDestroyTerritory );
	}
}
