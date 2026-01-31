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
 * auto trace = EXTrace.Start(EXTrace.<ID>, <instance|typename|null>);  //! Includes only 1st line of stack
 * auto trace = EXTrace.StartStack(EXTrace.<ID>, <instance|typename|null>);  //! Includes whole stack
 * auto trace = EXTrace.Start0(EXTrace.<ID>, <instance|typename|null>);  //! Excludes stack
 *
 * Calling trace.Dump(<int> depth) will output the current stack with the desired depth (this happens automatically when the trace is destroyed).
 * Leave out the depth parameter to use default depth.
 * You can also change the default depth on-the-fly by calling trace.SetDepth(<int> depth).
 *
 * static bool EXTrace.<ID> needs to exist in Core\DayZExpansion_Core\3_Game\ExpansionTracing.c
 * and be set to return value of IsEnabled() (will enable if DayZDiag, DayZ Experimental or CLI param `extrace` is set to true)
 * or uncomment global define EXPANSIONTRACE in DayZExpansion_Core_Defines.c
 * DON'T use without wrapping in ifdef EXPANSIONTRACE ... #endif in OnUpdate or anywhere else it would get called rapidly!
 */
class EXTrace
{
	//! Please keep these alphabetically ordered when inserting new variables

	static bool ACTIONS = IsEnabled();

	static bool AI = IsEnabled();

	static bool AI_PROFILE = IsEnabled();

	static bool BASEBUILDING = IsEnabled();

	static bool BOOK;

	static bool CE = IsEnabled();

	static bool CHAT = IsEnabled();

	static bool CHICKEN;

	static bool COT_GROUPS = IsEnabled();

	static bool DATACOLLECTION = IsEnabled();

	static bool FSM;

	static bool GENERAL_ITEMS = IsEnabled();

	static bool GLOBAL;

	static bool GROUPS = IsEnabled();
	
	static bool GARAGE = IsEnabled();

	static bool HARDLINE = IsEnabled();

	static bool KILLFEED = IsEnabled();

	static bool LIGHTHOUSE = IsEnabled();

	static bool LIGHTS = IsEnabled();

	static bool LOADOUTS = IsEnabled();

	static bool MAPPING = IsEnabled();

	static bool MARKER;

	static bool MARKET = IsEnabled();

	static bool MISC = IsEnabled();

	static bool MISSIONS = IsEnabled();

	static bool NOTIFICATIONS = IsEnabled();
	
	static bool NAMALSKADVENTURE = IsEnabled();
	
	static bool P2PMARKET = IsEnabled();

	static bool PATH_INTERPOLATION;
	
	static bool PERSONALSTORAGE = IsEnabled();

	static bool PLAYER = IsEnabled();

	static bool PLAYER_CONSTANT; // Will fill up the logs

	static bool PLAYER_MONITOR;

	static bool PROFILING = IsEnabled();

	static bool QUESTS = IsEnabled();

	static bool RESPAWN = IsEnabled();

	static bool RPC = IsEnabled();

	static bool SETTINGS = IsEnabled();

	static bool SOUND = IsEnabled();

	static bool CLIENT_SETTINGS = IsEnabled();

	static bool SPAWNSELECTION;

	static bool SKIN;
	
	static bool TELEPORTER = IsEnabled();

	static bool TERRITORY;

	static bool UI = IsEnabled();

	static bool VEHICLES = IsEnabled();

	static bool WEAPONS = IsEnabled();

	static bool ZONES;

	//! -----------------------------------------------------------------------

	protected static bool s_Initialized;
	protected static bool s_Enabled;

	static bool IsEnabled()
	{
		if (!s_Initialized)
		{
#ifdef DIAG
			s_Enabled = true;
#else
	#ifdef BUILD_EXPERIMENTAL
		#ifndef DEVELOPER
			s_Enabled = true;
		#endif
	#endif
#endif

			string extrace;
			if (GetCLIParam("extrace", extrace))
			{
				if (extrace != "0" && extrace != "false")
					s_Enabled = true;
				else
					s_Enabled = false;
				if (s_Enabled)
					EXPrint("Command line parameter `extrace` found - enabling tracing");
				else
					EXPrint("Command line parameter `extrace` " + extrace + " found - disabling tracing");
			}

			if (s_Enabled)
				CF_Log.Level = CF_LogLevel.TRACE;

			s_Initialized = true;
		}

		return s_Enabled;
	}

