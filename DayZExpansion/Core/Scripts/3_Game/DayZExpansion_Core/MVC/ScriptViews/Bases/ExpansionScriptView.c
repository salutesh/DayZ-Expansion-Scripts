/**
 * ExpansionScriptViewBase.c
 *
 * DayZ Expansion Mod
 * www.dayzexpansion.com
 * © 2022 DayZ Expansion Mod Team
 *
 * This work is licensed under the Creative Commons Attribution-NonCommercial-NoDerivatives 4.0 International License.
 * To view a copy of this license, visit http://creativecommons.org/licenses/by-nc-nd/4.0/.
 *
*/

class ExpansionViewController: ViewController 
{
	void ExpansionViewController()
	{
		Debug_Logging = false;
	}
};

class ExpansionScriptViewBase: ScriptView
{
	private bool m_IsVisible = true;
	
	void ExpansionScriptViewBase()
	{
		Debug_Logging = false;
	}
	
	void SetIsVisible(bool state)
	{
		m_IsVisible = state;
	}
	
	bool IsVisible()
	{
		return m_IsVisible;
	}
	
	void Show();
	
	void OnShow();
	
	void Hide();
	
	void OnHide();
	
	bool CanShow();
	
	void Refresh();
}