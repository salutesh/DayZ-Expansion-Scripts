class ExpansionWeaponFireRocket: ExpansionWeaponFireBase
{
	override void FireServer( Weapon_Base weapon, int muzzleIndex, DayZPlayerImplement player, vector pos, vector dir )
	{
		ExpansionRocket rocket;
		if ( Class.CastTo( rocket, GetGame().CreateObject( "ExpansionRocket", pos ) ) )
		{
			rocket.SetDirection( dir );
			rocket.SetPosition( pos );

			rocket.CreateDynamicPhysics( PhxInteractionLayers.DYNAMICITEM );
			rocket.EnableDynamicCCD( true );

			vector velocity = dir * 70.0;
			float mass = dBodyGetMass( rocket );

			SetVelocity( rocket, velocity );
			dBodyApplyForce( rocket, velocity * mass );
		}
	}
	
	override void FireClient( Weapon_Base weapon, int muzzleIndex, DayZPlayerImplement player, vector pos, vector dir )
	{
	}
}