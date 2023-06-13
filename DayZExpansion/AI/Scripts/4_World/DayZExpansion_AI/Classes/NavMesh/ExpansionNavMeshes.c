class ExpansionNavMeshes
{
	static ExpansionNavMesh Get(Object object)
	{
		ExpansionModelInfo info = ExpansionModelInfo.Get(object);

		if (info)
			return info.NavMesh;

		return null;
	}
};
