class ExpansionActionSelectNextPlacement: ActionInteractBase
{
	private Hologram m_Hologram;
	private int m_NextType;

	void ExpansionActionSelectNextPlacement()
	{
		m_CommandUID = DayZPlayerConstants.CMD_ACTIONFB_INTERACT;
		m_HUDCursorIcon = CursorIcons.Cursor;
	}
	
	override string GetText()
	{
		string type = m_Hologram.GetPossiblePlacingTypes()[m_NextType];
		
		string displayName;
		GetGame().ConfigGetText( "cfgVehicles " + type + " displayName", displayName );

		return string.Format( Widget.TranslateString( "#STR_USRACT_CHOOSE_NEXT_PLACEMENT" ), Widget.TranslateString( displayName ) );
	}

	override bool HasTarget()
	{
		return false;
	}

	override bool ActionCondition( PlayerBase player, ActionTarget target, ItemBase item )
	{
		if ( IsMissionClient() )
		{
			m_Hologram = player.GetHologramLocal();
		} else
		{
			m_Hologram = player.GetHologramServer();
		}
		
		if ( !m_Hologram )
			return false;

		if ( m_Hologram.GetPlacingTypeCount() > 1 )
		{
			m_NextType = m_Hologram.GetPlacingTypeChosen() + 1;
			if ( m_NextType >= m_Hologram.GetPlacingTypeCount() )
				m_NextType = 0;

			return true;
		}

		return false;
	}
	
	override void OnStartClient( ActionData action_data )
	{
		super.OnStartClient( action_data );

		m_Hologram.SetPlacingTypeChosen( m_NextType );
	}

	override void OnStartServer( ActionData action_data )
	{
		super.OnStartServer( action_data );
		
		m_Hologram.SetPlacingTypeChosen( m_NextType );
	}
}