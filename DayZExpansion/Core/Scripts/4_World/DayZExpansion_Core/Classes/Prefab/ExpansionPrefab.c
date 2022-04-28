class ExpansionPrefab : ExpansionPrefabObject
{
	[NonSerialized()]
	static ref array<ExpansionPrefabObject> s_Begin = new array<ExpansionPrefabObject>();

	[NonSerialized()]
	static ref array<CF_String> s_Directories = new array<CF_String>();

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

		foreach (string directory : s_Directories)
		{
			prefab.m_Path = directory + "/" + name + ".json";
			if (FileExist(prefab.m_Path))
			{
				JsonFileLoader<ExpansionPrefab>.JsonLoadFile(prefab.m_Path, prefab);
				return prefab;
			}
		}

		return null;
	}

	string GetPath()
	{
		return m_Path;
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

		foreach (CF_String directory : s_Directories)
		{
			// If the directory doesn't start with '$' then it can't be written into in most cases
			if (directory.IndexOf("$") != 0)
				continue;

			prefab.m_Path = directory + "/" + name + ".json";
		}

		if (prefab.m_Path == string.Empty)
		{
			return null;
		}

		s_Prefabs.Insert(name, prefab);
		return prefab;
	}
};
