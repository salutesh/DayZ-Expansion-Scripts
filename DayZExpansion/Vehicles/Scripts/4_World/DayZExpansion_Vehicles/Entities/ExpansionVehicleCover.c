/**
 * ExpansionVehicleCover.c
 *
 * DayZ Expansion Mod
 * www.dayzexpansion.com
 * © 2022 DayZ Expansion Mod Team
 *
 * This work is licensed under the Creative Commons Attribution-NonCommercial-NoDerivatives 4.0 International License. 
 * To view a copy of this license, visit http://creativecommons.org/licenses/by-nc-nd/4.0/.
 *
*/

class ExpansionVehicleCover: ExpansionEntityStoragePlaceholder
{
	override void SetActions()
	{
		super.SetActions();

		AddAction(ExpansionActionUncoverVehicle);
	}
};

class Expansion_Generic_Vehicle_Cover: ExpansionVehicleCover {};

#ifndef CARCOVER
class CarCoverBase: ExpansionVehicleCover {};

class CamoNetCivil: CamoNet {};
class CamoNetDesert: CamoNet {};
class CamoNetWinter: CamoNet {};
#endif
