/**
 * Particle.c
 *
 * DayZ Expansion Mod
 * www.dayzexpansion.com
 * © 2021 DayZ Expansion Mod Team
 *
 * This work is licensed under the Creative Commons Attribution-NonCommercial-NoDerivatives 4.0 International License.
 * To view a copy of this license, visit http://creativecommons.org/licenses/by-nc-nd/4.0/.
 *
*/

modded class Particle
{
    //! Hack fix for heli smoke when it shouldn't until we have a better fix https://exp.thurston.pw/T524
	bool IsPlaying()
	{
		return m_IsPlaying;
	}
}