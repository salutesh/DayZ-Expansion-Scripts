/**
 * ActionGiveBloodTarget.c
 *
 * DayZ Expansion Mod
 * www.dayzexpansion.com
 * © 2022 DayZ Expansion Mod Team
 *
 * This work is licensed under the Creative Commons Attribution-NonCommercial-NoDerivatives 4.0 International License. 
 * To view a copy of this license, visit http://creativecommons.org/licenses/by-nc-nd/4.0/.
 *
*/

modded class ActionGiveBloodTarget
{
	override bool ActionCondition ( PlayerBase player, ActionTarget target, ItemBase item )
	{
		if (!super.ActionCondition(player, target, item))
			return false;

		DayZPlayerImplement targetPlayer;
		if (Class.CastTo(targetPlayer, target.GetObject()) && !targetPlayer.Expansion_CanBeDamaged())
			return false;

		return true;
	}
};