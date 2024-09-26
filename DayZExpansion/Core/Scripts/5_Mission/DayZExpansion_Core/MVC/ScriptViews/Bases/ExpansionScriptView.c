/**
 * ExpansionScriptView.c
 *
 * DayZ Expansion Mod
 * www.dayzexpansion.com
 * © 2022 DayZ Expansion Mod Team
 *
 * This work is licensed under the Creative Commons Attribution-NonCommercial-NoDerivatives 4.0 International License.
 * To view a copy of this license, visit http://creativecommons.org/licenses/by-nc-nd/4.0/.
 *
*/

#ifdef EXPANSIONUI
class ExpansionScriptView: ExpansionScriptViewBase
{
	protected ref Timer m_UpdateTimer;
	
	void ExpansionScriptView()
	{
		CreateUpdateTimer();
	}
	
	void ~ExpansionScriptView()
	{
		DestroyUpdateTimer();
	}
	
	override void Show(bool show)
	{
		SetIsVisible(show);
		GetLayoutRoot().Show(show);
		if (show)
			OnShow();
		else
			OnHide();
	}

	override void Show()
	{
		Show(true);
	}
	
	override void Hide()
	{
		Show(false);
	}
		
	float GetUpdateTickRate()
	{
		return -1;
	}
	
	protected void CreateUpdateTimer()
	{
		if (!m_UpdateTimer && GetUpdateTickRate() != -1)
		{
			m_UpdateTimer = new Timer(CALL_CATEGORY_GUI);
			m_UpdateTimer.Run(GetUpdateTickRate(), this, "Expansion_Update", NULL, true);
		}
	}
	
	protected void DestroyUpdateTimer()
	{
		if (m_UpdateTimer)
		{
			m_UpdateTimer.Stop();
			m_UpdateTimer = null;
		}
	}
	
	void Expansion_Update();
}
#endif
