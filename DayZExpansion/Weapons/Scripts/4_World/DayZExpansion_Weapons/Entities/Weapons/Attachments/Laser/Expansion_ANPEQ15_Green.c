/**
 * Expansion_ANPEQ15_Green.c
 *
 * DayZ Expansion Mod
 * www.dayzexpansion.com
 * © 2022 DayZ Expansion Mod Team
 *
 * This work is licensed under the Creative Commons Attribution-NonCommercial-NoDerivatives 4.0 International License. 
 * To view a copy of this license, visit http://creativecommons.org/licenses/by-nc-nd/4.0/.
 *
*/
class Expansion_ANPEQ15_Green: Expansion_Laser_Base
{
	override string LaserColor()
	{
		return "#(argb,8,8,3)color(0,0.501961,0,1.0,co)";
	}

	override string LaserMaterial()
	{
		return "DZ\\weapons\\projectiles\\data\\tracer_green.rvmat";
	}
	
	override vector LaserLightColor()
	{
		return "0.0 1.0 0.0";
	}

	override bool IsVisibleWithoutNVG()
	{
		return true;
	}
};

class Expansion_ANPEQ15_GreenNV: Expansion_ANPEQ15_Green
{
	override bool IsVisibleWithoutNVG()
	{
		return false;
	}
};
