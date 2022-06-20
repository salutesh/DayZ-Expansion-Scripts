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

#ifdef EXPANSION_OBSOLETE_CAMERA
modded class DayZPlayerCamera3rdPerson
{
	override void OnUpdate( float pDt, out DayZPlayerCameraResult pOutResult )
	{
#ifdef EXPANSIONTRACE
		auto trace = CF_Trace_1(ExpansionTracing.VEHICLES, this, "OnUpdate").Add(pDt);
#endif

		super.OnUpdate( pDt, pOutResult );
		
		PlayerBase.Cast( m_pPlayer ).SetHeadInvisible_Ex( false );

		ExpansionVehicleBase vehicle;
		if ( Class.CastTo( vehicle, pOutResult.m_CollisionIgnoreEntity ) )
		{
			if ( !dBodyIsDynamic( pOutResult.m_CollisionIgnoreEntity ))
			{
				pOutResult.m_CollisionIgnoreEntity = NULL;
			}
		}
	}
};
#endif
