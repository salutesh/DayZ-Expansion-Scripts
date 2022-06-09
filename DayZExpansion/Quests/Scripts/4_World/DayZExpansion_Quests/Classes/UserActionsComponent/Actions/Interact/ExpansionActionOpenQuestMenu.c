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
#ifdef EXPANSIONMODAI
	private ExpansionQuestNpcAIBase m_NPCAIObject;
#else
	private ExpansionQuestNpcBase m_NPCObject;
#endif
	private ExpansionQuestModule m_QuestModule;
	private ExpansionScriptViewMenuBase m_Menu;
	string m_ActionText = "Get Quest";

	void ExpansionActionOpenQuestMenu()
	{
		m_CommandUID = DayZPlayerConstants.CMD_ACTIONMOD_ATTACHITEM;
	}

	override void CreateConditionComponents()
	{
		m_ConditionItem = new CCINone;
		m_ConditionTarget = new CCTObject(UAMaxDistances.BASEBUILDING);
	}

	override string GetText()
	{
		return m_ActionText;
	}

	override typename GetInputType()
	{
		return InteractActionInput;
	}
	
	override void OnStartClient( ActionData action_data )
	{
		super.OnStartClient(action_data);
		
		if (!m_QuestModule)
			m_QuestModule = ExpansionQuestModule.Cast(CF_ModuleCoreManager.Get(ExpansionQuestModule));

		m_QuestModule.RequestPlayerQuests();
	}
	
	override bool ActionCondition( PlayerBase player, ActionTarget target, ItemBase item )
	{
		super.ActionCondition(player, target, item);

	#ifdef EXPANSIONMODAI
		if (!Class.CastTo(m_NPCAIObject, target.GetParentOrObject()))
			return false;
	#else
		if (!Class.CastTo(m_NPCObject, target.GetParentOrObject()))
			return false;
	#endif

		if (!Class.CastTo(m_QuestModule, CF_ModuleCoreManager.Get(ExpansionQuestModule)))
			return false;

		//! Dont show action if menu is already opened
		if (Class.CastTo(m_Menu, GetDayZGame().GetExpansionGame().GetExpansionUIManager().GetMenu()))
			return false;

		ExpansionQuestNpcData questNPCData;
		int npcID;

	#ifdef EXPANSIONMODAI
		npcID = m_NPCAIObject.GetQuestNPCID();
		questNPCData = m_QuestModule.GetQuestNPCDataByID(npcID);
	#else
		npcID = m_NPCObject.GetQuestNPCID();
		questNPCData = m_QuestModule.GetQuestNPCDataByID(npcID);
	#endif
		
		if (questNPCData) m_ActionText = "Talk to " + questNPCData.GetNPCName();
		else m_ActionText = "Talk to Unknown";

		return true;
	}

	override void OnExecuteServer(ActionData action_data)
	{
		super.OnExecuteServer(action_data);

		PlayerBase player = action_data.m_Player;
		if (!player)
			return;

		if (!player.GetIdentity())
			return;

		int npcID;
	#ifdef EXPANSIONMODAI
		if (!Class.CastTo(m_NPCAIObject, action_data.m_Target.GetParentOrObject()))
			return;

		npcID = m_NPCAIObject.GetQuestNPCData().GetID();
	#else
		if (!Class.CastTo(m_NPCObject, action_data.m_Target.GetParentOrObject()))
			return;

		npcID = m_NPCObject.GetQuestNPCData().GetID();
	#endif

		if (!Class.CastTo(m_QuestModule, CF_ModuleCoreManager.Get(ExpansionQuestModule)))
			return;

		/*if (!m_QuestModule.HasQuestAtNPC(npcID, player.GetIdentity().GetId()))
			return;*/

		m_QuestModule.RequestOpenQuestMenu(npcID, player.GetIdentity());
	}
}