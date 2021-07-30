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
		if (GetLayoutRoot().IsVisible()) 
			SetIsVisible(true);
		
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
		super.Hide();
		
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

		//! XXX: GetGame().GetInput().ChangeGameFocus(1) works, but disables ESC key...
		//GetGame().GetInput().ChangeGameFocus(1);
		
		LockInputs(true, lockMovement);
		GetUApi().UpdateControls();
	}
	
	override void UnlockControls()
	{
		super.UnlockControls();
		
		GetGame().GetMission().PlayerControlEnable(true);
				
		ShowHud(true);
		ShowUICursor(false);
		
		//GetGame().GetInput().ResetGameFocus();
		
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
		mission.GetHud().ShowQuickbarUI(state);
		mission.GetHud().ShowHudUI(state);
		if (state)
			mission.GetHud().ShowCursor();
		else
			mission.GetHud().HideCursor();
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
}