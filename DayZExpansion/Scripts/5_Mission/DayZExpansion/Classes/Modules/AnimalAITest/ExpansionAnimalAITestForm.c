/**
 * ExpansionAnimalAITestForm.c
 *
 * DayZ Expansion Mod
 * www.dayzexpansion.com
 * © 2020 DayZ Expansion Mod Team
 *
 * This work is licensed under the Creative Commons Attribution-NonCommercial-NoDerivatives 4.0 International License. 
 * To view a copy of this license, visit http://creativecommons.org/licenses/by-nc-nd/4.0/.
 *
*/

class ExpansionAnimalAITestForm extends JMFormBase
{
	private DayZCreature m_Creature;
	private ref DayZCreatureAnimScriptDebug m_EntityAnim;

	private Widget m_ActionsWrapper;
	private Widget m_ActionsFilterWrapper;

	private UIActionEditableText m_Int;
	private UIActionEditableText m_Float;

	private TextListboxWidget m_MissionsList;

	private autoptr array< string > m_Commands = new array< string >;

	void ExpansionAnimalAITestForm()
	{
	}

	void ~ExpansionAnimalAITestForm()
	{
	}

	override void OnInit()
	{
		m_ActionsFilterWrapper = layoutRoot.FindAnyWidget( "actions_filter_wrapper" );

		UIActionManager.CreateButton( m_ActionsFilterWrapper, "Spawn", this, "Spawn" );

		m_MissionsList = TextListboxWidget.Cast( layoutRoot.FindAnyWidget("game_missions_list") );

		m_ActionsWrapper = layoutRoot.FindAnyWidget( "actions_wrapper" );

		UIActionManager.CreateButton( m_ActionsWrapper, "Run", this, "Run" );
		m_Int = UIActionManager.CreateEditableText( m_ActionsWrapper, "Int: " );
		m_Int.SetOnlyNumbers( true, true );
		m_Float = UIActionManager.CreateEditableText( m_ActionsWrapper, "Float: " );
		m_Float.SetOnlyNumbers( true, false );

		UpdateList();
	}

	void UpdateList()
	{
		m_MissionsList.ClearItems();

		for ( int i = 0; i < m_Commands.Count(); i++ )
		{
			m_MissionsList.AddItem( m_Commands[i], NULL, 0 );
		}
	}

	int GetCurrentSelectedCommand()
	{
		if ( m_MissionsList.GetSelectedRow() != -1 )
		{
			string mission_name;
			m_MissionsList.GetItemText( m_MissionsList.GetSelectedRow(), 0, mission_name );
			return m_Commands.Find( mission_name );
		}

		return -1;
	}

	void Spawn( UIEvent eid, ref UIActionBase action ) 
	{
		m_Commands.Clear();

		if ( !Class.CastTo( m_Creature, GetGame().CreateObject( "Animal_CanisLupus_Grey", GetCursorPos() ) ) )
			return;

		m_EntityAnim = new DayZCreatureAnimScriptDebug();
		m_EntityAnim.SetCreature( m_Creature );

		for ( int i = 0; i < m_EntityAnim.GetCommandCount(); i++ )
			m_Commands.Insert( m_EntityAnim.GetCommandName( i ) );

		UpdateList();
	}

	void Run( UIEvent eid, ref UIActionBase action ) 
	{
		int command = GetCurrentSelectedCommand();
		if ( command < 0 )
			return;

		int iNum = m_Int.GetText().ToInt();
		float fNum = m_Float.GetText().ToFloat();

		m_EntityAnim.ActivateCommand( command, iNum, fNum );
	}
}