/**
 * ExpansionScriptViewMenu.c
 *
 * DayZ Expansion Mod
 * www.dayzexpansion.com
 * © 2020 DayZ Expansion Mod Team
 *
 * This work is licensed under the Creative Commons Attribution-NonCommercial-NoDerivatives 4.0 International License.
 * To view a copy of this license, visit http://creativecommons.org/licenses/by-nc-nd/4.0/.
 *
*/

class ExpansionScriptViewMenu: ScriptView
{
	private int m_SVMenuID = -1;
	private bool m_IsVisible = false;
	private bool m_Active = false;
	protected ref Timer m_UpdateTimer;

	void ExpansionScriptViewMenu()
	{
		//Debug_Logging = true;
		
		if (GetLayoutRoot().IsVisible()) SetIsVisible(true);
		
		CreateUpdateTimer();
	}
	
	void ~ExpansionScriptViewMenu()
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
	
	void SetID(int id)
	{
		m_SVMenuID = id;
	}
	
	int GetID()
	{
		return m_SVMenuID;
	}
	
	void Show()
	{
		if (!CanShow())
			return;
		
		SetIsVisible(true);
		GetLayoutRoot().Show(true);
		OnShow();
	}
	
	void OnShow()
	{
		LockControls(true);
		PPEffects.SetBlurMenu(0.5);
		SetFocus(GetLayoutRoot());
		CreateUpdateTimer();
	}
	
	void Hide()
	{
		SetIsVisible(false);
		GetLayoutRoot().Show(false);
		OnHide();
	}
	
	void OnHide()
	{
		LockControls(false);
		PPEffects.SetBlurMenu(0.0);
		DestroyUpdateTimer();
	}
	
	void LockControls(bool active)
	{
		m_Active = active;
		
		Input input = GetGame().GetInput();
		Mission mission = GetGame().GetMission();
		
		// we are in 4_world and this game is bad :)
		Widget hud_root;
		EnScript.GetClassVar(GetGame().GetMission(), "m_HudRootWidget", 0, hud_root);
		if (hud_root) {
			hud_root.Show(!m_Active);
		}
		
		GetGame().GetPlayer().GetInputController().SetDisabled(m_Active);
		if (m_Active == true)
		{
			input.ChangeGameFocus(1, INPUT_DEVICE_MOUSE);
			input.ChangeGameFocus(1, INPUT_DEVICE_KEYBOARD);
			input.ChangeGameFocus(1, INPUT_DEVICE_GAMEPAD);
			mission.PlayerControlDisable(INPUT_EXCLUDE_ALL);
		}
		else
		{
			input.ChangeGameFocus(-1, INPUT_DEVICE_MOUSE);
			input.ChangeGameFocus(-1, INPUT_DEVICE_KEYBOARD);
			input.ChangeGameFocus(-1, INPUT_DEVICE_GAMEPAD);
			mission.PlayerControlEnable(m_Active);
		}
		
		ShowHud(m_Active);
		ShowUICursor(m_Active);
	}
	
	void ShowHud(bool state)
	{
		Mission mission = GetGame().GetMission();
		mission.GetHud().ShowHud(!m_Active);
		mission.GetHud().ShowQuickbarPlayer(!m_Active);
	}
	
	void ShowUICursor(bool state)
	{
		UIManager vanillaUIManager = GetGame().GetUIManager();
		vanillaUIManager.ShowUICursor(m_Active);
	}
	
	bool CanShow()
	{
		return true;
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
	
	void Close()
	{
		Hide();
		GetDayZGame().GetExpansionGame().GetExpansionUIManager().CloseMenu();
	}
}