	//! -----------------------------------------------------------------------

	static const typename NULLTYPE;

	static const string INDENT = "                        ";
	static string s_Indent;

	static ref CF_Date s_StartDate = CF_Date.Now();
	static ref map<string, ref Param4<int, float, int, float>> s_Profiling = new map<string, ref Param4<int, float, int, float>>;

	protected string m_Instance;
	protected string m_Params[10];
	protected int m_ParamsCount;
	protected int m_Ticks;
	protected int m_Depth = 1;  //! Default depth = 1 (first line of stack). Set to -1 to include whole stack, set to 0 to exclude stack.
	protected int m_Start = 2;  //! Default start = 2 (second line of stack)
	protected bool m_End;
	protected bool m_Silent;
	protected string m_ClassName;
	protected string m_FuncName;

	private void EXTrace(Class instance, typename typeName, bool initialDump = true)
	{
		if (instance)
		{
			m_Instance = instance.ToString();
			m_ClassName = instance.ClassName();
		}
		else if (typeName)
		{
			m_ClassName = typeName.ToString();
		}

		m_Ticks = TickCount(0);

		if (initialDump)
			InitialDump();
	}

	void ~EXTrace()
	{
		if (m_Silent)
		{
			float elapsed = TickCount(m_Ticks) * 0.0001;
			if (m_FuncName)
				Profile(elapsed);
			else
				ProcessStack(1, m_Start, true, false, elapsed);
		}
		else
		{
			if (m_Depth == -1)
				m_Depth = 1;
			s_Indent = s_Indent.Substring(0, s_Indent.Length() - 1);
			Dump(m_Depth, m_Start, true);
		}
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

		float elapsed = TickCount(m_Ticks) * 0.0001;
		string params;

		for (int i = 0; i < m_ParamsCount; i++)
		{
			params += " " + m_Params[i];
		}

		string prefix;

		if (m_Instance)
			prefix = m_Instance;
		else if (m_ClassName)
			prefix = m_ClassName;

		if (prefix)
		{
			if (depth != 0)
				prefix += "::";
		}

		if (depth == 0)
		{
			DumpLine(prefix + params, elapsed, end);
		}
		else
		{
			ProcessStack(depth, start + 1, end, true, elapsed, prefix, params);
		}

		m_Ticks = TickCount(0);
	}

	void ProcessStack(int depth = 1, int start = 1, bool end = false, bool dump = true, float elapsed = 0, string prefix = string.Empty, string params = string.Empty)
	{
		string stack;
		DumpStackString(stack);

		TStringArray stacka = {};
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
				if (end || m_ClassName)
				{
					line = line.Substring(0, line.IndexOf("("));
					m_FuncName = line;
				}

				if (end)
					Profile(elapsed);

				if (dump)
					DumpLine(prefix + line + params, elapsed, end);
				else
					break;
			}

			n++;

			if (depth > 0)
				depth--;
		}
	}

	void Profile(float elapsed)
	{
		string name = m_ClassName;

		if (m_FuncName)
		{
			if (name)
				name += "::";
			name += m_FuncName;
		}

		int hitchCount;
		float hitchTime;
		if (elapsed > 1.0)
		{
			hitchCount = 1;
			hitchTime = elapsed;
		}

		Param4<int, float, int, float> profile;
		if (s_Profiling.Find(name, profile))
		{
			profile.param1 += 1;
			profile.param2 += elapsed;
			profile.param3 += hitchCount;
			profile.param4 += hitchTime;
		}
		else
		{
			s_Profiling[name] = new Param4<int, float, int, float>(1, elapsed, hitchCount, hitchTime);
		}
	}

	//! @note max script log printable text length client 240, server 1026 characters
	void DumpLine(string line, float elapsed, bool end = false)
	{
		if (end)
			PrintFormat("%1 [EXTRACE] %2 %3 ms", ExpansionStatic.GetISOTime(), s_Indent + "-" + line, elapsed.ToString());
		else
			PrintFormat("%1 [EXTRACE] %2", ExpansionStatic.GetISOTime(), s_Indent + "+" + line);
	}

