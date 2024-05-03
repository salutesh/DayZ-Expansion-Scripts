/**
 * ActionUnrestrainTargetHands.c
 *
 * DayZ Expansion Mod
 * www.dayzexpansion.com
 * © 2024 DayZ Expansion Mod Team
 *
 * This work is licensed under the Creative Commons Attribution-NonCommercial-NoDerivatives 4.0 International License. 
 * To view a copy of this license, visit http://creativecommons.org/licenses/by-nc-nd/4.0/.
 *
*/

modded class ActionUnrestrainTargetHands
{
	override void OnFinishProgressServer(ActionData action_data)
	{
		eAIBase ai;
		if (!Class.CastTo(ai, action_data.m_Target.GetObject()))
		{
			super.OnFinishProgressServer(action_data);
			return;
		}

		if (CanReceiveAction(action_data.m_Target) && ai.IsRestrained())
		{
			EntityAI item_in_hands = ai.GetItemInHands();
			if (item_in_hands)
			{
				ActionUnrestrainTarget.eAI_Unrestrain(item_in_hands, null, ai);
			}
			else
			{
				ErrorEx("Failed to obtain item in target player's hands during empty handed unrestraining");
			}
		}
	}
}
