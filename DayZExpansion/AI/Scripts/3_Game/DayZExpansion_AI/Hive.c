Hive Expansion_GlobalGetHive()
{
	#ifdef EXTRACE_DIAG
	auto trace = EXTrace.Start(EXTrace.AI, null);
	#endif

	return GetHive();
}
