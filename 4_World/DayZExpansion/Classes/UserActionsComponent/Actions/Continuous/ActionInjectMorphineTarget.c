/**
 * ActionInjectMorphineTarget.c.c
 *
 * DayZ Expansion Mod
 * www.dayzexpansion.com
 * © 2020 DayZ Expansion Mod Team
 *
 * This work is licensed under the Creative Commons Attribution-NonCommercial-NoDerivatives 4.0 International License. 
 * To view a copy of this license, visit http://creativecommons.org/licenses/by-nc-nd/4.0/.
 *
*/

modded class ActionInjectMorphineTarget
{
	private ExpansionHumanityModule m_HumanityModule;

	void ActionInjectMorphineTarget()
	{
		Class.CastTo( m_HumanityModule, GetModuleManager().GetModule( ExpansionHumanityModule ) );
	}

    override void ApplyModifiers( ActionData action_data )
	{
		if ( m_HumanityModule && GetExpansionSettings().GetGeneral().EnableHumanity )
		{
			m_HumanityModule.OnInjectMorphineToTargetActionEvent( action_data.m_Player, action_data.m_Target.GetObject() );
		}

        super.ApplyModifiers(action_data);
    }
}
