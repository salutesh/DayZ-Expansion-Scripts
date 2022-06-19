/**
 * ExpansionQuestNpc.c
 *
 * DayZ Expansion Mod
 * www.dayzexpansion.com
 * © 2022 DayZ Expansion Mod Team
 *
 * This work is licensed under the Creative Commons Attribution-NonCommercial-NoDerivatives 4.0 International License.
 * To view a copy of this license, visit http://creativecommons.org/licenses/by-nc-nd/4.0/.
 *
*/

class ExpansionQuestNpcBase extends DayZPlayer
{
	protected ref TInputActionMap m_InputActionMap;
	bool m_ActionsInitialize;
	private ref ExpansionHumanCommandTrader_ST m_CommandTraderTable;
	private int m_QuestNPCID = -1;
	private ref ExpansionQuestNpcData m_QuestNPCData;

	// ------------------------------------------------------------
	// ExpansionQuestNpcBase Constructor
	// ------------------------------------------------------------
	void ExpansionQuestNpcBase()
	{
		if (IsMissionHost())
			SetAllowDamage(false);

		RegisterNetSyncVariableInt("m_QuestNPCID");
	}

	override void DeferredInit()
    {
		super.DeferredInit();

		SetSynchDirty();

	#ifdef EXPANSIONMODQUESTSMODULEDEBUG
		Print("-----------------------------------------------------------------------------------------");
		Print(ToString() + "::DeferredInit - NPC ID: " + m_QuestNPCID);
		Print(ToString() + "::DeferredInit - NPC type: " + GetType());
		Print("-----------------------------------------------------------------------------------------");
	#endif
    }

	// --------------------------------------------------
	// ExpansionQuestNpcBase SetActions
	//---------------------------------------------------
	void SetActions()
	{
		//AddAction(ExpansionActionOpenQuestMenu);
	}

	// ------------------------------------------------------------
	// ExpansionQuestNpcBase InitializeActions
	// ------------------------------------------------------------
	void InitializeActions()
	{
		m_InputActionMap = new TInputActionMap;
		SetActions();
	}

	// ------------------------------------------------------------
	// ExpansionQuestNpcBase GetActions
	// ------------------------------------------------------------
	override void GetActions(typename action_input_type, out array<ActionBase_Basic> actions)
	{
		if(!m_ActionsInitialize)
		{
			m_ActionsInitialize = true;
			InitializeActions();
		}

		actions = m_InputActionMap.Get(action_input_type);
	}

	// ------------------------------------------------------------
	// ExpansionQuestNpcBase AddAction
	// ------------------------------------------------------------
	void AddAction(typename actionName)
	{
		ActionBase action = ActionManagerBase.GetAction(actionName);

		typename ai = action.GetInputType();
		if(!ai)
		{
			m_ActionsInitialize = false;
			return;
		}

		array<ActionBase_Basic> action_array = m_InputActionMap.Get( ai );

		if(!action_array)
		{
			action_array = new array<ActionBase_Basic>;
			m_InputActionMap.Insert(ai, action_array);
		}
		action_array.Insert(action);
	}

	// ------------------------------------------------------------
	// ExpansionQuestNpcBase RemoveAction
	// ------------------------------------------------------------
	void RemoveAction(typename actionName)
	{
		ActionBase action = ActionManagerBase.GetAction(actionName);
		typename ai = action.GetInputType();
		array<ActionBase_Basic> action_array = m_InputActionMap.Get( ai );

		if(action_array)
		{
			for(int i = 0; i < action_array.Count(); i++)
			{
				if(action == action_array.Get(i))
				{
					action_array.Remove(i);
				}
			}
			action_array = new array<ActionBase_Basic>;
			m_InputActionMap.Insert(ai, action_array);
		}
		action_array.Insert(action);
	}

	// ------------------------------------------------------------
	// ExpansionQuestNpcBase IsInventoryVisible
	// ------------------------------------------------------------
	override bool IsInventoryVisible()
	{
		return false;
	}

	// ------------------------------------------------------------
	// ExpansionQuestNpcBase CanDisplayCargo
	// ------------------------------------------------------------
	override bool CanDisplayCargo()
	{
		return false;
	}

