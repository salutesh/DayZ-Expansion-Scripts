/**
 * ExpansionSound.c
 *
 * DayZ Expansion Mod
 * www.dayzexpansion.com
 * © 2023 DayZ Expansion Mod Team
 *
 * This work is licensed under the Creative Commons Attribution-NonCommercial-NoDerivatives 4.0 International License.
 * To view a copy of this license, visit http://creativecommons.org/licenses/by-nc-nd/4.0/.
 *
*/

class ExpansionSound
{
	ExpansionSoundSet m_SoundSet;
	vector m_Position;
	Object m_Parent;
	ref EffectSound m_Sound;
	float m_FadeIn;
	float m_FadeOut;
	bool m_Loop;
	bool m_IsLinked;

	void ExpansionSound(notnull ExpansionSoundSet soundSet, vector position, Object parent = null)
	{
		m_SoundSet = soundSet;
		m_Position = position;
		m_Parent = parent;

	#ifndef SERVER
		m_IsLinked = true;
	#endif
	}

	void ~ExpansionSound()
	{
		if (!g_Game)
			return;

		if (m_IsLinked)
			Unlink();

	#ifdef DIAG_DEVELOPER
		EXTrace.Print(EXTrace.SOUND, this, "::~ExpansionSound() " + m_SoundSet.m_Name + " ID " + m_SoundSet.m_ID + " " + m_Position + " " + m_Parent + " " + m_FadeIn + " " + m_FadeOut + " " + m_Loop);
	#endif
	}

	static ExpansionSound Play(string soundSetName, vector position, float fadeIn = -1.0, float fadeOut = -1.0, int loop = -1)
	{
		auto soundSet = ExpansionSoundSet.Get(soundSetName);

		if (!soundSet)
			return null;

		return soundSet.Play(position, fadeIn, fadeOut, loop);
	}

	static ExpansionSound Play(int soundSetID, vector position, float fadeIn = -1.0, float fadeOut = -1.0, int loop = -1)
	{
		auto soundSet = ExpansionSoundSet.Get(soundSetID);

		if (!soundSet)
			return null;

		return soundSet.Play(position, fadeIn, fadeOut, loop);
	}

	static ExpansionSound Play(string soundSetName, notnull Object parent, float fadeIn = -1.0, float fadeOut = -1.0, int loop = -1)
	{
		auto soundSet = ExpansionSoundSet.Get(soundSetName);

		if (!soundSet)
			return null;

		return soundSet.Play(parent, fadeIn, fadeOut, loop);
	}

	static ExpansionSound Play(int soundSetID, notnull Object parent, float fadeIn = -1.0, float fadeOut = -1.0, int loop = -1)
	{
		auto soundSet = ExpansionSoundSet.Get(soundSetID);

		if (!soundSet)
			return null;

		return soundSet.Play(parent, fadeIn, fadeOut, loop);
	}

	void Play(float fadeIn = -1.0, float fadeOut = -1.0, int loop = -1)
	{
	#ifdef EXTRACE_DIAG
		auto trace = EXTrace.Start(EXTrace.SOUND, this, m_SoundSet.m_Name, "ID " + m_SoundSet.m_ID, m_Position.ToString(), "" + m_Parent, fadeIn.ToString(), fadeOut.ToString(), loop.ToString());
	#endif

		if (fadeIn >= 0.0)
			m_FadeIn = fadeIn;

		if (fadeOut >= 0.0)
			m_FadeOut = fadeOut;

		if (loop >= 0)
			m_Loop = loop;

	#ifdef SERVER
		auto rpc = ExpansionSoundModule.s_Instance.Expansion_CreateRPC("RPC_Play");
		WriteParams(rpc);
		rpc.Write(m_FadeIn);
		rpc.Write(m_FadeOut);
		rpc.Write(m_Loop);
		Send(rpc);
	#else
		if (m_Sound)
			m_Sound.Event_OnSoundWaveEnded.Remove(OnStop);

		if (m_Parent)
			m_Sound = SEffectManager.Expansion_PlaySoundOnObject(m_SoundSet.m_Name, m_Parent, m_FadeIn, m_FadeOut, m_Loop);
		else
			m_Sound = SEffectManager.Expansion_PlaySound(m_SoundSet.m_Name, m_Position, m_FadeIn, m_FadeOut, m_Loop);

		m_Sound.Event_OnSoundWaveEnded.Insert(OnStop);
	#endif
	}

