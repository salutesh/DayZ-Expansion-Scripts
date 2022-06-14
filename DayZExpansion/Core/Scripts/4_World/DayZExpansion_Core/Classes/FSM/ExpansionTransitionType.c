class ExpansionTransitionType
{
	private static ref map<string, autoptr ExpansionTransitionType> m_Types = new map<string, autoptr ExpansionTransitionType>();

	string m_ClassName;

	void ExpansionTransitionType()
	{
#ifdef EAI_TRACE
		auto trace = CF_Trace_0(this, "ExpansionTransitionType");
#endif
	}

	static void UnloadAll()
	{
		m_Types.Clear();
	}

	static bool Contains(string name)
	{
#ifdef EAI_TRACE
		auto trace = CF_Trace_1("ExpansionTransitionType", "Contains").Add(name);
#endif

		return m_Types.Contains(name);
	}

	static void Add(ExpansionTransitionType type)
	{
#ifdef EAI_TRACE
		auto trace = CF_Trace_1("ExpansionTransitionType", "Add").Add(type);
#endif

		m_Types.Insert(type.m_ClassName, type);
	}

	static ExpansionTransitionType Get(string type)
	{
#ifdef EAI_TRACE
		auto trace = CF_Trace_1("ExpansionTransitionType", "Get").Add(type);
#endif

		return m_Types[type];
	}

	static ExpansionTransitionType LoadXML(ExpansionFSMType fsmType, CF_XML_Tag xml_root_tag, FileHandle file)
	{
#ifdef EAI_TRACE
		auto trace = CF_Trace_1("ExpansionTransitionType", "LoadXML").Add(fsmName);
#endif

		string fsmName = fsmType.m_Name;

		string from_state_name;
		auto from_state = xml_root_tag.GetTag("from_state");
		if (from_state.Count() > 0)
		{
			auto from_state_name_attr = from_state[0].GetAttribute("name");
			if (from_state_name_attr)
			{
				from_state_name = from_state_name_attr.ValueAsString();
			}
		}
		string from_state_class = "ExpansionState";
		if (from_state_name != "")
		{
			from_state_class = "Expansion_" + fsmName + "_" + from_state_name + "_State_" + ExpansionFSMType.s_ReloadNumber;
		}

		string to_state_name;
		auto to_state = xml_root_tag.GetTag("to_state");
		if (to_state.Count() > 0)
		{
			auto to_state_name_attr = to_state[0].GetAttribute("name");
			if (to_state_name_attr)
			{
				to_state_name = to_state_name_attr.ValueAsString();
			}
		}
		string to_state_class = "ExpansionState";
		if (to_state_name != "")
		{
			to_state_class = "Expansion_" + fsmName + "_" + to_state_name + "_State_" + ExpansionFSMType.s_ReloadNumber;
		}

		string event_name;
		auto evt = xml_root_tag.GetTag("event");
		if (evt.Count() > 0)
		{
			auto evt_name_attr = evt[0].GetAttribute("name");
			if (evt_name_attr)
			{
				event_name = evt_name_attr.ValueAsString();
			}
		}
		string event_class = "";
		if (event_name != "")
		{
			event_class = "Expansion_" + event_name + "_Event";
		}

		string class_name = "Expansion_" + fsmName + "_" + from_state_name + "_" + to_state_name + "_Transition_" + ExpansionFSMType.s_ReloadNumber;

		if (ExpansionTransitionType.Contains(class_name))
		{
			return ExpansionTransitionType.Get(class_name);
		}

		ExpansionTransitionType new_type = new ExpansionTransitionType();
		new_type.m_ClassName = class_name;

		FPrintln(file, "class " + class_name + " extends " + fsmType.m_Type + "Transition {");

		FPrintln(file, "private " + from_state_class + " src;");
		FPrintln(file, "private " + to_state_class + " dst;");

		FPrintln(file, "Expansion_" + fsmName + "_FSM_" + ExpansionFSMType.s_ReloadNumber + " fsm;");

		FPrintln(file, "void " + class_name + "(ExpansionFSM _fsm) {");
		FPrintln(file, "Class.CastTo(fsm, _fsm);");
		FPrintln(file, "m_ClassName = \"" + class_name + "\";");
		FPrintln(file, "Class.CastTo(src, _fsm.GetState(\"" + from_state_class + "\"));");
		FPrintln(file, "Class.CastTo(dst, _fsm.GetState(\"" + to_state_class + "\"));");
		FPrintln(file, "}");

		auto guard = xml_root_tag.GetTag("guard");
		if (guard.Count() > 0)
		{
			FPrintln(file, "override int Guard() {");
#ifdef EAI_TRACE
			FPrintln(file, "auto trace = CF_Trace_0(this, \"Guard\");");
#endif
			FPrintln(file, guard[0].GetContent().GetContent());
			FPrintln(file, "}");
		}

		FPrintln(file, "override ExpansionState GetSource() { return src; }");
		FPrintln(file, "override ExpansionState GetDestination() { return dst; }");
		FPrintln(file, "override string GetEvent() { return \"" + event_class + "\"; }");

		FPrintln(file, "}");

		ExpansionTransitionType.Add(new_type);

		return new_type;
	}
};
