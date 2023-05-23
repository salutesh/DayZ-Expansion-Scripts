modded class CF_Trace
{
	void Debug(string message, string param1 = "", string param2 = "", string param3 = "", string param4 = "", string param5 = "", string param6 = "", string param7 = "", string param8 = "", string param9 = "")
	{
		if (!IsLogged())
		{
			return;
		}

		CF_Log.Debug(CF_Trace.Depth() + " " + message, param1, param2, param3, param4, param5, param6, param7, param8, param9);
	}

	void Info(string message, string param1 = "", string param2 = "", string param3 = "", string param4 = "", string param5 = "", string param6 = "", string param7 = "", string param8 = "", string param9 = "")
	{
		if (!IsLogged())
		{
			return;
		}

		CF_Log.Info(CF_Trace.Depth() + " " + message, param1, param2, param3, param4, param5, param6, param7, param8, param9);
	}

	void Warn(string message, string param1 = "", string param2 = "", string param3 = "", string param4 = "", string param5 = "", string param6 = "", string param7 = "", string param8 = "", string param9 = "")
	{
		if (!IsLogged())
		{
			return;
		}

		CF_Log.Warn(CF_Trace.Depth() + " " + message, param1, param2, param3, param4, param5, param6, param7, param8, param9);
	}

	void Error(string message, string param1 = "", string param2 = "", string param3 = "", string param4 = "", string param5 = "", string param6 = "", string param7 = "", string param8 = "", string param9 = "")
	{
		if (!IsLogged())
		{
			return;
		}

		CF_Log.Error(CF_Trace.Depth() + " " + message, param1, param2, param3, param4, param5, param6, param7, param8, param9);
	}

	void Critical(string message, string param1 = "", string param2 = "", string param3 = "", string param4 = "", string param5 = "", string param6 = "", string param7 = "", string param8 = "", string param9 = "")
	{
		if (!IsLogged())
		{
			return;
		}

		CF_Log.Critical(CF_Trace.Depth() + " " + message, param1, param2, param3, param4, param5, param6, param7, param8, param9);
	}
};

#ifndef CF_TRACE_STACK_NAME_ASSUMPTION_FIX
static CF_Trace CF_Trace_0(bool doLog, Class instance)
{
	auto trace = new CF_Trace(CF_Trace.FormatInstance(instance), "", 0, doLog);
	return trace;
}

static CF_Trace CF_Trace_1(bool doLog, Class instance)
{
	auto trace = new CF_Trace(CF_Trace.FormatInstance(instance), "", 1, doLog);
	return trace;
}

static CF_Trace CF_Trace_2(bool doLog, Class instance)
{
	auto trace = new CF_Trace(CF_Trace.FormatInstance(instance), "", 2, doLog);
	return trace;
}

static CF_Trace CF_Trace_3(bool doLog, Class instance)
{
	auto trace = new CF_Trace(CF_Trace.FormatInstance(instance), "", 3, doLog);
	return trace;
}

static CF_Trace CF_Trace_4(bool doLog, Class instance)
{
	auto trace = new CF_Trace(CF_Trace.FormatInstance(instance), "", 4, doLog);
	return trace;
}

static CF_Trace CF_Trace_5(bool doLog, Class instance)
{
	auto trace = new CF_Trace(CF_Trace.FormatInstance(instance), "", 5, doLog);
	return trace;
}

static CF_Trace CF_Trace_6(bool doLog, Class instance)
{
	auto trace = new CF_Trace(CF_Trace.FormatInstance(instance), "", 6, doLog);
	return trace;
}

static CF_Trace CF_Trace_7(bool doLog, Class instance)
{
	auto trace = new CF_Trace(CF_Trace.FormatInstance(instance), "", 7, doLog);
	return trace;
}

static CF_Trace CF_Trace_8(bool doLog, Class instance)
{
	auto trace = new CF_Trace(CF_Trace.FormatInstance(instance), "", 8, doLog);
	return trace;
}

static CF_Trace CF_Trace_9(bool doLog, Class instance)
{
	auto trace = new CF_Trace(CF_Trace.FormatInstance(instance), "", 9, doLog);
	return trace;
}
#endif
