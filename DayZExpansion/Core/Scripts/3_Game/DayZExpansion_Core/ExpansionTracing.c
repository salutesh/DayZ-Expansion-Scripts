/**
 * ExpansionTracing.c
 *
 * DayZ Expansion Mod
 * www.dayzexpansion.com
 * © 2022 DayZ Expansion Mod Team
 *
 * This work is licensed under the Creative Commons Attribution-NonCommercial-NoDerivatives 4.0 International License. 
 * To view a copy of this license, visit http://creativecommons.org/licenses/by-nc-nd/4.0/.
 *
*/

/*! Call tracing that can be enabled on a per-ID basis.
 * Insert in each method at the top one of the following:
 * 
 * auto trace = EXTrace.Start(EXTrace.<ID>);  //! Includes only 1st line of stack
 * auto trace = EXTrace.StartStack(EXTrace.<ID>);  //! Includes whole stack
 * auto trace = EXTrace.Start0(EXTrace.<ID>);  //! Excludes stack
 * 
 * Calling trace.Dump(<int> depth) will output the current stack with the desired depth (this happens automatically when the trace is destroyed).
 * Leave out the depth parameter to use default depth.
 * You can also change the default depth on-the-fly by calling trace.SetDepth(<int> depth).
 * 
 * static bool EXTrace.<ID> needs to exist in Core\DayZExpansion_Core\3_Game\ExpansionTracing.c
 * and be set to ENABLE (will enable if DayZDiag or DayZ Experimental) or uncomment global define EXPANSIONTRACE in DayZExpansion_Core_Defines.c
 * DON'T use without wrapping in ifdef EXPANSIONTRACE ... #endif in OnUpdate or anywhere else it would get called rapidly!
 */
class EXTrace
{
	//! Please keep these at the top and use below instead of true to enable tracing when using DayZDiag or DayZ Experimental
#ifdef DIAG
	static bool ENABLE = true;
#else
	#ifdef BUILD_EXPERIMENTAL
		static bool ENABLE = true;
	#else
		static bool ENABLE = false;
	#endif
#endif

	//! -----------------------------------------------------------------------

	//! Please keep these alphabetically ordered when inserting new variables

	static bool AI = ENABLE;

	static bool BASEBUILDING;

	static bool BOOK;
	
	static bool CE;

	static bool CHAT = ENABLE;

	static bool CHICKEN;
	
	static bool COT_GROUPS = ENABLE;
	
	static bool DATACOLLECTION = ENABLE;

	static bool GENERAL_ITEMS;

	static bool GLOBAL;

	static bool KILLFEED = ENABLE;

	static bool LIGHTHOUSE;

	static bool LIGHTS;

	static bool MAPPING;
	
	static bool MARKER;

	static bool MARKET;

	static bool MISSIONS;

	static bool NOTIFICATIONS;

	static bool PLAYER = ENABLE;

	static bool PLAYER_MONITOR;
	
	static bool RESPAWN = ENABLE;

	static bool SETTINGS;
	
	static bool SPAWNSELECTION;

	static bool SKIN;

	static bool TERRITORY;

	static bool UI;

	static bool VEHICLES = ENABLE;

	static bool WEAPONS;

	static bool ZONES;

	//! -----------------------------------------------------------------------
	
	static string m_Indent = "                                  ";
	protected string m_Instance;
	protected string m_Params[10];
	protected int m_ParamsCount;
	protected int m_Ticks;
	protected int m_Depth;  //! Default depth = 1 (first line of stack). Set to -1 to include whole stack, set to 0 to exclude stack.
	protected bool m_End;

	void EXTrace(Class instance = null)
	{
		if (instance)
			m_Instance = instance.ToString();
		m_Ticks = TickCount(0);
		m_Depth = 1;
	}

	void ~EXTrace()
	{
		Dump(m_Depth, 2, true);
	}

	void Dump(int depth = 1, int start = 1, bool end = false)
	{
		if (m_End)
			return;
		m_End = end;
		int elapsed = TickCount(m_Ticks);
		string res;
		for (int i = 0; i < m_ParamsCount; i++)
		{
			res += m_Params[i] + " ";
		}
		if (m_Instance)
		{
			res += m_Instance;
			if (depth != 0)
				res += "::";
		}
		if (depth == 0)
		{
			DumpLine(res, elapsed, end);
		}
		else
		{
			string stack;
			DumpStackString(stack);
			TStringArray stacka();
			stack.Split("\n", stacka);
			int n = start;
			while (depth != 0 && n < stacka.Count())
			{
				if (n > start)
					PrintFormat("%1%2", m_Indent, stacka[n++]);
				else
					DumpLine(res + stacka[n++], elapsed, end);
				if (depth > 0)
					depth--;
			}
		}
		m_Ticks = TickCount(0);
	}

	//! @note max script log line length 256 characters
	void DumpLine(string res, int elapsed, bool end = false)
	{
		string prefix;
		if (end)
			prefix = "~";
		PrintFormat("%1 [%2EXTRACE] %3ms %4", ExpansionStatic.GetTimestamp(), prefix, (elapsed / 10000.0).ToString(), res);
	}

	EXTrace Add(string param)
	{
		m_Params[m_ParamsCount++] = param;
		return this;
	}

	static void Add(EXTrace trace, string param)
	{
		if (!trace)
			return;

		trace.Add(param);
	}

	void SetDepth(int depth)
	{
		m_Depth = depth;
	}

	//! Includes 1st line of stack
	static EXTrace Start(bool yes = true, Class instance = null, string param0 = "", string param1 = "", string param2 = "", string param3 = "", string param4 = "", string param5 = "", string param6 = "", string param7 = "", string param8 = "", string param9 = "")
	{
		//! Unconditionally conditionally enable if define not defined kappa
#ifndef EXPANSIONTRACE
		if (!yes)
			return null;
#endif

		auto trace = new EXTrace(instance);
		if (param0)
			trace.Add(param0);
		if (param1)
			trace.Add(param1);
		if (param2)
			trace.Add(param2);
		if (param3)
			trace.Add(param3);
		if (param4)
			trace.Add(param4);
		if (param5)
			trace.Add(param5);
		if (param6)
			trace.Add(param6);
		if (param7)
			trace.Add(param7);
		if (param8)
			trace.Add(param8);
		if (param9)
			trace.Add(param9);
		return trace;
	}

	//! Includes whole stack
	static EXTrace StartStack(bool yes = true, Class instance = null, string param0 = "", string param1 = "", string param2 = "", string param3 = "", string param4 = "", string param5 = "", string param6 = "", string param7 = "", string param8 = "", string param9 = "")
	{
		//! Unconditionally conditionally enable if define not defined kappa
#ifndef EXPANSIONTRACE
		if (!yes)
			return null;
#endif

		auto trace = Start(yes, instance, param0, param1, param2, param3, param4, param5, param6, param7, param8, param9);
		trace.SetDepth(-1);
		return trace;
	}

	//! Excludes stack
	static EXTrace Start0(bool yes = true, Class instance = null, string param0 = "", string param1 = "", string param2 = "", string param3 = "", string param4 = "", string param5 = "", string param6 = "", string param7 = "", string param8 = "", string param9 = "")
	{
		//! Unconditionally conditionally enable if define not defined kappa
#ifndef EXPANSIONTRACE
		if (!yes)
			return null;
#endif

		auto trace = Start(yes, instance, param0, param1, param2, param3, param4, param5, param6, param7, param8, param9);
		trace.SetDepth(0);
		return trace;
	}
}

typedef EXTrace ExpansionTracing;
