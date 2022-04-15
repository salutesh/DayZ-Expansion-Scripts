/**
 * DayZPlayerCamera3rdPerson.c
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
modded class DayZPlayerCamera3rdPerson
{
	override void OnUpdate(float pDt, out DayZPlayerCameraResult pOutResult)
	{
		super.OnUpdate(pDt, pOutResult);
		
		Ex_OnUpdate(pDt, pOutResult);

		ExpansionVehicleBase vehicle;
		if (Class.CastTo(vehicle, pOutResult.m_CollisionIgnoreEntity))
		{
			if (!dBodyIsDynamic(pOutResult.m_CollisionIgnoreEntity))
			{
				pOutResult.m_CollisionIgnoreEntity = NULL;
			}
		}

		Ex_OnPostUpdate(pDt, pOutResult);
	}

	override void Ex_OnUpdate(float pDt, out DayZPlayerCameraResult pOutResult)
	{
		super.Ex_OnUpdate(pDt, pOutResult);

		if (m_Ex_Player.IsAttached())
		{
			pOutResult.m_CollisionIgnoreEntity = m_pPlayer.GetParent();
			pOutResult.m_fIgnoreParentRoll = 1.0;
			pOutResult.m_fIgnoreParentPitch = 1.0;
		}
	}
};
#endif
