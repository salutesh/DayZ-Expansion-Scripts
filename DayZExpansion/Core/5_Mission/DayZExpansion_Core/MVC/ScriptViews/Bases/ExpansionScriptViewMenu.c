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

class ExpansionScriptViewMenu: ExpansionScriptViewMenuBase
{
	protected ref Timer m_UpdateTimer;
	
	void ExpansionScriptViewMenu()
	{		
		if (GetLayoutRoot().IsVisible()) SetIsVisible(true);
		CreateUpdateTimer();
	}
	
	void ~ExpansionScriptViewMenu()
	{
		DestroyUpdateTimer();
		
		Hide();
	}
	
	override void Show()
	{
		super.Show();
		
		if (!CanShow())
			return;
		
		SetIsVisible(true);
		GetLayoutRoot().Show(true);
		OnShow();
	}
	
	override void OnShow()
	{
		super.OnShow();
		
		LockControls();
		PPEffects.SetBlurMenu(0.5);
		SetFocus(GetLayoutRoot());
		CreateUpdateTimer();
	}
	
	override void Hide()
	{
		super.Hide();
		
		SetIsVisible(false);
		GetLayoutRoot().Show(false);
		OnHide();
	}
	
	override void OnHide()
	{
		super.OnHide();
		
		UnlockControls();
		PPEffects.SetBlurMenu(0.0);
		DestroyUpdateTimer();
	}
		
	override void LockControls()
	{
		super.LockControls();
		
		if (UseMouse())
		{
			GetGame().GetInput().ChangeGameFocus(1, INPUT_DEVICE_MOUSE);
		}

		if (UseKeyboard())
		{
			GetGame().GetInput().ChangeGameFocus(1, INPUT_DEVICE_KEYBOARD);
		}
		
		GetGame().GetPlayer().GetInputController().SetDisabled(true);
		
		Mission mission = GetGame().GetMission();
		mission.PlayerControlDisable(INPUT_EXCLUDE_ALL);
		
		ShowHud(false);
		ShowUICursor(true);
		
		//! This causes a crash to desktop when trying to jump after it has been disabled and re-enabled
		//GetGame().GetPlayer().DisableSimulation(true);
		
		LockInputs(true);
		GetUApi().UpdateControls();
	}
	
	override void UnlockControls()
	{
		super.UnlockControls();
		
		if (UseMouse())
		{
			GetGame().GetInput().ChangeGameFocus(-1, INPUT_DEVICE_MOUSE);
		}

		if (UseKeyboard())
		{
			GetGame().GetInput().ChangeGameFocus(-1, INPUT_DEVICE_KEYBOARD);
		}
		
		GetGame().GetPlayer().GetInputController().SetDisabled(false);
		
		Mission mission = GetGame().GetMission();
		mission.PlayerControlEnable(true);
		
		ShowHud(true);
		ShowUICursor(false);
		
		//! This causes a crash to desktop when trying to jump after it has been disabled and re-enabled
		//GetGame().GetPlayer().DisableSimulation(false);
		
		UnlockInputs();
		GetUApi().UpdateControls();
	}
	
	override void LockInputs(bool state)
	{
		super.LockInputs(state);
		
		//! Vanilla
		GetUApi().GetInputByName("UAUIGesturesOpen").ForceDisable(state);
		GetUApi().GetInputByName("UAGear").ForceDisable(state);
		GetUApi().GetInputByName("UAChat").ForceDisable(state);
		GetUApi().GetInputByName("UADefaultAction").ForceDisable(state);  //! Left mouse button
		
		//! COT
		GetUApi().GetInputByName("UACOTModuleToggleCOT").ForceDisable(state);
		GetUApi().GetInputByName("UACOTToggleButtons").ForceDisable(state);
		GetUApi().GetInputByName("UACOTTogglePlayer").ForceDisable(state);
		GetUApi().GetInputByName("UACOTToggleCamera").ForceDisable(state);
		GetUApi().GetInputByName("UACOTToggleESP").ForceDisable(state);
		GetUApi().GetInputByName("UACOTToggleMap").ForceDisable(state);
		GetUApi().GetInputByName("UACOTToggleTeleport").ForceDisable(state);
		GetUApi().GetInputByName("UATeleportModuleTeleportCursor").ForceDisable(state);
		GetUApi().GetInputByName("UATeleportModuleWorldToModelLookAt").ForceDisable(state);
		GetUApi().GetInputByName("UAObjectModuleSpawnInfected").ForceDisable(state);
		GetUApi().GetInputByName("UACameraToolSpeedIncrease").ForceDisable(state);
		GetUApi().GetInputByName("UACameraToolZoomBackwards").ForceDisable(state);
		GetUApi().GetInputByName("UACameraToolZoomForwards").ForceDisable(state);
		GetUApi().GetInputByName("UACameraToolSpeedDecrease").ForceDisable(state);
		GetUApi().GetInputByName("UAObjectModuleSpawnAnimal").ForceDisable(state);
		GetUApi().GetInputByName("UAObjectModuleSpawnWolf").ForceDisable(state);
		
		//! Expansion
		GetUApi().GetInputByName("UAExpansionAutoRunToggle").ForceDisable(state);
		GetUApi().GetInputByName("UAExpansionEarplugsToggle").ForceDisable(state);
		GetUApi().GetInputByName("UAExpansion3DMarkerToggle").ForceDisable(state);
		GetUApi().GetInputByName("UAExpansionServerMarkersToggle").ForceDisable(state);
		GetUApi().GetInputByName("UAExpansionMapToggle").ForceDisable(state);
		GetUApi().GetInputByName("UAExpansionQuickMarker").ForceDisable(state);
		GetUApi().GetInputByName("UAExpansionChatSwitchChannel").ForceDisable(state);
	}
	
