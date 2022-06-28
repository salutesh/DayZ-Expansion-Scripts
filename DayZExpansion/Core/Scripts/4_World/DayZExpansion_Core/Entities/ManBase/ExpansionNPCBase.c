/**
 * ExpansionNPCBase.c
 *
 * DayZ Expansion Mod
 * www.dayzexpansion.com
 * © 2022 DayZ Expansion Mod Team
 *
 * This work is licensed under the Creative Commons Attribution-NonCommercial-NoDerivatives 4.0 International License. 
 * To view a copy of this license, visit http://creativecommons.org/licenses/by-nc-nd/4.0/.
 *
*/

class ExpansionNPCBase extends DayZPlayerImplement
{
	protected ref TInputActionMap m_InputActionMap;

	bool m_ActionsInitialize;

	void ExpansionNPCBase()
	{
		#ifdef EXPANSIONMODAI
		if (GetGame().IsServer())
		{
			eAIGroup group = eAIGroup.CreateGroup();
			group.SetFaction(new eAIFactionPassive());
			SetGroup(group);
		}
		#endif
	}

	void SetActions()
	{
	}

	void InitializeActions()
	{
		m_InputActionMap = new TInputActionMap;
		SetActions();
	}

	override void GetActions(typename action_input_type, out array<ActionBase_Basic> actions)
	{
		if (!m_ActionsInitialize)
		{
			m_ActionsInitialize = true;
			InitializeActions();
		}
		
		actions = m_InputActionMap.Get(action_input_type);
	}

	void AddAction(typename actionName)
	{
		ActionBase action = ActionManagerBase.GetAction(actionName);

		typename ai = action.GetInputType();
		if (!ai)
		{
			m_ActionsInitialize = false;
			return;
		}

		array<ActionBase_Basic> action_array = m_InputActionMap.Get(ai);
		
		if (!action_array)
		{
			action_array = new array<ActionBase_Basic>;
			m_InputActionMap.Insert(ai, action_array);
		}

		action_array.Insert(action); 
	}

	void RemoveAction(typename actionName)
	{
		ActionBase action = ActionManagerBase.GetAction(actionName);
		typename ai = action.GetInputType();
		array<ActionBase_Basic> action_array = m_InputActionMap.Get(ai);
		
		if (action_array)
		{
			for (int i = 0; i < action_array.Count(); i++)
			{
				if (action == action_array.Get(i))
				{
					action_array.Remove(i);
				}
			}
			action_array = new array<ActionBase_Basic>;
			m_InputActionMap.Insert(ai, action_array);
		}
		action_array.Insert(action); 
	}

	override bool IsInventoryVisible()
	{
		return false;
	}

	override bool CanDisplayCargo()
	{
		return false;
	}

	override bool CanPutIntoHands(EntityAI parent)
	{
		return false;
	}

	override void CommandHandler(float pDt, int pCurrentCommandID, bool pCurrentCommandFinished) 
	{
	}
}

class ExpansionNPCMirek: ExpansionNPCBase {};
class ExpansionNPCDenis: ExpansionNPCBase {};
class ExpansionNPCBoris: ExpansionNPCBase {};
class ExpansionNPCCyril: ExpansionNPCBase {};
class ExpansionNPCElias: ExpansionNPCBase {};
class ExpansionNPCFrancis: ExpansionNPCBase {};
class ExpansionNPCGuo: ExpansionNPCBase {};
class ExpansionNPCHassan: ExpansionNPCBase {};
class ExpansionNPCIndar: ExpansionNPCBase {};
class ExpansionNPCJose: ExpansionNPCBase {};
class ExpansionNPCKaito: ExpansionNPCBase {};
class ExpansionNPCLewis: ExpansionNPCBase {};
class ExpansionNPCManua: ExpansionNPCBase {};
class ExpansionNPCNiki: ExpansionNPCBase {};
class ExpansionNPCOliver: ExpansionNPCBase {};
class ExpansionNPCPeter: ExpansionNPCBase {};
class ExpansionNPCQuinn: ExpansionNPCBase {};
class ExpansionNPCRolf: ExpansionNPCBase {};
class ExpansionNPCSeth: ExpansionNPCBase {};
class ExpansionNPCTaiki: ExpansionNPCBase {};
class ExpansionNPCLinda: ExpansionNPCBase {};
class ExpansionNPCMaria: ExpansionNPCBase {};
class ExpansionNPCFrida: ExpansionNPCBase {};
class ExpansionNPCGabi: ExpansionNPCBase {};
class ExpansionNPCHelga: ExpansionNPCBase {};
class ExpansionNPCIrena: ExpansionNPCBase {};
class ExpansionNPCJudy: ExpansionNPCBase {};
class ExpansionNPCKeiko: ExpansionNPCBase {};
class ExpansionNPCEva: ExpansionNPCBase {};
class ExpansionNPCNaomi: ExpansionNPCBase {};
class ExpansionNPCBaty: ExpansionNPCBase {};
