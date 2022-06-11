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
#endif
	private ExpansionQuestNpcBase m_NPCObject;
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

		//! Dont show action if menu is already opened
		if (Class.CastTo(m_Menu, GetDayZGame().GetExpansionGame().GetExpansionUIManager().GetMenu()))
			return false;
		
		ExpansionQuestNpcData questNPCData;
		int npcID;
		
	#ifdef EXPANSIONMODAI
		if (Class.CastTo(m_NPCAIObject, target.GetParentOrObject()))
		{
			npcID = m_NPCAIObject.GetQuestNPCID();
			questNPCData = m_NPCAIObject.GetQuestNPCData();
		}
	#endif
		
		if (Class.CastTo(m_NPCObject, target.GetParentOrObject()))
		{
			npcID = m_NPCObject.GetQuestNPCID();
			questNPCData = m_NPCObject.GetQuestNPCData();
		}
		
		if (questNPCData)
		{
			m_ActionText = "Talk to " + questNPCData.GetNPCName();
		}
		else
		{
			m_ActionText = "Talk to Unknown";
		}

		return true;
	}

	override void OnExecuteServer(ActionData action_data)
	{
		super.OnExecuteServer(action_data);

		PlayerBase player = action_data.m_Player;
		if (!player || !player.GetIdentity())
			return;
		
		if (!Class.CastTo(m_QuestModule, CF_ModuleCoreManager.Get(ExpansionQuestModule)))
			return;

		int npcID;
	#ifdef EXPANSIONMODAI
		if (Class.CastTo(m_NPCAIObject, action_data.m_Target.GetParentOrObject()))
			npcID = m_NPCAIObject.GetQuestNPCData().GetID();
	#endif
		
		if (Class.CastTo(m_NPCObject, action_data.m_Target.GetParentOrObject()))
			npcID = m_NPCObject.GetQuestNPCData().GetID();

		m_QuestModule.RequestOpenQuestMenu(npcID, player.GetIdentity());
	}
}