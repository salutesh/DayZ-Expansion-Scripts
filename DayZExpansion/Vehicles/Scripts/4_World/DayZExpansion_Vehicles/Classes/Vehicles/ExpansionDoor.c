
class ExpansionDoor
{
	dJoint m_Joint;
	bool m_JointExists;

	CarScript m_Car;
	CarDoor m_Door;

	vector m_Transform0[4];
	vector m_Transform1[4];

	vector m_ProxyTransform[4];
	int m_InventorySlotID;

	string m_InventorySlot;
	string m_Selection;
	string m_Animation;

	bool m_IsDoor;
	bool m_IsValid;

	void ExpansionDoor(CarScript car, string slot, string rootPath)
	{
		m_Car = car;
		m_InventorySlot = slot;
		m_InventorySlotID = InventorySlots.GetSlotIdFromString(m_InventorySlot);

#ifdef EXPANSION_VEHICLE_DOOR_JOINTS
		if (rootPath == string.Empty)
		{
			m_IsDoor = true;
			m_IsValid = false;

			string compareSlot
			
			int crewSize = m_Car.CrewSize();
			for (int index = 0; index < crewSize; index++)
			{
				string slotName = m_Car.GetDoorInvSlotNameFromSeatPos(index);
				slotName.ToLower();
				slot.ToLower();
				if (slotName == slot)
				{
					m_Selection = m_Car.GetDoorSelectionNameFromSeatPos(index);
					m_IsValid = true;
					break;
				}
			}

			m_Animation = m_Car.GetAnimSourceFromSelection(m_Selection);
		}
		else
		{
			m_IsValid = true;
			
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

		UpdateTransforms();
#endif
	}

	void UpdateTransforms()
	{
		if (!m_IsValid)
			return;
		
		LOD lod;
		Selection selection;

		vector pos0, pos1, pos2;

		vector hingeTransform[4];

		lod = m_Car.GetLODByName("memory");
		selection = lod.GetSelectionByName("axis_" + m_Selection);
		if (!selection)
			return;

		pos0 = selection.GetVertexPosition(lod, 0);
		pos1 = selection.GetVertexPosition(lod, 1);

		m_Transform0[3] = (pos0 + pos1) * 0.5;
		m_Transform0[2] = -vector.Direction(pos0, pos1).Normalized();

		lod = m_Car.GetLODByName("geometryView");
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
			if (m_Car.GetAnimationPhase(m_Animation) > 0.0)
			{
				CreateHinge();
			}
		}

		OnDebug(pDt);
	}

	void OnDebug(float pDt)
	{		
		m_Car.m_State.DBGDrawSphereMS(m_ProxyTransform[3], 0.15, 0x1f7f7fff);
		m_Car.m_State.DBGDrawSphereMS(m_ProxyTransform[3] + m_ProxyTransform[0], 0.1, 0x1f7f7fff);
		m_Car.m_State.DBGDrawSphereMS(m_ProxyTransform[3] + m_ProxyTransform[1], 0.1, 0x1f7f7fff);
		m_Car.m_State.DBGDrawSphereMS(m_ProxyTransform[3] + m_ProxyTransform[2], 0.1, 0x1f7f7fff);

		m_Car.m_State.DBGDrawSphereMS(m_Transform0[3], 0.15, 0x1f7fff7f);
		m_Car.m_State.DBGDrawSphereMS(m_Transform0[3] + m_Transform0[0], 0.1, 0x1f7fff7f);
		m_Car.m_State.DBGDrawSphereMS(m_Transform0[3] + m_Transform0[1], 0.1, 0x1f7fff7f);
		m_Car.m_State.DBGDrawSphereMS(m_Transform0[3] + m_Transform0[2], 0.1, 0x1f7fff7f);

		m_Car.m_State.DBGDrawSphereMS(m_Transform1[3], 0.15, 0x1fff7f7f);
		m_Car.m_State.DBGDrawSphereMS(m_Transform1[3] + m_Transform1[0], 0.1, 0x1fff7f7f);
		m_Car.m_State.DBGDrawSphereMS(m_Transform1[3] + m_Transform1[1], 0.1, 0x1fff7f7f);
		m_Car.m_State.DBGDrawSphereMS(m_Transform1[3] + m_Transform1[2], 0.1, 0x1fff7f7f);
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

		m_Joint = dJointCreateHinge2(m_Car, m_Door, m_Transform0, m_Transform1, true, 0.0);
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
