// Copyright 2021 William Bowers
//
// Licensed under the Apache License, Version 2.0 (the "License");
// you may not use this file except in compliance with the License.
// You may obtain a copy of the License at
//
//    http://www.apache.org/licenses/LICENSE-2.0
//
// Unless required by applicable law or agreed to in writing, software
// distributed under the License is distributed on an "AS IS" BASIS,
// WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
// See the License for the specific language governing permissions and
// limitations under the License.


class eAIActionManager: ActionManagerBase
{
	protected bool								m_ActionPossible;
	protected ref array<ref InventoryLocation>	m_ReservedInventoryLocations;
	protected ref eAIInventoryActionHandler		m_InventoryActionHandler;
	protected ref InventoryLocation				m_HandInventoryLocationTest;
	protected ref TTypeNameActionInputMap		m_RegistredInputsMap;
	
	protected ref ActionData 					m_PendingActionData;

	//protected bool								m_ActionWantEnd;		//If set action try end // send request to server to end
	protected bool 								m_ActionWantEndRequest_Send;		//Request to server was sended
	protected bool								m_ActionInputWantEnd_Send;	
	
	void eAIActionManager(PlayerBase player)
	{		
		m_InventoryActionHandler = new eAIInventoryActionHandler(player);

		RegisterInputs(player);
	}
	
	void RegisterInputs(PlayerBase player)
	{
		if (!m_RegistredInputsMap)
		{
			m_RegistredInputsMap = new TTypeNameActionInputMap;

			for (int i = 0; i < m_ActionsArray.Count(); i++)
			{	
				ActionBase action = m_ActionsArray.Get(i);
				typename input_type_name = action.GetInputType();
				ActionInput ai = ActionInput.Cast(m_RegistredInputsMap.Get(input_type_name));
				if(!ai)
				{
					ai = ActionInput.Cast(input_type_name.Spawn());
					m_RegistredInputsMap.Insert(input_type_name, ai);
				}
				action.SetInput(ai);
			}
		
			foreach (auto type, auto ain : m_RegistredInputsMap)
			{
				ain.eAI_Init(player, this);
			}
		}
	}

	//------------------------------------------
	//EVENTS 
	//------------------------------------------
	// Continuous---------------------------------------------
	override void OnJumpStart()
	{
		if(m_CurrentActionData)
		{
			if( m_CurrentActionData.m_State == UA_AM_PENDING || m_CurrentActionData.m_State == UA_AM_REJECTED || m_CurrentActionData.m_State == UA_AM_ACCEPTED)
			{
				OnActionEnd();
				m_PendingActionAcknowledgmentID = -1;
			}
			else
			{
				m_CurrentActionData.m_Action.Interrupt(m_CurrentActionData);
			}
		}
	}
	
	override bool ActionPossibilityCheck(int pCurrentCommandID)
	{
		if (pCurrentCommandID == DayZPlayerConstants.COMMANDID_SCRIPT)
			return super.ActionPossibilityCheck(DayZPlayerConstants.COMMANDID_MOVE);

		return super.ActionPossibilityCheck(pCurrentCommandID);
	}
	
	override void Update(int pCurrentCommandID)
	{
		m_InventoryActionHandler.OnUpdate();
		super.Update(pCurrentCommandID);
		m_ActionPossible = ActionPossibilityCheck(pCurrentCommandID);
		
		if (m_PendingActionData) //SP only
		{
			m_CurrentActionData = m_PendingActionData;
			
			m_CurrentActionData.m_Action.Start(m_CurrentActionData);
			
			if ( m_CurrentActionData.m_Action.IsInstant() )
				OnActionEnd();
			
			m_PendingActionData = null;
		}
		
		if (m_CurrentActionData)
		{
			if (m_CurrentActionData.m_State != UA_AM_PENDING && m_CurrentActionData.m_State != UA_AM_REJECTED && m_CurrentActionData.m_State != UA_AM_ACCEPTED)
				m_CurrentActionData.m_Action.OnUpdateServer(m_CurrentActionData);
				//m_CurrentActionData.m_Action.OnUpdateClient(m_CurrentActionData);
			
			switch (m_CurrentActionData.m_State)
			{
				case UA_AM_PENDING:
					break;
			
				case UA_AM_ACCEPTED:
					// check pCurrentCommandID before start or reject 
					if ( m_ActionPossible && pCurrentCommandID != DayZPlayerConstants.COMMANDID_SWIM && pCurrentCommandID != DayZPlayerConstants.COMMANDID_LADDER && ( !m_Player.IsRestrained() || m_CurrentActionData.m_Action.CanBeUsedInRestrain() ) )
					{
						m_CurrentActionData.m_State = UA_START;
						m_CurrentActionData.m_Action.Start(m_CurrentActionData);
						
						if ( m_CurrentActionData.m_Action.IsInstant() )
							OnActionEnd();
					}
					else
					{
						OnActionEnd();
					}
					m_PendingActionAcknowledgmentID = -1;
					break;
				
				case UA_AM_REJECTED:
					OnActionEnd();
					m_PendingActionAcknowledgmentID = -1;
			
					//m_Player.GetDayZPlayerInventory().UnlockHands();
					break;
			
				default:
					if ( m_ActionWantEndRequest )
					{
						if ( GetGame().IsMultiplayer() && !m_CurrentActionData.m_Action.IsLocal() )
						{
							if ( !m_ActionWantEndRequest_Send && ScriptInputUserData.CanStoreInputUserData() )
							{
								if( LogManager.IsActionLogEnable() )
								{	
									Debug.ActionLog("Time stamp: " + m_Player.GetSimulationTimeStamp(), m_CurrentActionData.m_Action.ToString() , "n/a", "EndRequest", m_CurrentActionData.m_Player.ToString() );
								}
								ScriptInputUserData ctx = new ScriptInputUserData;
								ctx.Write(INPUT_UDT_STANDARD_ACTION_END_REQUEST);
								ctx.Send();
								
								m_ActionWantEndRequest_Send = true;
									
								m_ActionWantEndRequest = false;
								m_CurrentActionData.m_Action.EndRequest(m_CurrentActionData);
							}
						}
						else
						{
							m_ActionWantEndRequest = false;
							m_CurrentActionData.m_Action.EndRequest(m_CurrentActionData);
						}
					}
					
					if ( m_ActionInputWantEnd )
					{
						if ( GetGame().IsMultiplayer() && !m_CurrentActionData.m_Action.IsLocal() )
						{
							if ( !m_ActionInputWantEnd_Send && ScriptInputUserData.CanStoreInputUserData() )
							{
								if( LogManager.IsActionLogEnable() )
								{
									Debug.ActionLog("Time stamp: " + m_Player.GetSimulationTimeStamp(), m_CurrentActionData.m_Action.ToString() , "n/a", "EndInput", m_CurrentActionData.m_Player.ToString() );
								}
								ScriptInputUserData ctxi = new ScriptInputUserData;
								ctxi.Write(INPUT_UDT_STANDARD_ACTION_INPUT_END);
								ctxi.Send();
								
								m_ActionInputWantEnd_Send = true;
									
								m_ActionInputWantEnd = false;
								m_CurrentActionData.m_Action.EndInput(m_CurrentActionData);
							}
						}
						else
						{
							if (!m_ActionInputWantEnd_Send)
							{
								m_ActionInputWantEnd_Send = true;
								m_ActionInputWantEnd = false;
								m_CurrentActionData.m_Action.EndInput(m_CurrentActionData);
							}
						}
					}
				break;
			}
		}
	}

