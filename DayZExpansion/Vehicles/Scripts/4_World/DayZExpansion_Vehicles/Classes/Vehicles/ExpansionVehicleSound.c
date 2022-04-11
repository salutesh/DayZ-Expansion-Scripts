/**
 * ExpansionVehicleSoundManager.c
 *
 * DayZ Expansion Mod
 * www.dayzexpansion.com
 * © 2022 DayZ Expansion Mod Team
 *
 * This work is licensed under the Creative Commons Attribution-NonCommercial-NoDerivatives 4.0 International License. 
 * To view a copy of this license, visit http://creativecommons.org/licenses/by-nc-nd/4.0/.
 *
*/

class ExpansionVehicleSound
{
	ExpansionVehicleBase m_Vehicle;

	ExpansionVehicleSoundShader m_Shader;

	float m_Volume;
	float m_Frequency;

	string m_SoundSetName;

	ref SoundParams m_SoundParams;
	ref SoundObjectBuilder m_SoundObjectBuilder;
	ref SoundObject m_SoundObject;
	AbstractWave m_AbstractWave;

	void ExpansionVehicleSound(ExpansionVehicleBase vehicle, string soundSetName)
	{
		m_Vehicle = vehicle;
		m_SoundSetName = soundSetName;

		string path;

		array<string> soundShaders = new array<string>;
		path = "CfgSoundSets " + soundSetName + " soundShaders";
		if (GetGame().ConfigIsExisting(path))
			GetGame().ConfigGetTextArray(path, soundShaders);

		if (soundShaders.Count() > 0)
			ExpansionVehicleSoundManager.Get(soundShaders[0], m_Shader);

		path = "CfgSoundSets " + soundSetName + " positionOffset";
		TFloatArray values = new TFloatArray;
		if (GetGame().ConfigIsExisting(path))
			GetGame().ConfigGetFloatArray(path, values);
	}

	bool Update(float pDt, array<float> variables)
	{
		if (!m_SoundParams)
		{
			m_SoundParams = new SoundParams(m_SoundSetName);
			if (!m_SoundParams.IsValid())
				return false;
		}

#ifdef EXPANSION_SOUND_CRASH_IDENTIFY
		Print(m_SoundSetName);
#endif

		if (!m_SoundObjectBuilder)
		{
			m_SoundObjectBuilder = new SoundObjectBuilder(m_SoundParams);
			if (!m_SoundObjectBuilder)
				return false;

			m_SoundObjectBuilder.UpdateEnvSoundControllers(m_Vehicle.GetPosition());

			for (int i = 0; i < ExpansionVehicleSoundManager.s_SoundShaderParameters.Count(); i++)
			{
				m_SoundObjectBuilder.SetVariable(ExpansionVehicleSoundManager.s_SoundShaderParameters[i], variables[i]);
			}
		}

#ifdef EXPANSION_SOUND_CRASH_IDENTIFY
		Print(m_SoundObjectBuilder);
#endif

		if (!m_SoundObject)
		{
			m_SoundObject = m_SoundObjectBuilder.BuildSoundObject();
			if (!m_SoundObject)
				return false;

			m_SoundObject.SetPosition(m_Vehicle.GetPosition());
			m_SoundObject.SetKind(WaveKind.WAVEEFFECTEX);
		}

		/*
		 * Sound object will fail to create - possible cause is a
		 * parameter is defined in the config that is missing from
		 * 's_SoundShaderParameters', or 'playTrigger' is defined
		 * in the sound set.
		 */
#ifdef EXPANSION_SOUND_CRASH_IDENTIFY
		Print(m_SoundObject);
#endif

		if (!m_AbstractWave)
		{
			m_AbstractWave = GetGame().GetSoundScene().Play3D(m_SoundObject, m_SoundObjectBuilder);
			m_AbstractWave.Play();
			m_AbstractWave.Loop(true);

			if (!m_AbstractWave)
				return false;
		}

		m_AbstractWave.SetPosition(m_Vehicle.GetPosition());

		m_Shader.Calculate(variables, m_Volume, m_Frequency);

		m_AbstractWave.SetVolume(m_Volume);
		m_AbstractWave.SetFrequency(m_Frequency);

		return true;
	}

#ifdef CF_DebugUI
	bool CF_OnDebugUpdate(CF_Debug instance, CF_DebugUI_Type type)
	{
		instance.Add("Sound Set", m_SoundSetName);

		instance.Add("Volume", m_Volume);
		instance.Add("Frequency", m_Frequency);

		return true;
	}
#endif
};
