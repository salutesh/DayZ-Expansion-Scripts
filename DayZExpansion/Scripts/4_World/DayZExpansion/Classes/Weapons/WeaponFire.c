/**
 * WeaponFire.c
 *
 * DayZ Expansion Mod
 * www.dayzexpansion.com
 * © 2021 DayZ Expansion Mod Team
 *
 * This work is licensed under the Creative Commons Attribution-NonCommercial-NoDerivatives 4.0 International License.
 * To view a copy of this license, visit http://creativecommons.org/licenses/by-nc-nd/4.0/.
 *
*/

#ifdef EXPANSION_WEAPON_BULLET_TRACE
modded class WeaponFire
{
	override void OnEntry( WeaponEventBase e )
	{
		super.OnEntry( e );

		array<Object> collided();
		array<CargoBase> proxyCargos();

		GetGame().GetObjectsAtPosition3D( m_weapon.GetPosition(), 2.5, collided, proxyCargos );

		for ( int i = 0; i < collided.Count(); ++i )
			if ( GetGame().ConfigIsExisting( "CfgAmmo " + collided[i].GetType() ) )
				ExpansionWorld.m_bullets.Insert( new ExpansionBulletTrace( collided[i] ) );
	}
};
#endif