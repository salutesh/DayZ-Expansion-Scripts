/**
 * Static_Power_Pole_Wood4_Lamp.c
 *
 * DayZ Expansion Mod
 * www.dayzexpansion.com
 * © 2020 DayZ Expansion Mod Team
 *
 * This work is licensed under the Creative Commons Attribution-NonCommercial-NoDerivatives 4.0 International License. 
 * To view a copy of this license, visit http://creativecommons.org/licenses/by-nc-nd/4.0/.
 *
*/

/**@class		Static_Power_Pole_Wood4_Lamp
 * @brief		
 **/
class Static_Power_Pole_Wood4_Lamp: ExpansionLampLightBase
{
	override vector GetLampPosition()
	{
		return "-0.000977 3.65 0.418945";
	}
};
class bldr_Power_Pole_Wood4_Lamp: Static_Power_Pole_Wood4_Lamp {};