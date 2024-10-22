/**
 * NamEventManager.c
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
modded class NamEventManager
{
	override void StartEvent(typename event_type, bool force = false)
	{
#ifdef EXTRACE
		auto trace = EXTrace.Start(EXTrace.NAMALSKADVENTURE, this);
#endif

		super.StartEvent(event_type, force);

		ExpansionNamalskModule.GetModuleInstance().OnNamalskEventStart(event_type);
		ExpansionAnomaliesModule.GetModuleInstance().OnNamalskEventStart(event_type);
	}

	override void CancelEvent(typename event_type)
	{
#ifdef EXTRACE
		auto trace = EXTrace.Start(EXTrace.NAMALSKADVENTURE, this);
#endif

		super.CancelEvent(event_type);

		ExpansionNamalskModule.GetModuleInstance().OnNamalskEventCancel(event_type);
		ExpansionAnomaliesModule.GetModuleInstance().OnNamalskEventCancel(event_type);
	}

	override private void ServerEventThread()
	{
#ifdef EXTRACE
		auto trace = EXTrace.Start(EXTrace.NAMALSKADVENTURE, this);
#endif

		super.ServerEventThread();

	#ifdef DIAG_DEVELOPER
		ExpansionNotification(new StringLocaliser("NAMALSK EVENT MANAGER"), new StringLocaliser("SERVER EVENT THREAD - %1", m_LastEventType.ToString()), ExpansionIcons.GetPath("Exclamationmark"), COLOR_EXPANSION_NOTIFICATION_INFO, 7, ExpansionNotificationType.TOAST).Create();
	#endif
	}

	typename GetLastEventType()
	{
		return m_LastEventType;
	}
};
#endif
