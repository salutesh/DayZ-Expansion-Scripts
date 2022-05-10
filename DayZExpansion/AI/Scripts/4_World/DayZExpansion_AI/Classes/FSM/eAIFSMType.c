class eAIFSMType
{
	static int s_ReloadNumber = 0;
	
	private static autoptr map<string, eAIFSMType> m_SpawnableTypes = new map<string, eAIFSMType>();
	private static autoptr array<autoptr eAIFSMType> m_Types = new array<autoptr eAIFSMType>();

	string m_ClassName;
	ScriptModule m_Module;

	autoptr array<string> m_Variables = new array<string>();
	autoptr array<autoptr eAIStateType> m_States = new array<autoptr eAIStateType>();
	autoptr array<autoptr eAITransitionType> m_Transitions = new array<autoptr eAITransitionType>();

	void eAIFSMType()
	{
		#ifdef EAI_TRACE
		auto trace = CF_Trace_0(this, "eAIFSMType");
		#endif
		
	}

	static void UnloadAll()
	{
		s_ReloadNumber++;
		
		m_Types.Clear();
		m_SpawnableTypes.Clear();
	}

	static bool Contains(string name)
	{
		#ifdef EAI_TRACE
		auto trace = CF_Trace_1("eAIFSMType", "Contains").Add(name);
		#endif
		
		return m_SpawnableTypes.Contains(name);
	}
	
	static void AddSpawnable(string name, eAIFSMType type)
	{
		#ifdef EAI_TRACE
		auto trace = CF_Trace_2("eAIFSMType", "AddSpawnable").Add(name).Add(type);
		#endif
		
		m_SpawnableTypes.Insert(name, type);
	}

	static void Add(eAIFSMType type)
	{
		#ifdef EAI_TRACE
		auto trace = CF_Trace_1("eAIFSMType", "Add").Add(type);
		#endif
		
		m_Types.Insert(type);
	}

	static eAIFSMType Get(string type)
	{
		#ifdef EAI_TRACE
		auto trace = CF_Trace_1("eAIFSMType", "Get").Add(type);
		#endif
		
		return m_SpawnableTypes[type];
	}

	eAIFSM Spawn(eAIBase unit, eAIState parentState)
	{
		#ifdef EAI_TRACE
		auto trace = CF_Trace_2(this, "Spawn").Add(unit).Add(parentState);
		#endif
		
		eAIFSM retValue = null;
		m_Module.CallFunctionParams(null, "Create_" + m_ClassName, retValue, new Param2<eAIBase, eAIState>(unit, parentState));
		return retValue;
	}

	static eAIFSM Spawn(string type, eAIBase unit, eAIState parentState)
	{
		#ifdef EAI_TRACE
		auto trace = CF_Trace_3("eAIFSMType", "Spawn").Add(type).Add(unit).Add(parentState);
		#endif
		
		return m_SpawnableTypes[type].Spawn(unit, parentState);
	}
	
	static eAIFSMType LoadXML(string path, string fileName)
	{
		#ifdef EAI_TRACE
		auto trace = CF_Trace_2("eAIFSMType", "LoadXML").Add(path).Add(fileName);
		#endif

		if (eAIFSMType.Contains(fileName)) return eAIFSMType.Get(fileName);

		if (!FileExist(EXPANSION_AI_FOLDER))
			MakeDirectory(EXPANSION_AI_FOLDER);

		if (!FileExist(EXPANSION_AI_UTILS_FOLDER))
			MakeDirectory(EXPANSION_AI_UTILS_FOLDER);
			
		string script_path = EXPANSION_AI_UTILS_FOLDER + fileName + ".c";
		FileHandle file = OpenFile(script_path, FileMode.WRITE);
		if (!file) return null;

		eAIFSMType new_type = LoadXML(path, fileName, file);
		
		CloseFile(file);

		ScriptModule module = GetGame().GetMission().MissionScript;
		new_type.m_Module = ScriptModule.LoadScript(module, script_path, false);
		if (new_type.m_Module == null)
		{
			Error("There was an error loading in the fsm.");
			return null;
		}

		AddSpawnable(fileName, new_type);

		return new_type;
	}

	static eAIFSMType LoadXML(string path, string fileName, FileHandle file)
	{
		#ifdef EAI_TRACE
		auto trace = CF_Trace_3("eAIFSMType", "LoadXML").Add(path).Add(fileName).Add("FileHandle");
		#endif

		string actualFilePath = path + "/" + fileName + ".xml";
		CF_Log.Debug(actualFilePath);
		if (!FileExist(actualFilePath)) return null;
		
		CF_XML_Document document;
		CF_XML.ReadDocument(actualFilePath, document);
		
		string name = document.Get("fsm")[0].GetAttribute("name").ValueAsString();
		string class_name = "eAI_" + name + "_FSM_" + s_ReloadNumber;

		auto files = document.Get("fsm");
		files = files[0].GetTag("files");
		if (files.Count() > 0)
		{
			files = files[0].GetTag("file");

			for (int i = 0; i < files.Count(); i++)
			{
				string subFSMName = files[i].GetAttribute("name").ValueAsString();
				eAIFSMType.LoadXML(path, subFSMName, file);
			}
		}

		eAIFSMType new_type = new eAIFSMType();
		new_type.m_ClassName = class_name;		

		auto states = document.Get("fsm");
		states = states[0].GetTag("states");
		auto defaultStateAttrib = states[0].GetAttribute("default");
		string defaultState = "";
		if (defaultStateAttrib)	defaultState = "eAI_" + name + "_" + defaultStateAttrib.ValueAsString() + "_State_" + eAIFSMType.s_ReloadNumber;
		states = states[0].GetTag("state");

		foreach (auto state : states)
		{
			eAIStateType stateType = eAIStateType.LoadXML(name, state, file);
			new_type.m_States.Insert(stateType);
		}

		auto transitions = document.Get("fsm");
		transitions = transitions[0].GetTag("transitions");
		transitions = transitions[0].GetTag("transition");
		foreach (auto transition : transitions)
		{
			eAITransitionType transitionType = eAITransitionType.LoadXML(name, transition, file);
			new_type.m_Transitions.Insert(transitionType);
		}

		FPrintln(file, "class " + class_name + " extends eAIFSM {");

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
				if (variable.GetAttribute("default")) variable_default = variable.GetAttribute("default").ValueAsString();

				new_type.m_Variables.Insert(variable_name);

				string variable_line = "" + variable_type + " " + variable_name;
				if (variable_default != "") variable_line = variable_line + " = " + variable_default;
				variable_line = variable_line + ";";

				FPrintln(file, variable_line);
			}
		}
			
		FPrintln(file, "void " + class_name + "(eAIBase unit, eAIState parentState) {");
		FPrintln(file, "m_Name = \"" + name + "\";");
		FPrintln(file, "m_DefaultState = \"" + defaultState + "\";");
		FPrintln(file, "Setup();");
		FPrintln(file, "SortTransitions();");
		FPrintln(file, "}");

		FPrintln(file, "void Setup() {");
#ifdef EAI_TRACE
		FPrintln(file, "auto trace = CF_Trace_0(this, \"Setup\");");
#endif

		foreach (auto stateType0 : new_type.m_States)
		{	
			FPrintln(file, "AddState(new " + stateType0.m_ClassName + "(this, m_Unit));");
		}

		foreach (auto transitionType0 : new_type.m_Transitions)
		{
			FPrintln(file, "AddTransition(new " + transitionType0.m_ClassName + "(this, m_Unit));");
		}

		FPrintln(file, "}");

		FPrintln(file, "}");

		FPrintln(file, "eAIFSM Create_" + class_name + "(eAIBase unit, eAIState parentState) {");
		FPrintln(file, "return new " + class_name + "(unit, parentState);");
		FPrintln(file, "}");

		eAIFSMType.Add(new_type);
		
		return new_type;
	}
};
