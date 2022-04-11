/**
 * ExpansionESPModificationModule.c
 *
 * DayZ Expansion Mod
 * www.dayzexpansion.com
 * © 2022 DayZ Expansion Mod Team
 *
 * This work is licensed under the Creative Commons Attribution-NonCommercial-NoDerivatives 4.0 International License. 
 * To view a copy of this license, visit http://creativecommons.org/licenses/by-nc-nd/4.0/.
 *
*/

#ifdef JM_COT
#ifdef EXPANSIONCOTESPMODULE
modded class JMESPModule
{
	JMESPMeta EXP_GetMeta( Object target )
	{
		return m_MappedESPObjects.Get( target );
	}
};

class ExpansionESPModificationModule : JMModuleBase
{
	private JMESPModule m_BaseModule;

	override void OnMissionStart()
	{
		super.OnMissionStart();
		
		m_BaseModule = JMESPModule.Cast( GetModuleManager().GetModule( JMESPModule ) );
	}

	override int GetRPCMin()
	{
		return ExpansionESPModificationModuleRPC.INVALID;
	}

	override int GetRPCMax()
	{
		return ExpansionESPModificationModuleRPC.COUNT;
	}
};
#endif
#endif
