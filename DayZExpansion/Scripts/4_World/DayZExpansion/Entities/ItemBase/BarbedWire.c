/**
 * BarbedWire.c
 *
 * DayZ Expansion Mod
 * www.dayzexpansion.com
 * © 2021 DayZ Expansion Mod Team
 *
 * This work is licensed under the Creative Commons Attribution-NonCommercial-NoDerivatives 4.0 International License. 
 * To view a copy of this license, visit http://creativecommons.org/licenses/by-nc-nd/4.0/.
 *
 */

modded class BarbedWire
{
	override void ExpansionOnDestroyed( Object killer )
	{
		//TODO: Maybe spawn wire as replacement?
		if ( GetHealth() == 0 && !ToDelete() )
			Delete();
	}
}
