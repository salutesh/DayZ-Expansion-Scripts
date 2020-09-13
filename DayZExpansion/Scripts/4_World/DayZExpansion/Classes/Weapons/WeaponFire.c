modded class WeaponFire
{
	override void OnEntry( WeaponEventBase e )
	{
		super.OnEntry( e );

		#ifdef EXPANSION_WEAPON_BULLET_TRACE
		array<Object> collided();
		array<CargoBase> proxyCargos();

		GetGame().GetObjectsAtPosition3D( m_weapon.GetPosition(), 2.5, collided, proxyCargos );

		for ( int i = 0; i < collided.Count(); ++i )
			if ( GetGame().ConfigIsExisting( "CfgAmmo " + collided[i].GetType() ) )
				ExpansionWorld.m_bullets.Insert( new ExpansionBulletTrace( collided[i] ) );
		#endif
	}
};