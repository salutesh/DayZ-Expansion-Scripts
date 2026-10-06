#ifndef DAYZ_1_29
#ifndef SERVER
modded class Land_Underground_EntranceBase
{
	//! Vanilla calls UpdatePathgraphDoorByAnimationSourceName in OnUpdateServer which crashes on client under 1.30 Experimental
	override void OnUpdateServer()
	{
		m_AnimPhase = m_AnimTimerDoorServer.GetValue() / AdjustTime(GetOpeningTime());// make 0..1
		SetAnimationPhaseNow("EntranceDoor",m_AnimPhase);
	}
}
#endif
#endif

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
