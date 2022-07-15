class ExpansionPrefab : ExpansionPrefabObject
{
	[NonSerialized()]
	static ref array<ExpansionPrefabObject> s_Begin = new array<ExpansionPrefabObject>();

	[NonSerialized()]
	static ref map<string, ref ExpansionPrefab> s_Prefabs = new map<string, ref ExpansionPrefab>();

	[NonSerialized()]
	string m_Path;

	static ExpansionPrefab Load(string name, bool forceReload = false)
	{
		ExpansionPrefab prefab;
		if (s_Prefabs.Find(name, prefab) && !forceReload)
		{
			return prefab;
		}

		if (!prefab)
		{
			prefab = new ExpansionPrefab();
			s_Prefabs.Insert(name, prefab);
		}

		prefab.m_Path = EXPANSION_LOADOUT_FOLDER + name + ".json";
		if (FileExist(prefab.m_Path))
		{
			JsonFileLoader<ExpansionPrefab>.JsonLoadFile(prefab.m_Path, prefab);
			return prefab;
		}

		return null;
	}

	override string GetPath()
	{
		return m_Path;
	}

	static string GetPath(string name)
	{
		return EXPANSION_LOADOUT_FOLDER + name + ".json";
	}

	override void Save()
	{
		JsonFileLoader<ExpansionPrefab>.JsonSaveFile(m_Path, this);
	}

	Object Spawn(vector position, vector orientation)
	{
		Object object = GetGame().CreateObjectEx(ClassName, position, ECE_PLACE_ON_SURFACE);
		object.SetOrientation(orientation);
		return Spawn(object);
	}

	Object SpawnOn(Object other)
	{
		return Spawn(other);
	}

	static ExpansionPrefabObject Create(string name)
	{
		s_Begin.Clear();

		ExpansionPrefab prefab = new ExpansionPrefab();

		prefab.m_Path = GetPath(name);

		if (prefab.m_Path == string.Empty)
		{
			return null;
		}

		s_Prefabs.Insert(name, prefab);
		return prefab;
	}
};
