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

[CF_RegisterModule(ExpansionESPModificationModule)]
class ExpansionESPModificationModule : CF_ModuleGame
{
	private JMESPModule m_BaseModule;

	override void OnInit()
	{
		super.OnInit();

		EnableMissionStart();
		Expansion_EnableRPCManager();
	}

	override void OnMissionStart(Class sender, CF_EventArgs args)
	{
		super.OnMissionStart(sender, args);
		
		m_BaseModule = JMESPModule.Cast( CF_ModuleCoreManager.Get( JMESPModule ) );
	}
};
#endif
#endif
