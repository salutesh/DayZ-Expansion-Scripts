modded class ItemBase
{
	bool m_Expansion_AcceptingAttachment;
	bool m_Expansion_CanPlayerAttach;
	bool m_Expansion_CanPlayerAttachSet;

	dJoint m_Expansion_TowJoint;
	int m_Expansion_TowConnectionIndex;
	bool m_Expansion_IsBeingTowed;
	bool m_Expansion_IsTowing;

	EntityAI m_Expansion_ParentTow;
	int m_Expansion_ParentTowNetworkIDLow;
	int m_Expansion_ParentTowNetworkIDHigh;
	int m_Expansion_ParentTowPersistentIDA;
	int m_Expansion_ParentTowPersistentIDB;
	int m_Expansion_ParentTowPersistentIDC;
	int m_Expansion_ParentTowPersistentIDD;

	EntityAI m_Expansion_ChildTow;
	int m_Expansion_ChildTowNetworkIDLow;
	int m_Expansion_ChildTowNetworkIDHigh;
	int m_Expansion_ChildTowPersistentIDA;
	int m_Expansion_ChildTowPersistentIDB;
	int m_Expansion_ChildTowPersistentIDC;
	int m_Expansion_ChildTowPersistentIDD;

	void ItemBase()
	{
		RegisterNetSyncVariableBool("m_Expansion_IsBeingTowed");
		RegisterNetSyncVariableBool("m_Expansion_IsTowing");
		RegisterNetSyncVariableInt("m_Expansion_TowConnectionIndex");
		RegisterNetSyncVariableInt("m_Expansion_ParentTowNetworkIDLow");
		RegisterNetSyncVariableInt("m_Expansion_ParentTowNetworkIDHigh");
		RegisterNetSyncVariableInt("m_Expansion_ChildTowNetworkIDLow");
		RegisterNetSyncVariableInt("m_Expansion_ChildTowNetworkIDHigh");
	}

	void Expansion_CreateTow(Object tow, int index)
	{
		CarScript car;
		ItemBase item;

		if (!Class.CastTo(car, tow) && !Class.CastTo(item, tow))
			return;

		if ((item && item.Expansion_IsBeingTowed()) || (car && car.Expansion_IsBeingTowed()) || !IsMissionHost())
			return;

		m_Expansion_ChildTow = EntityAI.Cast(tow);
		m_Expansion_IsTowing = true;

		bool success = false;

		if (car)
			success = car.Expansion_OnTowCreated(this, Expansion_GetTowPosition(), index);
		else if (item)
			success = item.Expansion_OnTowCreated(this, Expansion_GetTowPosition(), index);

		if (!success)
		{
			m_Expansion_ChildTow = null;
			m_Expansion_IsTowing = false;
			return;
		}

		if (!IsMissionOffline())
		{
			m_Expansion_ChildTow.GetNetworkID(m_Expansion_ChildTowNetworkIDLow, m_Expansion_ChildTowNetworkIDHigh);
		}

		if (GetGame().IsServer())
			SetSynchDirty();
	}

	bool Expansion_OnTowCreated(Object parent, vector towPos, int index)
	{
		if (index < 0 || index >= Expansion_NumberTowConnections())
			return false;

		vector connectionPoint;
		vector connectionSize;

		Expansion_GetTowConnection(index, connectionPoint, connectionSize);

		m_Expansion_TowJoint = dJointCreateBallSocket(parent, this, towPos, connectionPoint, false, 0.1);

		m_Expansion_TowConnectionIndex = index;
		m_Expansion_ParentTow = EntityAI.Cast(parent);
		m_Expansion_IsBeingTowed = true;

		if (GetGame().IsServer() && GetGame().IsMultiplayer())
			m_Expansion_ParentTow.GetNetworkID(m_Expansion_ParentTowNetworkIDLow, m_Expansion_ParentTowNetworkIDHigh);

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
				car.Expansion_OnTowDestroyed();
			}

			if (Class.CastTo(item, m_Expansion_ChildTow))
			{
				item.Expansion_OnTowDestroyed();
			}

			m_Expansion_ChildTow = NULL;

			m_Expansion_IsTowing = false;

			if (GetGame().IsServer())
				SetSynchDirty();
		}
	}

	void Expansion_OnTowDestroyed()
	{
		dJointDestroy(m_Expansion_TowJoint);

		m_Expansion_ParentTow = null;
		m_Expansion_IsBeingTowed = false;

		if (GetGame().IsServer())
			SetSynchDirty();
	}

	bool Expansion_IsBeingTowed()
	{
		return m_Expansion_IsBeingTowed;
	}

	bool Expansion_IsTowing()
	{
		return m_Expansion_IsTowing;
	}

	int Expansion_NumberTowConnections()
	{
		return 0;
	}

	void Expansion_GetTowConnection(int index, out vector position, out vector size)
	{
	}

	bool Expansion_GetOverlappingTowConnection(vector towPosition, float towRadius, out int index)
	{
		index = -1;

		towPosition = WorldToModel(towPosition);

		for (int i = 0; i < Expansion_NumberTowConnections(); i++)
		{
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
		vector minMax[2];
		GetCollisionBox(minMax);
		return Vector(0.0, minMax[0][1], minMax[0][2] - dBodyGetCenterOfMass(this)[2]);
	}

	vector Expansion_GetTowDirection()
	{
		return -GetDirection();
	}

	float Expansion_GetTowLength()
	{
		return 0.4;
	}

	bool Expansion_CanConnectTow(notnull Object other)
	{
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
#else
		if (!m_Expansion_CanPlayerAttachSet)
		{
			m_Expansion_CanPlayerAttachSet = true;
			foreach (ExpansionVehiclesConfig vehcfg : GetExpansionSettings().GetVehicle().VehiclesConfig)
			{
				if (IsKindOf(vehcfg.ClassName))
				{
					m_Expansion_CanPlayerAttach = vehcfg.CanPlayerAttach;
					break;
				}
			}
		}
#endif

		return m_Expansion_CanPlayerAttach;
	}

	bool Expansion_CanObjectAttach(Object obj)
	{
		return Expansion_CanPlayerAttach();
	}

	override void OnVariablesSynchronized()
	{
		super.OnVariablesSynchronized();

		if (!IsMissionOffline())
		{
			if (m_Expansion_IsBeingTowed)
			{
				m_Expansion_ParentTow = EntityAI.Cast(GetGame().GetObjectByNetworkId(m_Expansion_ParentTowNetworkIDLow, m_Expansion_ParentTowNetworkIDHigh));
			}
			else
			{
				m_Expansion_ParentTow = NULL;
			}

			if (m_Expansion_IsTowing)
			{
				m_Expansion_ChildTow = EntityAI.Cast(GetGame().GetObjectByNetworkId(m_Expansion_ChildTowNetworkIDLow, m_Expansion_ChildTowNetworkIDHigh));
			}
			else
			{
				m_Expansion_ChildTow = NULL;
			}
		}
	}

	override void OnStoreSave(ParamsWriteContext ctx)
	{
		super.OnStoreSave(ctx);

//! If we are saving game version target for ModStorage support (1st stable) or later
#ifdef CF_MODSTORAGE
		if (GetGame().SaveVersion() >= EXPANSION_VERSION_GAME_MODSTORAGE_TARGET)
			return;
#endif

		ctx.Write(m_Expansion_IsBeingTowed);
		ctx.Write(m_Expansion_IsTowing);

		if (m_Expansion_IsBeingTowed)
		{
			ctx.Write(m_Expansion_TowConnectionIndex);
			ctx.Write(m_Expansion_ParentTowPersistentIDA);
			ctx.Write(m_Expansion_ParentTowPersistentIDB);
			ctx.Write(m_Expansion_ParentTowPersistentIDC);
			ctx.Write(m_Expansion_ParentTowPersistentIDD);
		}

		if (m_Expansion_IsTowing)
		{
			ctx.Write(m_Expansion_ChildTowPersistentIDA);
			ctx.Write(m_Expansion_ChildTowPersistentIDB);
			ctx.Write(m_Expansion_ChildTowPersistentIDC);
			ctx.Write(m_Expansion_ChildTowPersistentIDD);
		}
	}

	override bool OnStoreLoad(ParamsReadContext ctx, int version)
	{
		if (Expansion_Assert_False(super.OnStoreLoad(ctx, version), "[" + this + "] Failed reading OnStoreLoad super"))
			return false;

#ifdef CF_MODSTORAGE
		if (version > EXPANSION_VERSION_GAME_MODSTORAGE_TARGET || m_ExpansionSaveVersion > EXPANSION_VERSION_SAVE_MODSTORAGE_TARGET)
			return true;
#endif

		if (GetExpansionSaveVersion() < 34)
			return true;

		if (Expansion_Assert_False(ctx.Read(m_Expansion_IsBeingTowed), "[" + this + "] Failed reading m_Expansion_IsBeingTowed"))
			return false;
		if (Expansion_Assert_False(ctx.Read(m_Expansion_IsTowing), "[" + this + "] Failed reading m_Expansion_IsTowing"))
			return false;

		if (m_Expansion_IsBeingTowed)
		{
			if (GetExpansionSaveVersion() >= 34)
			{
				if (Expansion_Assert_False(ctx.Read(m_Expansion_TowConnectionIndex), "[" + this + "] Failed reading m_Expansion_TowConnectionIndex"))
					return false;
			}

			if (Expansion_Assert_False(ctx.Read(m_Expansion_ParentTowPersistentIDA), "[" + this + "] Failed reading m_Expansion_ParentTowPersistentIDA"))
				return false;
			if (Expansion_Assert_False(ctx.Read(m_Expansion_ParentTowPersistentIDB), "[" + this + "] Failed reading m_Expansion_ParentTowPersistentIDB"))
				return false;
			if (Expansion_Assert_False(ctx.Read(m_Expansion_ParentTowPersistentIDC), "[" + this + "] Failed reading m_Expansion_ParentTowPersistentIDC"))
				return false;
			if (Expansion_Assert_False(ctx.Read(m_Expansion_ParentTowPersistentIDD), "[" + this + "] Failed reading m_Expansion_ParentTowPersistentIDD"))
				return false;
		}

		if (m_Expansion_IsTowing)
		{
			if (Expansion_Assert_False(ctx.Read(m_Expansion_ChildTowPersistentIDA), "[" + this + "] Failed reading m_Expansion_ChildTowPersistentIDA"))
				return false;
			if (Expansion_Assert_False(ctx.Read(m_Expansion_ChildTowPersistentIDB), "[" + this + "] Failed reading m_Expansion_ChildTowPersistentIDB"))
				return false;
			if (Expansion_Assert_False(ctx.Read(m_Expansion_ChildTowPersistentIDC), "[" + this + "] Failed reading m_Expansion_ChildTowPersistentIDC"))
				return false;
			if (Expansion_Assert_False(ctx.Read(m_Expansion_ChildTowPersistentIDD), "[" + this + "] Failed reading m_Expansion_ChildTowPersistentIDD"))
				return false;
		}

		m_Expansion_IsBeingTowed = false;
		m_Expansion_IsTowing = false;

		m_Expansion_ParentTowPersistentIDA = 0;
		m_Expansion_ParentTowPersistentIDB = 0;
		m_Expansion_ParentTowPersistentIDC = 0;
		m_Expansion_ParentTowPersistentIDD = 0;
		m_Expansion_ChildTowPersistentIDA = 0;
		m_Expansion_ChildTowPersistentIDB = 0;
		m_Expansion_ChildTowPersistentIDC = 0;
		m_Expansion_ChildTowPersistentIDD = 0;

		return true;
	}
};
