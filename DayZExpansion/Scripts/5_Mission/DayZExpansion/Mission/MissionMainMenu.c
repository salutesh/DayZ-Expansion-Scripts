/**
 * MissionMainMenu.c
 *
 * DayZ Expansion Mod
 * www.dayzexpansion.com
 * © 2022 DayZ Expansion Mod Team
 *
 * This work is licensed under the Creative Commons Attribution-NonCommercial-NoDerivatives 4.0 International License.
 * To view a copy of this license, visit http://creativecommons.org/licenses/by-nc-nd/4.0/.
 *
*/

modded class MissionMainMenu
{
	override void PlayMusic()
	{
		bool ex_PlayExpMusic = true;
		#ifndef NAMALSK_SURVIVAL
			ex_PlayExpMusic = false;
		#endif
		#ifndef FALLUJAH_MAP
			ex_PlayExpMusic = false;
		#endif
		
		if (ex_PlayExpMusic && !m_MenuMusic)
		{
			SoundParams soundParams = new SoundParams("Expansion_Music_2_SoundSet");
			SoundObjectBuilder soundBuilder	= new SoundObjectBuilder(soundParams);
			SoundObject soundObject = soundBuilder.BuildSoundObject();
			soundObject.SetKind(WaveKind.WAVEMUSIC);
			m_MenuMusic = GetGame().GetSoundScene().Play2D(soundObject, soundBuilder);
			m_MenuMusic.Loop(true);
			m_MenuMusic.Play();
		}
		else
		{
			super.PlayMusic();
		}
	}
};
