class ExpansionTowConnection
{
	dJoint m_Joint;
	EntityAI m_Parent;
	bool m_Attached;
};

modded class ItemBase
{
	ref map<int, ref ExpansionTowConnection> m_Expansion_Connections = new map<int, ref ExpansionTowConnection>();
	int m_Expansion_TowConnectionSynchMask;
	int m_Expansion_TowConnectionMask;

	int m_Expansion_TowConnectionIndex;
	bool m_Expansion_IsTowing;

	EntityAI m_Expansion_ChildTow;

	int m_Expansion_ChildTowNetworkIDLow;
	int m_Expansion_ChildTowNetworkIDHigh;

	void ItemBase()
	{
		RegisterNetSyncVariableBool("m_Expansion_IsTowing");

		RegisterNetSyncVariableInt("m_Expansion_TowConnectionSynchMask");

		RegisterNetSyncVariableInt("m_Expansion_ChildTowNetworkIDLow");
		RegisterNetSyncVariableInt("m_Expansion_ChildTowNetworkIDHigh");

		for (int i = 0; i < Expansion_NumberTowConnections(); i++)
		{
			m_Expansion_Connections[i] = new ExpansionTowConnection();
		}
	}

	void Expansion_CreateTow(Object tow, int index)
	{
		CarScript car;
		ItemBase item;

		if (!Class.CastTo(car, tow) && !Class.CastTo(item, tow))
			return;

		if ((car && car.Expansion_IsBeingTowed()) || !IsMissionHost())
			return;

		m_Expansion_ChildTow = EntityAI.Cast(tow);
		m_Expansion_IsTowing = true;
		m_Expansion_TowConnectionIndex = index;

		bool success = false;

		if (car)
			success = car.Expansion_OnTowCreated(this, Expansion_GetTowPosition(), m_Expansion_TowConnectionIndex);
		else if (item)
			success = item.Expansion_OnTowCreated(this, Expansion_GetTowPosition(), m_Expansion_TowConnectionIndex);

		if (!success)
		{
			m_Expansion_ChildTow = null;
			m_Expansion_IsTowing = false;
			m_Expansion_TowConnectionIndex = -1;
			return;
		}

		if (!GetGame().IsClient())
		{
			m_Expansion_ChildTow.GetNetworkID(m_Expansion_ChildTowNetworkIDLow, m_Expansion_ChildTowNetworkIDHigh);

			SetSynchDirty();
		}
	}

	bool Expansion_OnTowCreated(Object parent, vector towPos, int index)
	{
		if (index < 0 || index >= Expansion_NumberTowConnections())
			return false;

		vector connectionPoint;
		vector connectionSize;

		Expansion_GetTowConnection(index, connectionPoint, connectionSize);

		m_Expansion_TowConnectionSynchMask |= 1 << index;
		m_Expansion_TowConnectionMask = m_Expansion_TowConnectionSynchMask;

		m_Expansion_Connections[index].m_Joint = dJointCreateBallSocket(parent, this, towPos, connectionPoint, false, 0.1);
		m_Expansion_Connections[index].m_Parent = EntityAI.Cast(parent);
		m_Expansion_Connections[index].m_Attached = true;

		if (GetGame().IsServer())
			SetSynchDirty();

		return true;
	}

	EntityAI Expansion_GetTowedEntity()
	{
		if (m_Expansion_IsTowing)
			return m_Expansion_ChildTow;

		return NULL;
	}

	void Expansion_DestroyTow()
	{
		if (m_Expansion_IsTowing)
		{
			CarScript car;
			ItemBase item;

			if (Class.CastTo(car, m_Expansion_ChildTow))
			{
				car.Expansion_OnTowDestroyed(this, m_Expansion_TowConnectionIndex);
			}

			if (Class.CastTo(item, m_Expansion_ChildTow))
			{
				item.Expansion_OnTowDestroyed(this, m_Expansion_TowConnectionIndex);
			}

			m_Expansion_ChildTow = NULL;
			m_Expansion_IsTowing = false;
			m_Expansion_TowConnectionIndex = -1;

			if (GetGame().IsServer())
				SetSynchDirty();
		}
	}

	void Expansion_OnTowDestroyed(EntityAI parent, int connectionIndex)
	{
		dJointDestroy(m_Expansion_Connections[connectionIndex].m_Joint);
		m_Expansion_Connections[connectionIndex].m_Parent = null;
		m_Expansion_Connections[connectionIndex].m_Attached = false;

		m_Expansion_TowConnectionSynchMask &= ~(1 << connectionIndex);
		m_Expansion_TowConnectionMask = m_Expansion_TowConnectionSynchMask;

		if (GetGame().IsServer())
			SetSynchDirty();
	}

	bool Expansion_IsTowing()
	{
		return m_Expansion_IsTowing;
	}

	int Expansion_NumberTowConnections()
	{
#ifdef EXPANSIONTRACE
		auto trace = CF_Trace_0(ExpansionTracing.VEHICLES, this, "GetAutoHoverTargetHeight");
#endif

		return 0;
	}

	void Expansion_GetTowConnection(int index, out vector position, out vector size)
	{
#ifdef EXPANSIONTRACE
		auto trace = CF_Trace_1(ExpansionTracing.VEHICLES, this, "Expansion_GetTowConnection").Add(index);
#endif

	}

	bool Expansion_GetOverlappingTowConnection(vector towPosition, float towRadius, out int index)
	{
		index = -1;

		towPosition = WorldToModel(towPosition);

		for (int i = 0; i < Expansion_NumberTowConnections(); i++)
		{
			if (m_Expansion_Connections[i].m_Attached)
				continue;

			vector conPos, conSize;
			Expansion_GetTowConnection(i, conPos, conSize);

			if (Math3D.IntersectSphereBox(towPosition, towRadius, conPos - conSize, conPos + conSize))
			{
				index = i;
				return true;
			}
		}

		return false;
	}

	vector Expansion_GetTowPosition()
	{
#ifdef EXPANSIONTRACE
		auto trace = CF_Trace_0(ExpansionTracing.VEHICLES, this, "Expansion_GetTowPosition");
#endif

		vector minMax[2];
		GetCollisionBox(minMax);
		return Vector(0.0, minMax[0][1], minMax[0][2] - dBodyGetCenterOfMass(this)[2]);
	}

	vector Expansion_GetTowDirection()
	{
#ifdef EXPANSIONTRACE
		auto trace = CF_Trace_0(ExpansionTracing.VEHICLES, this, "Expansion_GetTowDirection");
#endif

		return -GetDirection();
	}

	float Expansion_GetTowLength()
	{
#ifdef EXPANSIONTRACE
		auto trace = CF_Trace_0(ExpansionTracing.VEHICLES, this, "Expansion_GetTowLength");
#endif

		return 0.4;
	}

	bool Expansion_CanConnectTow(notnull Object other)
	{
#ifdef EXPANSIONTRACE
		auto trace = CF_Trace_1(ExpansionTracing.VEHICLES, this, "Expansion_CanConnectTow").Add(other);
#endif

		return false;
	}

	bool Expansion_IsPlane()
	{
		return false;
	}

	bool Expansion_IsBoat()
	{
		return false;
	}

	bool Expansion_IsHelicopter()
	{
		return false;
	}

	bool Expansion_IsCar()
	{
		return false;
	}

	bool Expansion_CanPlayerAttach()
	{
#ifdef EXPANSION_PLAYER_ATTACHMENT_CANATTACH_OVERRIDE
		m_Expansion_CanPlayerAttach = true;
#endif

		return m_Expansion_CanPlayerAttach;
	}

	override bool Expansion_CanObjectAttach(Object obj)
	{
		return Expansion_CanPlayerAttach();
	}

	override void OnVariablesSynchronized()
	{
		super.OnVariablesSynchronized();

		if (IsMissionOffline())
			return;

		m_Expansion_ChildTow = NULL;
		if (m_Expansion_IsTowing)
		{
			m_Expansion_ChildTow = EntityAI.Cast(GetGame().GetObjectByNetworkId(m_Expansion_ChildTowNetworkIDLow, m_Expansion_ChildTowNetworkIDHigh));
		}

		if (m_Expansion_TowConnectionMask != m_Expansion_TowConnectionSynchMask)
		{
			m_Expansion_TowConnectionMask = m_Expansion_TowConnectionSynchMask;

			for (int i = 0; i < Expansion_NumberTowConnections(); i++)
			{
				int attached = (m_Expansion_TowConnectionMask >> i) & 1;
				m_Expansion_Connections[i].m_Attached = attached;
			}
		}
	}

	override void OnStoreSave(ParamsWriteContext ctx)
	{
		super.OnStoreSave(ctx);

//! If we are saving game version target for ModStorage support (1st stable) or later
#ifdef EXPANSION_MODSTORAGE
		if (GetGame().SaveVersion() >= EXPANSION_VERSION_GAME_MODSTORAGE_TARGET)
			return;
#endif

		ctx.Write(false);
		ctx.Write(false);
	}

	override bool OnStoreLoad(ParamsReadContext ctx, int version)
	{
		if (Expansion_Assert_False(super.OnStoreLoad(ctx, version), "[" + this + "] Failed reading OnStoreLoad super"))
			return false;

#ifdef EXPANSION_MODSTORAGE
		if (version > EXPANSION_VERSION_GAME_MODSTORAGE_TARGET || m_ExpansionSaveVersion > EXPANSION_VERSION_SAVE_MODSTORAGE_TARGET)
			return true;
#endif

		if (GetExpansionSaveVersion() < 34)
			return true;

		bool l_Expansion_IsBeingTowed;
		bool l_Expansion_IsTowing;

		int l_Expansion_TowConnectionIndex;

		int l_Expansion_ParentTowPersistentIDA;
		int l_Expansion_ParentTowPersistentIDB;
		int l_Expansion_ParentTowPersistentIDD;
		int l_Expansion_ParentTowPersistentIDC;

		int l_Expansion_ChildTowPersistentIDA;
		int l_Expansion_ChildTowPersistentIDB;
		int l_Expansion_ChildTowPersistentIDD;
		int l_Expansion_ChildTowPersistentIDC;

		if (Expansion_Assert_False(ctx.Read(l_Expansion_IsBeingTowed), "[" + this + "] Failed reading l_Expansion_IsBeingTowed"))
			return false;
		if (Expansion_Assert_False(ctx.Read(l_Expansion_IsTowing), "[" + this + "] Failed reading l_Expansion_IsTowing"))
			return false;

		if (l_Expansion_IsBeingTowed)
		{
			if (GetExpansionSaveVersion() >= 34)
			{
				if (Expansion_Assert_False(ctx.Read(l_Expansion_TowConnectionIndex), "[" + this + "] Failed reading l_Expansion_TowConnectionIndex"))
					return false;
			}

			if (Expansion_Assert_False(ctx.Read(l_Expansion_ParentTowPersistentIDA), "[" + this + "] Failed reading l_Expansion_ParentTowPersistentIDA"))
				return false;
			if (Expansion_Assert_False(ctx.Read(l_Expansion_ParentTowPersistentIDB), "[" + this + "] Failed reading l_Expansion_ParentTowPersistentIDB"))
				return false;
			if (Expansion_Assert_False(ctx.Read(l_Expansion_ParentTowPersistentIDC), "[" + this + "] Failed reading l_Expansion_ParentTowPersistentIDC"))
				return false;
			if (Expansion_Assert_False(ctx.Read(l_Expansion_ParentTowPersistentIDD), "[" + this + "] Failed reading l_Expansion_ParentTowPersistentIDD"))
				return false;
		}

		if (l_Expansion_IsTowing)
		{
			if (Expansion_Assert_False(ctx.Read(l_Expansion_ChildTowPersistentIDA), "[" + this + "] Failed reading l_Expansion_ChildTowPersistentIDA"))
				return false;
			if (Expansion_Assert_False(ctx.Read(l_Expansion_ChildTowPersistentIDB), "[" + this + "] Failed reading l_Expansion_ChildTowPersistentIDB"))
				return false;
			if (Expansion_Assert_False(ctx.Read(l_Expansion_ChildTowPersistentIDC), "[" + this + "] Failed reading l_Expansion_ChildTowPersistentIDC"))
				return false;
			if (Expansion_Assert_False(ctx.Read(l_Expansion_ChildTowPersistentIDD), "[" + this + "] Failed reading l_Expansion_ChildTowPersistentIDD"))
				return false;
		}

		return true;
	}
};
