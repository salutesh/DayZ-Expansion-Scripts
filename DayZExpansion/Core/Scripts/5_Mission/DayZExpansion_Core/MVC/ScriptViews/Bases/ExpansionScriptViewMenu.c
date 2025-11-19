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

#ifdef EXPANSIONUI
class ExpansionScriptViewMenu: ExpansionScriptViewMenuBase
{
	protected ref Timer m_UpdateTimer;
	protected bool m_MovementLocked;
	
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
	}

	override void Show()
	{
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
		SetIsVisible(false);
		GetLayoutRoot().Show(false);
		OnHide();
	}
	
	override void OnHide()
	{
		if (!g_Game)
			return;
		
		super.OnHide();
		
		PPEffects.SetBlurMenu(0.0);
		DestroyUpdateTimer();

		UnlockControls();
	}
		
	override void LockControls(bool lockMovement = true)
	{
		ShowHud(false);
		ShowUICursor(true);
		
		LockInputs(true, lockMovement);
	}
	
	override void UnlockControls()
	{
		ShowHud(true);
		ShowUICursor(false);
		
		UnlockInputs();
	}
	
	override void LockInputs(bool state, bool lockMovement = true)
	{
		m_MovementLocked = lockMovement;
		
		TIntArray inputIDs = new TIntArray;
		GetUApi().GetActiveInputs(inputIDs);

		TStringArray skip = new TStringArray;
		skip.Insert("UAUIBack");

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
			skip.Insert("UALeanLeft");
			skip.Insert("UALeanRight");
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

		GetUApi().UpdateControls();
	}
	
	override void UnlockInputs()
	{		
		LockInputs(false, m_MovementLocked);
	}
	
	override void ShowHud(bool state)
	{
		super.ShowHud(state);

		MissionGameplay mission = MissionGameplay.Cast(g_Game.GetMission());
		
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
		
		UIManager vanillaUIManager = g_Game.GetUIManager();
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
			m_UpdateTimer.Run(GetUpdateTickRate(), this, "Expansion_Update", NULL, true);
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

	override void CloseMenu()
	{
		if (!CanClose())
			return;

		if (GetDayZGame().GetExpansionGame().GetExpansionUIManager())
		{
			GetDayZGame().GetExpansionGame().GetExpansionUIManager().CloseMenu();
		}
	}
	
	void Expansion_Update();
}
#endif
