/**
 * ExpansionQuestObjectiveActionEventData.c
 *
 * DayZ Expansion Mod
 * www.dayzexpansion.com
 * © 2022 DayZ Expansion Mod Team
 *
 * This work is licensed under the Creative Commons Attribution-NonCommercial-NoDerivatives 4.0 International License.
 * To view a copy of this license, visit http://creativecommons.org/licenses/by-nc-nd/4.0/.
 *
*/

class ExpansionQuestObjectiveActionEventData
{
	ActionBase m_ActionBase;
	ActionData m_ActionData;
	string m_ActionName;
	EntityAI m_Target;

	int m_ConditionInt;

	void SetTargetActionTarget()
	{
		m_Target = EntityAI.Cast(m_ActionData.m_Target.GetParentOrObject());
	}

	void SetTargetActionPlayer()
	{
		m_Target = m_ActionData.m_Player;
	}

	void SetTargetActionMainItem()
	{
		m_Target = m_ActionData.m_MainItem;
	}
};