/**
 * CarBattery.c
 *
 * DayZ Expansion Mod
 * www.dayzexpansion.com
 * © 2024 DayZ Expansion Mod Team
 *
 * This work is licensed under the Creative Commons Attribution-NonCommercial-NoDerivatives 4.0 International License. 
 * To view a copy of this license, visit http://creativecommons.org/licenses/by-nc-nd/4.0/.
 *
*/

#ifdef EXPANSIONMODGARAGE
modded class CarBattery
{
	override bool CanPutIntoHands(EntityAI player)
	{
		if (HasEnergyManager())
		{
			ItemBase poweredDevice = ItemBase.Cast(GetCompEM().GetPluggedDevice());
			if (poweredDevice && poweredDevice.IsInherited(ExpansionParkingMeter))
				return true;
		}

		return super.CanPutIntoHands(parent);
	}
}
#endif
