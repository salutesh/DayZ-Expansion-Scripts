/**
 * BarbedWire.c
 *
 * DayZ Expansion Mod
 * www.dayzexpansion.com
 * © 2022 DayZ Expansion Mod Team
 *
 * This work is licensed under the Creative Commons Attribution-NonCommercial-NoDerivatives 4.0 International License. 
 * To view a copy of this license, visit http://creativecommons.org/licenses/by-nc-nd/4.0/.
 *
 */

modded class BarbedWire
{
	override void ExpansionOnDestroyed( Object killer )
	{
		if ( IsDamageDestroyed() )  //! When called by EEKilled, don't override vanilla behavior
			return;

		//! When called by ExpansionActionDestroyBarbedWire health is at 1%, set health to 0 so EEKilled is called (destroy sound)
		SetHealth( 0 );

		//TODO: Maybe spawn wire as replacement?
		if ( !ToDelete() )
			Delete();
	}
}
