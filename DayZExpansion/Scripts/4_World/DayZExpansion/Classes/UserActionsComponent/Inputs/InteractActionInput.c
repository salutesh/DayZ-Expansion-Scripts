modded class InteractActionInput
{
    override void UpdatePossibleActions( PlayerBase player, ActionTarget target, ItemBase item, int action_condition_mask )
    {
		if ( ForceActionCheck( player ) )
		{
			m_SelectActions.Clear();
			m_SelectActions.Insert( m_ForcedActionData.m_Action );
			return;
		}
        
        if ( GetGame().IsClient() || !GetGame().IsMultiplayer() )
        {
            if ( player.GetHologramLocal() )
            {
                m_SelectActions.Insert( ActionManagerBase.GetAction( ExpansionActionSelectNextPlacement ) );
            }
        }

        super.UpdatePossibleActions( player, target, item, action_condition_mask );
    }
}