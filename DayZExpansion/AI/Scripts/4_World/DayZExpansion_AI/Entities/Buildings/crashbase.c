modded class CrashBase
{
	static ref CF_DoublyLinkedNodes_WeakRef<CrashBase> s_Expansion_HeliCrashes = new CF_DoublyLinkedNodes_WeakRef<CrashBase>();

	ref CF_DoublyLinkedNode_WeakRef<CrashBase> m_Expansion_Node;

	//! We are only interested in crashes spawned by events, not random static objs part of the map
	//! Server-only
	override void EEOnCECreate()
	{
		super.EEOnCECreate();

		m_Expansion_Node = s_Expansion_HeliCrashes.Add(this);
	}

	//! We are only interested in crashes spawned by events, not random static objs part of the map
	//! Server-only
	override void AfterStoreLoad()
	{
		super.AfterStoreLoad();

		m_Expansion_Node = s_Expansion_HeliCrashes.Add(this);
	}

	void ~CrashBase()
	{
	#ifndef DIAG_DEVELOPER
		if (!g_Game)
			return;
	#endif

		if (s_Expansion_HeliCrashes)
			s_Expansion_HeliCrashes.Remove(m_Expansion_Node);
	}
}