	// ------------------------------------------------------------
	// ExpansionQuestNpcBase CanPutIntoHands
	// ------------------------------------------------------------
	override bool CanPutIntoHands(EntityAI parent)
	{
		return false;
	}

	// ------------------------------------------------------------
	// ExpansionQuestNpcBase CommandHandler
	// ------------------------------------------------------------
	override void CommandHandler(float pDt, int pCurrentCommandID, bool pCurrentCommandFinished)
	{
		if (pCurrentCommandID == DayZPlayerConstants.COMMANDID_SCRIPT)
		{
			HumanCommandScript hcs = GetCommand_Script();

			ExpansionHumanCommandTrader traderCommand;
			if (Class.CastTo(traderCommand, hcs))
			{
				return;
			}
		}

		if (m_CommandTraderTable == NULL)
			m_CommandTraderTable = new ExpansionHumanCommandTrader_ST(this);

		StartCommand_Script(new ExpansionHumanCommandTrader(this, m_CommandTraderTable));
	}

	// ------------------------------------------------------------
	// ExpansionQuestNpcBase SetQuestNPCID
	// ------------------------------------------------------------
	void SetQuestNPCID(int id)
	{
		m_QuestNPCID = id;
		SetSynchDirty();
	}

	// ------------------------------------------------------------
	// ExpansionQuestNpcBase GetQuestNPCID
	// ------------------------------------------------------------
	int GetQuestNPCID()
	{
		return m_QuestNPCID;
	}

	// ------------------------------------------------------------
	// ExpansionQuestNpcBase SetQuestNPCData
	// ------------------------------------------------------------
	void SetQuestNPCData(ExpansionQuestNpcData questNPCData)
	{
		m_QuestNPCData = questNPCData;
	}

	// ------------------------------------------------------------
	// ExpansionQuestNpcBase GetQuestNPCData
	// ------------------------------------------------------------
	ExpansionQuestNpcData GetQuestNPCData()
	{
		return m_QuestNPCData;
	}
};

class ExpansionNpcMirek: ExpansionQuestNpcBase {};
class ExpansionNpcDenis: ExpansionQuestNpcBase {};
class ExpansionNpcBoris: ExpansionQuestNpcBase {};
class ExpansionNpcCyril: ExpansionQuestNpcBase {};
class ExpansionNpcElias: ExpansionQuestNpcBase {};
class ExpansionNpcFrancis: ExpansionQuestNpcBase {};
class ExpansionNpcGuo: ExpansionQuestNpcBase {};
class ExpansionNpcHassan: ExpansionQuestNpcBase {};
class ExpansionNpcIndar: ExpansionQuestNpcBase {};
class ExpansionNpcJose: ExpansionQuestNpcBase {};
class ExpansionNpcKaito: ExpansionQuestNpcBase {};
class ExpansionNpcLewis: ExpansionQuestNpcBase {};
class ExpansionNpcManua: ExpansionQuestNpcBase {};
class ExpansionNpcNiki: ExpansionQuestNpcBase {};
class ExpansionNpcOliver: ExpansionQuestNpcBase {};
class ExpansionNpcPeter: ExpansionQuestNpcBase {};
class ExpansionNpcQuinn: ExpansionQuestNpcBase {};
class ExpansionNpcRolf: ExpansionQuestNpcBase  {};
class ExpansionNpcSeth: ExpansionQuestNpcBase {};
class ExpansionNpcTaiki: ExpansionQuestNpcBase {};
class ExpansionNpcLinda: ExpansionQuestNpcBase {};
class ExpansionNpcMaria: ExpansionQuestNpcBase {};
class ExpansionNpcFrida: ExpansionQuestNpcBase {};
class ExpansionNpcGabi: ExpansionQuestNpcBase {};
class ExpansionNpcHelga: ExpansionQuestNpcBase {};
class ExpansionNpcIrena: ExpansionQuestNpcBase {};
class ExpansionNpcJudy: ExpansionQuestNpcBase {};
class ExpansionNpcKeiko: ExpansionQuestNpcBase {};
class ExpansionNpcEva: ExpansionQuestNpcBase {};
class ExpansionNpcNaomi: ExpansionQuestNpcBase {};
class ExpansionNpcBaty: ExpansionQuestNpcBase {};