/**
 * ZombieBase.c
 *
 * DayZ Expansion Mod
 * www.dayzexpansion.com
 * © 2020 DayZ Expansion Mod Team
 *
 * This work is licensed under the Creative Commons Attribution-NonCommercial-NoDerivatives 4.0 International License. 
 * To view a copy of this license, visit http://creativecommons.org/licenses/by-nc-nd/4.0/.
 *
*/

modded class ZombieBase
{
	// ------------------------------------------------------------
	// ZombieBase RegisterTransportHit
	// ------------------------------------------------------------
	override void RegisterTransportHit( Transport transport )
	{
		if ( transport.IsInherited( ExpansionHelicopterScript ) )
			return;
		if ( transport.IsInherited( ExpansionVehicleBase ) )
			return;

		super.RegisterTransportHit( transport );
	}
	
	// ------------------------------------------------------------
	// ZombieBase ModCommandHandlerBefore
	// ------------------------------------------------------------
	/*override bool ModCommandHandlerBefore( float pDt, int pCurrentCommandID, bool pCurrentCommandFinished )
	{
		if ( pCurrentCommandID == DayZInfectedConstants.COMMANDID_SCRIPT)
		{
		}
		
		return super.ModCommandHandlerBefore( pDt, pCurrentCommandID, pCurrentCommandFinished );
	}*/
}