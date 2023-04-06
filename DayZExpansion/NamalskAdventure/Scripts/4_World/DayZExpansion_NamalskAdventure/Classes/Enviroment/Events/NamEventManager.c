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

modded class NamEventManager
{
	override void StartEvent(typename event_type, bool force = false)
	{
		auto trace = EXTrace.Start(EXTrace.NAMALSKADVENTURE, this);

		super.StartEvent(event_type, force);

		ExpansionNamalskModule expNamalskModule = ExpansionNamalskModule.GetModuleInstance();
		if (expNamalskModule)
			expNamalskModule.OnNamalskEventStart(event_type);
	}

	override void CancelEvent(typename event_type)
	{
		auto trace = EXTrace.Start(EXTrace.NAMALSKADVENTURE, this);

		super.CancelEvent(event_type);

		ExpansionNamalskModule expNamalskModule = ExpansionNamalskModule.GetModuleInstance();
		if (expNamalskModule)
			expNamalskModule.OnNamalskEventCancel(event_type);
	}

	override private void ServerEventThread()
	{
		auto trace = EXTrace.Start(EXTrace.NAMALSKADVENTURE, this);

		super.ServerEventThread();

		m_ServerEventTimer.Run(m_NextEventIn, this, "ServerEventThread");

	#ifdef DIAG
		ExpansionNotification(new StringLocaliser("NAMALSK EVENT MANAGER"), new StringLocaliser("SERVER EVENT THREAD - %1", m_LastEventType.ToString()), ExpansionIcons.GetPath("Exclamationmark"), COLOR_EXPANSION_NOTIFICATION_INFO, 7, ExpansionNotificationType.TOAST).Create();
	#endif
	}

	typename GetLastEventType()
	{
		return m_LastEventType;
	}
};