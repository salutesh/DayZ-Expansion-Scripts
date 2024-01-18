/**
 * SEffectManager.c
 *
 * DayZ Expansion Mod
 * www.dayzexpansion.com
 * © 2022 DayZ Expansion Mod Team
 *
 * This work is licensed under the Creative Commons Attribution-NonCommercial-NoDerivatives 4.0 International License.
 * To view a copy of this license, visit http://creativecommons.org/licenses/by-nc-nd/4.0/.
 *
 */

modded class SEffectManager
{
	//! @brief Like PlaySound, but allows passing in autodestroy parameter
	static EffectSound Expansion_PlaySound(string sound_set, vector position, float play_fade_in = 0, float stop_fade_out = 0, bool loop = false, bool autodestroy = true)
	{
		EffectSound effect_sound = CreateSound(sound_set, position, play_fade_in, stop_fade_out, loop, false);

		effect_sound.SetAutodestroy(autodestroy);
		effect_sound.SoundPlay();
		
		return effect_sound;
	}

	//! @brief Like PlaySoundOnObject, but allows passing in autodestroy parameter
	static EffectSound Expansion_PlaySoundOnObject(string sound_set, Object parent_object, float play_fade_in = 0, float stop_fade_out = 0, bool loop = false, bool autodestroy = true)
	{
		EffectSound effect_sound = CreateSound(sound_set, parent_object.GetPosition(), play_fade_in, stop_fade_out, loop);
		
		effect_sound.SetParent(parent_object);
		effect_sound.SetLocalPosition(vector.Zero);
		effect_sound.SetAutodestroy(autodestroy);
		effect_sound.SoundPlay();
		
		return effect_sound;
	}
}
