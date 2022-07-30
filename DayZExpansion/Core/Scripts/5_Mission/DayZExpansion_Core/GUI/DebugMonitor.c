/**
 * DebugMonitor.c
 *
 * DayZ Expansion Mod
 * www.dayzexpansion.com
 * © 2022 DayZ Expansion Mod Team
 *
 * This work is licensed under the Creative Commons Attribution-NonCommercial-NoDerivatives 4.0 International License. 
 * To view a copy of this license, visit http://creativecommons.org/licenses/by-nc-nd/4.0/.
 *
*/

modded class DebugMonitor
{
	static ref DebugMonitor Instance;

	void DebugMonitor()
	{
		Instance = this;
	}

	bool IsVisible()
	{
		return m_WidgetRoot.IsVisible();
	}

	void Show()
	{
		m_WidgetRoot.Show(true);
	}
}

#ifdef JM_COT
#ifndef JM_COT_DEBUGMONITOR_REMOVED
modded class JMDebugMonitor
{
	static ref JMDebugMonitor Instance;

	bool m_ForcedHidden;

	void JMDebugMonitor()
	{
		Instance = this;
	}

	bool IsVisible()
	{
		return layoutRoot.IsVisible();
	}

	override void Show()
	{
		if (m_ForcedHidden)
			return;

		super.Show();
	}

	void ForceHide()
	{
		Hide();
		m_ForcedHidden = true;
	}

	void ForceShow()
	{
		m_ForcedHidden = false;
		Show();
	}
}
#endif
#endif
