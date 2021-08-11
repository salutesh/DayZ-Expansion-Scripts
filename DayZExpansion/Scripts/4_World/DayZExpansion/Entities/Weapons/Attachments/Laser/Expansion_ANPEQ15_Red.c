/**
 * Expansion_ANPEQ15_Red.c
 *
 * DayZ Expansion Mod
 * www.dayzexpansion.com
 * © 2021 DayZ Expansion Mod Team
 *
 * This work is licensed under the Creative Commons Attribution-NonCommercial-NoDerivatives 4.0 International License. 
 * To view a copy of this license, visit http://creativecommons.org/licenses/by-nc-nd/4.0/.
 *
*/
class Expansion_ANPEQ15_Red extends Expansion_Laser_Base
{
	override string LaserColor()
	{
		return "#(argb,8,8,3)color(1,0,0,1.0,co)";
	}

	override string LaserMaterial()
	{
		return "dz\\weapons\\projectiles\\data\\tracer_red.rvmat";
	}

	override vector LaserLightColor()
	{
		return "1.0 0.0 0.0";
	}

	override bool IsVisibleWithoutNVG()
	{
		return true;
	}
};