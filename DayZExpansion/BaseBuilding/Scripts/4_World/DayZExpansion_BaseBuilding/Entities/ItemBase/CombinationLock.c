/**
 * CombinationLock.c
 *
 * DayZ Expansion Mod
 * www.dayzexpansion.com
 * © 2022 DayZ Expansion Mod Team
 *
 * This work is licensed under the Creative Commons Attribution-NonCommercial-NoDerivatives 4.0 International License. 
 * To view a copy of this license, visit http://creativecommons.org/licenses/by-nc-nd/4.0/.
 *
*/

modded class CombinationLock
{
	override void ExpansionOnDestroyed( Object killer )
	{
		if ( IsDamageDestroyed() )
			return;

		SetHealth( 0 );

	#ifdef DAYZ_1_29
		UnlockServer( EntityAI.Cast( killer ), GetHierarchyParent() );
	#else
		//! 1.30+
		UnlockOnServer(EntityAI.Cast( killer ), GetHierarchyParent());
	#endif
	}
}
