/**
 * ExpansionEngineStartSounds.c
 *
 * DayZ Expansion Mod
 * www.dayzexpansion.com
 * © 2023 DayZ Expansion Mod Team
 *
 * This work is licensed under the Creative Commons Attribution-NonCommercial-NoDerivatives 4.0 International License.
 * To view a copy of this license, visit http://creativecommons.org/licenses/by-nc-nd/4.0/.
 *
 */

class ExpansionEngineStartSound
{
	CarScript m_Vehicle;
	float m_StartDelay;
	float m_StopDelay;
	string m_SoundSet;
	float m_FadeIn;
	float m_FadeOut;
	bool m_Loop;
	float m_Volume;

	void ExpansionEngineStartSound(CarScript vehicle, float startDelay, float stopDelay, string soundSet, float fadeIn = 0, float fadeOut = 0, bool loop = false, float volume = 0.0)
	{
		m_Vehicle = vehicle;
		m_StartDelay = startDelay;
		m_StopDelay = stopDelay;
		m_SoundSet = soundSet;
		m_FadeIn = fadeIn;
		m_FadeOut = fadeOut;
		m_Loop = loop;
		m_Volume = volume;
	}

	void PlayAfterStartDelay()
	{
		GetGame().GetCallQueue(CALL_CATEGORY_SYSTEM).CallLater(Play, m_StartDelay * 1000);
	}

	void Play()
	{
		if (m_Vehicle && (m_StartDelay == 0 || m_Vehicle.Expansion_EngineIsOn()))
		{
			Man player = GetGame().GetPlayer();

			string soundset = m_SoundSet;

			if (!soundset.Contains("_SoundSet"))
			{
				if (player.IsCameraInsideVehicle())
					soundset += "_Int_SoundSet";
				else
					soundset += "_Ext_SoundSet";
			}

			vector position = m_Vehicle.ModelToWorld(m_Vehicle.GetEnginePos());

			EffectSound sound = SEffectManager.CreateSound(soundset, position, m_FadeIn, m_FadeOut, m_Loop, false);

			if (m_Volume > 0.0)
				sound.SetSoundMaxVolume(m_Volume);

			sound.SoundPlay();
			if (m_StopDelay > 0)
				GetGame().GetCallQueue(CALL_CATEGORY_SYSTEM).CallLater(sound.Stop, m_StopDelay * 1000);
		}
	}
}

class ExpansionEngineStartSounds
{
	CarScript m_Vehicle;
	ref array<ref ExpansionEngineStartSound> m_Sounds = {};

	void ExpansionEngineStartSounds(CarScript vehicle)
	{
		m_Vehicle = vehicle;
	}

	void Clear()
	{
		m_Sounds.Clear();
	}

	int Count()
	{
		return m_Sounds.Count();
	}

	void Insert(float startDelay, float stopDelay, string soundSet, float fadeIn = 0, float fadeOut = 0, bool loop = false, float volume = 0.0)
	{
		m_Sounds.Insert(new ExpansionEngineStartSound(m_Vehicle, startDelay, stopDelay, soundSet, fadeIn, fadeOut, loop, volume));
	}

	void Play()
	{
		foreach (ExpansionEngineStartSound sound: m_Sounds)
		{
			sound.PlayAfterStartDelay();
		}
	}
}
