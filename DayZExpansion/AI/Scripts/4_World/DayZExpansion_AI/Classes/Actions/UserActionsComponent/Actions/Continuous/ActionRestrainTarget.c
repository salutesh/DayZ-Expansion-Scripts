/**
 * ActionRestrainTarget.c
 *
 * DayZ Expansion Mod
 * www.dayzexpansion.com
 * © 2024 DayZ Expansion Mod Team
 *
 * This work is licensed under the Creative Commons Attribution-NonCommercial-NoDerivatives 4.0 International License. 
 * To view a copy of this license, visit http://creativecommons.org/licenses/by-nc-nd/4.0/.
 *
*/

modded class ActionRestrainTarget
{
	override void OnStartServer(ActionData action_data)
	{
		eAIBase ai;
		if (Class.CastTo(ai, action_data.m_Target.GetObject()))
		{
			eAIGroup group = ai.GetGroup();
			eAIGroup playerGroup = action_data.m_Player.GetGroup();

			if (group == playerGroup)
				return;

			if (ai.GetGroup().GetFaction().IsGuard())
				action_data.m_Player.m_eAI_LastAggressionTime = ExpansionStatic.GetTime(true);  //! Aggro guards in area (if any)

			if (!ai.Expansion_CanBeDamaged())
				return;

			float threat = 1.0;

			auto info = action_data.m_Player.GetTargetInformation();
			auto target = info.AddAI(ai);

			auto state = ai.eAI_GetTargetInformationState(info, false);
			if (threat > state.m_ThreatLevelActive)
				state.SetInitial(threat, action_data.m_Player.GetPosition());
		}

		super.OnStartServer(action_data);
	}

	override void OnFinishProgressServer(ActionData action_data)
	{
		eAIBase ai;
		if (!Class.CastTo(ai, action_data.m_Target.GetObject()))
		{
			super.OnFinishProgressServer(action_data);
			return;
		}

		if (CanReceiveAction(action_data.m_Target) && !ai.IsRestrained())
		{
			EntityAI item_in_hands_target = ai.GetHumanInventory().GetEntityInHands();
			EntityAI item_in_hands_source = action_data.m_Player.GetHumanInventory().GetEntityInHands();

			if (!item_in_hands_source)
			{
				Error("Restraining target failed, nothing in hands");
				return;
			}
	
			string new_item_name = MiscGameplayFunctions.ObtainRestrainItemTargetClassname(item_in_hands_source);
			if (item_in_hands_target)
			{
				Print("Restraining player with item in hands, first drop & then restrain");

				ai.eAI_DropItemInHandsImpl(false, false);
			}
			else
			{
				Print("Restraining player with empty hands");
			}

			item_in_hands_source.DeleteSafe();
			action_data.m_Player.OnItemInHandsChanged();

			ai.GetHumanInventory().CreateInHands(new_item_name);
			ai.SetRestrained(true);
			ai.OnItemInHandsChanged();
			
			action_data.m_Player.GetSoftSkillsManager().AddSpecialty(m_SpecialtyWeight);
		}
	}
}
