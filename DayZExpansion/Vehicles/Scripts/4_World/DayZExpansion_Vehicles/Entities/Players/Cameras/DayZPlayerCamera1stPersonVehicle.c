/**
 * DayZPlayerCamera1stPerson.c
 *
 * DayZ Expansion Mod
 * www.dayzexpansion.com
 * © 2022 DayZ Expansion Mod Team
 *
 * This work is licensed under the Creative Commons Attribution-NonCommercial-NoDerivatives 4.0 International License.
 * To view a copy of this license, visit http://creativecommons.org/licenses/by-nc-nd/4.0/.
 *
*/

#ifndef EXPANSION_OBSOLTE_CAMERA
modded class DayZPlayerCamera1stPersonVehicle
{
	override void OnUpdate(float pDt, out DayZPlayerCameraResult pOutResult)
	{
		super.OnUpdate(pDt, pOutResult);

		if (m_Ex_Player.GetCommand_ExpansionVehicle())
		{
			ForceFreelook(true);
		}
	}
};
#endif
