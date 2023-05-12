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

/*
 * Call tracing that can be enabled on a per-ID basis.
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

	static bool BASEBUILDING = ENABLE;

	static bool BOOK;

	static bool CE;

	static bool CHAT = ENABLE;

	static bool CHICKEN;

	static bool COT_GROUPS = ENABLE;

	static bool DATACOLLECTION = ENABLE;

	static bool GENERAL_ITEMS = ENABLE;

	static bool GLOBAL;

	static bool GROUPS = ENABLE;
	
	static bool GARAGE = ENABLE;

	static bool HARDLINE = ENABLE;

	static bool KILLFEED = ENABLE;

	static bool LIGHTHOUSE = ENABLE;

	static bool LIGHTS;

	static bool MAPPING = ENABLE;

	static bool MARKER;

	static bool MARKET = ENABLE;

	static bool MISSIONS = ENABLE;

	static bool NOTIFICATIONS;
	
	static bool NAMALSKADVENTURE = ENABLE;
	
	static bool P2PMARKET = ENABLE;

	static bool PATH_INTERPOLATION;
	
	static bool PERSONALSTORAGE = ENABLE;

	static bool PLAYER = ENABLE;

	static bool PLAYER_CONSTANT = PLAYER && true; // Will fill up the logs

	static bool PLAYER_MONITOR;

	static bool QUESTS = ENABLE;

	static bool RESPAWN = ENABLE;

	static bool SETTINGS;

	static bool SPAWNSELECTION;

	static bool SKIN;
	
	static bool TELEPORTER = ENABLE;

	static bool TERRITORY;

	static bool UI = ENABLE;

	static bool VEHICLES = ENABLE;

	static bool WEAPONS = ENABLE;

	static bool ZONES;

	//! -----------------------------------------------------------------------

	static const string INDENT = "                        ";
	static string s_Indent;
	protected string m_Instance;
	protected string m_Params[10];
	protected int m_ParamsCount;
	protected int m_Ticks;
	protected int m_Depth;  //! Default depth = 1 (first line of stack). Set to -1 to include whole stack, set to 0 to exclude stack.
	protected int m_Start;  //! Default start = 2 (second line of stack)
	protected bool m_End;

	private void EXTrace(Class instance = null, bool initialDump = true)
	{
		if (instance)
			m_Instance = instance.ToString();
		m_Depth = 1;
		m_Start = 2;
		m_Ticks = TickCount(0);
		if (initialDump)
			InitialDump();
	}

	void ~EXTrace()
	{
		s_Indent = s_Indent.Substring(0, s_Indent.Length() - 1);
		if (m_Depth == -1)
			m_Depth = 1;
		Dump(m_Depth, m_Start, true);
	}

	void InitialDump(int initialDumpStart = 1)
	{
		Dump(m_Depth, m_Start + initialDumpStart);
		s_Indent += " ";
	}

	void Dump(int depth = 1, int start = 1, bool end = false)
	{
		if (m_End)
			return;
		m_End = end;
		int elapsed = TickCount(m_Ticks);
		string params;
		for (int i = 0; i < m_ParamsCount; i++)
		{
			params += " " + m_Params[i];
		}
		string prefix;
		if (m_Instance)
		{
			prefix = m_Instance;
			if (depth != 0)
				prefix += "::";
		}
		if (depth == 0)
		{
			DumpLine(prefix + params, elapsed, end);
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
				string line = stacka[n];
				if (n > start)
				{
					PrintFormat("%1%2", s_Indent + INDENT, line);
				}
				else
				{
					if (end || m_Instance)
						line = line.Substring(0, line.IndexOf("("));
					DumpLine(prefix + line + params, elapsed, end);
				}
				n++;
				if (depth > 0)
					depth--;
			}
		}
		m_Ticks = TickCount(0);
	}

	//! @note max script log line length 256 characters
	void DumpLine(string line, int elapsed, bool end = false)
	{
		if (end)
			PrintFormat("%1 [EXTRACE] %2 %3 ms", ExpansionStatic.GetISOTime(), s_Indent + "-" + line, (elapsed / 10000.0).ToString());
		else
			PrintFormat("%1 [EXTRACE] %2", ExpansionStatic.GetISOTime(), s_Indent + "+" + line);
	}

	EXTrace Add(bool param)
	{
		return Add(param.ToString());
	}

	EXTrace Add(int param)
	{
		return Add(param.ToString());
	}

	EXTrace Add(float param)
	{
		return Add(param.ToString());
	}

	EXTrace Add(vector param, bool beautify = true)
	{
		return Add(param.ToString(beautify));
	}

	EXTrace Add(typename param)
	{
		return Add(param.ToString());
	}

	EXTrace Add(Class param)
	{
		return Add(param.ToString());
	}

	EXTrace Add(string param)
	{
		m_Params[m_ParamsCount++] = param;
		return this;
	}

	static void Add(EXTrace trace, bool param)
	{
		Add(trace, param.ToString());
	}

	static void Add(EXTrace trace, int param)
	{
		Add(trace, param.ToString());
	}

	static void Add(EXTrace trace, float param)
	{
		Add(trace, param.ToString());
	}

	static void Add(EXTrace trace, vector param, bool beautify = true)
	{
		if (!trace)
			return;

		trace.Add(param, beautify);
	}

	static void Add(EXTrace trace, typename param)
	{
		Add(trace, param.ToString());
	}

	static void Add(EXTrace trace, Class param)
	{
		Add(trace, param.ToString());
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

	void SetStart(int start)
	{
		m_Start = start;
	}

	void Print(string msg)
	{
		PrintFormat("%1 [EXPRINT] %2", ExpansionStatic.GetISOTime(), s_Indent + msg);
	}

	static void Print(bool yes, Class instance, string msg)
	{
		//! Unconditionally conditionally enable if define not defined kappa
#ifndef EXPANSIONTRACE
		if (!yes || !msg)
			return;
#endif

		string ts = ExpansionStatic.GetISOTime();

		if (instance)
			PrintFormat("%1 [EXPRINT] %2 %3", ts, s_Indent + instance.ToString(), msg);
		else
			PrintFormat("%1 [EXPRINT] %2", ts, s_Indent + msg);
	}

	static void PrintHit(bool yes = true, Class instance = null, string msg = "", TotalDamageResult damageResult = null, int damageType = 0, EntityAI source = null, int component = 0, string dmgZone = "", string ammo = "", vector modelPos = "0 0 0", float speedCoef = 1.0)
	{
		//! Unconditionally conditionally enable if define not defined kappa
#ifndef EXPANSIONTRACE
		if (!yes)
			return;
#endif

		if (msg)
			msg += " ";

		msg += damageResult.GetDamage(dmgZone, "Health").ToString() + " ";
		if (instance && instance.IsInherited(Man))
		{
			msg += damageResult.GetDamage(dmgZone, "Blood").ToString() + " ";
			msg += damageResult.GetDamage(dmgZone, "Shock").ToString() + " ";
		}

		msg += typename.EnumToString(DamageType, damageType) + " ";

		msg += source.ToString() + " ";
		msg += component.ToString() + " ";
		msg += dmgZone + " ";
		msg += ammo + " ";
		msg += modelPos.ToString() + " ";
		msg += speedCoef.ToString();

		Print(true, instance, msg);
	}

	//! Includes 1st line of stack
	static EXTrace Start(bool yes = true, Class instance = null, string param0 = "", string param1 = "", string param2 = "", string param3 = "", string param4 = "", string param5 = "", string param6 = "", string param7 = "", string param8 = "", string param9 = "", int initialDumpStart = 1)
	{
		//! Unconditionally conditionally enable if define not defined kappa
#ifndef EXPANSIONTRACE
		if (!yes)
			return null;
#endif

		auto trace = new EXTrace(instance, false);
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
		if (initialDumpStart)
			trace.InitialDump(initialDumpStart);
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

		auto trace = Start(yes, instance, param0, param1, param2, param3, param4, param5, param6, param7, param8, param9, 0);
		trace.SetDepth(-1);
		trace.InitialDump(1);
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

		auto trace = Start(yes, instance, param0, param1, param2, param3, param4, param5, param6, param7, param8, param9, 0);
		trace.SetDepth(0);
		trace.InitialDump(1);
		return trace;
	}
}

typedef EXTrace ExpansionTracing;