	static ExpansionSound Stop(string soundSetName, vector position, float fadeOut = -1.0)
	{
		auto soundSet = ExpansionSoundSet.Get(soundSetName);

		if (!soundSet)
			return null;

		return soundSet.Stop(position, fadeOut);
	}

	static ExpansionSound Stop(int soundSetID, vector position, float fadeOut = -1.0)
	{
		auto soundSet = ExpansionSoundSet.Get(soundSetID);

		if (!soundSet)
			return null;

		return soundSet.Stop(position, fadeOut);
	}

	static ExpansionSound Stop(string soundSetName, notnull Object parent, float fadeOut = -1.0)
	{
		auto soundSet = ExpansionSoundSet.Get(soundSetName);

		if (!soundSet)
			return null;

		return soundSet.Stop(parent, fadeOut);
	}

	static ExpansionSound Stop(int soundSetID, notnull Object parent, float fadeOut = -1.0)
	{
		auto soundSet = ExpansionSoundSet.Get(soundSetID);

		if (!soundSet)
			return null;

		return soundSet.Stop(parent, fadeOut);
	}

	void Stop(float fadeOut = -1.0)
	{
	#ifdef EXTRACE_DIAG
		auto trace = EXTrace.Start(EXTrace.SOUND, this, m_SoundSet.m_Name, "ID " + m_SoundSet.m_ID, m_Position.ToString(), "" + m_Parent, fadeOut.ToString());
	#endif

	#ifdef SERVER
		auto rpc = ExpansionSoundModule.s_Instance.Expansion_CreateRPC("RPC_Stop");
		WriteParams(rpc);
		rpc.Write(fadeOut);
		Send(rpc);
	#else
		if (m_Sound)
		{
			if (fadeOut >= 0)
				m_Sound.SetSoundFadeOut(fadeOut);

			m_Sound.SoundStop();
		}
	#endif
	}

	static ExpansionSound FadeIn(string soundSetName, vector position, float duration = -1.0)
	{
		auto soundSet = ExpansionSoundSet.Get(soundSetName);

		if (!soundSet)
			return null;

		return soundSet.FadeIn(position, duration);
	}

	static ExpansionSound FadeIn(int soundSetID, vector position, float duration = -1.0)
	{
		auto soundSet = ExpansionSoundSet.Get(soundSetID);

		if (!soundSet)
			return null;

		return soundSet.FadeIn(position, duration);
	}

	static ExpansionSound FadeIn(string soundSetName, notnull Object parent, float duration = -1.0)
	{
		auto soundSet = ExpansionSoundSet.Get(soundSetName);

		if (!soundSet)
			return null;

		return soundSet.FadeIn(parent, duration);
	}

	static ExpansionSound FadeIn(int soundSetID, notnull Object parent, float duration = -1.0)
	{
		auto soundSet = ExpansionSoundSet.Get(soundSetID);

		if (!soundSet)
			return null;

		return soundSet.FadeIn(parent, duration);
	}

	void FadeIn(float duration = -1.0)
	{
	#ifdef EXTRACE_DIAG
		auto trace = EXTrace.Start(EXTrace.SOUND, this, m_SoundSet.m_Name, "ID " + m_SoundSet.m_ID, m_Position.ToString(), "" + m_Parent, duration.ToString());
	#endif

	#ifdef SERVER
		auto rpc = ExpansionSoundModule.s_Instance.Expansion_CreateRPC("RPC_FadeIn");
		WriteParams(rpc);
		rpc.Write(duration);
		Send(rpc);
	#else
		if (duration < 0)
			duration = m_FadeIn;

		Play(duration, m_FadeOut, m_Loop);
	#endif
	}

	static ExpansionSound FadeOut(string soundSetName, vector position, float duration = -1.0)
	{
		auto soundSet = ExpansionSoundSet.Get(soundSetName);

		if (!soundSet)
			return null;

		return soundSet.FadeOut(position, duration);
	}

	static ExpansionSound FadeOut(int soundSetID, vector position, float duration = -1.0)
	{
		auto soundSet = ExpansionSoundSet.Get(soundSetID);

		if (!soundSet)
			return null;

		return soundSet.FadeOut(position, duration);
	}

