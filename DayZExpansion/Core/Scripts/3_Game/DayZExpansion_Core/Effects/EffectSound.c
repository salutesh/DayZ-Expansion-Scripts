/**
 * EffectSound.c
 *
 * DayZ Expansion Mod
 * www.dayzexpansion.com
 * © 2023 DayZ Expansion Mod Team
 *
 * This work is licensed under the Creative Commons Attribution-NonCommercial-NoDerivatives 4.0 International License.
 * To view a copy of this license, visit http://creativecommons.org/licenses/by-nc-nd/4.0/.
 *
 */

modded class EffectSound
{
#ifdef DIAG_DEVELOPER
	void ~EffectSound()
	{
		if (!g_Game)
			return;

		if (ExpansionSoundSet.s_RegisteredSoundSets_ByName.Contains(GetSoundSet()))
			EXTrace.Print(EXTrace.SOUND, this, "~EffectSound " + GetSoundSet() + " " + GetPosition() + " " + GetParent());
	}
#endif

	float Expansion_GetSoundFadeInDuration()
	{
		return m_SoundFadeInDuration;
	}

	float Expansion_GetSoundFadeOutDuration()
	{
		return m_SoundFadeOutDuration;
	}

	bool Expansion_GetSoundLoop()
	{
		return m_SoundLoop;
	}
}
