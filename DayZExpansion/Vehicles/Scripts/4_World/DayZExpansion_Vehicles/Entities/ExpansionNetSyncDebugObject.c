class ExpansionNetSyncDebugObject extends ItemBase
{
	int m_NetIDLow;
	int m_NetIDHigh;

	float m_PosX;
	float m_PosY;
	float m_PosZ;
	float m_OriX;
	float m_OriY;
	float m_OriZ;

	EntityAI m_Object;

	void ExpansionNetSyncDebugObject()
	{
		SetEventMask(EntityEvent.SIMULATE | EntityEvent.POSTSIMULATE | EntityEvent.INIT | EntityEvent.CONTACT | EntityEvent.FRAME | EntityEvent.PHYSICSMOVE);

		m_NetIDLow = 0;
		m_NetIDHigh = 0;
		
		m_PosX = 0;
		m_PosY = 0;
		m_PosZ = 0;
		m_OriX = 0;
		m_OriY = 0;
		m_OriZ = 0;

		RegisterNetSyncVariableInt("m_NetIDLow");
		RegisterNetSyncVariableInt("m_NetIDHigh");

		RegisterNetSyncVariableFloat("m_PosX");
		RegisterNetSyncVariableFloat("m_PosY");
		RegisterNetSyncVariableFloat("m_PosZ");
		RegisterNetSyncVariableFloat("m_OriX");
		RegisterNetSyncVariableFloat("m_OriY");
		RegisterNetSyncVariableFloat("m_OriZ");
	}

	void SetSyncObject(EntityAI object)
	{
		m_Object = object;
		m_Object.GetNetworkID(m_NetIDLow, m_NetIDHigh);
	}

	override bool OnNetworkTransformUpdate(out vector pos, out vector ypr)
	{
		FixScale();

		return false;
	}

	override void EOnFrame(IEntity other, float timeSlice)
	{
		FixScale();
	}

	override void EOnPostFrame(IEntity other, int extra)
	{
		FixScale();
	}

	override void EOnSimulate(IEntity other, float dt)
	{
		FixScale();
	}

	void FixScale()
	{
		if (!m_Object)
			m_Object = GetGame().GetObjectByNetworkId(m_NetIDLow, m_NetIDHigh);

		if (!m_Object)
			return;

		if (GetGame().IsServer())
		{
			m_PosX = m_Object.GetPosition()[0];
			m_PosY = m_Object.GetPosition()[1];
			m_PosZ = m_Object.GetPosition()[2];
			m_OriX = m_Object.GetOrientation()[0];
			m_OriY = m_Object.GetOrientation()[1];
			m_OriZ = m_Object.GetOrientation()[2];

			SetSynchDirty();
		}
		
		vector info[2];
		m_Object.ClippingInfo(info);

		vector trans[4];

		Math3D.YawPitchRollMatrix(Vector(m_OriX, m_OriY, m_OriZ), trans);

		//trans[0] = trans[0] * (Math.AbsFloat(info[0][0]) + Math.AbsFloat(info[1][0])) * 0.5;
		//trans[1] = trans[1] * (Math.AbsFloat(info[0][1]) + Math.AbsFloat(info[1][1])) * 0.5;
		//trans[2] = trans[2] * (Math.AbsFloat(info[0][2]) + Math.AbsFloat(info[1][2])) * 0.5;

		trans[3] = Vector(m_PosX, m_PosY, m_PosZ);

		SetTransform(trans);
	}
};