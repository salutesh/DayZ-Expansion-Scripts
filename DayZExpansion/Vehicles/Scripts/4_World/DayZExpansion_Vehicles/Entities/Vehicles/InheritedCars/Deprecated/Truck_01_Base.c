/**
 * Truck_01_Base.c
 *
 * DayZ Expansion Mod
 * www.dayzexpansion.com
 * © 2020 DayZ Expansion Mod Team
 *
 * This work is licensed under the Creative Commons Attribution-NonCommercial-NoDerivatives 4.0 International License.
 * To view a copy of this license, visit http://creativecommons.org/licenses/by-nc-nd/4.0/.
 *
*/
modded class Truck_01_Base
{
	override bool CanObjectAttach( Object obj )
	{
		return false;
	}

	override float GetTransportCameraDistance()
	{
		return 6.0;
	}

	override float GetCameraHeight()
	{
		return 3.2;
	}
}