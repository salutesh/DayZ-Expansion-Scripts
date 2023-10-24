/**
 * ExpansionHologram.c
 *
 * DayZ Expansion Mod
 * www.dayzexpansion.com
 * © 2022 DayZ Expansion Mod Team
 *
 * This work is licensed under the Creative Commons Attribution-NonCommercial-NoDerivatives 4.0 International License. 
 * To view a copy of this license, visit http://creativecommons.org/licenses/by-nc-nd/4.0/.
 *
*/

/**@class		Hologram
 * @brief		
 **/
modded class Hologram
{
	//! Expansion switch for snap
	protected bool m_UsingSnap;

	protected bool m_BBCanSnap;

	protected string m_BBType;

	protected float m_BBxOffset;
	protected float m_BByOffset;
	protected float m_BBzOffset;

	protected float m_BBxSize;
	protected float m_BBySize;
	protected float m_BBzSize;

	protected vector m_BBSize;

	protected autoptr array<Object> m_DebugPositions;
	protected autoptr array<Object> m_DebugDirections;

	protected Object m_DebugPositionHighlighted;
	protected Object m_DebugDirectionHighlighted;

	protected Object m_DebugProjectionPosition;

	protected Object m_DebugPointerPosition;

	protected ExpansionSnappingPosition m_SnapPosition;
	protected ExpansionSnappingDirection m_SnapDirection;

	protected int m_SnapPosArrIndexChosen;
	protected int m_SnapDirArrIndexChosen;

	protected int m_OffsetTypeIndex;
	protected int m_OffsetType;

	protected int m_SnapPosIndexChosen;
	protected int m_SnapDirIndexChosen;

	protected vector m_SnapMP;
	protected vector m_SnapMD;

	protected vector m_SnapWP;
	protected vector m_SnapWD;

	protected vector m_SnapWVector;

	protected vector m_SnapMPC;
	protected vector m_SnapMDC;

	protected vector m_SnapWPC;
	protected vector m_SnapWDC;

	protected vector m_SnapMVectorC;

	protected float m_SnapDistance;

	protected vector m_PointerPosition;

	protected vector m_PlayerDirection;
	protected vector m_PlayerHeadPosition;

	protected int m_PlacingTypeChosen;
	protected Object m_PlacingParent;
	protected vector m_PlacingPositionMS;
	protected vector m_PlacingOrientationMS;

	protected float m_projDirection;

	protected float m_ExTimeSlice;

	void Hologram( PlayerBase player, vector pos, ItemBase item )
	{
		#ifdef EXPANSION_DEBUG_UI_HOLOGRAM
		CF_Debug.Create(this);
		#endif

		m_UsingSnap = true;
		m_projDirection = 0;

		m_DebugPositions = new array< Object >;
		m_DebugDirections = new array< Object >;
	}
	
	void ~Hologram()
	{
		#ifdef EXPANSION_DEBUG_UI_HOLOGRAM
		CF_Debug.Destroy(this);
		#endif

		ClearDebugObjects();
	}

	void ClearDebugObjects()
	{
#ifdef EXPANSIONTRACE
		auto trace = CF_Trace_0(ExpansionTracing.BASEBUILDING, this, "ClearDebugObjects");
#endif

		for ( int i = 0; i < m_DebugPositions.Count(); i++ )
		{
			GetGame().ObjectDelete( m_DebugPositions[i] );
		}

		for ( i = 0; i < m_DebugDirections.Count(); i++ )
		{
			GetGame().ObjectDelete( m_DebugDirections[i] );
		}

		m_DebugPositions.Clear();
		m_DebugDirections.Clear();

		if ( m_DebugPositionHighlighted )
		{
			GetGame().ObjectDelete( m_DebugPositionHighlighted );
		}

		if ( m_DebugDirectionHighlighted )
		{
			GetGame().ObjectDelete( m_DebugDirectionHighlighted );
		}

		if ( m_DebugProjectionPosition )
		{
			GetGame().ObjectDelete( m_DebugProjectionPosition );
		}

		if ( m_DebugPointerPosition )
		{
			GetGame().ObjectDelete( m_DebugPointerPosition );
		}
	}

	override void EvaluateCollision( ItemBase action_item = NULL  )
	{
		if ( GetProjectionEntity().IsInherited(GardenPlot) )
		{
			super.EvaluateCollision( action_item );
		}
		//! When CanBuildAnywhere is enabled, check for !IsCollidingZeroPos() needs to be done here,
		//! else players can place at (0, 0, 0) when looking to the sky no matter where they are on the map
		//! and thus loose the item they are placing
		else if ( GetExpansionSettings().GetBaseBuilding().CanBuildAnywhere && !IsCollidingZeroPos() )
		{
			SetIsColliding( false );
		}
		else 
		{
			super.EvaluateCollision( action_item );
		}
	}

	private Object CreateDebugObject( string name, vector pos )
	{
	#ifndef EXPANSION_DEBUG_SNAPPING
		return NULL;
	#endif

		return GetGame().CreateObject( name, pos, true );
	}

	vector m_ExPointerRayStart;
	vector m_ExPointerHitPosition;
	vector m_ExPointerRayEnd;
	vector m_ExProjectionPosition;
	vector m_ExProjectionOrientation;

	#ifdef CF_DebugUI
	bool CF_OnDebugUpdate(CF_Debug instance, CF_DebugUI_Type type)
	{
		instance.Add("Ray Start", m_ExPointerRayStart);
		instance.Add("Hit Position", m_ExPointerHitPosition);
		instance.Add("Ray End", m_ExPointerRayEnd);

		instance.Add("Projection Position (MS)", m_PlacingPositionMS );
		instance.Add("Projection Orientation (MS)", m_PlacingOrientationMS );

		instance.Add("Player Position", m_Player.GetPosition() );

		instance.Add("Projection Position", m_ExProjectionPosition );
		instance.Add("Projection Orientation", m_ExProjectionOrientation );
		return true;
	}
	#endif
	
	vector GetPointerPosition()
	{
#ifdef EXPANSIONTRACE
		auto trace = CF_Trace_0(ExpansionTracing.BASEBUILDING, this, "GetPointerPosition");
#endif

		if ( !GetGame().GetPlayer() )
		{
			return "0 0 0";
		}

		vector position = GetGame().GetCurrentCameraPosition();
		m_ExPointerRayStart = position + GetGame().GetCurrentCameraDirection() * 0.1;
		m_ExPointerRayEnd = position + GetGame().GetCurrentCameraDirection() * 3.0;

		Object hitObj;
		vector hitNormal;
		float hitFraction;

		if ( !DayZPhysics.RayCastBullet( m_ExPointerRayStart, m_ExPointerRayEnd, 0xFFFFFFFF, m_Projection, hitObj, m_ExPointerHitPosition, hitNormal, hitFraction ) )
			m_ExPointerHitPosition = m_ExPointerRayEnd;

		return m_ExPointerHitPosition;
	}

	override void UpdateHologram( float timeslice )
	{
		m_ExTimeSlice = timeslice;

		super.UpdateHologram(timeslice);
	}

#ifndef SERVER
	override void SetProjectionPosition(vector position)
	{
		m_ExProjectionPosition = position;

		GenerateSnappingPosition( m_Projection );

		if ( m_BBCanSnap )
		{
			m_ExProjectionOrientation = AlignProjectionOnTerrain( m_ExTimeSlice );
			HandleSnapping( m_ExProjectionPosition, m_ExProjectionOrientation );

			m_Projection.SetPosition( m_ExProjectionPosition );
		}
		else
		{
			super.SetProjectionPosition(position);
		}
	}

	override void SetProjectionOrientation(vector orientation)
	{
		if ( !m_BBCanSnap )
			m_ExProjectionOrientation = orientation;

		super.SetProjectionOrientation( m_ExProjectionOrientation );
	}
#endif

	void HandleSnapping( out vector projPosition, out vector projOrientation )
	{
#ifdef EXPANSIONTRACE
		auto trace = CF_Trace_0(ExpansionTracing.BASEBUILDING, this, "HandleSnapping");
#endif

		projOrientation[1] = 0;
		projOrientation[2] = 0;

		m_PlacingParent = NULL;
		m_PlacingPositionMS = vector.Zero;
		m_PlacingOrientationMS = vector.Zero;

		array< ref ExpansionSnappingPosition > snappingData = new array< ref ExpansionSnappingPosition >;

		array< Object > objects = new array< Object >;
		array< CargoBase > proxyCargos = new array< CargoBase >;

		GetGame().GetObjectsAtPosition3D( GetGame().GetPlayer().GetPosition(), LARGE_PROJECTION_DISTANCE_LIMIT * 2.0, objects, proxyCargos );

		GenerateSnappingPositions( objects, snappingData );

		if ( m_UsingSnap && snappingData.Count() > 0 )
		{
			m_SnapPosition = NULL;
			m_SnapDirection = NULL;

			m_SnapPosArrIndexChosen = -1;
			m_SnapDirArrIndexChosen = -1;
			
			m_SnapPosIndexChosen = -1;
			m_SnapDirIndexChosen = -1;

			m_SnapMP = "-1 -1 -1";
			m_SnapMD = "0 0 0";

			m_SnapWP = "-1 -1 -1";
			m_SnapWD = "0 0 0";

			m_SnapMPC = "-1 -1 -1";
			m_SnapMDC = "0 0 0";

			m_SnapWPC = "-1 -1 -1";
			m_SnapWDC = "0 0 0";

			m_SnapDistance = 20.0;
			
			m_PointerPosition = GetPointerPosition();
			m_PlayerDirection = MiscGameplayFunctions.GetHeadingVector( m_Player );

			MiscGameplayFunctions.GetHeadBonePos( m_Player, m_PlayerHeadPosition );

			if ( !m_DebugPointerPosition )
				m_DebugPointerPosition = CreateDebugObject( "ExpansionDebugBox_Blue", m_PointerPosition );
			
			if ( m_DebugPointerPosition )
				m_DebugPointerPosition.SetPosition( m_PointerPosition );

			int directionIndex = 0;

			for ( int i = 0; i < snappingData.Count(); i++ )
			{
				m_SnapMP = snappingData[i].Position;
				m_SnapWP = snappingData[i].Target.ModelToWorld( m_SnapMP );

				bool hadAnyValidDirections = false;

				for ( int k = 0; k < snappingData[i].Directions.Count(); k++ )
				{
					bool isValid = true;

					int offsetTypeTempIndex = snappingData[i].Directions[k].Allow.Find( m_BBType );

					m_SnapMD = snappingData[i].Directions[k].Position;
					m_SnapWD = snappingData[i].Target.ModelToWorld( m_SnapMD );
					m_SnapWVector = vector.Direction( m_SnapWP, m_SnapWD ).Normalized();

					if ( offsetTypeTempIndex < 0 )
					{
						isValid = false;
					}

					if ( isValid )
					{
						switch ( m_BBType )
						{
						case "WALL":
							isValid = CanWallBePlacedAtDirection();
							break;
						case "STAIR":
							isValid = CanStairsBePlacedAtDirection();
							break;
						case "FLOOR":
							isValid = CanFloorBePlacedAtDirection();
							break;
						case "RAMP":
							isValid = CanRampBePlacedAtDirection();
							break;
						}
					}

					DrawDebugDir( m_SnapWD, directionIndex, !isValid );

					float nSnapDistance = vector.Distance( m_SnapWD, m_PointerPosition );

					if ( nSnapDistance <= m_SnapDistance && isValid )
					{
						hadAnyValidDirections = true;

						m_SnapDistance = nSnapDistance;

						m_SnapMPC = m_SnapMP;
						m_SnapMDC = m_SnapMD;

						m_SnapWPC = m_SnapWP;
						m_SnapWDC = m_SnapWD;
				
						m_SnapPosition = snappingData[i];
						m_SnapDirection = snappingData[i].Directions[k];
						
						m_SnapPosArrIndexChosen = i;
						m_SnapDirArrIndexChosen = directionIndex;

						m_OffsetTypeIndex = offsetTypeTempIndex;
						m_OffsetType = m_SnapDirection.Offset[m_OffsetTypeIndex];

						m_SnapPosIndexChosen = snappingData[i].Index;
						m_SnapDirIndexChosen = snappingData[i].Directions[k].Index;
					}

					directionIndex++;
				}

				DrawDebugPos( m_SnapWP, i, !hadAnyValidDirections );
			}

			DrawHighlightedDebugPos( m_SnapWPC, m_SnapPosArrIndexChosen );
			DrawHighlightedDebugDir( m_SnapWDC, m_SnapDirArrIndexChosen );

			if ( m_SnapPosition && m_SnapDirection )
			{
				m_SnapMVectorC = vector.Direction( m_SnapMPC, m_SnapMDC ).Normalized();

				switch ( m_BBType )
				{
					case "WALL":
						HandleWall( projPosition, projOrientation );
						break;
					case "STAIR":
						HandleStairs( projPosition, projOrientation );
						break;
					case "FLOOR":
						HandleFloor( projPosition, projOrientation );
						break;
					case "RAMP":
						HandleRamp( projPosition, projOrientation );
						break;
				}
		
				m_PlacingParent = m_SnapPosition.Target;
				m_PlacingPositionMS = projPosition + "0 0 0";
				m_PlacingOrientationMS = projOrientation + "0 0 0";

				Transform snapTrans = Transform.GetObject( m_SnapPosition.Target );
				Transform localTrans = Transform.YawPitchRoll( m_PlacingOrientationMS, m_PlacingPositionMS );
				Transform worldTrans = new Transform();

				Math3D.MatrixMultiply4( snapTrans.data, localTrans.data, worldTrans.data );
				worldTrans.UpdateUnion();

				projPosition = worldTrans.GetOrigin();
				projOrientation = worldTrans.GetYawPitchRoll();

				if ( !IsFacingCamera( projOrientation ) )
				{
					m_PlacingOrientationMS[0] = m_PlacingOrientationMS[0] + 180.0;
					projOrientation[0] = projOrientation[0] + 180.0;
				}

				m_PlacingOrientationMS[0] = Math.NormalizeAngle( m_PlacingOrientationMS[0] );
				projOrientation[0] = Math.NormalizeAngle( projOrientation[0] );
			}
		} else
		{
			ClearDebugObjects();
		}

		if ( !m_DebugProjectionPosition )
			m_DebugProjectionPosition = CreateDebugObject( "ExpansionDebugBox_Blue", projPosition );

		if ( m_DebugProjectionPosition )
			m_DebugProjectionPosition.SetPosition( projPosition );	
	}

	bool IsFacingCamera( vector orientation )
	{
#ifdef EXPANSIONTRACE
		auto trace = CF_Trace_0(ExpansionTracing.BASEBUILDING, this, "IsFacingCamera");
#endif

		vector ref_dir = orientation.AnglesToVector();
		vector cam_dir = GetGame().GetCurrentCameraDirection();
		
		ref_dir[1] = 0;
		ref_dir.Normalize();
		
		cam_dir[1] = 0;
		cam_dir.Normalize();
		
		float dot = vector.Dot( cam_dir, ref_dir );
		
		if ( dot >= 0 )
		{
			return true;
		}

		return false;
	}

	void SetProjDirection(float direction)
	{
		m_projDirection = direction;
	}

	float GetProjDirection()
	{
		return m_projDirection;
	}

	void NextDirection()
	{
		#ifdef EXPANSION_ROTATE_SNAPPING_ENABLED
		if ( m_projDirection < 370 )
			m_projDirection = m_projDirection + 10.0;
		else
			m_projDirection = 0 ;
		#endif
	}

	void PreviousDirection()
	{
		#ifdef EXPANSION_ROTATE_SNAPPING_ENABLED
		if ( m_projDirection >= 0 )
			m_projDirection = m_projDirection - 10.0;
		else
			m_projDirection = 370;
		#endif
	}

	bool CanWallBePlacedAtDirection()
	{
		return true;
	}

	void HandleWall( out vector position, out vector orientation )
	{
#ifdef EXPANSIONTRACE
		auto trace = CF_Trace_0(ExpansionTracing.BASEBUILDING, this, "HandleWall");
#endif

		vector orient = m_SnapMVectorC.VectorToAngles();

		#ifdef EXPANSION_ROTATE_SNAPPING_ENABLED
		float tempv = 1;
		if (m_projDirection != 0)
			tempv = m_projDirection;
		#endif

		orientation[0] = orient[0] + 90.0;
		orientation[1] = 0.0;
		orientation[2] = 0.0;

		switch ( m_OffsetType )
		{
			case 0:
				position[0] = m_BBxOffset * m_SnapMVectorC[0];
				position[1] = 0;
				position[2] = m_BBxOffset * m_SnapMVectorC[2];
				break;
			case 1:
				position[0] = 0;
				position[1] = 0;
				position[2] = 0;
				break;
			case 2:
				orient = vector.Direction( Vector( 0, m_SnapMPC[1], 0 ), m_SnapMPC ).Normalized().VectorToAngles();

				orientation[0] = orient[0];

				position[0] = 0;
				position[1] = 0;
				position[2] = 0;
				break;
			case 3:
				orient = vector.Direction( Vector( 0, m_SnapMPC[1], 0 ), m_SnapMPC ).Normalized().VectorToAngles();

				orientation[0] = orient[0];

				position[0] = m_BBxOffset * m_SnapMVectorC[0];
				position[1] = m_BBySize * m_SnapMVectorC[1];
				position[2] = m_BBzOffset * m_SnapMVectorC[2];
				break;
			case 4:
				orient = vector.Direction( Vector( 0, m_SnapMPC[1], 0 ), m_SnapMPC ).Normalized().VectorToAngles();

				orientation[0] = orient[0] - 90;

				position[0] = m_BBxOffset * m_SnapMVectorC[0];
				position[1] = m_BBySize * m_SnapMVectorC[1];
				position[2] = m_BBzOffset * m_SnapMVectorC[2];
				break;
		}

		position = m_SnapPosition.Position + position;
	}

	bool CanStairsBePlacedAtDirection()
	{
		if ( vector.Dot( m_PlayerHeadPosition - m_SnapWP, m_SnapWVector ) < 0 )
		{
			return false;
		}

		return true;
	}

	void HandleStairs( out vector position, out vector orientation )
	{
#ifdef EXPANSIONTRACE
		auto trace = CF_Trace_0(ExpansionTracing.BASEBUILDING, this, "HandleStairs");
#endif

		orientation[0] = 180.0;
		orientation[1] = 0.0;
		orientation[2] = 0.0;

		switch ( m_OffsetType )
		{
		case 0:
			position[0] = 0;
			position[1] = 0;
			position[2] = 0;
			break;
		case 1:
			position[0] = 0;
			position[1] = 0;
			position[2] = m_BBzOffset;
			break;
		case 2:
			position[0] = 0;
			position[1] = -m_BBySize;
			position[2] = m_BBzOffset;
			break;
		case 3:
			position[0] = m_BBxOffset;
			position[1] = m_BBySize;
			position[2] = m_BBzOffset;
			break;
		case 4:
			position[0] = -m_BBxOffset;
			position[1] = -m_BBySize;
			position[2] = m_BBzOffset;
			break;
		case 5:
			position[0] = m_BBxOffset;
			position[1] = -m_BBySize;
			position[2] = m_BBzOffset;
			break;
		}

		Transform snapTrans = Transform.YawPitchRoll( m_SnapMVectorC.VectorToAngles(), m_SnapPosition.Position );
		Transform localTrans = Transform.YawPitchRoll( orientation, position );
		Transform worldTrans = new Transform();

		Math3D.MatrixMultiply4( snapTrans.data, localTrans.data, worldTrans.data );
		worldTrans.UpdateUnion();

		position = worldTrans.GetOrigin();
		orientation = worldTrans.GetYawPitchRoll();
	}

	bool CanFloorBePlacedAtDirection()
	{
		return true;
	}

	void HandleFloor( out vector position, out vector orientation )
	{
#ifdef EXPANSIONTRACE
		auto trace = CF_Trace_0(ExpansionTracing.BASEBUILDING, this, "HandleFloor");
#endif

		vector orient = m_SnapMVectorC.VectorToAngles();

		orientation[0] = orient[0] + 180;
		orientation[1] = 0.0;
		orientation[2] = 0.0;

		switch ( m_OffsetType )
		{
		case 0:
			position[0] = 0;
			position[1] = 0;
			position[2] = 0;
			break;
		case 1:
			position[0] = m_BBxOffset * m_SnapMVectorC[0];
			position[1] = 0;
			position[2] = m_BBzOffset * m_SnapMVectorC[2];
			break;
		}

		position = m_SnapPosition.Position + position;
	}

	bool CanRampBePlacedAtDirection()
	{
		return true;
	}

	void HandleRamp( out vector position, out vector orientation )
	{
#ifdef EXPANSIONTRACE
		auto trace = CF_Trace_0(ExpansionTracing.BASEBUILDING, this, "HandleRamp");
#endif

		vector orient = m_SnapMVectorC.VectorToAngles();

		orientation[0] = orient[0] + 180;
		orientation[1] = 0.0;
		orientation[2] = 0.0;

		switch ( m_OffsetType )
		{
		case 0:
			position[0] = 0;
			position[1] = 0;
			position[2] = 0;
			break;
		case 1:
			position[0] = m_BBxOffset * m_SnapMVectorC[0];
			position[1] = 0;
			position[2] = m_BBzOffset * m_SnapMVectorC[2];
			break;
		case 2:
			position[0] = m_BBxOffset * m_SnapMVectorC[0];
			position[1] = -m_BBySize;
			position[2] = m_BBzOffset * m_SnapMVectorC[2];
			break;
		case 3:
			position[0] = m_BBxOffset * m_SnapMVectorC[0];
			position[1] = m_BBySize * m_SnapMVectorC[1];
			position[2] = m_BBzOffset * m_SnapMVectorC[2];
			break;
		}

		position = m_SnapPosition.Position + position;
	}

	void DrawHighlightedDebugPos( vector position, int index )
	{
#ifdef EXPANSIONTRACE
		auto trace = CF_Trace_0(ExpansionTracing.BASEBUILDING, this, "DrawHighlightedDebugPos");
#endif

		if ( !m_DebugPositionHighlighted )
			m_DebugPositionHighlighted = CreateDebugObject( "ExpansionDebugBox", position );

		if ( m_DebugPositionHighlighted )
			m_DebugPositionHighlighted.SetPosition( position );

		DrawDebugPos( "0 0 0", index, false );
	}

	void DrawHighlightedDebugDir( vector position, int index )
	{
#ifdef EXPANSIONTRACE
		auto trace = CF_Trace_0(ExpansionTracing.BASEBUILDING, this, "DrawHighlightedDebugDir");
#endif

		if ( !m_DebugDirectionHighlighted )
			m_DebugDirectionHighlighted = CreateDebugObject( "ExpansionDebugBox", position );

		if ( m_DebugDirectionHighlighted )
			m_DebugDirectionHighlighted.SetPosition( position );

		DrawDebugDir( "0 0 0", index, false );
	}

	void DrawDebugPos( vector position, int index, bool block )
	{
#ifdef EXPANSIONTRACE
		auto trace = CF_Trace_0(ExpansionTracing.BASEBUILDING, this, "DrawDebugPos");
#endif

		string type = "ExpansionDebugBox_Orange";
		if ( block )
		{
			type = "ExpansionDebugBox_Red";
		}

		if ( index >= m_DebugPositions.Count() )
		{
			m_DebugPositions.Insert( CreateDebugObject( type, position ) );
		}
		else if ( m_DebugPositions[index] )
		{
			if ( m_DebugPositions[index].GetType() != type )
			{
				GetGame().ObjectDelete( m_DebugPositions[index] );
				m_DebugPositions.Set( index, CreateDebugObject( type, position ) );
			}
			else
			{
				m_DebugPositions[index].SetPosition( position );
			}
		}

	}

	void DrawDebugDir( vector position, int index, bool block )
	{
#ifdef EXPANSIONTRACE
		auto trace = CF_Trace_0(ExpansionTracing.BASEBUILDING, this, "DrawDebugDir");
#endif

		string type = "ExpansionDebugBox_Orange";
		if ( block )
		{
			type = "ExpansionDebugBox_Red";
		}

		if ( index >= m_DebugDirections.Count() )
		{
			m_DebugDirections.Insert( CreateDebugObject( type, position ) );
		} else
		{
			if ( m_DebugDirections[index] && m_DebugDirections[index].GetType() != type )
			{
				GetGame().ObjectDelete( m_DebugDirections[index] );
				m_DebugDirections.Set( index, CreateDebugObject( type, position ) );
			}
		}

		if ( m_DebugDirections[index] )
			m_DebugDirections[index].SetPosition( position );
	}

	void GenerateSnappingPosition( Object object )
	{
#ifdef EXPANSIONTRACE
		auto trace = CF_Trace_0(ExpansionTracing.BASEBUILDING, this, "GenerateSnappingPosition");
#endif

		m_BBCanSnap = false;
		
		if ( object != NULL )
		{
			string hasSnapping = "cfgVehicles " + object.GetType() + " ExpansionSnapping";
			if ( GetGame().ConfigIsExisting( hasSnapping ) )
			{
				m_BBCanSnap = true;
			} else
			{
				m_BBCanSnap = false;
			}

			string typePath = "cfgVehicles " + object.GetType() + " ExpansionSnapping type";
			if ( GetGame().ConfigIsExisting( typePath ) && m_BBCanSnap )
			{
				GetGame().ConfigGetText( typePath, m_BBType );
			} else
			{
				m_BBCanSnap = false;
			}

			string xSizePath = "cfgVehicles " + object.GetType() + " ExpansionSnapping xSize";
			if ( GetGame().ConfigIsExisting( xSizePath ) && m_BBCanSnap )
			{
				m_BBxSize = GetGame().ConfigGetFloat( xSizePath );
			} else
			{
				m_BBCanSnap = false;
			}

			string ySizePath = "cfgVehicles " + object.GetType() + " ExpansionSnapping ySize";
			if ( GetGame().ConfigIsExisting( ySizePath ) && m_BBCanSnap )
			{
				m_BBySize = GetGame().ConfigGetFloat( ySizePath );
			} else
			{
				m_BBCanSnap = false;
			}

			string zSizePath = "cfgVehicles " + object.GetType() + " ExpansionSnapping zSize";
			if ( GetGame().ConfigIsExisting( zSizePath ) && m_BBCanSnap )
			{
				m_BBzSize = GetGame().ConfigGetFloat( zSizePath );
			} else
			{
				m_BBCanSnap = false;
			}

			string xOffsetPath = "cfgVehicles " + object.GetType() + " ExpansionSnapping xOffset";
			if ( GetGame().ConfigIsExisting( xOffsetPath ) && m_BBCanSnap )
			{
				m_BBxOffset = GetGame().ConfigGetFloat( xOffsetPath );
			} else
			{
				m_BBCanSnap = false;
			}

			string yOffsetPath = "cfgVehicles " + object.GetType() + " ExpansionSnapping yOffset";
			if ( GetGame().ConfigIsExisting( yOffsetPath ) && m_BBCanSnap )
			{
				m_BByOffset = GetGame().ConfigGetFloat( yOffsetPath );
			} else
			{
				m_BBCanSnap = false;
			}

			string zOffsetPath = "cfgVehicles " + object.GetType() + " ExpansionSnapping zOffset";
			if ( GetGame().ConfigIsExisting( zOffsetPath ) && m_BBCanSnap )
			{
				m_BBzOffset = GetGame().ConfigGetFloat( zOffsetPath );
			} else
			{
				m_BBCanSnap = false;
			}
		}
	}

	void GenerateSnappingPositions( array<Object> objects, out array< ref ExpansionSnappingPosition > data )
	{
#ifdef EXPANSIONTRACE
		auto trace = CF_Trace_0(ExpansionTracing.BASEBUILDING, this, "GenerateSnappingPositions");
#endif

		if ( data == NULL )
			data = new array< ref ExpansionSnappingPosition >;
			
		data.Clear();

		if ( objects )
		{
			int snapIdx = 0;
			for ( int i = 0; i < objects.Count(); i++ )
			{
				if ( objects[i] == m_Projection )
					continue;

				snapIdx = 0;

				string typePath = "cfgVehicles " + objects[i].GetType() + " ExpansionSnapping type";
				string type = "";
				if ( GetGame().ConfigIsExisting( typePath ) )
				{
					GetGame().ConfigGetText( typePath, type );
				}

				string defaultHidePath = "cfgVehicles " + objects[i].GetType() + " ExpansionSnapping default_hide";
				array< int > defaultHide = new array< int >;
				if ( GetGame().ConfigIsExisting( defaultHidePath ) )
				{
					GetGame().ConfigGetIntArray( defaultHidePath, defaultHide );
				}

				while ( objects[i].MemoryPointExists( "ex_snap_pos_" + snapIdx ) )
				{
					ExpansionSnappingPosition snapData = new ExpansionSnappingPosition;
					snapData.Target = objects[i];
					snapData.Index = snapIdx;
					snapData.Position = objects[i].GetMemoryPointPos( "ex_snap_pos_" + snapIdx );
					snapData.Type = type;

					bool canShow = true;
					if ( defaultHide.Find( snapIdx ) >= 0 )
					{
						ExpansionBaseBuilding ebb;
						if ( Class.CastTo( ebb, objects[i] ) && !ebb.GetConstruction().IsPartBuiltForSnapPoint( snapIdx ) )
							canShow = false;
					}

					if ( canShow )
					{
						snapData.GenerateDirections();

						data.Insert( snapData );
					}

					snapIdx++;
				}
			}
		}
	}

	array< string > GetPossiblePlacingTypes()
	{
		ExpansionKitBase kit;
		if ( Class.CastTo( kit, m_Player.GetHumanInventory().GetEntityInHands() ) )
			return kit.GetPlacingTypes();

		return new array< string >;
	}

	int GetPlacingTypeCount()
	{
		ExpansionKitBase kit;
		if ( Class.CastTo( kit, m_Player.GetHumanInventory().GetEntityInHands() ) )
			return kit.GetPlacingTypes().Count();
		return -1;
	}

	void SetPlacingTypeChosen( int type )
	{
		ExpansionKitBase kit;
		if ( Class.CastTo( kit, m_Player.GetHumanInventory().GetEntityInHands() ) && type >= 0 && type < kit.GetPlacingTypes().Count() )
		{
			m_PlacingTypeChosen = type;

			if ( m_Projection )
			{
				GetGame().ObjectDelete( m_Projection );
			}

			EntityAI projection_entity;
			if ( GetGame().IsMultiplayer() && GetGame().IsServer() )
			{	
				projection_entity = EntityAI.Cast( GetGame().CreateObjectEx( ProjectionBasedOnParent(), GetProjectionEntityPosition( m_Player ), ECE_PLACE_ON_SURFACE ) );
				SetProjectionEntity( projection_entity );
				SetAnimations();
			} else
			{
				projection_entity = EntityAI.Cast( GetGame().CreateObjectEx( ProjectionBasedOnParent(), GetProjectionEntityPosition( m_Player ), ECE_TRACE|ECE_LOCAL ) );
				SetProjectionEntity( projection_entity );
				SetAnimations();
				CreateTrigger();
			}
		}
	}

	int GetPlacingTypeChosen()
	{
		return m_PlacingTypeChosen;
	}

	Object GetPlacingParent()
	{
		return m_PlacingParent;
	}

	vector GetPlacingPositionMS()
	{
		return m_PlacingPositionMS;
	}

	vector GetPlacingOrientationMS()
	{
		return m_PlacingOrientationMS;
	}

	// ------------------------------------------------------------
	override string ProjectionBasedOnParent()
	{
		ItemBase item_in_hands = ItemBase.Cast( m_Player.GetHumanInventory().GetEntityInHands() );

		ExpansionKitBase kit;
		if ( Class.CastTo( kit, item_in_hands ) && kit.GetPlacingTypes().Count() > 0 )
			return kit.GetPlacingTypes()[m_PlacingTypeChosen];

		if ( item_in_hands.IsInherited( ExpansionExplosive ) )
			return item_in_hands.GetType() + "Placing";

		if ( item_in_hands.IsInherited( ExpansionKitBase ) )
			return item_in_hands.GetType() + "Placing";

		if ( item_in_hands.IsInherited( ExpansionSafeMedium ) )
			return item_in_hands.GetType() + "Placing";

		if ( item_in_hands.IsInherited( ExpansionStove ) )
			return item_in_hands.GetType() + "Placing";
		
		if ( item_in_hands.IsInherited( ExpansionCone ) )
			return item_in_hands.GetType() + "Placing";

		if ( item_in_hands.IsInherited( ExpansionSignDanger ) )
			return item_in_hands.GetType() + "Placing";

		return super.ProjectionBasedOnParent();
	}

	// ------------------------------------------------------------
	// IsCollidingAngle
	// ------------------------------------------------------------
	//! NOTE: Will only be evaluated if CanBuildAnywhere is false
	override bool IsCollidingAngle()
	{
		if ( ExpansionCollisionException() )
			return false;
		
		return super.IsCollidingAngle();
	}

	// ------------------------------------------------------------
	// IsCollidingBBox
	// ------------------------------------------------------------
	//! NOTE: Will only be evaluated if CanBuildAnywhere is false
	override bool IsCollidingBBox( ItemBase action_item = NULL )
	{
		if ( ExpansionCollisionException( action_item ) )
			return false;
		
		return super.IsCollidingBBox( action_item );
	}

	// ------------------------------------------------------------
	// IsBehindObstacle
	// ------------------------------------------------------------
	//! NOTE: Will only be evaluated if CanBuildAnywhere is false
 	override bool IsBehindObstacle()
	{
		if ( ExpansionCollisionException() )
			return false;
			
		return super.IsBehindObstacle();
	}

	// ------------------------------------------------------------
	// IsBaseViable
	// ------------------------------------------------------------
	//! NOTE: Will only be evaluated if CanBuildAnywhere is false
	override bool IsBaseViable()
	{
		if ( ExpansionCollisionException() )
			return true;

		return super.IsBaseViable();
	}

	// ------------------------------------------------------------
	// IsClippingRoof
	// ------------------------------------------------------------
	//! NOTE: Will only be evaluated if CanBuildAnywhere is false
	override bool IsClippingRoof()
	{
		if ( ExpansionCollisionException() )
			return false;

		return super.IsClippingRoof();
	}

	// ------------------------------------------------------------
	// IsCollidingPlayer
	// ------------------------------------------------------------
	//! NOTE: Will only be evaluated if CanBuildAnywhere is false
	override bool IsCollidingPlayer()
	{
		if ( ExpansionCollisionException() )
			return false;

		return super.IsCollidingPlayer();
	}

	// ------------------------------------------------------------
	// HeightPlacementCheck
	// ------------------------------------------------------------
	//! NOTE: Will only be evaluated if CanBuildAnywhere is false
	override bool HeightPlacementCheck()
	{
		if ( ExpansionCollisionException() )
			return true;

		return super.HeightPlacementCheck();
	}

	bool ExpansionCollisionException( ItemBase action_item = NULL )
	{
		if ( !action_item )
			action_item = m_Player.GetItemInHands();

		if ( action_item )
		{
			//! Explosives always placeable
			if ( action_item.IsInherited( ExpansionExplosive ) )
				return true;

			//! Kits always placeable
			if ( action_item.IsInherited( ExpansionKitBase ) )
				return true;
		}

		return false;
	}

	// ------------------------------------------------------------
	// Expansion SetUsingSnap
	// ------------------------------------------------------------
	void SetUsingSnap( bool state )
	{
		m_UsingSnap = state;
	}

	// ------------------------------------------------------------
	// Expansion IsUsingSnap
	// ------------------------------------------------------------
	bool IsUsingSnap()
	{
		return m_UsingSnap;
	}
};