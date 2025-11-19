class ExpansionFSMType
{
	static int s_ReloadNumber = 0;
	static ref map<string, bool> s_Loaded = new map<string, bool>;
	
	private static autoptr map<string, ExpansionFSMType> m_SpawnableTypes = new map<string, ExpansionFSMType>();
	private static autoptr array<autoptr ExpansionFSMType> m_Types = new array<autoptr ExpansionFSMType>();

	string m_Name;
	string m_Type;

	string m_ClassName;
	ScriptModule m_Module;

	autoptr array<string> m_Variables = new array<string>();
	autoptr array<autoptr ExpansionStateType> m_States = new array<autoptr ExpansionStateType>();
	autoptr array<autoptr ExpansionTransitionType> m_Transitions = new array<autoptr ExpansionTransitionType>();

	void ExpansionFSMType()
	{
		#ifdef EAI_TRACE
		auto trace = CF_Trace_0(this, "ExpansionFSMType");
		#endif
		
	}

	static void UnloadAll()
	{
		s_ReloadNumber++;
		s_Loaded.Clear();
		
		m_Types.Clear();
		m_SpawnableTypes.Clear();
	}

	static bool Contains(string name)
	{
		#ifdef EAI_TRACE
		auto trace = CF_Trace_1("ExpansionFSMType", "Contains").Add(name);
		#endif
		
		return m_SpawnableTypes.Contains(name);
	}
	
	static void AddSpawnable(string name, ExpansionFSMType type)
	{
		#ifdef EAI_TRACE
		auto trace = CF_Trace_2("ExpansionFSMType", "AddSpawnable").Add(name).Add(type);
		#endif
		
		m_SpawnableTypes.Insert(name, type);
	}

	static void Add(ExpansionFSMType type)
	{
		#ifdef EAI_TRACE
		auto trace = CF_Trace_1("ExpansionFSMType", "Add").Add(type);
		#endif
		
		m_Types.Insert(type);
	}

	static ExpansionFSMType Get(string type)
	{
		#ifdef EAI_TRACE
		auto trace = CF_Trace_1("ExpansionFSMType", "Get").Add(type);
		#endif
		
		return m_SpawnableTypes[type];
	}

	ExpansionFSM Spawn(ExpansionFSMOwnerType owner, ExpansionState parentState)
	{
		#ifdef EAI_TRACE
		auto trace = CF_Trace_1(this, "Spawn").Add(owner).Add(parentState);
		#endif
		
		ExpansionFSM retValue = null;
		Param params = new Param2<ExpansionFSMOwnerType, ExpansionState>(owner, parentState);
		m_Module.CallFunctionParams(null, "Create_" + m_ClassName, retValue, params);
		return retValue;
	}

	static ExpansionFSM Spawn(string type, ExpansionFSMOwnerType owner, ExpansionState parentState)
	{
		#ifdef EAI_TRACE
		auto trace = CF_Trace_3("ExpansionFSMType", "Spawn").Add(type).Add(owner).Add(parentState);
		#endif
		
		return m_SpawnableTypes[type].Spawn(owner, parentState);
	}
	
	static ExpansionFSMType LoadXML(string path, string fileName)
	{
		#ifdef EAI_TRACE
		auto trace = CF_Trace_2("ExpansionFSMType", "LoadXML").Add(path).Add(fileName);
		#endif

		if (ExpansionFSMType.Contains(fileName))
		{
			return ExpansionFSMType.Get(fileName);
		}

		bool success;
		if (s_Loaded.Find(path + "/" + fileName, success) && !success)
			return null;

		if (!FileExist(EXPANSION_AI_FOLDER))
		{
			ExpansionStatic.MakeDirectoryRecursive(EXPANSION_AI_FOLDER);
		}

		if (!FileExist(EXPANSION_AI_FSM_FOLDER))
		{
			ExpansionStatic.MakeDirectoryRecursive(EXPANSION_AI_FSM_FOLDER);
		}
			
		string script_path = EXPANSION_AI_FSM_FOLDER + fileName + ".c";
		FileHandle file = OpenFile(script_path, FileMode.WRITE);
		if (!file)
		{
			return null;
		}

		ExpansionFSMType new_type = LoadXML(path, fileName, file);
		
		CloseFile(file);

		ScriptModule module = g_Game.GetMission().MissionScript;
		new_type.m_Module = ScriptModule.LoadScript(module, script_path, false);
		if (new_type.m_Module == null)
		{
			Error("There was an error loading in the FSM generated from " + path + "/" + fileName + ".xml, check $profile:script_<date>_<time>.log and " + script_path);
			s_Loaded[path + "/" + fileName] = false;
			return null;
		}

		s_Loaded[path + "/" + fileName] = true;

#ifdef DIAG_DEVELOPER
		//! Create a backup to ease debugging
		CopyFile(script_path, script_path + ".bak");
#endif

		AddSpawnable(fileName, new_type);

		return new_type;
	}

	static ExpansionFSMType LoadXML(string path, string fileName, FileHandle file)
	{
		#ifdef EAI_TRACE
		auto trace = CF_Trace_3("ExpansionFSMType", "LoadXML").Add(path).Add(fileName).Add("FileHandle");
		#endif

		string actualFilePath = path + "/" + fileName + ".xml";
		CF_Log.Debug(actualFilePath);
		if (!FileExist(actualFilePath))
		{
			return null;
		}

		CF_XML_Document document;
		CF_XML.ReadDocument(actualFilePath, document);

		#ifdef DIAG_DEVELOPER
		//! XXX: CF_XML_Document::Save doesn't write a proper representation of the parsed document
		document.Save(EXPANSION_AI_FSM_FOLDER + fileName + ".xml");
		#endif
		
		string name = document.Get("fsm")[0].GetAttribute("name").ValueAsString();
		string type = document.Get("fsm")[0].GetAttribute("type").ValueAsString();
		string class_name = "Expansion_" + name + "_FSM_" + s_ReloadNumber;

		auto files = document.Get("fsm");
		files = files[0].GetTag("files");
		if (files.Count() > 0)
		{
			files = files[0].GetTag("file");

			for (int i = 0; i < files.Count(); i++)
			{
				string subFSMName = files[i].GetAttribute("name").ValueAsString();
				ExpansionFSMType.LoadXML(path, subFSMName, file);
			}
		}

		ExpansionFSMType new_type = new ExpansionFSMType();
		new_type.m_Name = name;
		new_type.m_Type = type;
		new_type.m_ClassName = class_name;

		auto states = document.Get("fsm");
		states = states[0].GetTag("states");
		auto defaultStateAttrib = states[0].GetAttribute("default");
		string defaultState = "";
		if (defaultStateAttrib)
		{
			defaultState = "Expansion_" + name + "_" + defaultStateAttrib.ValueAsString() + "_State_" + ExpansionFSMType.s_ReloadNumber;
		}

		states = states[0].GetTag("state");

		foreach (auto state : states)
		{
			ExpansionStateType stateType = ExpansionStateType.LoadXML(new_type, state, file);
			new_type.m_States.Insert(stateType);
		}

		auto transitions = document.Get("fsm");
		transitions = transitions[0].GetTag("transitions");
		transitions = transitions[0].GetTag("transition");
		foreach (auto transition : transitions)
		{
			array<autoptr ExpansionTransitionType> transitionTypes = ExpansionTransitionType.LoadXML(new_type, transition, file);
			foreach (auto transitionType: transitionTypes)
			{
				new_type.m_Transitions.Insert(transitionType);
			}
		}

		FPrintln(file, "class " + class_name + ": " + type + "FSM {");

		auto variables = document.Get("fsm");
		variables = variables[0].GetTag("variables");
		if (variables.Count() > 0)
		{
			variables = variables[0].GetTag("variable");
			foreach (auto variable : variables)
			{
				string variable_name = variable.GetAttribute("name").ValueAsString();
				string variable_type = variable.GetAttribute("type").ValueAsString();
				string variable_default = "";
				if (variable.GetAttribute("default"))
				{
					variable_default = variable.GetAttribute("default").ValueAsString();
				}

				new_type.m_Variables.Insert(variable_name);

				string variable_line = "" + variable_type + " " + variable_name;
				if (variable_default != "") variable_line += " = " + variable_default;
				variable_line += ";";

				FPrintln(file, "	" + variable_line);
			}
		}
			
		FPrintln(file, "	void " + class_name + "(ExpansionFSMOwnerType owner, ExpansionState parentState) {");
		FPrintln(file, "		m_Name = \"" + name + "\";");
		FPrintln(file, "		m_DefaultState = \"" + defaultState + "\";");
		FPrintln(file, "		Setup();");
		FPrintln(file, "	}");

		FPrintln(file, "	void Setup() {");
#ifdef EAI_TRACE
		FPrintln(file, "		auto trace = CF_Trace_0(this, \"Setup\");");
#endif

		foreach (auto stateType0 : new_type.m_States)
		{
			FPrintln(file, "		AddState(new " + stateType0.m_ClassName + "(this));");
		}

		foreach (auto transitionType0 : new_type.m_Transitions)
		{
			FPrintln(file, "		AddTransition(new " + transitionType0.m_ClassName + "(this));");
		}

		FPrintln(file, "	}");

		FPrintln(file, "}");

		FPrintln(file, "ExpansionFSM Create_" + class_name + "(ExpansionFSMOwnerType owner, ExpansionState parentState) {");
		FPrintln(file, "	return new " + class_name + "(owner, parentState);");
		FPrintln(file, "}");

		ExpansionFSMType.Add(new_type);
		
		return new_type;
	}
};
