class ExpansionWeaponFireFlareGun: ExpansionWeaponFireBase
{
	override void FireServer( Weapon_Base weapon, int muzzleIndex, DayZPlayerImplement player, vector pos, vector dir )
	{
		ExpansionRoadflare flare;
		if ( Class.CastTo( flare, GetGame().CreateObject( "ExpansionRoadflare", pos ) ) )
		{
			flare.SetDirection( dir );
			flare.SetPosition( pos );

			flare.CreateDynamicPhysics( PhxInteractionLayers.DYNAMICITEM );
			flare.EnableDynamicCCD( true );

			vector velocity = dir * 300.0;
			float mass = dBodyGetMass( flare );

			SetVelocity( flare, velocity );
			dBodyApplyForce( flare, velocity * mass );
		}
	}
	
	override void FireClient( Weapon_Base weapon, int muzzleIndex, DayZPlayerImplement player, vector pos, vector dir )
	{
	}
}