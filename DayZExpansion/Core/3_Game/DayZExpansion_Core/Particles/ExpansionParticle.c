/**
 * ExpansionParticle.c
 *
 * DayZ Expansion Mod
 * www.dayzexpansion.com
 * © 2021 DayZ Expansion Mod Team
 *
 * This work is licensed under the Creative Commons Attribution-NonCommercial-NoDerivatives 4.0 International License. 
 * To view a copy of this license, visit http://creativecommons.org/licenses/by-nc-nd/4.0/.
 *
*/

modded class EffectParticle
{
	override void AttachTo(Object obj, vector local_pos = "0 0 0", vector local_ori = "0 0 0", bool force_rotation_to_world = false)
	{
		if ( GetParticle() )
		{
			if ( obj )
			{
				super.AttachTo(obj, local_pos, local_ori, force_rotation_to_world);
			}
		}
	}
};