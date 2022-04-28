class ExpansionNavMeshes
{
	static autoptr array<ref ExpansionNavMesh> m_Navmeshes = new array<ref ExpansionNavMesh>();
	static autoptr map<string, ExpansionNavMesh> m_Objects = new map<string, ExpansionNavMesh>();

	static ExpansionNavMesh Get(Object object)
	{
		if (!object)
		{
			return null;
		}

		string modelName = object.ConfigGetString("model");

		ExpansionNavMesh navMesh;
		if (!m_Objects.Find(modelName, navMesh))
		{
			navMesh = new ExpansionNavMesh();
			
			array<LOD> lods();
			object.GetLODS(lods);
												
			foreach (auto lod : lods)
			{
				string lodName = lod.GetName(object);
				
				if (lodName != "paths") continue;
				
				navMesh.Generate(lod);
				break;
			}
			
			m_Navmeshes.Insert(navMesh);
			m_Objects.Insert(modelName, navMesh);
		}

		return navMesh;
	}
};
