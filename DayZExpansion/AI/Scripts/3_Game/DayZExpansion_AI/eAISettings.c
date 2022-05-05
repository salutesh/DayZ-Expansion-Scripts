class eAISettings : JsonApiStruct
{
	static ref ScriptInvoker ON_UPDATE = new ScriptInvoker();

	private static const string PATH = "$profile:eAI/eAISettings.json";

	private static ref eAISettings m_Instance = new eAISettings();

	private string m_LoadingArray;

	private CF_LogLevel m_LogLevel = CF_LogLevel.TRACE;
	private bool m_LogLevelSavedAsString = true;

	private float m_Accuracy_Min = 0.5;
	private float m_Accuracy_Max = 0.5;

	private ref array<string> m_LoadoutDirectories = {"$profile:eAI/"};
	private bool m_LoadoutDirectoriesSavedAsArray = false;

	private ref array<string> m_Admins = {};

	private int m_MaxDynamicPatrols = -1;

	private bool m_Vaulting = true;

	void SetLogLevel(CF_LogLevel logLevel)
	{
#ifdef EAI_TRACE
		auto trace = CF_Trace_1(this, "SetLogLevel").Add(logLevel);
#endif

		m_LogLevel = logLevel;
	}

	static CF_LogLevel GetLogLevel()
	{
#ifdef EAI_TRACE
		auto trace = CF_Trace_0("eAISettings", "GetLogLevel");
#endif

		return m_Instance.m_LogLevel;
	}

	void SetMaximumDynamicPatrols(int num)
	{
#ifdef EAI_TRACE
		auto trace = CF_Trace_1(this, "SetMaximumDynamicPatrols").Add(num);
#endif

		m_MaxDynamicPatrols = num;
	}

	static int GetMaximumDynamicPatrols()
	{
#ifdef EAI_TRACE
		auto trace = CF_Trace_0("eAISettings", "GetMaximumDynamicPatrols");
#endif

		return m_Instance.m_MaxDynamicPatrols;
	}

	static bool GetVaulting()
	{
		return m_Instance.m_Vaulting;
	}

	void SetAccuracyMin(float accuracy)
	{
#ifdef EAI_TRACE
		auto trace = CF_Trace_1(this, "SetAccuracyMin").Add(accuracy);
#endif

		m_Accuracy_Min = accuracy;
	}

	void SetAccuracyMax(float accuracy)
	{
#ifdef EAI_TRACE
		auto trace = CF_Trace_1(this, "SetAccuracyMax").Add(accuracy);
#endif

		m_Accuracy_Max = accuracy;
	}

	static float GetAccuracyMin()
	{
#ifdef EAI_TRACE
		auto trace = CF_Trace_0("eAISettings", "GetAccuracyMin");
#endif

		return m_Instance.m_Accuracy_Min;
	}

	static float GetAccuracyMax()
	{
#ifdef EAI_TRACE
		auto trace = CF_Trace_0("eAISettings", "GetAccuracyMax");
#endif

		return m_Instance.m_Accuracy_Max;
	}

	void AddLoadoutDirectory(string path)
	{
#ifdef EAI_TRACE
		auto trace = CF_Trace_1(this, "AddLoadoutDirectory").Add(path);
#endif

		m_LoadoutDirectories.Insert(path);
	}

	void ClearLoadoutDirectories()
	{
#ifdef EAI_TRACE
		auto trace = CF_Trace_0(this, "ClearLoadoutDirectories");
#endif

		m_LoadoutDirectories.Clear();
	}

	static array<string> GetLoadoutDirectories()
	{
#ifdef EAI_TRACE
		auto trace = CF_Trace_0("eAISettings", "GetLoadoutDirectories");
#endif

		return m_Instance.m_LoadoutDirectories;
	}

	void AddAdmin(string admin)
	{
#ifdef EAI_TRACE
		auto trace = CF_Trace_1(this, "AddAdmin").Add(admin);
#endif

		m_Admins.Insert(admin);

		array<PlayerIdentity> identities();
		GetGame().GetPlayerIndentities(identities);
		foreach (auto identity : identities)
		{
			if (identity.GetPlainId() == admin)
			{
				GetRPCManager().SendRPC("eAI", "RPC_SetAdmin", new Param1<bool>(true), true, identity);
			}
		}
	}

	void ClearAdmins()
	{
		m_Admins.Clear();

		GetRPCManager().SendRPC("eAI", "RPC_SetAdmin", new Param1<bool>(false), true, null);
	}

	static array<string> GetAdmins()
	{
		return m_Instance.m_Admins;
	}

	override void OnObject(string name)
	{
#ifdef EAI_TRACE
		auto trace = CF_Trace_1(this, "OnObject").Add(name);
#endif
	}

	override void OnInteger(string name, int value)
	{
#ifdef EAI_TRACE
		auto trace = CF_Trace_2(this, "OnInteger").Add(name).Add(value);
#endif

		if (name == "eAILogLevel")
		{
			m_LogLevelSavedAsString = false;
			SetLogLevel(value);
			return;
		}

		if (name == "MaxDynamicPatrols")
		{
			SetMaximumDynamicPatrols(value);
			return;
		}

		if (name == "Vaulting")
		{
			m_Vaulting = value;
			return;
		}
	}

	override void OnString(string name, string value)
	{
#ifdef EAI_TRACE
		auto trace = CF_Trace_2(this, "OnString").Add(name).Add(value);
#endif

		if (name == "eAILogLevel")
		{
			m_LogLevelSavedAsString = true;
			if (value == "TRACE")
				SetLogLevel(CF_LogLevel.TRACE);
			if (value == "DEBUG")
				SetLogLevel(CF_LogLevel.DEBUG);
			if (value == "INFO")
				SetLogLevel(CF_LogLevel.INFO);
			if (value == "WARNING")
				SetLogLevel(CF_LogLevel.WARNING);
			if (value == "ERROR")
				SetLogLevel(CF_LogLevel.ERROR);
			if (value == "NONE")
				SetLogLevel(CF_LogLevel.NONE);
			return;
		}

		if (name == "LoadoutDirectories")
		{
			ClearLoadoutDirectories();
			AddLoadoutDirectory(value);
			return;
		}
	}

	override void OnFloat(string name, float value)
	{
#ifdef EAI_TRACE
		auto trace = CF_Trace_2(this, "OnFloat").Add(name).Add(value);
#endif

		if (name == "AccuracyMin" || name == "Accuracy")
		{
			SetAccuracyMin(value);
			return;
		}

		if (name == "AccuracyMax" || name == "Accuracy")
		{
			SetAccuracyMax(value);
			return;
		}
	}

	override void OnStartArray(string name)
	{
#ifdef EAI_TRACE
		auto trace = CF_Trace_1(this, "OnStartArray").Add(name);
#endif

		m_LoadingArray = name;

		if (m_LoadingArray == "LoadoutDirectories")
		{
			ClearLoadoutDirectories();
			return;
		}

		if (m_LoadingArray == "Admins")
		{
			ClearAdmins();
			return;
		}
	}

	override void OnEndArray(int itemCount)
	{
#ifdef EAI_TRACE
		auto trace = CF_Trace_1(this, "OnEndArray").Add(itemCount);
#endif

		m_LoadingArray = "";
	}

	override void OnItemString(int index, string value)
	{
#ifdef EAI_TRACE
		auto trace = CF_Trace_2(this, "OnItemString").Add(index).Add(value);
#endif

		if (m_LoadingArray == "LoadoutDirectories")
		{
			AddLoadoutDirectory(value);
			return;
		}

		if (m_LoadingArray == "Admins")
		{
			AddAdmin(value);
			return;
		}
	}

	override void OnPack()
	{
#ifdef EAI_TRACE
		auto trace = CF_Trace_0(this, "OnPack");
#endif

		if (m_LogLevelSavedAsString)
		{
			StoreString("eAILogLevel", typename.EnumToString(CF_LogLevel, m_LogLevel));
		}
		else
		{
			StoreInteger("eAILogLevel", m_LogLevel);
		}

		StoreInteger("MaxDynamicPatrols", m_MaxDynamicPatrols);
		StoreFloat("AccuracyMin", m_Accuracy_Min);
		StoreFloat("AccuracyMax", m_Accuracy_Max);

		if (m_LoadoutDirectoriesSavedAsArray || m_LoadoutDirectories.Count() > 1)
		{
			StartArray("LoadoutDirectories");
			foreach (string loadoutDirectory : m_LoadoutDirectories)
			{
				ItemString(loadoutDirectory);
			}
			EndArray();
		}
		else if (m_LoadoutDirectories.Count() == 1)
		{
			StoreString("LoadoutDirectories", m_LoadoutDirectories[0]);
		}

		StartArray("Admins");
		foreach (string admin : m_Admins)
		{
			ItemString(admin);
		}
		EndArray();
	}

	override void OnSuccess(int errorCode)
	{
#ifdef EAI_TRACE
		auto trace = CF_Trace_1(this, "OnSuccess").Add(errorCode);
#endif

		ON_UPDATE.Invoke();
	}

	override void OnError(int errorCode)
	{
#ifdef EAI_TRACE
		auto trace = CF_Trace_1(this, "OnError").Add(errorCode);
#endif

		Print(errorCode);
		ON_UPDATE.Invoke();
	}

	static void Init()
	{
#ifdef EAI_TRACE
		auto trace = CF_Trace_0("eAISettings", "Init");
#endif

		if (!GetJsonApi())
		{
			CreateJsonApi();
		}

		if (!FileExist(PATH))
		{
			m_Instance.Save();
			ON_UPDATE.Invoke();
			return;
		}

		m_Instance.Load();
	}

	static eAISettings Get()
	{
#ifdef EAI_TRACE
		auto trace = CF_Trace_0("eAISettings", "Get");
#endif

		return m_Instance;
	}

	private void eAISettings() {}

	void Load()
	{
#ifdef EAI_TRACE
		auto trace = CF_Trace_0(this, "Load");
#endif

		FileHandle file_handle = OpenFile(PATH, FileMode.READ);
		string content;
		string line_content;
		while (FGets(file_handle, line_content) >= 0)
		{
			content += line_content;
		}

		CloseFile(file_handle);

		ExpandFromRAW(content);
	}

	void Save()
	{
#ifdef EAI_TRACE
		auto trace = CF_Trace_0(this, "Save");
#endif

		InstantPack();
		SaveToFile(PATH);
	}
};
