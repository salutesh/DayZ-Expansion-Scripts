
class ExpansionDoor
{
	dJoint m_Joint;
	bool m_JointExists;

	EntityAI m_Vehicle;
	CarDoor m_Door;
	ExpansionPhysicsState m_State;

	vector m_Transform0[4];
	vector m_Transform1[4];

	vector m_ProxyTransform[4];
	int m_InventorySlotID;

	string m_InventorySlot;
	string m_Selection;
	string m_Animation;

	bool m_IsDoor;
	bool m_IsValid;
	bool m_IsHood;
	bool m_IsTrunk;

	void ExpansionDoor(ExpansionVehicle vehicle, string slot, string rootPath)
	{
		m_Vehicle = vehicle.GetEntity();
		m_State = vehicle.GetPhysicsState();
		m_InventorySlot = slot;
		m_InventorySlotID = InventorySlots.GetSlotIdFromString(m_InventorySlot);
		InventorySlots.GetSelectionForSlotId(m_InventorySlotID, m_Selection);

		slot.ToLower();

		if (rootPath == string.Empty)
		{
			m_IsDoor = true;
			
			if (!m_Selection)
			{
				int crewSize = vehicle.CrewSize();
				for (int index = 0; index < crewSize; index++)
				{
					string slotName = vehicle.GetDoorInvSlotNameFromSeatPos(index);
					slotName.ToLower();
					if (slotName == slot)
					{
						m_Selection = vehicle.GetDoorSelectionNameFromSeatPos(index);
						break;
					}
				}
			}

			if (m_Selection)
				m_Animation = vehicle.GetAnimSourceFromSelection(m_Selection);
		}
		else
		{
			string path;

			path = rootPath + " selection";
			if (GetGame().ConfigIsExisting(path))
				m_Selection = GetGame().ConfigGetTextOut(path);

			path = rootPath + " animation";
			if (GetGame().ConfigIsExisting(path))
				m_Animation = GetGame().ConfigGetTextOut(path);

			path = rootPath + " isDoor";
			if (GetGame().ConfigIsExisting(path))
				m_IsDoor = GetGame().ConfigGetInt(path);
		}

		if (m_Selection && m_Animation)
			m_IsValid = true;

		UpdateTransforms();

		string selection = m_Selection;
		selection.ToLower();

		if (slot.Contains("hood") || selection.Contains("hood"))
			m_IsHood = true;
		else if (slot.Contains("trunk") || selection.Contains("trunk"))
			m_IsTrunk = true;

	#ifdef DIAG_DEVELOPER
		EXTrace.Print(EXTrace.VEHICLES, this, string.Format("%1 %2 %3 isDoor %4 isHood %5 isTrunk %6", vehicle, m_InventorySlot, m_InventorySlotID, m_IsDoor, m_IsHood, m_IsTrunk));
	#endif
	}

	void UpdateTransforms()
	{
#ifdef EXPANSION_VEHICLE_DOOR_JOINTS
		if (!m_IsValid)
			return;
		
		LOD lod;
		Selection selection;

		vector pos0, pos1, pos2;

		vector hingeTransform[4];

		lod = m_Vehicle.GetLODByName("memory");
		selection = lod.GetSelectionByName("axis_" + m_Selection);
		if (!selection)
			return;

		pos0 = selection.GetVertexPosition(lod, 0);
		pos1 = selection.GetVertexPosition(lod, 1);

		m_Transform0[3] = (pos0 + pos1) * 0.5;
		m_Transform0[2] = -vector.Direction(pos0, pos1).Normalized();

		lod = m_Vehicle.GetLODByName("geometryView");
		selection = lod.GetSelectionByName(m_Selection);
		if (!selection)
			return;

		pos0 = selection.GetVertexPosition(lod, 0);
		pos1 = selection.GetVertexPosition(lod, 1);
		pos2 = selection.GetVertexPosition(lod, 2);

		m_ProxyTransform[3] = pos0;
		m_ProxyTransform[2] = vector.Direction(pos0, pos1).Normalized();
		m_ProxyTransform[1] = vector.Direction(pos0, pos2).Normalized();
		m_ProxyTransform[0] = m_ProxyTransform[1] * m_ProxyTransform[2];

		m_Transform0[0] = m_ProxyTransform[0];
		m_Transform0[1] = m_Transform0[0] * m_Transform0[2];

		Math3D.MatrixInvMultiply4(m_Transform0, m_ProxyTransform, m_Transform1);
		m_Transform1[3] = -m_Transform1[3];
#endif
	}

