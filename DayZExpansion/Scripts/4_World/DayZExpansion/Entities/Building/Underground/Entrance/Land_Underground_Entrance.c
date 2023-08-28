modded class Land_Underground_Entrance
{
	static bool m_Expansion_IsIntroScene;

	override void RequestLatentTransition(float time, EUndegroundEntranceState targetState = EUndegroundEntranceState.UNINITIALIZED)
	{
		//! If Expansion intro scene, keep door open
		if (!m_Expansion_IsIntroScene || m_DoorState != EUndegroundEntranceState.OPENING_F)
			super.RequestLatentTransition(time, targetState);
	}
}
