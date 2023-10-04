/**
 * ActionFillGeneratorTank.c
 *
 * DayZ Expansion Mod
 * www.dayzexpansion.com
 * © 2023 DayZ Expansion Mod Team
 *
 * This work is licensed under the Creative Commons Attribution-NonCommercial-NoDerivatives 4.0 International License.
 * To view a copy of this license, visit http://creativecommons.org/licenses/by-nc-nd/4.0/.
 *
*/

modded class ActionFillGeneratorTank
{
	override bool ActionCondition( PlayerBase player, ActionTarget target, ItemBase item )
	{
		Expansion_Satellite_Generator satelliteGen;
		if (Class.CastTo(satelliteGen, target.GetObject()))
		{
			string selection = satelliteGen.GetActionComponentName(target.GetComponentIndex());
			if (satelliteGen.IsFuelTank(selection))
				return satelliteGen.CanAddFuel(item);
		}

		return super.ActionCondition(player,target,item);
	}
};
