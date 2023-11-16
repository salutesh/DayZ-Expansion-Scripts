Hive Expansion_GlobalGetHive()
{
	#ifdef DIAG
	auto trace = EXTrace.Start(EXTrace.AI, null);
	#endif

	return GetHive();
}
