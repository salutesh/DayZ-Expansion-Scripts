class Expansion_Laser_Base extends UniversalLight
{
	private Expansion_Laser_Beam m_Laser;
	
	void Expansion_Laser_Base()
	{
		SetFlags( EntityFlags.ACTIVE | EntityFlags.SOLID | EntityFlags.VISIBLE, false );
		SetEventMask( EntityEvent.FRAME );
	}

	void ~Expansion_Laser_Base()
	{
		if ( m_Laser )
		{
			GetGame().ObjectDelete( m_Laser );
		}
	}
	
	override void OnWorkStart()
	{
		if ( !GetGame().IsServer()  ||  !GetGame().IsMultiplayer() )
		{
			Object laser = GetGame().CreateObject( "Expansion_Laser_Beam", GetMemoryPointPos( "beamStart" ), true, false, true );

			m_Laser = Expansion_Laser_Beam.Cast( laser );
			m_Laser.SetObjectTexture( 0, LaserColor() );
			m_Laser.SetObjectMaterial( 0, LaserMaterial() );
			Print( m_Laser );

			AddChild( m_Laser, -1 );

			SetFlags( EntityFlags.ACTIVE | EntityFlags.SOLID | EntityFlags.VISIBLE, false );

			Update();
		}
	}
	
	override void OnWorkStop()
	{
		if ( !m_Laser )
			return;

		RemoveChild( m_Laser );
			
		GetGame().ObjectDelete( m_Laser );
	}

	override void UpdateLaser()
	{
		if ( m_Laser )
		{
			float distance = GetDistance();

			vector fd = -vector.Direction( GetMemoryPointPos( "beamStart" ), GetMemoryPointPos( "beamEnd" ) ).Normalized();

			vector transform[4] = { 
				Vector( fd[2], fd[1], fd[0] ),
				Vector( 0, 1, 0 ),
				Vector( fd[0], fd[1], fd[2] ) * distance,
				GetMemoryPointPos( "beamStart" )
			};

			m_Laser.SetTransform( transform );
		}
	}

	float GetDistance()
	{
		float distance = 600;

		vector beamStart = ModelToWorld( GetMemoryPointPos( "beamStart" ) );
		vector beamEnd = ModelToWorld( GetMemoryPointPos( "beamEnd" ) );

		vector direction = vector.Direction( beamStart, beamEnd ).Normalized();

		vector from = beamStart;
		vector to = beamStart + ( direction * distance );
		
		vector contact_pos;
		vector contact_dir;
		int contactComponent;
		bool is_collision = DayZPhysics.RaycastRV( from, to, contact_pos, contact_dir, contactComponent, NULL, NULL, GetHierarchyRoot(), false, false, ObjIntersectIFire );
		if ( is_collision )
		{
			return vector.Distance( from, contact_pos );
		}

		return distance;
	}

	override void SetActions()
	{
		super.SetActions();
		
		AddAction( ActionTurnOnWhileInHands );
		AddAction( ActionTurnOffWhileInHands );
	}

	string LaserColor()
	{
		return "#(argb,8,8,3)color(1,0,0,1.0,co)";
	}

	string LaserMaterial()
	{
		return "dz\\weapons\\projectiles\\data\\tracer_red.rvmat";
	}

	vector LaserLightColor()
	{
		return "1.0 0.0 0.0";
	}
}

class Expansion_ANPEQ15_Red extends Expansion_Laser_Base
{
	override string LaserColor()
	{
		return "#(argb,8,8,3)color(1,0,0,1.0,co)";
	}

	override string LaserMaterial()
	{
		return "dz\\weapons\\projectiles\\data\\tracer_red.rvmat";
	}

	override vector LaserLightColor()
	{
		return "1.0 0.0 0.0";
	}
};

class Expansion_ANPEQ15_Green extends Expansion_Laser_Base
{
	override string LaserColor()
	{
		return "#(argb,8,8,3)color(0,0.501961,0,1.0,co)";
	}

	override string LaserMaterial()
	{
		return "DZ\\weapons\\projectiles\\data\\tracer_green.rvmat";
	}
	
	override vector LaserLightColor()
	{
		return "0.0 1.0 0.0";
	}
};