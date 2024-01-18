modded class EffectSound
{
	override bool CanDestroy()
	{
		if (super.CanDestroy())
			return true;

		return !m_SoundWaveIsPlaying;
	}
}
