class ExpansionPrefab : ExpansionPrefabObject
{
	[NonSerialized()]
	static ref array<ExpansionPrefabObject> s_Begin = new array<ExpansionPrefabObject>();

	[NonSerialized()]
	static ref map<string, ref ExpansionPrefab> s_Prefabs = new map<string, ref ExpansionPrefab>();

	[NonSerialized()]
	string m_Path;

	static ExpansionPrefab Load(string name, bool forceReload = false, string folder = EXPANSION_LOADOUT_FOLDER)
	{
	#ifdef EXTRACE_DIAG
		auto trace = EXTrace.Start(EXTrace.LOADOUTS, ExpansionPrefab);
	#endif

		string path = GetPath(name, folder);

		ExpansionPrefab prefab;
		if (s_Prefabs.Find(path, prefab) && !forceReload)
		{
			return prefab;
		}

		if (!prefab)
		{
			prefab = new ExpansionPrefab();
			prefab.m_Name = name;
			prefab.m_Path = path;
			Insert(prefab);
		}

		if (FileExist(prefab.m_Path))
		{
			JsonFileLoader<ExpansionPrefab>.JsonLoadFile(prefab.m_Path, prefab);
			return prefab;
		}

		return null;
	}

	protected static void Insert(ExpansionPrefab prefab)
	{
		s_Prefabs.Insert(prefab.m_Path, prefab);
	}

	override string GetPath()
	{
		return m_Path;
	}

	static string GetPath(string name, string folder = EXPANSION_LOADOUT_FOLDER)
	{
		return folder + name + ".json";
	}

	override void Save()
	{
		JsonFileLoader<ExpansionPrefab>.JsonSaveFile(m_Path, this);
	}

	Object Spawn(vector position, vector orientation = vector.Zero, int rotationFlags = RF_DEFAULT)
	{
		int flags = ECE_PLACE_ON_SURFACE;
		if (g_Game.IsKindOf(ClassName, "DZ_LightAI"))
			flags |= ECE_INITAI;

		Object object = g_Game.CreateObjectEx(ClassName, position, flags, rotationFlags);

		if (orientation != vector.Zero)
			object.SetOrientation(orientation);

		return Spawn(object);
	}

	Object SpawnOn(Object other)
	{
		return Spawn(other);
	}

	static ExpansionPrefabObject Create(string name, string folder = EXPANSION_LOADOUT_FOLDER)
	{
		s_Begin.Clear();

		ExpansionPrefab prefab = new ExpansionPrefab();

		prefab.m_Path = GetPath(name, folder);

		if (prefab.m_Path == string.Empty)
		{
			return null;
		}

		Insert(prefab);
		return prefab;
	}
};
