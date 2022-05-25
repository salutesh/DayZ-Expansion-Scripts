/**
 * CF_Localiser.c
 *
 * DayZ Expansion Mod
 * www.dayzexpansion.com
 * © 2022 DayZ Expansion Mod Team
 *
 * This work is licensed under the Creative Commons Attribution-NonCommercial-NoDerivatives 4.0 International License. 
 * To view a copy of this license, visit http://creativecommons.org/licenses/by-nc-nd/4.0/.
 *
*/

modded class CF_Localiser
{
	override CF_Localiser Set(int index, string value)
	{
		string valueUpper = value;
		valueUpper.ToUpper();
		bool translates = valueUpper.IndexOf("STR_") == 0;
		return Set(index, value, translates);
	}
}
