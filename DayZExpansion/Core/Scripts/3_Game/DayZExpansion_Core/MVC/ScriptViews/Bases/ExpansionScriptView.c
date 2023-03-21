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
	protected bool m_IsVisible;

	void ExpansionScriptViewBase()
	{
		Debug_Logging = false;
		m_IsVisible = !IsInherited(ExpansionScriptViewMenuBase);
	}

	void Destroy()
	{
		if (!g_Game)
			return;

		delete this;
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
	
	static array<ScriptView> GetAll()
	{
		return All;
	}
	
	static ScriptView GetSciptViewByClassName(string name)
	{
		Print("ExpansionScriptViewBase::GetSciptViewByClassName - Search for view with name: " + name);
		foreach (ScriptView view: All)
		{
			string viewName = view.ClassName();
			Print("ExpansionScriptViewBase::GetSciptViewByClassName - View name: " + viewName);
			if (viewName == name)
			{
				Print("ExpansionScriptViewBase::GetSciptViewByClassName - Found and return view: " + viewName);
				return view;
			}
		}
		
		Print("ExpansionScriptViewBase::GetSciptViewByClassName - Could not find view name. Return NULL.");
		
		return null;
	}
}