	void OnUpdate(float pDt)
	{
		if (!m_IsValid || !m_Door)
			return;
		
		if (m_JointExists)
		{
			if (!dBodyIsDynamic(m_Door))
			{
				CreateDynamic();
			}
		}
		else
		{
			if (m_Vehicle.GetAnimationPhase(m_Animation) > 0.0)
			{
				CreateHinge();
			}
		}

		OnDebug(pDt);
	}

	void OnDebug(float pDt)
	{
		m_State.DBGDrawSphereMS(m_ProxyTransform[3], 0.15, 0x1f7f7fff);
		m_State.DBGDrawSphereMS(m_ProxyTransform[3] + m_ProxyTransform[0], 0.1, 0x1f7f7fff);
		m_State.DBGDrawSphereMS(m_ProxyTransform[3] + m_ProxyTransform[1], 0.1, 0x1f7f7fff);
		m_State.DBGDrawSphereMS(m_ProxyTransform[3] + m_ProxyTransform[2], 0.1, 0x1f7f7fff);

		m_State.DBGDrawSphereMS(m_Transform0[3], 0.15, 0x1f7fff7f);
		m_State.DBGDrawSphereMS(m_Transform0[3] + m_Transform0[0], 0.1, 0x1f7fff7f);
		m_State.DBGDrawSphereMS(m_Transform0[3] + m_Transform0[1], 0.1, 0x1f7fff7f);
		m_State.DBGDrawSphereMS(m_Transform0[3] + m_Transform0[2], 0.1, 0x1f7fff7f);

		m_State.DBGDrawSphereMS(m_Transform1[3], 0.15, 0x1fff7f7f);
		m_State.DBGDrawSphereMS(m_Transform1[3] + m_Transform1[0], 0.1, 0x1fff7f7f);
		m_State.DBGDrawSphereMS(m_Transform1[3] + m_Transform1[1], 0.1, 0x1fff7f7f);
		m_State.DBGDrawSphereMS(m_Transform1[3] + m_Transform1[2], 0.1, 0x1fff7f7f);
	}

	void SetDoor(CarDoor door)
	{
		if (m_Door != door)
		{
			DestroyHinge();
		}

		m_Door = door;
	}

	void CreateDynamic()
	{
		InventoryLocation src = new InventoryLocation;
		InventoryLocation dst = new InventoryLocation;
		if (!m_Door.GetInventory().GetCurrentInventoryLocation(src))
		{
			return;
		}

		if (src.GetType() == InventoryLocationType.GROUND)
		{
			m_Door.SetTransform(m_ProxyTransform);
		}
		else
		{
			vector transform[4];
			m_Door.GetTransform(transform);
			dst.SetGround(m_Door, transform);
			m_Door.GetInventory().TakeToDst(InventoryMode.SERVER, src, dst);
		}

		m_Door.CreateDynamicPhysics(PhxInteractionLayers.DYNAMICITEM);
		m_Door.SetDynamicPhysicsLifeTime(-1);
	}

	void CreateHinge()
	{
		DestroyHinge();

		vector transform[4];
		m_Door.GetTransform(transform);

		CreateDynamic();

		m_Joint = dJointCreateHinge2(m_Vehicle, m_Door, m_Transform0, m_Transform1, true, 0.0);
		m_JointExists = true;
		//dJointHingeSetLimits(m_Joint, 0, 90, 0.5, 0.5, 0.5);

		m_Door.SetTransform(transform);
	}

	void DestroyHinge()
	{
		if (!m_JointExists)
			return;

		dJointDestroy(m_Joint);
	}
};
