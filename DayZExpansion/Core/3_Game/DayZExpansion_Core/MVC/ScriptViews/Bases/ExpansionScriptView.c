/**
 * ExpansionScriptView.c
 *
 * DayZ Expansion Mod
 * www.dayzexpansion.com
 * © 2020 DayZ Expansion Mod Team
 *
 * This work is licensed under the Creative Commons Attribution-NonCommercial-NoDerivatives 4.0 International License.
 * To view a copy of this license, visit http://creativecommons.org/licenses/by-nc-nd/4.0/.
 *
*/

class ExpansionScriptView: ScriptView
{
	private bool m_IsVisible = true;
	protected ref Timer m_UpdateTimer;
	
	void ExpansionScriptView()
	{		
		//Debug_Logging = true;
		
		CreateUpdateTimer();
	}
	
	void ~ExpansionScriptView()
	{
		DestroyUpdateTimer();
	}
	
	void SetIsVisible(bool state)
	{
		m_IsVisible = state;
	}
	
	bool IsVisible()
	{
		return m_IsVisible;
	}
	
	void Show()
	{			
		SetIsVisible(true);
		GetLayoutRoot().Show(true);
		OnShow();
	}
	
	void OnShow();
	
	void Hide()
	{
		SetIsVisible(false);
		GetLayoutRoot().Show(false);
		OnHide();
	}
	
	void OnHide();
	
	bool CanShow()
	{
		return false;
	}
	
	float GetUpdateTickRate()
	{
		return -1;
	}
	
	private void CreateUpdateTimer()
	{
		if (!m_UpdateTimer && GetUpdateTickRate() != -1)
		{
			m_UpdateTimer = new Timer(CALL_CATEGORY_GUI);
			m_UpdateTimer.Run(GetUpdateTickRate(), this, "Update", NULL, true);
		}
	}
	
	private void DestroyUpdateTimer()
	{
		if (m_UpdateTimer)
		{
			m_UpdateTimer.Stop();
			delete m_UpdateTimer;
		}
	}
	
	void Update();
}