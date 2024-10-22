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

#ifdef NAMALSK_SURVIVAL
enum ExpansionEVRStormPhase
{
	NONE = 0,
	INIT_PHASE = 1,
	MID_PHASE = 2,
	BLOWOUT = 3,
	END_PHASE = 4
};

modded class EVRStorm
{
	protected ExpansionEVRStormPhase m_EVRStormPhase = ExpansionEVRStormPhase.NONE;
	
	override void InitPhaseServer()
	{
#ifdef EXTRACE
		auto trace = EXTrace.Start(EXTrace.NAMALSKADVENTURE, this);
#endif

		m_EVRStormPhase = ExpansionEVRStormPhase.INIT_PHASE;
		ExpansionNamalskModule.GetModuleInstance().OnEVRStormInitPhaseServer();
		ExpansionAnomaliesModule.GetModuleInstance().OnEVRStormInitPhaseServer();

	#ifdef DIAG_DEVELOPER
		ExpansionNotification(new StringLocaliser("EVR STORM"), new StringLocaliser("INIT PHASE STARTED"), ExpansionIcons.GetPath("Exclamationmark"), COLOR_EXPANSION_NOTIFICATION_INFO, 7, ExpansionNotificationType.TOAST).Create();
	#endif

		super.InitPhaseServer();
	}

	override void MidPhaseServer()
	{
#ifdef EXTRACE
		auto trace = EXTrace.Start(EXTrace.NAMALSKADVENTURE, this);
#endif

		m_EVRStormPhase = ExpansionEVRStormPhase.MID_PHASE;
		ExpansionNamalskModule.GetModuleInstance().OnEVRStormMidPhaseServer();
		ExpansionAnomaliesModule.GetModuleInstance().OnEVRStormMidPhaseServer();

	#ifdef DIAG_DEVELOPER
		ExpansionNotification(new StringLocaliser("EVR STORM"), new StringLocaliser("MID PHASE STARTED"), ExpansionIcons.GetPath("Exclamationmark"), COLOR_EXPANSION_NOTIFICATION_INFO, 7, ExpansionNotificationType.TOAST).Create();
	#endif

		super.MidPhaseServer();
	}

	override void EndPhaseServer()
	{
#ifdef EXTRACE
		auto trace = EXTrace.Start(EXTrace.NAMALSKADVENTURE, this);
#endif

		m_EVRStormPhase = ExpansionEVRStormPhase.END_PHASE;

		super.EndPhaseServer();

		ExpansionNamalskModule.GetModuleInstance().OnEVRStormEndPhaseServer();
		ExpansionAnomaliesModule.GetModuleInstance().OnEVRStormEndPhaseServer();

		m_EVRStormPhase = ExpansionEVRStormPhase.NONE;

	#ifdef DIAG_DEVELOPER
		ExpansionNotification(new StringLocaliser("EVR STORM"), new StringLocaliser("END PHASE STARTED"), ExpansionIcons.GetPath("Exclamationmark"), COLOR_EXPANSION_NOTIFICATION_INFO, 7, ExpansionNotificationType.TOAST).Create();
	#endif

	}

	override void CreateBlowoutFinalServer()
	{
#ifdef EXTRACE
		auto trace = EXTrace.Start(EXTrace.NAMALSKADVENTURE, this);
#endif

		m_EVRStormPhase = ExpansionEVRStormPhase.BLOWOUT;

		super.CreateBlowoutFinalServer();
		
		ExpansionNamalskModule.GetModuleInstance().OnEVRStormFinalBlowout();
		ExpansionAnomaliesModule.GetModuleInstance().OnEVRStormFinalBlowout();

	#ifdef DIAG_DEVELOPER
		ExpansionNotification(new StringLocaliser("EVR STORM"), new StringLocaliser("EVR STORM BLOWOUT"), ExpansionIcons.GetPath("Exclamationmark"), COLOR_EXPANSION_NOTIFICATION_INFO, 7, ExpansionNotificationType.TOAST).Create();
	#endif
	}
	
	override void OnEVRFinished(notnull SurvivorBase player)
	{
#ifdef EXTRACE
		auto trace = EXTrace.Start(EXTrace.NAMALSKADVENTURE, this);
#endif

		if (!player.GetIdentity())
			return;

		super.OnEVRFinished(player);
		
		ExpansionNamalskModule.GetModuleInstance().OnEVRStormFinished(player);
		ExpansionAnomaliesModule.GetModuleInstance().OnEVRStormFinished(player);

	#ifdef DIAG_DEVELOPER
		ExpansionNotification(new StringLocaliser("EVR STORM"), new StringLocaliser("EVR STORM FINISHED"), ExpansionIcons.GetPath("Exclamationmark"), COLOR_EXPANSION_NOTIFICATION_INFO, 7, ExpansionNotificationType.TOAST).Create(player.GetIdentity());
	#endif
	}

	/*override void InitPhaseClient(float phase_time)
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
	}*/

	ExpansionEVRStormPhase GetEVRStormPhase()
	{
		return m_EVRStormPhase;
	}

	protected void ExDebugPrint(string text)
	{
		EXTrace.Print(EXTrace.NAMALSKADVENTURE, this, text);
	}
};
#endif