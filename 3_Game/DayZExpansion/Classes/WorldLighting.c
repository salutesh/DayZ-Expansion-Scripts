/*
 * WorldLighting.c
 *
 * DayZ Expansion Mod
 * www.dayzexpansion.com
 * © 2020 DayZ Expansion Mod Team
 *
 * This work is licensed under the Creative Commons Attribution-NonCommercial-NoDerivatives 4.0 International License. 
 * To view a copy of this license, visit http://creativecommons.org/licenses/by-nc-nd/4.0/.
 *
*/

modded class WorldLighting
{
	protected string lighting_expansion = "DayZExpansion\\Dta\\World\\Lighting\\lighting_expansion.txt";
	
	override void SetGlobalLighting( int lightingID )
	{
		super.SetGlobalLighting( lightingID );
		
		switch ( lightingID )
		{
			case 2:
				GetGame().GetWorld().LoadNewLightingCfg( lighting_expansion );
				break;
		}
	}
}