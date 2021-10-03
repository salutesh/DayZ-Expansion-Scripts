/**
 * ActionManagerBase.c
 *
 * DayZ Expansion Mod
 * www.dayzexpansion.com
 * © 2021 DayZ Expansion Mod Team
 *
 * This work is licensed under the Creative Commons Attribution-NonCommercial-NoDerivatives 4.0 International License. 
 * To view a copy of this license, visit http://creativecommons.org/licenses/by-nc-nd/4.0/.
 *
*/

/**@class		ActionManagerBase
 * @brief		
 **/
modded class ActionManagerBase
{
	void ActionManagerBase(PlayerBase player)
	{
#ifdef CF_DebugUI
		CF_Debug.Create(this);
#endif
	}

	void ~ActionManagerBase()
	{
#ifdef CF_DebugUI
		CF_Debug.Destroy(this);
#endif
	}

#ifdef CF_DebugUI
	bool CF_OnDebugUpdate(CF_Debug instance, CF_DebugUI_Type type)
	{
		instance.Add(m_PrimaryAction);
		instance.Add(m_SecondaryAction);

		return true;
	}
#endif

	override bool ActionPossibilityCheck( int pCurrentCommandID )
	{
		if ( pCurrentCommandID == DayZPlayerConstants.COMMANDID_SCRIPT )
		{
			if ( m_Player.GetCommand_ExpansionVehicle() )
				return true;
		}

		return super.ActionPossibilityCheck( pCurrentCommandID );
	}
};
