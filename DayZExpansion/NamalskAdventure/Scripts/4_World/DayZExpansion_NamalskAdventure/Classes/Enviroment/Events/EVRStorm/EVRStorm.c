/**
 * EVRStorm.c
 *
 * DayZ Expansion Mod
 * www.dayzexpansion.com
 * © 2022 DayZ Expansion Mod Team
 *
 * This work is licensed under the Creative Commons Attribution-NonCommercial-NoDerivatives 4.0 International License.
 * To view a copy of this license, visit http://creativecommons.org/licenses/by-nc-nd/4.0/.
 *
*/

modded class EVRStorm
{
	override void InitPhaseServer()
	{
		EXTrace.Start(EXTrace.NAMALSKADVENTURE, this);
		
		super.InitPhaseServer();
		
		ExpansionNamalskModule expNamalskModule = ExpansionNamalskModule.GetModuleInstance();
		if (expNamalskModule)
		{
			expNamalskModule.OnEVRStormInitPhaseServer();
		}
	}

	override void MidPhaseServer()
	{
		EXTrace.Start(EXTrace.NAMALSKADVENTURE, this);
		
		super.MidPhaseServer();
	}

	override void EndPhaseServer()
	{
		EXTrace.Start(EXTrace.NAMALSKADVENTURE, this);
		
		super.EndPhaseServer();
		
		ExpansionNamalskModule expNamalskModule = ExpansionNamalskModule.GetModuleInstance();
		if (expNamalskModule)
		{
			expNamalskModule.OnEVRStormEndPhaseServer();
		}
	}

	override void InitPhaseClient(float phase_time)
	{
		EXTrace.Start(EXTrace.NAMALSKADVENTURE, this);
		
		super.InitPhaseClient(phase_time);
	}

	override void MidPhaseClient(float phase_time)
	{
		EXTrace.Start(EXTrace.NAMALSKADVENTURE, this);
		
		super.MidPhaseClient(phase_time);
	}

	override void EndPhaseClient(float phase_time)
	{
		EXTrace.Start(EXTrace.NAMALSKADVENTURE, this);
		
		super.EndPhaseClient(phase_time);
	}

	override void OnEVRFinished(notnull SurvivorBase player)
	{
		EXTrace.Start(EXTrace.NAMALSKADVENTURE, this);
		
		super.OnEVRFinished(player);
	}
	
	protected void DebugTrace(string text)
	{
		EXTrace.Start(EXTrace.NAMALSKADVENTURE, this, text);
	}
};