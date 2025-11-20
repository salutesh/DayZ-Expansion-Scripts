#ifndef DAYZ_1_28
//! 1.29+
modded class EmoteDabbing
{
	void EmoteDabbing()
	{
		m_StanceMaskAdditive = DayZPlayerConstants.STANCEMASK_CROUCH | DayZPlayerConstants.STANCEMASK_ERECT;
		m_StanceMaskFullbody = 0;
		m_AdditiveCallbackUID = DayZPlayerConstants.CMD_GESTUREMOD_DABBING;
		m_FullbodyCallbackUID = 0;
	}
}
#endif
