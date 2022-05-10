class ExpansionNavMeshes
{
	static ExpansionNavMesh Get(Object object)
	{
		return ExpansionModelInfo.Get(object).NavMesh;
	}
};
