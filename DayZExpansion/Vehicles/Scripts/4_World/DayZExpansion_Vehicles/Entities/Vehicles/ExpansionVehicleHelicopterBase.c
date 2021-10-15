/**
 * ExpansionVehicleHelicopterBase.c
 *
 * DayZ Expansion Mod
 * www.dayzexpansion.com
 * © 2021 DayZ Expansion Mod Team
 *
 * This work is licensed under the Creative Commons Attribution-NonCommercial-NoDerivatives 4.0 International License. 
 * To view a copy of this license, visit http://creativecommons.org/licenses/by-nc-nd/4.0/.
 *
*/

/**@class		ExpansionVehicleHelicopterBase
 * @brief		This class handle helicopter movement and physics
 **/
class ExpansionVehicleHelicopterBase extends ExpansionVehicleBase
{
	bool IsAutoHover()
	{
		return false;
	}

	void SwitchAutoHover()
	{
	}

	float GetAutoHoverTargetHeight()
	{
		return 0;
	}
};
