class eAIRegisterDynamicPatrolSpawner
{
	static ref TTypenameArray s_RegisteredTypes = {};

	void eAIRegisterDynamicPatrolSpawner(typename type)
	{
		s_RegisteredTypes.Insert(type);
	}
}
