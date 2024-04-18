/**
 * Expansion_Laser_Base.c
 *
 * DayZ Expansion Mod
 * www.dayzexpansion.com
 * © 2022 DayZ Expansion Mod Team
 *
 * This work is licensed under the Creative Commons Attribution-NonCommercial-NoDerivatives 4.0 International License. 
 * To view a copy of this license, visit http://creativecommons.org/licenses/by-nc-nd/4.0/.
 *
*/

class Expansion_Laser_Base: UniversalLight
{
	private Expansion_Laser_Beam m_Expansion_LaserBeam;

	void ~Expansion_Laser_Base()
	{
		if ( m_Expansion_LaserBeam )
		{
			GetGame().ObjectDelete( m_Expansion_LaserBeam );
		}
	}
	
	override void OnWorkStart()
	{
		if ( !GetGame().IsServer() || !GetGame().IsMultiplayer() )
		{
			Object laser = GetGame().CreateObject( "Expansion_Laser_Beam", GetMemoryPointPos( "beamStart" ), true, false, true );

			m_Expansion_LaserBeam = Expansion_Laser_Beam.Cast( laser );
			if ( !IsVisibleWithoutNVG() && !CheckNVGState() )
				m_Expansion_LaserBeam.SetObjectTexture( 0, InvisibleColor() );
			else
				m_Expansion_LaserBeam.SetObjectTexture( 0, LaserColor() );
				
			m_Expansion_LaserBeam.SetObjectMaterial( 0, LaserMaterial() );

			AddChild( m_Expansion_LaserBeam, -1 );

			Update();
		}
	}

	override void OnWorkStop()
	{
		if ( !m_Expansion_LaserBeam )
			return;

		RemoveChild( m_Expansion_LaserBeam );
			
		GetGame().ObjectDelete( m_Expansion_LaserBeam );
	}

	override void UpdateLaser()
	{
		if ( !m_Expansion_LaserBeam )
			return;
		
		if ( !IsVisibleWithoutNVG() )
		{
			if ( !CheckNVGState() )
				m_Expansion_LaserBeam.SetObjectTexture( 0, InvisibleColor() );
			else
				m_Expansion_LaserBeam.SetObjectTexture( 0, LaserColor() );
		}

		float distance = GetDistance();
		vector fd = -vector.Direction( GetMemoryPointPos( "beamStart" ), GetMemoryPointPos( "beamEnd" ) ).Normalized();

		vector transform[4] = { 
			Vector( fd[2], fd[1], fd[0] ),
			Vector( 0, 1, 0 ),
			Vector( fd[0], fd[1], fd[2] ) * distance,
			GetMemoryPointPos( "beamStart" )
		};

		m_Expansion_LaserBeam.SetTransform( transform );
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

	string InvisibleColor()
	{
		return "#(argb,8,8,3)color(0,0,0,0,co)";
	}

	string LaserMaterial()
	{
		return "dz\\weapons\\projectiles\\data\\tracer_red.rvmat";
	}

	vector LaserLightColor()
	{
		return "1.0 0.0 0.0";
	}

	bool IsVisibleWithoutNVG()
	{
		return false;
	}

	bool IsLaserIR()
	{
		return false;
	}

	bool CheckNVGState()
	{
		DayZPlayerImplement player;
		DayZPlayerCameraBase camera;
		if (Class.CastTo(player, GetGame().GetPlayer()) && Class.CastTo(camera, player.GetCurrentCamera()) && camera.IsCameraNV())
		{
			if (player.IsInOptics())
			{
				EntityAI entityInHands = player.GetHumanInventory().GetEntityInHands();
				if (entityInHands)
				{
					ItemOptics optic;
					Weapon weapon;

					if (Class.CastTo(optic, entityInHands) || (Class.CastTo(weapon, entityInHands) && Class.CastTo(optic, weapon.GetAttachedOptics())))
					{
						if (optic.GetCurrentNVType() == NVTypes.NONE || !optic.IsWorking())
							return false;
					}
				}
			}

			return true;
		}

		return false;
	}
};
