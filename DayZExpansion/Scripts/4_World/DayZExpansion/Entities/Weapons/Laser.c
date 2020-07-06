class Expansion_Laser_Base extends UniversalLight
{
	//Particle p;
	EntityAI p;
	Particle p1;
	Particle p2;
	Particle p3;
	protected ref Timer 	m_Timer;
	//LaserLight m_Light;
	bool createLight = false;
	string LaserColor()
	{
		return "#(argb,8,8,3)color(1,0,0,1.0,co)";
	}
	string LaserMaterial()
	{
		return "dz\\weapons\\projectiles\\data\\tracer_red.rvmat";
	}
	
	void Expansion_Laser_Base()
	{
		SetEventMask(EntityEvent.SIMULATE);
	}
	
	override void OnWorkStart()
	{
		if ( !GetGame().IsServer()  ||  !GetGame().IsMultiplayer() ) // Client side
		{
			//StartPeriodicMeasurement();
/* 			m_Light = LaserLight.Cast( ScriptedLightBase.CreateLight( LaserLight, "0 0 0") );
			m_Light.SetColorToRed(); */
		}
	}
	void ScaleLaser( Object obj, float scale )
    {
        vector currTrans[4];
        vector newTrans[4];

        obj.GetTransform( currTrans );
        vector scaleMatrix[4] = { 
            Vector( scale, 0, 0 ), 
            Vector( 0, scale, 0 ), 
            Vector( 0, 0, scale ), 
            "0 0 0 1" };

        Math3D.MatrixMultiply4( scaleMatrix, currTrans, newTrans );

        newTrans[3] = currTrans[3];

        obj.SetTransform( newTrans );
    }
	void StartPeriodicMeasurement()
	{
		if( !m_Timer )
		{
			m_Timer = new Timer;
		}
		m_Timer.Run( 0.01, this, "PrepareMeasurement", null, true );
	}	
	void PrepareMeasurement()
	{
		DoMeasurement();
		DoMeasurement();
	}
	
	void DoMeasurement()
	{
	}
	
	void StopPeriodicMeasurement()
	{
		if( m_Timer )
		{
			m_Timer.Stop();
		}
	}
	override void EOnSimulate(IEntity other, float dt)
	{
		if (GetCompEM().IsSwitchedOn())
		{
			vector 		from 			= ModelToWorld( GetMemoryPointPos("beamStart") );
			vector 		ori 			= GetOrientation() + Vector(90,0,0);
			vector 		to 				= ModelToWorld(GetMemoryPointPos("beamEnd") + "600 0 0" );
			vector 		contact_pos;
			vector 		contact_dir;
			int 		contactComponent;	
			bool is_collision = DayZPhysics.RaycastRV( from, to, contact_pos, contact_dir, contactComponent, NULL , NULL, GetHierarchyRootPlayer(), false, false, ObjIntersectIFire);
			if (is_collision)
			{
				float hitDistance = vector.Distance(contact_pos, from);
				// Generate result
				//m_Light.SetPosition(contact_pos);
				//..EntityAI LaserOBJ = this.FindAttachmentBySlotName("expansionLaserBeam");
				//ScaleLaser( LaserOBJ, 10);
				if (hitDistance < 6)
				{
					p = EntityAI.Cast(GetGame().CreateObject("Expansion_Laser_Impact1", contact_pos, true, false, true));
					p.SetObjectTexture( 0, LaserColor() );
					p.SetObjectMaterial( 0, LaserMaterial() );
				}
				if (hitDistance > 6 && hitDistance < 15)
				{
					p = EntityAI.Cast(GetGame().CreateObject("Expansion_Laser_Impact2", contact_pos, true, false, true));
					p.SetObjectTexture( 0, LaserColor() );
					p.SetObjectMaterial( 0, LaserMaterial() );
				}			
				if (hitDistance > 15 && hitDistance < 30)
				{
					p = EntityAI.Cast(GetGame().CreateObject("Expansion_Laser_Impact3", contact_pos, true, false, true));
					p.SetObjectTexture( 0, LaserColor() );
					p.SetObjectMaterial( 0, LaserMaterial() );
				}
				if (hitDistance > 30 && hitDistance < 50)
				{
					p = EntityAI.Cast(GetGame().CreateObject("Expansion_Laser_Impact4", contact_pos, true, false, true));
					p.SetObjectTexture( 0, LaserColor() );
					p.SetObjectMaterial( 0, LaserMaterial() );
				}
				if (hitDistance > 50 && hitDistance < 100)
				{
					p = EntityAI.Cast(GetGame().CreateObject("Expansion_Laser_Impact5", contact_pos, true, false, true));
					p.SetObjectTexture( 0, LaserColor() );
					p.SetObjectMaterial( 0, LaserMaterial() );
				}
				if (hitDistance > 100)
				{
					p = EntityAI.Cast(GetGame().CreateObject("Expansion_Laser_Impact6", contact_pos, true, false, true));
					p.SetObjectTexture( 0, LaserColor() );
					p.SetObjectMaterial( 0, LaserMaterial() );
				}
				p.SetPosition(contact_pos);
				p.Delete();
				
			}	
		};
	}
	override void OnWorkStop()
	{
		if ( !GetGame().IsServer()  ||  !GetGame().IsMultiplayer() ) // Client side
		{
			//StopPeriodicMeasurement();
		}
	}
	
	override void SetActions()
	{
		super.SetActions();
		
		AddAction(ActionTurnOnWhileInHands);
		AddAction(ActionTurnOffWhileInHands);
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
};