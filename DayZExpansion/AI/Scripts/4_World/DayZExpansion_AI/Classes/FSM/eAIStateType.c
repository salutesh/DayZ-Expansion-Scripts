class eAIStateType
{
	private static ref map<string, autoptr eAIStateType> m_Types = new map<string, autoptr eAIStateType>();

	string m_ClassName;
	ref array<string> m_Variables;

	void eAIStateType()
	{
		#ifdef EAI_TRACE
		auto trace = CF_Trace_0(this, "eAIStateType");
		#endif
		
		m_Variables = new array<string>();
	}

	static bool Contains(string name)
	{
		#ifdef EAI_TRACE
		auto trace = CF_Trace_1("eAIStateType", "Contains").Add(name);
		#endif
		
		return m_Types.Contains(name);
	}

	static void Add(eAIStateType type)
	{
		#ifdef EAI_TRACE
		auto trace = CF_Trace_1("eAIStateType", "Add").Add(type);
		#endif
		
		m_Types.Insert(type.m_ClassName, type);
	}

	static eAIStateType Get(string type)
	{
		#ifdef EAI_TRACE
		auto trace = CF_Trace_1("eAIStateType", "Get").Add(type);
		#endif
		
		return m_Types[type];
	}

	static eAIStateType LoadXML(string fsmName, CF_XML_Tag xml_root_tag, FileHandle file)
	{
		#ifdef EAI_TRACE
		auto trace = CF_Trace_2("eAIStateType", "LoadXML").Add(fsmName).Add(xml_root_tag);
		#endif

		string name = xml_root_tag.GetAttribute("name").ValueAsString();
		string child_fsm;
		if (xml_root_tag.GetAttribute("fsm")) child_fsm = xml_root_tag.GetAttribute("fsm").ValueAsString();
		string class_name = "eAI_" + fsmName + "_" + name + "_State_" + eAIFSMType.s_ReloadNumber;
		
		//if (child_fsm != "") class_name = "eAI_FSM_" + child_fsm + "_State";

		if (eAIStateType.Contains(class_name)) return eAIStateType.Get(class_name);

		eAIStateType new_type = new eAIStateType();
		new_type.m_ClassName = class_name;

		FPrintln(file, "class " + class_name + " extends eAIState {");

		if (child_fsm != "") FPrintln(file, "eAI_" + child_fsm + "_FSM_" + eAIFSMType.s_ReloadNumber + " sub_fsm;");
		FPrintln(file, "eAI_" + fsmName + "_FSM_" + eAIFSMType.s_ReloadNumber + " fsm;");

		auto variables = xml_root_tag.GetTag("variables");
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

		FPrintln(file, "void " + class_name + "(eAIFSM _fsm, eAIBase _unit) {");
		FPrintln(file, "Class.CastTo(fsm, _fsm);");
		FPrintln(file, "m_ClassName = \"" + class_name + "\";");
		if (child_fsm != "")
		{
			FPrintln(file, "m_Name = \"" + child_fsm + "\";");
			FPrintln(file, "m_SubFSM = new eAI_" + child_fsm + "_FSM_" + eAIFSMType.s_ReloadNumber + "(_unit, this);");
			FPrintln(file, "Class.CastTo(sub_fsm, m_SubFSM);");
		}
		else
		{
			FPrintln(file, "m_Name = \"" + name + "\";");
		}

		FPrintln(file, "}");

		auto event_entry = xml_root_tag.GetTag("event_entry");
		auto event_exit = xml_root_tag.GetTag("event_exit");
		auto event_update = xml_root_tag.GetTag("event_update");

		auto guard_exit = xml_root_tag.GetTag("guard_exit");

		if (child_fsm != "")
		{
			FPrintln(file, "override void OnEntry(string Event, eAIState From) {");
#ifdef EAI_TRACE
			FPrintln(file, "auto trace = CF_Trace_2(this, \"OnEntry\").Add(Event).Add(From);");
#endif
			FPrintln(file, "if (Event != \""+"\") m_SubFSM.Start(Event);");
			FPrintln(file, "else m_SubFSM.StartDefault();");
			FPrintTag0(file, event_entry);
			FPrintln(file, "}");

			FPrintln(file, "override void OnExit(string Event, bool Aborted, eAIState To) {");
#ifdef EAI_TRACE
			FPrintln(file, "auto trace = CF_Trace_3(this, \"OnExit\").Add(Event).Add(Aborted).Add(To);");
#endif
			FPrintln(file, "if (Aborted) m_SubFSM.Abort(Event);");
			FPrintTag0(file, event_exit);
			FPrintln(file, "}");

			FPrintln(file, "override int OnUpdate(float DeltaTime, int SimulationPrecision) {");
#ifdef EAI_TRACE
			FPrintln(file, "auto trace = CF_Trace_2(this, \"OnUpdate\").Add(DeltaTime).Add(SimulationPrecision);");
#endif
			FPrintln(file, "if (m_SubFSM.Update(DeltaTime, SimulationPrecision) == EXIT) return EXIT;");
			if (!FPrintTag0(file, event_update))
				FPrintln(file, "return CONTINUE;");
			FPrintln(file, "}");

			FPrintln(file, "override int ExitGuard(float DeltaTime = 0) {");
#ifdef EAI_TRACE
			FPrintln(file, "auto trace = CF_Trace_1(this, \"ExitGuard\").Add(DeltaTime);");
#endif
			FPrintln(file, "if (m_SubFSM.ExitGuard(DeltaTime) == CONTINUE) return CONTINUE;");
			if (!FPrintTag0(file, guard_exit))
				FPrintln(file, "return EXIT;");
			FPrintln(file, "}");
		}
		else
		{
			FPrintln(file, "override void OnEntry(string Event, eAIState From) {");
#ifdef EAI_TRACE
			FPrintln(file, "auto trace = CF_Trace_2(this, \"OnEntry\").Add(Event).Add(From);");
#endif
			FPrintTag0(file, event_entry);
			FPrintln(file, "}");

			FPrintln(file, "override void OnExit(string Event, bool Aborted, eAIState To) {");
#ifdef EAI_TRACE
			FPrintln(file, "auto trace = CF_Trace_3(this, \"OnExit\").Add(Event).Add(Aborted).Add(To);");
#endif
			FPrintTag0(file, event_exit);
			FPrintln(file, "}");

			FPrintln(file, "override int OnUpdate(float DeltaTime, int SimulationPrecision) {");
#ifdef EAI_TRACE
			FPrintln(file, "auto trace = CF_Trace_2(this, \"OnUpdate\").Add(DeltaTime).Add(SimulationPrecision);");
#endif
			if (!FPrintTag0(file, event_update))
				FPrintln(file, "return CONTINUE;");
			FPrintln(file, "}");

			FPrintln(file, "override int ExitGuard(float DeltaTime = 0) {");
#ifdef EAI_TRACE
			FPrintln(file, "auto trace = CF_Trace_1(this, \"ExitGuard\").Add(DeltaTime);");
#endif
			if (!FPrintTag0(file, guard_exit))
				FPrintln(file, "return EXIT;");
			FPrintln(file, "}");
		}

		FPrintln(file, "}");
		
		eAIStateType.Add(new_type);
		
		return new_type;
	}

	static bool FPrintTag0(FileHandle file, array<CF_XML_Tag> tags)
	{
		if (tags.Count() > 0)
		{
			string content = tags[0].GetContent().GetContent();
			if (content.Trim())
			{
				FPrintln(file, content);
				return true;
			}
		}

		return false;
	}
};