	static ExpansionSound FadeOut(string soundSetName, notnull Object parent, float duration = -1.0)
	{
		auto soundSet = ExpansionSoundSet.Get(soundSetName);

		if (!soundSet)
			return null;

		return soundSet.FadeOut(parent, duration);
	}

	static ExpansionSound FadeOut(int soundSetID, notnull Object parent, float duration = -1.0)
	{
		auto soundSet = ExpansionSoundSet.Get(soundSetID);

		if (!soundSet)
			return null;

		return soundSet.FadeOut(parent, duration);
	}

	void FadeOut(float duration = -1.0)
	{
	#ifdef EXTRACE_DIAG
		auto trace = EXTrace.Start(EXTrace.SOUND, this, m_SoundSet.m_Name, "ID " + m_SoundSet.m_ID, m_Position.ToString(), "" + m_Parent, duration.ToString());
	#endif

	#ifdef SERVER
		auto rpc = ExpansionSoundModule.s_Instance.Expansion_CreateRPC("RPC_FadeOut");
		WriteParams(rpc);
		rpc.Write(duration);
		Send(rpc);
	#else
		if (m_Sound)
		{
			if (duration >= 0)
				m_Sound.SetSoundFadeOut(duration);

			m_Sound.SoundStop();
		}
	#endif
	}

	static ExpansionSound SetVolume(string soundSetName, vector position, float duration)
	{
		auto soundSet = ExpansionSoundSet.Get(soundSetName);

		if (!soundSet)
			return null;

		return soundSet.SetVolume(position, duration);
	}

	static ExpansionSound SetVolume(int soundSetID, vector position, float duration)
	{
		auto soundSet = ExpansionSoundSet.Get(soundSetID);

		if (!soundSet)
			return null;

		return soundSet.SetVolume(position, duration);
	}

	static ExpansionSound SetVolume(string soundSetName, notnull Object parent, float duration)
	{
		auto soundSet = ExpansionSoundSet.Get(soundSetName);

		if (!soundSet)
			return null;

		return soundSet.SetVolume(parent, duration);
	}

	static ExpansionSound SetVolume(int soundSetID, notnull Object parent, float duration)
	{
		auto soundSet = ExpansionSoundSet.Get(soundSetID);

		if (!soundSet)
			return null;

		return soundSet.SetVolume(parent, duration);
	}

	void SetVolume(float volume)
	{
	#ifdef EXTRACE_DIAG
		auto trace = EXTrace.Start(EXTrace.SOUND, this, m_SoundSet.m_Name, "ID " + m_SoundSet.m_ID, m_Position.ToString(), "" + m_Parent, volume.ToString());
	#endif

	#ifdef SERVER
		auto rpc = ExpansionSoundModule.s_Instance.Expansion_CreateRPC("RPC_SetVolume");
		WriteParams(rpc);
		rpc.Write(volume);
		Send(rpc);
	#else
		if (m_Sound)
			m_Sound.SetSoundVolume(volume);
	#endif
	}

	protected void OnStop()
	{
	#ifdef EXTRACE_DIAG
		auto trace = EXTrace.Start(EXTrace.SOUND, this, m_SoundSet.m_Name, "ID " + m_SoundSet.m_ID);
	#endif

		Unlink();

		m_Sound = null;
	}

	void Unlink()
	{
	#ifndef SERVER
		if (m_Parent)
			m_SoundSet.m_ObjectSounds.Remove(m_Parent);
		else
			m_SoundSet.m_PositionalSounds.Remove(m_Position.ToString());
	#endif

		m_IsLinked = false;

	#ifdef DIAG_DEVELOPER
		EXTrace.Print(EXTrace.SOUND, this, "::Unlink()");
	#endif
	}

	protected void WriteParams(ParamsWriteContext ctx, Param params = null)
	{
		ctx.Write(m_SoundSet.m_ID);
		if (!m_Parent)
			ctx.Write(m_Position);
		if (params)
			params.Serialize(ctx);
	}

	protected void Send(ExpansionScriptRPC rpc)
	{
		if (m_Parent)
			GetDayZGame().GetExpansionGame().Expansion_SendNear(rpc, m_Parent.GetPosition(), m_SoundSet.m_Range, m_Parent, true);
		else
			GetDayZGame().GetExpansionGame().Expansion_SendNear(rpc, m_Position, m_SoundSet.m_Range, null, true);
	}
}
