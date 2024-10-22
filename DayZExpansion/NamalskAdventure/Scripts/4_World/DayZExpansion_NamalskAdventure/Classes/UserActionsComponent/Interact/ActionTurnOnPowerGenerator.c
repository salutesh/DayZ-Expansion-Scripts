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
			bool hasEnergyManager = satelliteGen.HasEnergyManager();
			bool canSwitchOn = satelliteGen.GetCompEM().CanSwitchOn();
			bool hasSparkplug = satelliteGen.HasSparkplug();
			bool canWork = satelliteGen.GetCompEM().CanWork();
			
			Print(ToString() + "::ActionCondition - Has energy manager=" + hasEnergyManager + " | Can switch on=" + canSwitchOn + " | Has sparkplug=" + hasSparkplug + " | Can work=" + canWork);
			if (hasEnergyManager && canSwitchOn && hasSparkplug && canWork)
			{
				string selection = satelliteGen.GetActionComponentName(target.GetComponentIndex());
				//if (satelliteGen.IsSwitch(selection)) //ToDo: Condition is borked, need to check what changed on the generator model and its selecten that we used here
					return true;
			}
		}

		if (!super.ActionCondition(player, target, item))
		{
			return false;
		}

		return false;
	}
};