/**
 * ActionTurnOnPowerGenerator.c
 *
 * DayZ Expansion Mod
 * www.dayzexpansion.com
 * © 2022 DayZ Expansion Mod Team
 *
 * This work is licensed under the Creative Commons Attribution-NonCommercial-NoDerivatives 4.0 International License.
 * To view a copy of this license, visit http://creativecommons.org/licenses/by-nc-nd/4.0/.
 *
*/

modded class ActionTurnOnPowerGenerator
{
	override bool ActionCondition(PlayerBase player, ActionTarget target, ItemBase item)
	{
		Expansion_Satellite_Generator satelliteGen;
		if (Class.CastTo(satelliteGen, target.GetObject()))
		{
			if (satelliteGen.HasEnergyManager() && satelliteGen.GetCompEM().CanSwitchOn() && satelliteGen.HasSparkplug() && satelliteGen.GetCompEM().CanWork())
			{
				string selection = satelliteGen.GetActionComponentName(target.GetComponentIndex());
				if (satelliteGen.IsSwitch(selection))
					return true;
			}
			return false;
		}

		return super.ActionCondition(player, target, item);
	}
};