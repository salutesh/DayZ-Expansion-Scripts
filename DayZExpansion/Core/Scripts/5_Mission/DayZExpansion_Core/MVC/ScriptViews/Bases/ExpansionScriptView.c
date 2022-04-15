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
	
	override void Show()
	{	
		SetIsVisible(true);
		GetLayoutRoot().Show(true);
		OnShow();
	}
	
	override void Hide()
	{
		SetIsVisible(false);
		GetLayoutRoot().Show(false);
		OnHide();
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
			m_UpdateTimer.Run(GetUpdateTickRate(), this, "Update", NULL, true);
		}
	}
	
	protected void DestroyUpdateTimer()
	{
		if (m_UpdateTimer)
		{
			m_UpdateTimer.Stop();
			delete m_UpdateTimer;
		}
	}
	
	void Update();
}