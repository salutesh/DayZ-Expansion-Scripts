//! https://feedback.bistudio.com/T176819

modded class Hologram
{
	ItemBase Expansion_GetParentItem()
	{
		return m_Parent;
	}
}

modded class ItemBase
{
	bool m_Expansion_IsBeingPlaced;

	override void EEItemLocationChanged(notnull InventoryLocation oldLoc, notnull InventoryLocation newLoc)
	{
		super.EEItemLocationChanged(oldLoc, newLoc);

		if (!GetGame().IsServer())
			return;

		if (oldLoc.GetType() == InventoryLocationType.UNKNOWN || newLoc.GetType() != InventoryLocationType.GROUND || !oldLoc.GetParent())
			return;

		if (m_Expansion_IsBeingPlaced)
		{
		#ifdef DIAG
			EXTrace.Print(EXTrace.GENERAL_ITEMS, this, "::EEItemLocationChanged - has been placed - return");
		#endif
			m_Expansion_IsBeingPlaced = false;
			return;
		}

		//! Looks like item was dropped

		if (!GetExpansionSettings().GetDebug().EnableItemDropPlacementFix)
			return;

	#ifdef DIAG
		EXTrace.Print(EXTrace.GENERAL_ITEMS, this, "::EEItemLocationChanged " + oldLoc.DumpToString() + " -> " + newLoc.DumpToString());
	#endif

		EntityAI root = oldLoc.GetParent().GetHierarchyRoot();

		Expansion_PlaceOnSurfaceProper(root);
	}

	void Expansion_PlaceOnSurfaceProper(notnull EntityAI root)
	{
	#ifdef DIAG
		auto trace = EXTrace.Start(EXTrace.GENERAL_ITEMS, this, root.ToString(), root.GetPosition().ToString());
	#endif

		vector pos = GetPosition();

		vector rayStart = pos;
		rayStart[1] = root.GetPosition()[1];
		vector rayEnd = rayStart - "0 1000 0";

		vector rootMinMax[2];
		if (root.IsMan())
		{
			rayStart[1] = rayStart[1] + 0.1;
		}
		else if (root.GetCollisionBox(rootMinMax))
		{
			vector rootTransform[4];
			GetTransform(rootTransform);
			vector rootP1 = rootMinMax[0].Multiply4(rootTransform);
			vector rootP2 = rootMinMax[1].Multiply4(rootTransform);
			vector rootCenter = vector.Zero.Multiply4(rootTransform);
			float rootOffsetY;
			if (rootP2[1] < rootCenter[1])
				rootOffsetY = rootCenter[1] - rootP2[1];
			else
				rootOffsetY = rootCenter[1] - rootP1[1];
			if (!rootOffsetY)
				rootOffsetY = 0.1;
			rayStart[1] = rayStart[1] + rootOffsetY;
		}

		//int component;
		//set<Object> results = new set<Object>;

		//RaycastRVParams params = new RaycastRVParams(rayStart, rayEnd, this);
		//params.flags = CollisionFlags.ALLOBJECTS;
		//params.sorted = true;
		//array<ref RaycastRVResult> results = {};
		//array<Object> excluded = {};
		//excluded.Insert(this);

		PhxInteractionLayers layers;
		layers |= PhxInteractionLayers.BUILDING;
		layers |= PhxInteractionLayers.VEHICLE;
		layers |= PhxInteractionLayers.WATERLAYER;
		layers |= PhxInteractionLayers.FENCE;
		layers |= PhxInteractionLayers.ITEM_LARGE;
		Object hitObj;
		vector hitPos;
		vector hitNormal;
		float hitFraction;

		//if (DayZPhysics.RaycastRV(rayStart, rayEnd, hitPos, hitNormal, component, results, null, this, true, false, ObjIntersectView, 0.0, CollisionFlags.ALLOBJECTS))
		//if (DayZPhysics.RaycastRVProxy(params, results, excluded))
		if (DayZPhysics.RayCastBullet(rayStart, rayEnd, layers, null, hitObj, hitPos, hitNormal, hitFraction))
		{
			//foreach (Object result: results)
			//foreach (RaycastRVResult result: results)
			{
				//hitObj = result;
				//hitObj = result.obj;

				if (!hitObj || hitObj == this || !hitObj.IsInherited(ItemBase))
					return;
					//continue;

				//hitPos = result.pos;

				//if (vector.DistanceSq(GetPosition(), hitPos) > 0.0001)
				{
					vector ori = GetOrientation();

					vector direction = root.GetDirection();
					float dot = vector.Dot(direction, vector.Forward);

					float angle = Math.Acos(dot);
					if (direction[0] < 0)
						angle = -angle;	

					float cosAngle = Math.Cos(angle);
					float sinAngle = Math.Sin(angle);

					//! -------------------------------------------------------

					//! See hologram.c, AlignProjectionOnTerrain

					vector projection_orientation_angles = Vector(angle * Math.RAD2DEG, 0.0, 0.0);
					vector mat0[3];
					vector mat1[3];
					vector mat2[3];
					vector projection_position = hitPos;
					vector normal;

					if ( hitNormal.Length() > 0 )
					{
						normal = hitNormal;
					} 
					else
					{
						normal = GetGame().SurfaceGetNormal( projection_position[0], projection_position[2] );
					}

					vector angles = normal.VectorToAngles();
					angles[1] = angles[1] + 270;

					angles[0] = Math.Clamp( angles[0], 0, 360 ); 
					angles[1] = Math.Clamp( angles[1], 0, 360 );
					angles[2] = Math.Clamp( angles[2], 0, 360 );

					projection_orientation_angles[0] = projection_orientation_angles[0] + ( 360 - angles[0] );

					Math3D.YawPitchRollMatrix( projection_orientation_angles, mat0 );
					Math3D.YawPitchRollMatrix( angles, mat1 );
					Math3D.MatrixMultiply3( mat1, mat0, mat2 );	

					vector y_p_r = Math3D.MatrixToAngles( mat2 );

					//! -------------------------------------------------------

					vector halfExtents;

					vector m4[4];
					m4[0] = mat2[0];
					m4[1] = mat2[1];
					m4[2] = mat2[2];
					m4[3] = hitPos;

					//! -------------------------------------------------------

					//! See inventory.c, SetGroundPosByOwnerBounds

					vector randomPos = Vector(Math.RandomFloat(-halfExtents[0], halfExtents[0]), 0, Math.RandomFloat(-halfExtents[2], halfExtents[2]));
					randomPos = vector.RotateAroundZero(randomPos, vector.Up, cosAngle, sinAngle);
					
					float dist = randomPos[0] * m4[1][0] + randomPos[1] * m4[1][1] + randomPos[2] * m4[1][2];

					m4[3][0] = m4[3][0] + randomPos[0];
					m4[3][1] = m4[3][1] - dist + halfExtents[1];
					m4[3][2] = m4[3][2] + randomPos[2];

					PlaceOnSurfaceRotated(m4, m4[3]);

					//! -------------------------------------------------------

					vector minMax[2];
					float offsetY;
					if (GetCollisionBox(minMax))
					{
						vector p1 = minMax[0].Multiply4(m4);
						vector p2 = minMax[1].Multiply4(m4);
						vector center = vector.Zero.Multiply4(m4);
						if (p2[1] < center[1])
							offsetY = center[1] - p2[1];
						else
							offsetY = center[1] - p1[1];
					}

					//vector hitObjMinMax[2];
				//#ifdef EXPANSIONMODBASEBUILDING
					//ExpansionBaseBuilding ebb;
					//if (Class.CastTo(ebb, hitObj))
						//ebb.ExpansionGetCollisionBox(hitObjMinMax);
					//else
				//#endif
						//hitObj.GetCollisionBox(hitObjMinMax);

					//offsetY += hitObjMinMax[1][1];

				#ifdef DIAG
					EXTrace.Print(EXTrace.GENERAL_ITEMS, this, "::EEItemLocationChanged " + pos + " " + ori + " -> " + m4[3] + " + <0 " + offsetY + " 0> " + Math3D.MatrixToAngles(m4));
				#endif

					m4[3][1] = m4[3][1] + offsetY;
					
					SetTransform(m4);

					//break;
				}
			}
		}
	}
};

modded class PlayerBase
{
	override void PlacingStartServer(ItemBase item)
	{
		if (item)
		{
		#ifdef DIAG
			EXTrace.Print(EXTrace.GENERAL_ITEMS, this, "::PlacingStartServer " + item);
		#endif
			item.m_Expansion_IsBeingPlaced = true;
		}

		super.PlacingStartServer(item);
	}

	override void PlacingCancelServer()
	{
		if (m_HologramServer)
		{
			ItemBase item = m_HologramServer.Expansion_GetParentItem();
			if (item)
			{
			#ifdef DIAG
				EXTrace.Print(EXTrace.GENERAL_ITEMS, this, "::PlacingCancelServer " + item);
			#endif
				item.m_Expansion_IsBeingPlaced = false;
			}
		}

		super.PlacingCancelServer();
	}

	override EntityAI SpawnEntityOnGroundPos(string object_name, vector pos)
	{
		EntityAI entity = super.SpawnEntityOnGroundPos(object_name, pos);

		if (GetExpansionSettings().GetDebug().EnableItemDropPlacementFix)
		{
			ItemBase item;
			if (Class.CastTo(item, entity))
				item.Expansion_PlaceOnSurfaceProper(this);
		}

		return entity;
	}
}
