/**
 * ExpansionDynamicSound.c
 *
 * DayZ Expansion Mod
 * www.dayzexpansion.com
 * © 2026 DayZ Expansion Mod Team
 *
 * This work is licensed under the Creative Commons Attribution-NonCommercial-NoDerivatives 4.0 International License. 
 * To view a copy of this license, visit http://creativecommons.org/licenses/by-nc-nd/4.0/.
 *
*/

class ExpansionDynamicSound
{
	string m_SoundSetName;

	ref SoundParams m_SoundParams;
	ref SoundObjectBuilder m_SoundObjectBuilder;
	ref SoundObject m_SoundObject;
	AbstractWave m_AbstractWave;

	void ExpansionDynamicSound(string soundSetName)
	{
		m_SoundSetName = soundSetName;

		m_SoundParams = new SoundParams(m_SoundSetName);

		if (!m_SoundParams.IsValid())
			EXError.ErrorOnce(this, "Invalid soundset " + m_SoundSetName);
		else
			m_SoundObjectBuilder = new SoundObjectBuilder(m_SoundParams);
	}

	void ~ExpansionDynamicSound()
	{
		if (m_AbstractWave)
			m_AbstractWave.Stop();
	}

	bool Update(float pDt, vector position, float frequency = 1.0, float volume = 1.0)
	{
		if (!m_SoundObjectBuilder)
			return false;

		m_SoundObjectBuilder.AddEnvSoundVariables(position);

		if (!m_SoundObject)
		{
			m_SoundObject = m_SoundObjectBuilder.BuildSoundObject();

			if (!m_SoundObject)
			{
				EXError.ErrorOnce(this, "Couldn't build SoundObject for soundset " + m_SoundSetName);
				return false;
			}

			m_SoundObject.SetKind(WaveKind.WAVEEFFECTEX);
			m_SoundObject.SetPosition(position);
		}

		if (!m_AbstractWave)
		{
			m_AbstractWave = g_Game.GetSoundScene().Play3D(m_SoundObject, m_SoundObjectBuilder);

			if (!m_AbstractWave)
			{
				EXError.ErrorOnce(this, "Couldn't play soundset " + m_SoundSetName);
				return false;
			}

			m_AbstractWave.Play();
			m_AbstractWave.Loop(true);
		}

		m_AbstractWave.SetPosition(position);
		m_AbstractWave.SetFrequency(frequency);
		m_AbstractWave.SetVolumeRelative(volume);

		return true;
	}
}
