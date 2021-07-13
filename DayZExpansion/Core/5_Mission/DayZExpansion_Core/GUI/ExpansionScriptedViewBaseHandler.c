/**
 * ExpansionScriptedViewBaseHandler.c
 *
 * DayZ Expansion Mod
 * www.dayzexpansion.com
 * © 2020 DayZ Expansion Mod Team
 *
 * This work is licensed under the Creative Commons Attribution-NonCommercial-NoDerivatives 4.0 International License.
 * To view a copy of this license, visit http://creativecommons.org/licenses/by-nc-nd/4.0/.
 *
*/

#ifdef DABS_FRAMEWORK
class ExpansionScriptedViewBaseHandler: ScriptedViewBaseHandler
{
	reference bool Debug_Logging;
	
	void ExpansionScriptedViewBaseHandler(ScriptedViewBase viewBase)
	{
		m_ScriptedViewBase = viewBase;
		m_LayoutRoot = CreateWidget(null);
	}
	
	protected Widget m_LayoutRoot;
	protected string GetLayoutFile();
	
	Widget GetLayoutRoot()
	{
		return m_LayoutRoot;
	}
	
	protected void SetParent(Widget parent)
	{
		m_LayoutRoot = CreateWidget(parent);
	}

	Widget CreateWidget(Widget parent)
	{
		Widget result;
		if (!GetLayoutFile())
		{
			Error("Layout file not found! Are you overriding GetLayoutFile?");
			return result;
		}

		WorkspaceWidget workspace = GetWorkbenchGame().GetWorkspace();
		if (!workspace)
		{
			Error("Workspace was null, try reloading Workbench");
			return result;
		}

		Log("Loading %1", GetLayoutFile());
		result = workspace.CreateWidgets(GetLayoutFile(), parent);
		if (!result)
		{
			Error("Invalid layout file %1", GetLayoutFile());
			return result;
		}

		return result;
	}
	
	void Trace(string message, string param1 = "", string param2 = "", string param3 = "", string param4 = "", string param5 = "", string param6 = "", string param7 = "", string param8 = "", string param9 = "")
	{
		//#ifdef COMPONENT_SYSTEM
		if (Debug_Logging) // || LBMLogLevel >= LogLevel.TRACE
			PrintFormat("[Trace] %1 - %2 ", Type(), string.Format(message, param1, param2, param3, param4, param5, param6, param7, param8, param9));
		//#endif
	}

	void Log(string message, string param1 = "", string param2 = "", string param3 = "", string param4 = "", string param5 = "", string param6 = "", string param7 = "", string param8 = "", string param9 = "")
	{
		//#ifdef COMPONENT_SYSTEM
		if (Debug_Logging) // || LBMLogLevel >= LogLevel.INFO
			PrintFormat("[Log] %1 - %2", Type(), string.Format(message, param1, param2, param3, param4, param5, param6, param7, param8, param9));
		//#endif
	}

	void Error(string message, string param1 = "", string param2 = "", string param3 = "", string param4 = "", string param5 = "", string param6 = "", string param7 = "", string param8 = "", string param9 = "")
	{
		string msg = string.Format(message, param1, param2, param3, param4, param5, param6, param7, param8, param9);
		string header = string.Format("[Error] %1", Type());
		Error2(header, msg);

#ifdef COMPONENT_SYSTEM
		Workbench.Dialog(header, msg);
#endif
	}
}
#endif