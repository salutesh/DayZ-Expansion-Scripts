/**
 * ActionGiveBloodTarget.c
 *
 * DayZ Expansion Mod
 * www.dayzexpansion.com
 * © 2023 DayZ Expansion Mod Team
 *
 * This work is licensed under the Creative Commons Attribution-NonCommercial-NoDerivatives 4.0 International License. 
 * To view a copy of this license, visit http://creativecommons.org/licenses/by-nc-nd/4.0/.
 *
*/

modded class ActionGiveBloodTarget
{
	override bool ActionCondition ( PlayerBase player, ActionTarget target, ItemBase item )
	{
		bool result = super.ActionCondition(player, target, item);

		eAIBase ai;
		if (Class.CastTo(ai, target.GetObject()) && !ai.Expansion_CanBeDamaged() && ai.GetGroup().GetFaction().IsGuard())
			return true;  //! Hoho

		return result;
	}

	override void OnStartServer(ActionData action_data)
	{
		eAIBase ai;
		if (Class.CastTo(ai, action_data.m_Target.GetObject()) && ai.GetGroup().GetFaction().IsGuard())
		{
			int agents = action_data.m_MainItem.GetAgents();
			int bloodType = action_data.m_MainItem.GetLiquidType();
			int bloodTypeTarget = ai.GetStatBloodType().Get();
			if (agents || !BloodTypes.MatchBloodCompatibility(bloodType, bloodTypeTarget))
			{
				action_data.m_Player.m_eAI_LastAggressionTime = ExpansionStatic.GetTime(true);  //! Aggro guards in area (if any)
				if (!ai.Expansion_CanBeDamaged())
					return;
			}
		}

		super.OnStartServer(action_data);
	}
};
