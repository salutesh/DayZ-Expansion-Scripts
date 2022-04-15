/**
 * ExpansionGallusGallusDomesticus.c
 *
 * DayZ Expansion Mod
 * www.dayzexpansion.com
 * © 2022 DayZ Expansion Mod Team
 *
 * This work is licensed under the Creative Commons Attribution-NonCommercial-NoDerivatives 4.0 International License. 
 * To view a copy of this license, visit http://creativecommons.org/licenses/by-nc-nd/4.0/.
 *
*/

modded class Animal_GallusGallusDomesticus
{
	static ref map<typename, ref TInputActionMap> m_GallusTypeActionsMap = new map<typename, ref TInputActionMap>;

	TInputActionMap m_InputActionMap;
	bool m_ActionsInitialize;

	void Animal_GallusGallusDomesticus()
	{
		m_ActionsInitialize = false;
	}

	void InitializeActions()
	{
		m_InputActionMap = m_GallusTypeActionsMap.Get( this.Type() );
		if ( !m_InputActionMap )
		{
			TInputActionMap iam = new TInputActionMap;
			m_InputActionMap = iam;
			SetActions();
			m_GallusTypeActionsMap.Insert( this.Type(), m_InputActionMap );
		}
	}
	
	override void GetActions(typename action_input_type, out array<ActionBase_Basic> actions)
	{
		if ( !m_ActionsInitialize )
		{
			m_ActionsInitialize = true;
			InitializeActions();
		}
		
		actions = m_InputActionMap.Get( action_input_type );
	}
	
	void SetActions()
	{
		// AddAction( ExpansionActionTakeChicken );
	}
	
	void AddAction( typename actionName )
	{
		ActionBase action = ActionManagerBase.GetAction(actionName);

		if ( !action )
		{
			Debug.LogError( "Action " + actionName + " dosn't exist!" );
			return;
		}		
		
		typename ai = action.GetInputType();
		if ( !ai )
		{
			m_ActionsInitialize = false;
			return;
		}

		array<ActionBase_Basic> action_array = m_InputActionMap.Get( ai );
		
		if ( !action_array ) 
		{
			action_array = new array<ActionBase_Basic>;
			m_InputActionMap.Insert( ai, action_array );
		}

		action_array.Insert( action );
	}
	
	void RemoveAction( typename actionName )
	{
		PlayerBase player = PlayerBase.Cast(GetGame().GetPlayer());
		ActionBase action = player.GetActionManager().GetAction(actionName);
		typename ai = action.GetInputType();
		array<ActionBase_Basic> action_array = m_InputActionMap.Get( ai );
		
		if ( action_array )
		{
			action_array.RemoveItem( action );
		}
	}
}