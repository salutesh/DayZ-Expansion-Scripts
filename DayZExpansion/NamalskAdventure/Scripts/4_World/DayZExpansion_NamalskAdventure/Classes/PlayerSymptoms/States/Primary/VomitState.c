/**
 * VomitState.c
 *
 * DayZ Expansion Mod
 * www.dayzexpansion.com
 * © 2023 DayZ Expansion Mod Team
 *
 * This work is licensed under the Creative Commons Attribution-NonCommercial-NoDerivatives 4.0 International License.
 * To view a copy of this license, visit http://creativecommons.org/licenses/by-nc-nd/4.0/.
 *
*/

modded class VomitSymptom
{
	override void OnAnimationStart()
	{
		super.OnAnimationStart();
		if(m_Player)
		{
			m_Player.GetStatRadiation().Add(-10.0);
			m_Player.ForceUpdateRadiation();
		}
		//Print("------------ vomit start -------------");
	}
}