#ifdef EXTRACE
	static void DumpProfilingData()
	{
		FileHandle file = OpenFile(string.Format("$profile:EXTrace_Profiling_%1.csv", s_StartDate.GetISODateTime("_", "-")), FileMode.WRITE);
		if (!file)
			return;

		string calls = ExpansionString.JustifyRight(" CALLS", 10, "-");
		string time = ExpansionString.JustifyRight(" TOTAL TIME MS", 16, "-");
		string avg = ExpansionString.JustifyRight(" AVG TIME MS", 16, "-");
		string hitchCalls = ExpansionString.JustifyRight(" HITCHES", 10, "-");
		string hitchTime = ExpansionString.JustifyRight(" TOTAL HITCH MS", 16, "-");
		string hitchAvg = ExpansionString.JustifyRight(" AVG HITCH MS", 16, "-");

		FPrintln(file, string.Format("%1 | %2 | %3 | %4 | %5 | %6 | %7", ExpansionString.JustifyLeft("NAME ", 90, "-"), calls, time, avg, hitchCalls, hitchTime, hitchAvg));

		TStringArray names = s_Profiling.GetKeyArray();
		names.Sort();

		Param4<int, float, int, float> profile;

		foreach (string name: names)
		{
			profile = s_Profiling[name];
			calls = ExpansionString.JustifyRight(profile.param1.ToString(), 10, " ");
			time = ExpansionString.JustifyRight(ExpansionStatic.FloatFixedDecimals(profile.param2, 4), 16, " ");
			avg = ExpansionString.JustifyRight(ExpansionStatic.FloatFixedDecimals(profile.param2 / profile.param1, 4), 16, " ");
			if (profile.param3)
			{
				hitchCalls = ExpansionString.JustifyRight(profile.param3.ToString(), 10, " ");
				hitchTime = ExpansionString.JustifyRight(ExpansionStatic.FloatFixedDecimals(profile.param4, 4), 16, " ");
				hitchAvg = ExpansionString.JustifyRight(ExpansionStatic.FloatFixedDecimals(profile.param4 / profile.param3, 4), 16, " ");
			}
			else
			{
				hitchCalls = "          ";
				hitchTime = "                ";
				hitchAvg = "                ";
			}
			FPrintln(file, string.Format("%1 | %2 | %3 | %4 | %5 | %6 | %7", ExpansionString.JustifyLeft(name, 90, " "), calls, time, avg, hitchCalls, hitchTime, hitchAvg));
		}

		CloseFile(file);
	}
#endif
	
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

