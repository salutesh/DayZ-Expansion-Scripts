/**
 * ExpansionSkinDamageZone.c
 *
 * DayZ Expansion Mod
 * www.dayzexpansion.com
 * © 2022 DayZ Expansion Mod Team
 *
 * This work is licensed under the Creative Commons Attribution-NonCommercial-NoDerivatives 4.0 International License. 
 * To view a copy of this license, visit http://creativecommons.org/licenses/by-nc-nd/4.0/.
 *
*/

class ExpansionSkinDamageZone : Managed
{
	string Zone;
	
	ref array< string > HiddenSelections;
	ref array< ref ExpansionSkinHealthLevel > HealthLevels;
	
	void ExpansionSkinDamageZone()
	{
		HiddenSelections = new array< string >;
		HealthLevels = new array< ref ExpansionSkinHealthLevel >;
	}
};