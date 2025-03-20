modded class CF_Log
{
	static ref ExpansionStaticCore s_Expansion_StaticCore;
	static ref ExpansionStaticCore s_Expansion_Static = new ExpansionStaticCore;

#ifndef CF_LOG_TIMESTAMP
	override static void Trace(string message, string param1 = "", string param2 = "", string param3 = "", string param4 = "", string param5 = "", string param6 = "", string param7 = "", string param8 = "", string param9 = "")
	{
		if (Level > CF_LogLevel.TRACE) return;

		PrintFormat("%1 [TRACE] %2", s_Expansion_Static.CF_GetISOTime(), string.Format(message, param1, param2, param3, param4, param5, param6, param7, param8, param9));
	}

	override static void Debug(string message, string param1 = "", string param2 = "", string param3 = "", string param4 = "", string param5 = "", string param6 = "", string param7 = "", string param8 = "", string param9 = "")
	{
		if (Level > CF_LogLevel.DEBUG) return;

		PrintFormat("%1 [DEBUG] %2", s_Expansion_Static.CF_GetISOTime(), string.Format(message, param1, param2, param3, param4, param5, param6, param7, param8, param9));
	}

	override static void Info(string message, string param1 = "", string param2 = "", string param3 = "", string param4 = "", string param5 = "", string param6 = "", string param7 = "", string param8 = "", string param9 = "")
	{
		if (Level > CF_LogLevel.INFO) return;

		PrintFormat("%1 [INFO] %2", s_Expansion_Static.CF_GetISOTime(), string.Format(message, param1, param2, param3, param4, param5, param6, param7, param8, param9));
	}

	override static void Warn(string message, string param1 = "", string param2 = "", string param3 = "", string param4 = "", string param5 = "", string param6 = "", string param7 = "", string param8 = "", string param9 = "")
	{
		if (Level > CF_LogLevel.WARNING) return;

		PrintFormat("%1 [WARNING] %2", s_Expansion_Static.CF_GetISOTime(), string.Format(message, param1, param2, param3, param4, param5, param6, param7, param8, param9));
	}

	override static void Error(string message, string param1 = "", string param2 = "", string param3 = "", string param4 = "", string param5 = "", string param6 = "", string param7 = "", string param8 = "", string param9 = "")
	{
		if (Level > CF_LogLevel.ERROR) return;

		PrintFormat("%1 [ERROR] %2", s_Expansion_Static.CF_GetISOTime(), string.Format(message, param1, param2, param3, param4, param5, param6, param7, param8, param9));

		PrintStack();
	}

	override static void Critical(string message, string param1 = "", string param2 = "", string param3 = "", string param4 = "", string param5 = "", string param6 = "", string param7 = "", string param8 = "", string param9 = "")
	{
		if (Level > CF_LogLevel.CRITICAL) return;

		PrintFormat("%1 [CRITICAL] %2", s_Expansion_Static.CF_GetISOTime(), string.Format(message, param1, param2, param3, param4, param5, param6, param7, param8, param9));

		PrintStack();
	}
#endif

	static void PrintStack()
	{
		string dump = "";
		DumpStackString(dump);
		array<string> outputs = new array<string>();
		dump.Split("\n", outputs);

		for (int i = 1; i < outputs.Count(); i++)
		{
			Print("\t" + outputs[i]);
		}
	}
};