#ifdef EXTRACE
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
#endif

	void SetDepth(int depth)
	{
		m_Depth = depth;
	}

	void SetFuncName(string funcName)
	{
		m_FuncName = funcName;
	}

	void SetStart(int start)
	{
		m_Start = start;
	}

	void SetSilent(bool silent)
	{
		m_Silent = silent;
	}

	void Print(string msg, string p1 = "", string p2 = "", string p3 = "", string p4 = "", string p5 = "", string p6 = "", string p7 = "", string p8 = "", string p9 = "")
	{
		PrintFormat("%1 [EXPRINT] %2%3", ExpansionStatic.GetISOTime(), s_Indent, string.Format(msg, p1, p2, p3, p4, p5, p6, p7, p8, p9));
	}

	static void Print(bool yes, Class instance, string msg, string p1 = "", string p2 = "", string p3 = "", string p4 = "", string p5 = "", string p6 = "", string p7 = "", string p8 = "", string p9 = "")
	{
		//! Unconditionally conditionally enable if define not defined kappa
#ifndef EXPANSIONTRACE
		if (!yes || !msg)
			return;
#endif

		PrintEx(instance, NULLTYPE, msg, p1, p2, p3, p4, p5, p6, p7, p8, p9);
	}

	static void Print(bool yes, typename typeName, string msg, string p1 = "", string p2 = "", string p3 = "", string p4 = "", string p5 = "", string p6 = "", string p7 = "", string p8 = "", string p9 = "")
	{
		//! Unconditionally conditionally enable if define not defined kappa
#ifndef EXPANSIONTRACE
		if (!yes || !msg)
			return;
#endif

		PrintEx(null, typeName, msg, p1, p2, p3, p4, p5, p6, p7, p8, p9);
	}

	static void PrintEx(Class instance, typename typeName, string msg, string p1 = "", string p2 = "", string p3 = "", string p4 = "", string p5 = "", string p6 = "", string p7 = "", string p8 = "", string p9 = "")
	{
		string ts = ExpansionStatic.GetISOTime();

		string prefix;
		if (instance)
			prefix = instance.ToString();
		else if (typeName)
			prefix = typeName.ToString();

		msg = string.Format(msg, p1, p2, p3, p4, p5, p6, p7, p8, p9);
		
		if (prefix)
			PrintFormat("%1 [EXPRINT] %2%3 %4", ts, s_Indent, prefix, msg);
		else
			PrintFormat("%1 [EXPRINT] %2 %3", ts, s_Indent, msg);
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

		if (damageResult)
		{
			msg += damageResult.GetDamage(dmgZone, "Health").ToString() + " ";
			if (instance && instance.IsInherited(Man))
			{
				msg += damageResult.GetDamage(dmgZone, "Blood").ToString() + " ";
				msg += damageResult.GetDamage(dmgZone, "Shock").ToString() + " ";
			}
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

#ifdef EXTRACE
	//! Includes 1st line of stack
	static EXTrace Start(bool yes = true, Class instance = null, string param0 = "", string param1 = "", string param2 = "", string param3 = "", string param4 = "", string param5 = "", string param6 = "", string param7 = "", string param8 = "", string param9 = "", int initialDumpStart = 1)
	{
		//! Unconditionally conditionally enable if define not defined kappa
#ifndef EXPANSIONTRACE
		if (!yes)
			return null;
#endif

		auto trace = new EXTrace(instance, NULLTYPE, false);
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

	//! Includes 1st line of stack
	static EXTrace Start(bool yes, typename typeName, string param0 = "", string param1 = "", string param2 = "", string param3 = "", string param4 = "", string param5 = "", string param6 = "", string param7 = "", string param8 = "", string param9 = "", int initialDumpStart = 1)
	{
		//! Unconditionally conditionally enable if define not defined kappa
#ifndef EXPANSIONTRACE
		if (!yes)
			return null;
#endif

		auto trace = new EXTrace(null, typeName, false);
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
	static EXTrace StartStack(bool yes, typename typeName, string param0 = "", string param1 = "", string param2 = "", string param3 = "", string param4 = "", string param5 = "", string param6 = "", string param7 = "", string param8 = "", string param9 = "")
	{
		//! Unconditionally conditionally enable if define not defined kappa
#ifndef EXPANSIONTRACE
		if (!yes)
			return null;
#endif

		auto trace = Start(yes, typeName, param0, param1, param2, param3, param4, param5, param6, param7, param8, param9, 0);
		trace.SetDepth(-1);
		trace.InitialDump(1);
		return trace;
	}

	//! Excludes stack
	static EXTrace Start0(bool yes, typename typeName, string param0 = "", string param1 = "", string param2 = "", string param3 = "", string param4 = "", string param5 = "", string param6 = "", string param7 = "", string param8 = "", string param9 = "")
	{
		//! Unconditionally conditionally enable if define not defined kappa
#ifndef EXPANSIONTRACE
		if (!yes)
			return null;
#endif

		auto trace = Start(yes, typeName, param0, param1, param2, param3, param4, param5, param6, param7, param8, param9, 0);
		trace.SetDepth(0);
		trace.InitialDump(1);
		return trace;
	}

	static EXTrace Profile(bool yes, Class instance)
	{
		//! Unconditionally conditionally enable if define not defined kappa
#ifndef EXPANSIONTRACE
		if (!yes)
			return null;
#endif

		auto trace = new EXTrace(instance, NULLTYPE, false);
		trace.SetSilent(true);
		return trace;
	}

	static EXTrace Profile(bool yes, Class instance, string funcName)
	{
		//! Unconditionally conditionally enable if define not defined kappa
#ifndef EXPANSIONTRACE
		if (!yes)
			return null;
#endif

		auto trace = new EXTrace(instance, NULLTYPE, false);
		trace.SetSilent(true);
		trace.SetFuncName(funcName);
		return trace;
	}

	static EXTrace Profile(bool yes, typename typeName)
	{
		//! Unconditionally conditionally enable if define not defined kappa
#ifndef EXPANSIONTRACE
		if (!yes)
			return null;
#endif

		auto trace = new EXTrace(null, typeName, false);
		trace.SetSilent(true);
		return trace;
	}

	static EXTrace Profile(bool yes, typename typeName, string funcName)
	{
		//! Unconditionally conditionally enable if define not defined kappa
#ifndef EXPANSIONTRACE
		if (!yes)
			return null;
#endif

		auto trace = new EXTrace(null, typeName, false);
		trace.SetSilent(true);
		trace.SetFuncName(funcName);
		return trace;
	}
#endif
}

typedef EXTrace ExpansionTracing;
