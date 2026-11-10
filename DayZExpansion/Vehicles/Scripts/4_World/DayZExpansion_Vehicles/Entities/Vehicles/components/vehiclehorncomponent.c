modded class VehicleHornComponent
{
	protected bool m_Expansion_IsInterruptDisabled;
	protected float m_Expansion_FadeIn;
	protected float m_Expansion_FadeOut;

	void Expansion_DisableInterrupt()
	{
		m_Expansion_IsInterruptDisabled = true;
	}

	void Expansion_SetFade(float fadeIn, float fadeOut = 0)
	{
		m_Expansion_FadeIn = fadeIn;
		m_Expansion_FadeOut = fadeOut;
	}

	bool Expansion_HasSound(int mode)
	{
		return m_SoundToModeMap.Contains(mode);
	}

	bool Expansion_IsPlaying()
	{
		if (m_SoundEffect)
			return m_SoundEffect.IsPlaying();

		return false;
	}

	override void HandleSound()
	{
		if (m_Expansion_IsInterruptDisabled)
		{
			if (!Expansion_IsPlaying() && Expansion_HasSound(m_Mode))
				m_Parent.PlaySoundSet(m_SoundEffect, m_SoundToModeMap[m_Mode], m_Expansion_FadeIn, m_Expansion_FadeOut, false);
		}
		else super.HandleSound();
	}
}
