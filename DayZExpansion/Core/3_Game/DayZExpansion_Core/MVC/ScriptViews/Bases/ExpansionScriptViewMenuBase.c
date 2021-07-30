/**
 * ExpansionScriptViewMenuBase.c
 *
 * DayZ Expansion Mod
 * www.dayzexpansion.com
 * © 2020 DayZ Expansion Mod Team
 *
 * This work is licensed under the Creative Commons Attribution-NonCommercial-NoDerivatives 4.0 International License.
 * To view a copy of this license, visit http://creativecommons.org/licenses/by-nc-nd/4.0/.
 *
*/

class ExpansionScriptViewMenuBase: ScriptView
{
	private int m_SVMenuID = -1;
	private bool m_IsVisible = false;
	
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
	
	void Show();
	void OnShow();
	
	void Hide();
	
	void OnHide();
	void LockControls(bool lockMovement = true);
	void UnlockControls();
	void LockInputs(bool state, bool lockMovement = true);
	void UnlockInputs();
	void ShowHud(bool state);
	void ShowUICursor(bool state);
	
	bool CanShow()
	{
		return true;
	}
};