/**
 * ExpansionJsonSchemaExporter.c
 *
 * DayZ Expansion Mod
 * www.dayzexpansion.com
 * © 2022 DayZ Expansion Mod Team
 *
 * This work is licensed under the Creative Commons Attribution-NonCommercial-NoDerivatives 4.0 International License.
 * To view a copy of this license, visit http://creativecommons.org/licenses/by-nc-nd/4.0/.
 *
*/

class ExpansionJsonSchemaExporter
{
	private ref map<string, ref ExpansionJSONObject> m_Defs;
	private ref map<string, bool> m_Visiting;
	private ref ExpansionJSONObject m_Schema;

	private ref ExpansionJSONObject m_BoolSchema;
	private ref ExpansionJSONObject m_VectorSchema;

	void ExpansionJsonSchemaExporter()
	{
		m_Defs = new map<string, ref ExpansionJSONObject>();
		m_Visiting = new map<string, bool>();
	}

	ExpansionJSONObject GetSchema()
	{
		return m_Schema;
	}

	string ToJSONString(typename type, bool pretty = true)
	{
		Export(type);
		return m_Schema.ToJSONString(pretty);
	}

	bool ToFile(typename type, string path, bool pretty = true)
	{
		Export(type);

		FileHandle file = OpenFile(path, FileMode.WRITE);
		if (!file)
		{
			return false;
		}

		FPrintln(file, ToJSONString(type, pretty));
		CloseFile(file);

		return true;
	}

	private ExpansionJSONObject Export(typename type)
	{
		m_Defs.Clear();
		m_Visiting.Clear();

		m_Schema = Build(type);
		return m_Schema;
	}

	private ExpansionJSONObject Build(typename type)
	{
		ExpansionJSONObject schema = new ExpansionJSONObject();
		schema.Set("$schema", new ExpansionJSONString("https://json-schema.org/draft/2020-12/schema"));
		schema.Set("$id", new ExpansionJSONString(type.ToString() + ".schema.json"));
		schema.Set("title", new ExpansionJSONString(type.ToString()));
		schema.Set("type", new ExpansionJSONString("object"));
		schema.Set("properties", BuildProperties(type));

		if (m_Defs.Count() > 0)
		{
			ExpansionJSONObject defs = new ExpansionJSONObject();
			foreach (string defName, ExpansionJSONObject defSchema : m_Defs)
			{
				defs.Set(defName, defSchema);
			}
			schema.Set("$defs", defs);
		}

		return schema;
	}

	private ExpansionJSONObject BuildProperties(typename type)
	{
		ExpansionJSONObject props = new ExpansionJSONObject();
		int count = type.GetVariableCount();

		for (int i = 0; i < count; i++)
		{
			string varName = type.GetVariableName(i);

			if (varName.IndexOf("m_") == 0 || varName.IndexOf("s_") == 0)
			{
				continue;
			}

			typename varType = type.GetVariableType(i);
			props.Set(varName, TypeToSchema(varType));
		}

		return props;
	}

	private ExpansionJSONElementBase TypeToSchema(typename type)
	{
		switch (type)
		{
			case int:
			{
				return SimpleType("integer");
			}
			case float:
			{
				return SimpleType("number");
			}
			case string:
			{
				return SimpleType("string");
			}
			case bool:
			{
				return BuildBoolSchema();
			}
			case vector:
			{
				return BuildVectorSchema();
			}
		}

		string typeName = type.ToString();

		string inner;
		ExpansionJSONObject obj;
		if (typeName.IndexOf("array<") == 0)
		{
			inner = ExtractGenericParam(typeName);
			obj = new ExpansionJSONObject();
			obj.Set("type", new ExpansionJSONString("array"));
			obj.Set("items", TypeNameToSchema(inner));
			return obj;
		}
		if (typeName.IndexOf("set<") == 0)
		{
			inner = ExtractGenericParam(typeName);
			obj = new ExpansionJSONObject();
			obj.Set("type", new ExpansionJSONString("array"));
			obj.Set("uniqueItems", new ExpansionJSONBool(true));
			obj.Set("items", TypeNameToSchema(inner));
			return obj;
		}
		if (typeName.IndexOf("map<") == 0)
		{
			string keyType, valueType;
			ExtractMapParams(typeName, keyType, valueType);
			obj = new ExpansionJSONObject();
			obj.Set("type", new ExpansionJSONString("object"));
			obj.Set("additionalProperties", TypeNameToSchema(valueType));
			return obj;
		}

		if (type.IsInherited(Class))
		{
			return BuildClassRef(type);
		}

		ExpansionJSONObject fallback = new ExpansionJSONObject();
		fallback.Set("type", new ExpansionJSONString("string"));
		fallback.Set("description", new ExpansionJSONString("unmapped: " + typeName));
		return fallback;
	}

