class ExpansionModelInfo
{
	static autoptr map<string, ref ExpansionModelInfo> s_All = new map<string, ref ExpansionModelInfo>();

	static ExpansionModelInfo Get(Object object)
	{
		//Clear();
		
		if (!object)
		{
			return null;
		}

		CF_String modelName = object.ConfigGetString("model");
		if (modelName == string.Empty)
		{
			modelName = object.GetDebugNameNative();
			
			int index = modelName.IndexOf(":") + 2;
			int length = modelName.Length() - index;
			if (index < 0 || length < 3)
			{
				return null;
			}
			
			modelName = modelName.Substring(index, length);
		}

		ExpansionModelInfo modelInfo;
		if (!s_All.Find(modelName, modelInfo))
		{
			modelInfo = new ExpansionModelInfo(object);
			s_All.Insert(modelName, modelInfo);
		}

		return modelInfo;
	}

	static void Clear()
	{
		s_All.Clear();
	}

	bool CanClimb = true;
	ref ExpansionNavMesh NavMesh = null;
	ref array<LOD> LODs = new array<LOD>();

	void ExpansionModelInfo(Object object)
	{
		object.GetLODS(LODs);
											
		foreach (auto lod : LODs)
		{
			if (!lod)
				continue;

			string lodName = lod.GetName(object);
			//Print("" + lodName + ": " + lod.GetPropertyCount());

			for (int property = 0; property < lod.GetPropertyCount(); property++)
			{
				string propertyName = lod.GetPropertyName(property);
				propertyName.ToLower();
				
				//Print(" " + propertyName + ": " + lod.GetPropertyValue(property));
				if (propertyName == "canclimb")
				{
					CanClimb = lod.GetPropertyValue(property) == "1";
				}
			}
			
			if (lodName == "paths")
			{
				NavMesh = new ExpansionNavMesh();
				NavMesh.Generate(object, lod);

				if (!NavMesh.IsValid())
				{
					NavMesh = null;
				}
			}
		}
	}
};