	override void UnlockInputs()
	{
		super.UnlockInputs();
		
		//! Vanilla
		GetUApi().GetInputByName("UAUIGesturesOpen").ForceEnable(true);
		GetUApi().GetInputByName("UAGear").ForceEnable(true);
		GetUApi().GetInputByName("UAChat").ForceEnable(true);
		GetUApi().GetInputByName("UADefaultAction").ForceEnable(true);  //! Left mouse button
		
		//! COT
		GetUApi().GetInputByName("UACOTModuleToggleCOT").ForceEnable(true);
		GetUApi().GetInputByName("UACOTToggleButtons").ForceEnable(true);
		GetUApi().GetInputByName("UACOTTogglePlayer").ForceEnable(true);
		GetUApi().GetInputByName("UACOTToggleCamera").ForceEnable(true);
		GetUApi().GetInputByName("UACOTToggleESP").ForceEnable(true);
		GetUApi().GetInputByName("UACOTToggleMap").ForceEnable(true);
		GetUApi().GetInputByName("UACOTToggleTeleport").ForceEnable(true);
		GetUApi().GetInputByName("UATeleportModuleTeleportCursor").ForceEnable(true);
		GetUApi().GetInputByName("UATeleportModuleWorldToModelLookAt").ForceEnable(true);
		GetUApi().GetInputByName("UAObjectModuleSpawnInfected").ForceEnable(true);
		GetUApi().GetInputByName("UACameraToolSpeedIncrease").ForceEnable(true);
		GetUApi().GetInputByName("UACameraToolZoomBackwards").ForceEnable(true);
		GetUApi().GetInputByName("UACameraToolZoomForwards").ForceEnable(true);
		GetUApi().GetInputByName("UACameraToolSpeedDecrease").ForceEnable(true);
		GetUApi().GetInputByName("UAObjectModuleSpawnAnimal").ForceEnable(true);
		GetUApi().GetInputByName("UAObjectModuleSpawnWolf").ForceEnable(true);
		
		//! Expansion
		GetUApi().GetInputByName("UAExpansionAutoRunToggle").ForceEnable(true);
		GetUApi().GetInputByName("UAExpansionEarplugsToggle").ForceEnable(true);
		GetUApi().GetInputByName("UAExpansion3DMarkerToggle").ForceEnable(true);
		GetUApi().GetInputByName("UAExpansionServerMarkersToggle").ForceEnable(true);
		GetUApi().GetInputByName("UAExpansionMapToggle").ForceEnable(true);
		GetUApi().GetInputByName("UAExpansionQuickMarker").ForceEnable(true);
		GetUApi().GetInputByName("UAExpansionChatSwitchChannel").ForceEnable(true);
		
		LockInputs(false);
	}
	
	override void ShowHud(bool state)
	{
		super.ShowHud(state);

		MissionGameplay mission = MissionGameplay.Cast(GetGame().GetMission());
		mission.GetHud().ShowQuickbarUI(state);
		mission.GetHud().ShowHudUI(state);
	}
	
	override void ShowUICursor(bool state)
	{
		super.ShowUICursor(state);
		
		UIManager vanillaUIManager = GetGame().GetUIManager();
		vanillaUIManager.ShowUICursor(state);
	}
	
	override bool CanShow()
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
	
	override void Close()
	{
		Hide();
		GetDayZGame().GetExpansionGame().GetExpansionUIManager().DestroySVMenu(GetID());
	}
}