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

	static array<autoptr ExpansionTransitionType> LoadXML(ExpansionFSMType fsmType, CF_XML_Tag xml_root_tag, FileHandle file)
	{
#ifdef EAI_TRACE
		auto trace = CF_Trace_1("ExpansionTransitionType", "LoadXML").Add(fsmType.m_Name);
#endif

		TStringArray from_state_names = {};
		auto from_state = xml_root_tag.GetTag("from_state");
		if (from_state.Count() > 0)
		{
			auto from_state_name_attr = from_state[0].GetAttribute("name");
			if (from_state_name_attr)
			{
				from_state_name_attr.ValueAsString().Split(",", from_state_names);
			}
		}

		array<autoptr ExpansionTransitionType> transitionTypes = {};

		if (from_state_names.Count())
		{
			foreach (string from_state_name: from_state_names)
			{
				from_state_name.Trim();
				transitionTypes.Insert(CreateTransition(fsmType, xml_root_tag, file, from_state_name));
			}
		}
		else
		{
			transitionTypes.Insert(CreateTransition(fsmType, xml_root_tag, file));
		}

		return transitionTypes;
	}

	static ExpansionTransitionType CreateTransition(ExpansionFSMType fsmType, CF_XML_Tag xml_root_tag, FileHandle file, string from_state_name = string.Empty)
	{
		string fsmName = fsmType.m_Name;

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

		bool inheritsFromCustom = false;

		string inherited = fsmType.m_Type + "Transition";
		if (xml_root_tag.GetAttribute("class"))
		{
			typename inherited_type = xml_root_tag.GetAttribute("class").ValueAsString().ToType();
			if (!inherited_type.IsInherited(inherited.ToType()))
			{
				Error("class doesn't inherit from fsm base type");
			}

			inherited = inherited_type.ToString();
			inheritsFromCustom = true;
		}

		string class_name = "Expansion_" + fsmName + "_" + from_state_name + "_" + to_state_name + "_Transition_" + ExpansionFSMType.s_ReloadNumber;

		if (ExpansionTransitionType.Contains(class_name))
		{
			return ExpansionTransitionType.Get(class_name);
		}

		ExpansionTransitionType new_type = new ExpansionTransitionType();
		new_type.m_ClassName = class_name;

		FPrintln(file, "class " + class_name + ": " + inherited + " {");

		FPrintln(file, "	private " + from_state_class + " src;");
		FPrintln(file, "	private " + to_state_class + " dst;");

		FPrintln(file, "	Expansion_" + fsmName + "_FSM_" + ExpansionFSMType.s_ReloadNumber + " fsm;");

		FPrintln(file, "	void " + class_name + "(ExpansionFSM _fsm) {");
		FPrintln(file, "		Class.CastTo(fsm, _fsm);");
		FPrintln(file, "		m_ClassName = \"" + class_name + "\";");
		FPrintln(file, "		Class.CastTo(src, _fsm.GetState(\"" + from_state_class + "\"));");
		FPrintln(file, "		Class.CastTo(dst, _fsm.GetState(\"" + to_state_class + "\"));");
		FPrintln(file, "	}");

		auto guard = xml_root_tag.GetTag("guard");
		if (guard.Count() > 0 && guard[0].GetContent().GetContent().Trim())
		{
			FPrintln(file, "	override int Guard() {");
#ifdef EAI_TRACE
			FPrintln(file, "	auto trace = CF_Trace_0(this, \"Guard\");");
#endif
			if (inheritsFromCustom) FPrintln(file, "		if (super.Guard() == FAIL) return FAIL;");
			ExpansionStateType.FPrintContent(file, guard[0].GetContent().GetContent());
			FPrintln(file, "	}");
		}

		FPrintln(file, "	override ExpansionState GetSource() { return src; }");
		FPrintln(file, "	override ExpansionState GetDestination() { return dst; }");
		FPrintln(file, "	override string GetEvent() { return \"" + event_class + "\"; }");

		FPrintln(file, "}");

		ExpansionTransitionType.Add(new_type);

		return new_type;
	}
};
