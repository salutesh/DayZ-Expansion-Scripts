class ExpansionPathPoint
{
	vector Position;
	Object Parent;

	int Flags;

	ExpansionNavMesh NavMesh;

	ExpansionPathPoint Next;

#ifdef EAI_DEBUG_PATH
#ifndef SERVER
	PGFilter m_Filter;

	void ExpansionPathPoint()
	{
		m_Filter = ExpansionPathFilters.GetInstance().m_PointFilter;
	}
#endif
#endif

	vector GetPosition()
	{
	#ifdef EXPANSION_AI_ATTACHMENT_PATH_FINDING
		if (Parent)
		{
			vector transform[4];
			Parent.GetTransform(transform);

			return Position.Multiply4(transform);
		}
	#endif

		return Position;
	}

	void OnParentUpdate()
	{
		if (Parent)
		{
			NavMesh = ExpansionNavMeshes.Get(Parent);

			if (!NavMesh)
			{
				Parent = null;
			}
		}

		if (Parent)
		{
			vector transform[4];
			Parent.GetTransform(transform);

			Position = Position.InvMultiply4(transform);
		}
		else
		{
			NavMesh = null;
		}
	}

#ifdef EAI_DEBUG_PATH
#ifndef SERVER
	void UpdateFlags(ExpansionPathHandler handler)
	{
		float distance = 1.0;

		Flags = 0;
		Flags |= Sample(distance, PGPolyFlags.WALK);
		Flags |= Sample(distance, PGPolyFlags.DISABLED);
		Flags |= Sample(distance, PGPolyFlags.DOOR);
		Flags |= Sample(distance, PGPolyFlags.INSIDE);
		Flags |= Sample(distance, PGPolyFlags.SWIM);
		Flags |= Sample(distance, PGPolyFlags.SWIM_SEA);
		Flags |= Sample(distance, PGPolyFlags.LADDER);
		Flags |= Sample(distance, PGPolyFlags.JUMP_OVER);
		Flags |= Sample(distance, PGPolyFlags.JUMP_DOWN);
		Flags |= Sample(distance, PGPolyFlags.CLIMB);
		Flags |= Sample(distance, PGPolyFlags.CRAWL);
		Flags |= Sample(distance, PGPolyFlags.CROUCH);
		Flags |= Sample(distance, PGPolyFlags.UNREACHABLE);
	}

	int Sample(float distance, PGPolyFlags flag)
	{
		if (!Next)
		{
			return 0;
		}
		
		m_Filter.SetFlags(flag, PGPolyFlags.NONE, PGPolyFlags.NONE);
		
		vector direction = vector.Direction(Position, Next.Position).Normalized();
		
		vector p0 = Position - (direction * distance);
		vector p1 = Position + (direction * distance);
		vector hitPos;
		vector hitNor;
		
		bool hit = g_Game.GetWorld().GetAIWorld().RaycastNavMesh(p0, p1, m_Filter, hitPos, hitNor);
				
		if (hit)
		{
			return flag;
		}

		return 0;
	}
#endif
#endif

	void Copy(ExpansionPathPoint other)
	{
		Position = other.Position;
		Parent = other.Parent;
		NavMesh = other.NavMesh;
	}

	bool FindPath(ExpansionPathHandler pathFinding, inout array<vector> path, out int pathGlueIdx = -1)
	{
		vector transform[4];
		pathFinding.m_Unit.GetTransform(transform);
		vector startPos = transform[3];

		//! Hack fix for pathfinding on Sakhal ice floes (would not find points otherwise)
		if (pathFinding.m_Unit.eAI_ShouldUseSurfaceUnderWaterForPathFinding())
			startPos[1] = pathFinding.m_Unit.m_eAI_SurfaceY;
		
		return FindPathFrom(startPos, pathFinding, path, pathGlueIdx);
	}

	bool FindPathFrom(vector startPos, ExpansionPathHandler pathFinding, inout array<vector> path, out int pathGlueIdx = -1)
	{
		bool found;

		int i;
		
		PGFilter filter = pathFinding.GetFilter();
		
	#ifdef EXPANSION_AI_ATTACHMENT_PATH_FINDING
		if (Parent)
		{
			vector transform[4];
			Parent.GetTransform(transform);
			
			startPos = startPos.InvMultiply4(transform);

			found = NavMesh.FindPath(startPos, Position, filter, path);
			
			for (i = 0; i < path.Count(); i++)
			{
				path[i] = path[i].Multiply4(transform);
			}
		}
		else
		{
	#endif
			bool isSwimming = pathFinding.m_Unit.IsSwimming();

			if (isSwimming && g_Game.GetWaterDepth(Position) > 0.0)
			{
				//! AIWorld::FindPath will use terrain under water, which can be problematic.
				//! If direct path to target is clear, use it if target position is in water.

				vector fwdPos = Vector(Position[0], startPos[1], Position[2]);
				vector fwdDir = vector.Direction(startPos, fwdPos);
				float fwdLen = fwdDir.Length();

				if (fwdLen > 0.55)
				{
					//! @note to have full swim speed due to speed handling in eAICommandMove, distance needs to be at least 4.472m
					vector rayEnd = startPos + fwdDir.Normalized() * Math.Min(fwdLen, 250.0);

					if (!IsBlockedGeom(startPos, rayEnd, pathFinding.m_Unit))
					{
						path.Insert(startPos);
						path.Insert(rayEnd);

						found = true;

						pathFinding.m_IsTargetUnreachable = false;
						pathFinding.m_IsUnreachable = false;
					}
				}
			}
			
			if (!found)
				found = pathFinding.m_AIWorld.FindPath(startPos, Position, filter, path);

			//! Deal with the case where AI is on top of an object and needs to take a leap of faith
			//! because there is no navmesh connection to ground
			if ((!found || (path.Count() == 2 && !Math.IsPointInCircle(Position, 1.0, path[1]) && Math.IsPointInCircle(pathFinding.m_Unit.GetPosition(), 0.55, path[1]))) && !pathFinding.m_Unit.m_eAI_Ladder)
			{
				pathGlueIdx = path.Count();

			#ifdef EXPANSION_AI_DEBUG_UNREACHABLE
				if (!found || pathFinding.m_IsTargetUnreachable)
				{
					if (!pathFinding.m_IsUnreachable)
						ExpansionStatic.MessageNearPlayers(pathFinding.m_Unit.GetPosition(), 100.0, pathFinding.m_Unit.ToString() + " unreachable IN (" + path.Count() + ")");
				}
			#endif

				vector endPos;

			/*
				if (found)
					endPos = path[1];
				else
			*/
					endPos = startPos;

				array<vector> tempPath = pathFinding.m_TempPoints;
				tempPath.Clear();

				vector dir = vector.Direction(endPos, Position);
				vector targetPos;

				if (dir.LengthSq() > 100.0)  //! Limit to 10 m so we have a chance the path actually ends at endPos
				{
					targetPos = endPos + dir.Normalized() * 10.0;  //! Let's just hope this doesn't turn a reachable Position into unreachable...
				}
				else
				{
					targetPos = Position;
				}

				if (pathFinding.m_AIWorld.FindPath(targetPos, endPos, filter, tempPath))
				{
					found = false;

					if (isSwimming && tempPath.Count() > 2)
						tempPath = OptimizePathForSwimming(tempPath, pathFinding);

					int count = tempPath.Count();

					pathFinding.m_TempCount = count;

					vector tempEnd = tempPath[count - 1];
					if (count > 2 || vector.DistanceSq(tempEnd, endPos) > 0.0001)
					{
					/*
						TVectorArray toTempEnd = {};
						if (pathFinding.m_AIWorld.FindPath(startPos, tempEnd, filter, toTempEnd))
						{
							path.Copy(toTempEnd);
							pathGlueIdx = path.Count();
							endPos = path[pathGlueIdx - 1];
						}

						endPos[1] = Math.Max(endPos[1], startPos[1]);
					*/
						//! Extend tempEnd out a bit (from endPos) so we can get off roofs (e.g. the porch of Land_Factory_Small)
						vector endToTemp = tempEnd - endPos;
						endToTemp[1] = 0;
						tempEnd = tempEnd + endToTemp.Normalized() * 0.5;
						vector checkPos = tempEnd;
						checkPos[1] = Math.Max(Math.Max(endPos[1], checkPos[1]), pathFinding.m_Unit.GetPosition()[1]) + 0.5;

						vector surfaceEndPosition = ExpansionStatic.GetSurfaceRoadPosition(endPos);
						vector surfacePosition = ExpansionStatic.GetSurfaceRoadPosition(tempEnd);
					#ifdef DIAG_DEVELOPER
						Object dbgObj = pathFinding.m_Unit.Expansion_DebugObject(88889, surfaceEndPosition, "ExpansionDebugConeSmall_Cyan");
						if (dbgObj)
							dbgObj.SetOrientation("0 180 0");
						dbgObj = pathFinding.m_Unit.Expansion_DebugObject(88888, surfacePosition, "ExpansionDebugConeSmall_Yellow");
						if (dbgObj)
							dbgObj.SetOrientation("0 180 0");
					#endif

						//! Always test these objects/locations when making changes:
						//! stockyard_oremound1.p3d (on the highest spot), e.g. on Sakhal at <13432.2, 10.5049, 11843.2>
						//! Land_Shed_W2 (inside) on Chernarus at <3144, 7932>
						//! Small ice lake on Sakhal, e.g. at <11014.1, 42.3505, 7414.21>
						float climbHeight = surfacePosition[1] - surfaceEndPosition[1];
						float fallHeight = surfaceEndPosition[1] - surfacePosition[1];
						if (Math.IsPointInCircle(tempEnd, 10.0, endPos) && climbHeight < 2.5 && fallHeight < DayZPlayerImplementFallDamage.HEALTH_HEIGHT_LOW && !pathFinding.IsBlockedPhysically(endPos + "0 0.5 0", checkPos))
						{
							if (isSwimming || !pathFinding.IsBlockedPhysically(checkPos, surfacePosition + "0 0.5 0"))
							{
								//! Swim start water level = 1.5 m, see DayZPlayerUtils::CheckWaterLevel
								if (pathFinding.m_IsSwimmingEnabled || g_Game.GetWaterDepth(surfacePosition) <= 1.5)
								{
									path.Clear();
									pathGlueIdx = 0;
									for (i = count - 1; i >= 0; i--)
									{
										//if (tempPath[i][1] - endPos[1] < 2.5)
											path.Insert(tempPath[i]);
										//else
											//break;
									}

									//if (i < 0)
										//pathFinding.m_IsUnreachable =  false;

									pathFinding.m_TempCount = 0;

									found = true;
									pathFinding.m_Time = -15.0;  //! Longer delay until next path recalculation
								}
							}
						}
					}
				}
				else
				{
					found = false;
				}

			#ifdef DIAG_DEVELOPER
				if (!pathFinding.m_IsUnreachable)
				{
					if (path.Count() == 0)
						EXTrace.Print(EXTrace.AI, this, pathFinding.m_Unit.ToString() + " unreachable (no path)");
					else if (!found)
						EXTrace.Print(EXTrace.AI, this, pathFinding.m_Unit.ToString() + " unreachable (not found)");
				}
			#endif

				if (found || path.Count() > 0)
				{
					pathFinding.m_IsTargetUnreachable = false;
					pathFinding.m_IsUnreachable = false;
				}
				else
				{
					pathFinding.m_IsTargetUnreachable = true;
					pathFinding.m_IsUnreachable = true;
				}

			#ifdef EXPANSION_AI_DEBUG_UNREACHABLE
				if (found)
				{
					string prefix;

					if (pathFinding.m_IsUnreachable)
						prefix = "un";

					ExpansionStatic.MessageNearPlayers(pathFinding.m_Unit.GetPosition(), 100.0, pathFinding.m_Unit.ToString() + " " + prefix + "reachable OUT (" + path.Count() + ")");
				}
			#endif
			}
			else if (isSwimming && path.Count() > 2)
			{
				path = OptimizePathForSwimming(path, pathFinding);
			}

	#ifdef EXPANSION_AI_ATTACHMENT_PATH_FINDING
		}
	#endif

	#ifdef DIAG_DEVELOPER
		pathFinding.m_Unit.Expansion_DebugObject(11110, startPos - "0 3.0 0", "ExpansionDebugNoticeMe_Blue", pathFinding.m_Unit.GetDirection());
		//EXTrace.Print(EXTrace.AI, pathFinding.m_Unit, "FindPath " + path.Count() + " points");
	#endif

	#ifdef EXPANSION_AI_DEBUG_UNREACHABLE
		if (path.Count() == 0)
			ExpansionStatic.MessageNearPlayers(pathFinding.m_Unit.GetPosition(), 100.0, pathFinding.m_Unit.ToString() + " no path found");
	#endif

		return found;
	}

	/**
	 * @brief create copy of path with all intermediate points removed that are under water and can be connected without hitting any geo,
	 * and insert points where necessary to allow climbing on blocking geo (very useful for Sakhal)
	 */
	array<vector> OptimizePathForSwimming(array<vector> path, ExpansionPathHandler pathFinding)
	{
		int lastIdx = path.Count() - 1;
		array<vector> condensedPath = {};
		vector prev;

	#ifdef DIAG_DEVELOPER
		vector origin;
	#endif

		foreach (int i, vector point: path)
		{
			float waterDepth = g_Game.GetWaterDepth(point);
			if (waterDepth > 0.0)
				point[1] = point[1] + waterDepth;

			if (i == 0 || i == lastIdx || waterDepth <= 0.0)
			{
				condensedPath.Insert(point);

				prev = point;
			#ifdef DIAG_DEVELOPER
				origin = point;
			#endif
			}
			else
			{
				vector next = path[i + 1];
				float waterDepthNext = g_Game.GetWaterDepth(next);
				if (waterDepthNext > 0.0)
					next[1] = next[1] + waterDepthNext;

				if (IsBlockedGeom(prev, next, pathFinding.m_Unit))
				{
					condensedPath.Insert(point);

					prev = point;
				#ifdef DIAG_DEVELOPER
					origin = point;
				#endif
				}
			#ifdef DIAG_DEVELOPER
				else
				{
					pathFinding.m_Unit.Expansion_DebugObject(96969 + i, point, "ExpansionDebugConeSmall_Orange", point - origin, origin, 300.0, ShapeFlags.NOZBUFFER);
					origin = point;
				}
			#endif
			}
		}

		return condensedPath;
	}

	bool IsBlockedGeom(vector begPos, vector endPos, eAIBase ignore = null, out vector contactPos = vector.Zero, out vector contactDir = vector.Zero, out int contactComponent = 0, out Object blockingObject = null)
	{
		PhxInteractionLayers layerMask;
		layerMask |= PhxInteractionLayers.BUILDING;
		layerMask |= PhxInteractionLayers.DOOR;
		layerMask |= PhxInteractionLayers.FENCE;
		layerMask |= PhxInteractionLayers.ITEM_LARGE;
		layerMask |= PhxInteractionLayers.VEHICLE;
		layerMask |= PhxInteractionLayers.ROADWAY;
		layerMask |= PhxInteractionLayers.TERRAIN;
		float hitFraction;

		//! @note can not use RaycastRV here, would result in false positives near piers
		//! To test this on Chernarus, spawn AI at <14297.8, -0.36239, 13245.3> (in water) and set a waypoint
		//! at <14297.7, 3.31738, 13250.3> (on pier)
		if (DayZPhysics.RayCastBullet(begPos, endPos, layerMask, ignore, blockingObject, contactPos, contactDir, hitFraction))
			return true;

		return false;
	}
};
