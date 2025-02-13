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

#ifdef EXPANSIONUI
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
	protected float m_Alpha;

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

	override bool IsVisible()
	{
		return m_IsVisible;
	}

	void SetAlpha(float alpha)
	{
		m_Alpha = alpha;
	}

	float GetAlpha()
	{
		return m_Alpha;
	}

	override void Show(bool show);

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

	static void DestroyAll()
	{
		for (int i = All.Count() - 1; i >= 0; i--)
		{
			ExpansionScriptViewBase scriptView;
			if (Class.CastTo(scriptView, All[i]))
				scriptView.Destroy();
		}
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

	void UpdatePlayerPreviewObject(Object previewObject)
	{
	}

	void SpawnPlayerPreviewAttachments(EntityAI item)
	{
	}

	void SetCurrentPreviewObject(Object obj)
	{
	}
}
#endif