	void UnlockInventory(ActionData action_data)
	{
		if( LogManager.IsActionLogEnable() )
		{
			Debug.ActionLog("(O) Inventory unlock", action_data.m_Action.ToString() , "n/a", "UnlockInventory", action_data.m_Player.ToString() );
		}

		if (action_data.m_Action)
			action_data.m_Action.ClearInventoryReservationEx(action_data);
	}
	
	protected void ActionStart(ActionBase action, ActionTarget target, ItemBase item, Param extra_data = NULL )
	{
		if ( !m_CurrentActionData && action ) 
		{
			m_ActionWantEndRequest_Send = false;
			m_ActionInputWantEnd_Send = false;
			m_ActionWantEndRequest = false;
			m_ActionInputWantEnd = false;
			
			if ( action.CanBePerformedFromQuickbar() )
				m_Player.SetActionEndInput(action);
			
			HandleInputsOnActionStart(action);

			if( LogManager.IsActionLogEnable() )
			{
				Debug.ActionLog("Item = " + item + ", " + target.DumpToString(), action.ToString() , "n/a", "ActionStart", m_Player.ToString() );
			}
			m_Interrupted = false;
			
			if( !action.SetupAction(m_Player, target, item, m_CurrentActionData, extra_data ))
			{
				DPrint("Can not inicialize action" + action + " - ActionManagerClient");
				m_CurrentActionData = NULL;
				return;
			}
			
			if( LogManager.IsActionLogEnable() )
			{
				Debug.ActionLog("Action data created wait to start", action.ToString() , "n/a", "ActionStart", m_Player.ToString() );
			}

			action.Start(m_CurrentActionData);
			if( action.IsInstant() )
				OnActionEnd();
		}
	}
	
	void PerformActionStart(ActionBase action, ActionTarget target, ItemBase item, Param extra_data = NULL )
	{
		m_PendingActionData = new ActionData;
	
		if (!action.SetupAction(m_Player,target,item,m_PendingActionData,extra_data))
			m_PendingActionData = null;
	}

	override void OnActionEnd()
	{
		if(m_CurrentActionData)
		{
			UnlockInventory(m_CurrentActionData);
			if( m_CurrentActionData.m_Action.RemoveForceTargetAfterUse() )
				m_InventoryActionHandler.DeactiveAction();
			
			super.OnActionEnd();
			HandleInputsOnActionEnd();
		}
	}

	void HandleInputsOnActionStart(ActionBase action)
	{
		foreach (auto type, auto ain : m_RegistredInputsMap)
		{
			if(action.GetInput() == ain)
			{
				ain.OnActionStart();
			}
			else
			{
				ain.Reset();
			}
		}
	}
	
	void HandleInputsOnActionEnd()
	{
		ResetInputsState();
	}
	
	void ResetInputsState()
	{
		foreach (auto type, auto ain : m_RegistredInputsMap)
		{
			ain.Reset();
		}
	}
	
	void ResetInputsActions()
	{
		foreach (auto type, auto ain : m_RegistredInputsMap)
		{
			ain.ActionsSelectReset();
		}
	}

	void SetInventoryAction(ActionBase action_name, ItemBase target_item, ItemBase main_item)
	{
		m_InventoryActionHandler.SetAction(action_name, target_item, main_item);
	}

	void SetInventoryAction(ActionBase action_name, ActionTarget target, ItemBase main_item)
	{
		m_InventoryActionHandler.SetAction(action_name, target, main_item);
	}
	
	void UnsetInventoryAction()
	{
		m_InventoryActionHandler.DeactiveAction();
	}
};
