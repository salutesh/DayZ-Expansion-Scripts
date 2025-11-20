/**
 * ExpansionActionOpenQuestMenu.c
 *
 * DayZ Expansion Mod
 * www.dayzexpansion.com
 * © 2022 DayZ Expansion Mod Team
 *
 * This work is licensed under the Creative Commons Attribution-NonCommercial-NoDerivatives 4.0 International License.
 * To view a copy of this license, visit http://creativecommons.org/licenses/by-nc-nd/4.0/.
 *
*/

class ExpansionActionOpenQuestMenu: ActionInteractBase
{
	protected ExpansionQuestModule m_Expansion_QuestModule;

	void ExpansionActionOpenQuestMenu()
	{
		m_CommandUID = DayZPlayerConstants.CMD_ACTIONMOD_ATTACHITEM;
	}

	override void CreateConditionComponents()
	{
		m_ConditionItem = new CCINone;
		m_ConditionTarget = new CCTCursor;
	}

	override typename GetInputType()
	{
		return InteractActionInput;
	}

	override bool ActionCondition(PlayerBase player, ActionTarget target, ItemBase item)
	{
		Object targetObject;
		if (!Class.CastTo(targetObject, target.GetParentOrObject()))
			return false;

		auto npc = ExpansionQuestNPCBase.Cast(targetObject);
	#ifdef EXPANSIONMODAI
		auto npcAI = ExpansionQuestNPCAIBase.Cast(targetObject);
	#endif
		auto npcObject = ExpansionQuestStaticObject.Cast(targetObject);

	#ifdef EXPANSIONMODAI
		if (!npc && !npcAI && !npcObject)
	#else
		if (!npc && !npcObject)
	#endif
			return false;

		if (!g_Game.IsDedicatedServer())
		{
			//! Client
			//! Dont show action if menu is already opened
			if (GetDayZGame().GetExpansionGame().GetExpansionUIManager().GetMenu())
				return false;

			string actionText;
		#ifdef EXPANSIONMODAI
			if (npc || npcAI)
		#else
			if (npc)
		#endif
				actionText = "#STR_EXPANSION_QUEST_ACTION_TALK";
			else
				actionText = "#STR_EXPANSION_QUEST_ACTION_READ";

			m_Text = actionText + " " + targetObject.GetDisplayName();
		}

		return true;
	}

	/**
	 * Client/server handshake
	 * 
	 * Server: ExpansionActionOpenQuestMenu::OnExecuteServer
	 * Server: ExpansionQuestModule::RequestOpenQuestMenu
	 * Client: ExpansionQuestModule::RPC_RequestOpenQuestMenu
	 * Client: Invoke ExpansionQuestMenu::SetQuests
	 **/
	override void OnExecuteServer(ActionData action_data)
	{
#ifdef EXTRACE
		auto trace = EXTrace.Start(EXTrace.QUESTS, this);
#endif

		super.OnExecuteServer(action_data);

		PlayerBase player = action_data.m_Player;
		if (!player || !player.GetIdentity())
			return;

		if (!CF_Modules<ExpansionQuestModule>.Get(m_Expansion_QuestModule))
			return;

		Object targetObject;
		if (!Class.CastTo(targetObject, action_data.m_Target.GetParentOrObject()))
			return;

		m_Expansion_QuestModule.RequestOpenQuestMenu(targetObject, player.GetIdentity());
	}
}
