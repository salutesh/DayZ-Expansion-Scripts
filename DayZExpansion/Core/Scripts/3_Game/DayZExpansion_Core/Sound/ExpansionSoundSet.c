/**
 * ExpansionSoundSet.c
 *
 * DayZ Expansion Mod
 * www.dayzexpansion.com
 * © 2023 DayZ Expansion Mod Team
 *
 * This work is licensed under the Creative Commons Attribution-NonCommercial-NoDerivatives 4.0 International License.
 * To view a copy of this license, visit http://creativecommons.org/licenses/by-nc-nd/4.0/.
 *
*/

class ExpansionSoundSet
{
	static ref map<int, ref ExpansionSoundSet> s_RegisteredSoundSets_ByID = new map<int, ref ExpansionSoundSet>;
	static ref map<string, ExpansionSoundSet> s_RegisteredSoundSets_ByName = new map<string, ExpansionSoundSet>;

	string m_Name;
	int m_ID;
	float m_Range;

#ifndef SERVER
	ref map<string, ref ExpansionSound> m_PositionalSounds = new map<string, ref ExpansionSound>;
	ref map<Object, ref ExpansionSound> m_ObjectSounds = new map<Object, ref ExpansionSound>;
#endif

	void ExpansionSoundSet(string name)
	{
		m_Name = name;
		m_ID = name.Hash();

		TStringArray soundShaders = {};
		g_Game.ConfigGetTextArray("CfgSoundSets " + name + " soundShaders", soundShaders);
		foreach (string soundShader: soundShaders)
		{
			m_Range = Math.Max(g_Game.ConfigGetFloat("CfgSoundShaders " + soundShader + " range"), m_Range);
		}

	#ifdef DIAG_DEVELOPER
		EXTrace.Print(EXTrace.SOUND, this, name + " ID " + m_ID + " range " + m_Range);
	#endif
	}

	static ExpansionSoundSet Register(string name)
	{
		ExpansionSoundSet soundSet;

		if (!s_RegisteredSoundSets_ByName.Find(name, soundSet))
		{
			soundSet = new ExpansionSoundSet(name);
			s_RegisteredSoundSets_ByID[soundSet.m_ID] = soundSet;
			s_RegisteredSoundSets_ByName[name] = soundSet;
		}

		return soundSet;
	}

	static ExpansionSoundSet Get(string name)
	{
		auto soundSet = s_RegisteredSoundSets_ByName[name];

		if (!soundSet)
			Error("Not a registered soundset: " + name);

		return soundSet;
	}

	static ExpansionSoundSet Get(int soundSetID)
	{
		auto soundSet = s_RegisteredSoundSets_ByID[soundSetID];

		if (!soundSet)
			Error("Not a registered soundset ID: " + soundSetID);

		return soundSet;
	}

	ExpansionSound GetSound(vector position, bool create = true)
	{
		ExpansionSound sound;

	#ifndef SERVER
		if (!m_PositionalSounds.Find(position.ToString(), sound) && create)
		{
	#endif
			sound = new ExpansionSound(this, position);
	#ifndef SERVER
			m_PositionalSounds[position.ToString()] = sound;
		}
	#endif

		return sound;
	}

	ExpansionSound GetSound(Object parent, bool create = true)
	{
		ExpansionSound sound;

	#ifndef SERVER
		if (!m_ObjectSounds.Find(parent, sound) && create)
		{
	#endif
			sound = new ExpansionSound(this, vector.Zero, parent);
	#ifndef SERVER
			m_ObjectSounds[parent] = sound;
		}
	#endif

		return sound;
	}

	ExpansionSound Play(vector position, float fadeIn = -1.0, float fadeOut = -1.0, int loop = -1)
	{
		ExpansionSound sound = GetSound(position);

		sound.Play(fadeIn, fadeOut, loop);

		return sound;
	}

	ExpansionSound Play(notnull Object parent, float fadeIn = -1.0, float fadeOut = -1.0, int loop = -1)
	{
		ExpansionSound sound = GetSound(parent);

		sound.Play(fadeIn, fadeOut, loop);

		return sound;
	}

	ExpansionSound Stop(vector position, float fadeOut = -1.0)
	{
		ExpansionSound sound = GetSound(position, false);

		if (sound)
			sound.Stop(fadeOut);

		return sound;
	}

	ExpansionSound Stop(notnull Object parent, float fadeOut = -1.0)
	{
		ExpansionSound sound = GetSound(parent, false);

		if (sound)
			sound.Stop(fadeOut);

		return sound;
	}

	ExpansionSound FadeIn(vector position, float duration = -1.0)
	{
		ExpansionSound sound = GetSound(position);

		sound.FadeIn(duration);

		return sound;
	}

	ExpansionSound FadeIn(notnull Object parent, float duration = -1.0)
	{
		ExpansionSound sound = GetSound(parent);

		sound.FadeIn(duration);

		return sound;
	}

	ExpansionSound FadeOut(vector position, float duration = -1.0)
	{
		ExpansionSound sound = GetSound(position, false);

		if (sound)
			sound.FadeOut(duration);

		return sound;
	}

	ExpansionSound FadeOut(notnull Object parent, float duration = -1.0)
	{
		ExpansionSound sound = GetSound(parent, false);

		if (sound)
			sound.FadeOut(duration);

		return sound;
	}

	ExpansionSound SetVolume(vector position, float volume)
	{
		ExpansionSound sound = GetSound(position, false);

		if (sound)
			sound.SetVolume(volume);

		return sound;
	}

	ExpansionSound SetVolume(notnull Object parent, float volume)
	{
		ExpansionSound sound = GetSound(parent, false);

		if (sound)
			sound.SetVolume(volume);

		return sound;
	}
}