	private ExpansionJSONElementBase TypeNameToSchema(string name)
	{
		name = name.Trim();

		switch (name)
		{
			case "int":
			{
				return SimpleType("integer");
			}
			case "float":
			{
				return SimpleType("number");
			}
			case "string":
			{
				return SimpleType("string");
			}
			case "bool":
			{
				return BuildBoolSchema();
			}
			case "vector":
			{
				return BuildVectorSchema();
			}
		}

		string inner;
		ExpansionJSONObject obj;
		if (name.IndexOf("array<") == 0)
		{
			inner = ExtractGenericParam(name);
			obj = new ExpansionJSONObject();
			obj.Set("type", new ExpansionJSONString("array"));
			obj.Set("items", TypeNameToSchema(inner));
			return obj;
		}
		if (name.IndexOf("set<") == 0)
		{
			inner = ExtractGenericParam(name);
			obj = new ExpansionJSONObject();
			obj.Set("type", new ExpansionJSONString("array"));
			obj.Set("uniqueItems", new ExpansionJSONBool(true));
			obj.Set("items", TypeNameToSchema(inner));
			return obj;
		}
		if (name.IndexOf("map<") == 0)
		{
			string keyType, valueType;
			ExtractMapParams(name, keyType, valueType);
			obj = new ExpansionJSONObject();
			obj.Set("type", new ExpansionJSONString("object"));
			obj.Set("additionalProperties", TypeNameToSchema(valueType));
			return obj;
		}

		typename resolved = name.ToType();
		if (resolved && resolved.IsInherited(Class))
		{
			return BuildClassRef(resolved);
		}

		ExpansionJSONObject fallback = new ExpansionJSONObject();
		fallback.Set("type", new ExpansionJSONString("string"));
		fallback.Set("description", new ExpansionJSONString("unresolved: " + name));
		return fallback;
	}

	private ExpansionJSONObject BuildClassRef(typename type)
	{
		string key = type.ToString();

		if (m_Visiting.Contains(key))
		{
			return MakeRef(key);
		}

		if (m_Defs.Contains(key))
		{
			return MakeRef(key);
		}

		m_Visiting.Set(key, true);

		ExpansionJSONObject def = new ExpansionJSONObject();
		def.Set("type", new ExpansionJSONString("object"));
		def.Set("title", new ExpansionJSONString(key));
		def.Set("properties", BuildProperties(type));

		m_Defs.Set(key, def);
		m_Visiting.Remove(key);

		return MakeRef(key);
	}

	private ExpansionJSONObject MakeRef(string defName)
	{
		ExpansionJSONObject refObj = new ExpansionJSONObject();
		refObj.Set("$ref", new ExpansionJSONString("#/$defs/" + defName));
		return refObj;
	}

	private ExpansionJSONObject BuildVectorSchema()
	{
		if (!m_VectorSchema)
		{
			ExpansionJSONArray prefixItems = new ExpansionJSONArray();
			prefixItems.Insert(SimpleType("number"));
			prefixItems.Insert(SimpleType("number"));
			prefixItems.Insert(SimpleType("number"));

			m_VectorSchema = new ExpansionJSONObject();
			m_VectorSchema.Set("type", new ExpansionJSONString("array"));
			m_VectorSchema.Set("prefixItems", prefixItems);
			m_VectorSchema.Set("items", new ExpansionJSONBool(false));
			m_VectorSchema.Set("minItems", new ExpansionJSONInt(3));
			m_VectorSchema.Set("maxItems", new ExpansionJSONInt(3));
		}

		return m_VectorSchema;
	}

	private ExpansionJSONObject BuildBoolSchema()
	{
		if (!m_BoolSchema)
		{
			m_BoolSchema = new ExpansionJSONObject();
			m_BoolSchema.Set("type", new ExpansionJSONString("integer"));
			m_BoolSchema.Set("minimum", new ExpansionJSONInt(0));
			m_BoolSchema.Set("maximum", new ExpansionJSONInt(1));
		}

		return m_BoolSchema;
	}

	private ExpansionJSONObject SimpleType(string jsonType)
	{
		ExpansionJSONObject obj = new ExpansionJSONObject();
		obj.Set("type", new ExpansionJSONString(jsonType));
		return obj;
	}

	private string StripRef(string typeName)
	{
		if (typeName[0] == "@")
		{
			return typeName.Substring(1, typeName.Length() - 1);
		}

		return typeName;
	}

	private string ExtractGenericParam(string typeName)
	{
		int start = typeName.IndexOf("<") + 1;
		int end = typeName.Length() - 1;
		return StripRef(typeName.Substring(start, end - start));
	}

	private void ExtractMapParams(string typeName, out string keyType, out string valueType)
	{
		int len = typeName.Length();
		int start = typeName.IndexOf("<") + 1;

		string inner = typeName.Substring(start, len - start - 1);
		int innerLen = inner.Length();

		int depth = 0;
		int splitAt = -1;

		for (int i = 0; i < innerLen; i++)
		{
			string c = inner[i];
			if (c == "<")
			{
				depth++;
			}
			else if (c == ">")
			{
				depth--;
			}
			else if (c == "," && depth == 0)
			{
				splitAt = i;
				break;
			}
		}

		keyType = StripRef(inner.Substring(0, splitAt));
		valueType = StripRef(inner.Substring(splitAt + 1, innerLen - splitAt - 1));
	}
}
