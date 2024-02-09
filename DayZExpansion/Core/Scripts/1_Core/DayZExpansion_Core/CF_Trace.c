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
