/**
 * ExpansionScriptViewMenu.c
 *
 * DayZ Expansion Mod
 * www.dayzexpansion.com
 * © 2022 DayZ Expansion Mod Team
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
		if (GetLayoutRoot().IsVisible()) 
			SetIsVisible(true);
		
		CreateUpdateTimer();
	}
	
	void ~ExpansionScriptViewMenu()
	{
		EXPrint("~" + ToString());
		DestroyUpdateTimer();
		
		Hide();
	}
	
	override void Show()
	{
		if (!CanShow())
			return;
		
		LockControls();
		SetIsVisible(true);
		GetLayoutRoot().Show(true);
		OnShow();
	}
	
	override void OnShow()
	{
		super.OnShow();
		
		PPEffects.SetBlurMenu(0.5);
		SetFocus(GetLayoutRoot());
		CreateUpdateTimer();
	}
	
	override void Hide()
	{
		UnlockControls();
		SetIsVisible(false);
		GetLayoutRoot().Show(false);
		OnHide();
	}
	
	override void OnHide()
	{
		super.OnHide();
		
		PPEffects.SetBlurMenu(0.0);
		DestroyUpdateTimer();
	}
		
	override void LockControls(bool lockMovement = true)
	{
		super.LockControls();
		
		GetGame().GetMission().PlayerControlDisable(INPUT_EXCLUDE_ALL);
		
		ShowHud(false);
		ShowUICursor(true);
		
		LockInputs(true, lockMovement);
		GetUApi().UpdateControls();
	}
	
	override void UnlockControls()
	{
		super.UnlockControls();
		
		GetGame().GetMission().PlayerControlEnable(true);
				
		ShowHud(true);
		ShowUICursor(false);
		
		UnlockInputs();
		GetUApi().UpdateControls();
	}
	
	override void LockInputs(bool state, bool lockMovement = true)
	{
		super.LockInputs(state);
		
		TIntArray inputIDs = new TIntArray;
		GetUApi().GetActiveInputs(inputIDs);

		TStringArray skip = new TStringArray;
		skip.Insert("UAUIBack");
		skip.Insert("UALeanLeft");
		skip.Insert("UALeanRight");

		if (!lockMovement)
		{
			//! Allow player movement
			skip.Insert("UAMoveForward");
			skip.Insert("UAMoveBack");
			skip.Insert("UAMoveLeft");
			skip.Insert("UAMoveRight");
			skip.Insert("UATurbo");
			skip.Insert("UAWalkRunTemp");
			skip.Insert("UAWalkRunToggle");
		}

		TIntArray skipIDs = new TIntArray;
		foreach (string inputName : skip)
		{
			skipIDs.Insert(GetUApi().GetInputByName(inputName).ID());
		}

		foreach (int inputID : inputIDs)
		{
			if (skipIDs.Find(inputID) == -1)
			{
				GetUApi().GetInputByID(inputID).ForceDisable(state);
			}
		}
	}
	
	override void UnlockInputs()
	{
		super.UnlockInputs();
		
		LockInputs(false);
	}
	
	override void ShowHud(bool state)
	{
		super.ShowHud(state);

		MissionGameplay mission = MissionGameplay.Cast(GetGame().GetMission());
		
		IngameHud hud = IngameHud.Cast(mission.GetHud());
		if (!hud || !hud.GetHudPanelWidget())
			return;
		
		hud.ShowQuickbarUI(state);
		hud.ShowHudUI(state);
		
		if (state)
			hud.ShowCursor();
		else
			hud.HideCursor();
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
	
	void CreateUpdateTimer()
	{
		if (!m_UpdateTimer && GetUpdateTickRate() != -1)
		{
			m_UpdateTimer = new Timer(CALL_CATEGORY_GUI);
			m_UpdateTimer.Run(GetUpdateTickRate(), this, "Update", NULL, true);
		}
	}
	
	void DestroyUpdateTimer()
	{
		if (m_UpdateTimer)
		{
			m_UpdateTimer.Stop();
			m_UpdateTimer = null;
		}
	}
	
	void Update();
	
	bool CanClose()
	{
		return true;
	}
}