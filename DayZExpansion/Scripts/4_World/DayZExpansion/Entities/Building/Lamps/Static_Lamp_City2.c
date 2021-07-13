/**
 * Static_Lamp_City2.c
 *
 * DayZ Expansion Mod
 * www.dayzexpansion.com
 * © 2020 DayZ Expansion Mod Team
 *
 * This work is licensed under the Creative Commons Attribution-NonCommercial-NoDerivatives 4.0 International License. 
 * To view a copy of this license, visit http://creativecommons.org/licenses/by-nc-nd/4.0/.
 *
*/

/**@class		Static_Lamp_City2
 * @brief		
 **/
class Static_Lamp_City2: ExpansionLampLightBase
{
	// ------------------------------------------------------------
	// GetLampColor
	// ------------------------------------------------------------
	override vector GetLampColor()
	{
		return "0.9 0.8 0.6";
	}
};
class bldr_Lamp_City2: Static_Lamp_City2 {};