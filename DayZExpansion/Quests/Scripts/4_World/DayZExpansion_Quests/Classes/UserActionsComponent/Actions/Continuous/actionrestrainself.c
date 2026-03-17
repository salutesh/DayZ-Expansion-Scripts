/**
 * ActionRestrainSelf.c
 *
 * DayZ Expansion Mod
 * www.dayzexpansion.com
 * © 2022 DayZ Expansion Mod Team
 *
 * This work is licensed under the Creative Commons Attribution-NonCommercial-NoDerivatives 4.0 International License.
 * To view a copy of this license, visit http://creativecommons.org/licenses/by-nc-nd/4.0/.
 *
*/

modded class ActionRestrainSelf
{
	override void OnFinishProgressServer(ActionData action_data)
	{
		PlayerBase targetPlayer = PlayerBase.Cast(action_data.m_Target.GetObject());
		if (targetPlayer)
		{
			ItemBase itemInHands = ItemBase.Cast(targetPlayer.GetHumanInventory().GetEntityInHands());
			if (itemInHands)
				itemInHands.CheckAssignedObjectivesForEntity(ExpansionQuestItemState.INV_EXIT, targetPlayer);
		}

		super.OnFinishProgressServer(action_data);
	}
};
