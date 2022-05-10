typedef Param6<int, bool, int, int, vector, vector> eAIAimingProfile_SyncParams;

class eAIAimingProfile
{
	Man m_Arbiter;

	EntityAI m_Hands;
	eAIBase m_Player;

	int m_LastUpdated;

	// model space position and direction, re-calculate worldspace on action
	vector m_Position;
	vector m_Direction;

	float m_Accuracy_Min;
	float m_Accuracy_Max;

	void eAIAimingProfile(eAIBase ai)
	{
		m_Player = ai;
		m_Accuracy_Min = GetExpansionSettings().GetAI().AccuracyMin;
		m_Accuracy_Max = GetExpansionSettings().GetAI().AccuracyMax;
	}

	void Update()
	{
#ifdef EAI_TRACE
		auto trace = CF_Trace_0(this, "Update");
#endif

		m_Hands = m_Player.GetHumanInventory().GetEntityInHands();
		m_LastUpdated = GetGame().GetTime();

		Weapon_Base weapon;
		if (Class.CastTo(weapon, m_Hands))
		{
			vector barrel_start = weapon.GetSelectionPositionLS("konec hlavne");
			vector barrel_end = weapon.GetSelectionPositionLS("usti hlavne");
			barrel_start = weapon.ModelToWorld(barrel_start);
			barrel_end = weapon.ModelToWorld(barrel_end);

			vector transform[4];
			m_Player.GetTransform(transform);

			barrel_start = barrel_start.InvMultiply4(transform);
			barrel_end = barrel_end.InvMultiply4(transform);

			m_Position = barrel_start;
			m_Direction = vector.Direction(barrel_start, barrel_end).Normalized();

			return;
		}

		//TODO: calculate raw direction and position from head bone
	}

	bool Get(out vector position, out vector direction)
	{
		bool verify = GetGame().IsServer() && GetGame().IsMultiplayer();

		if (!verify) Update();

		vector transform[4];
		m_Player.GetTransform(transform);

		//! TODO: Accuracy could be influenced by stamina and status effects
		// m_Player.GetStatStamina().Get();
		// m_Player.GetHealth("","");
		// m_Player.GetStatHeatComfort().Get(); <= freezing ?
		float accuracy = Math.RandomFloat(m_Accuracy_Min, m_Accuracy_Max);

		//! Lerp to adjust accuracy
		position = vector.Lerp(m_Position, "0 1.5 0", accuracy).Multiply4(transform);

		//! 100% guarantee to hit target
		vector aimDirection = vector.Direction(position, m_Player.GetAimPosition()).Normalized();

		//! Lerp to adjust accuracy
		direction = vector.Lerp(m_Direction.Multiply3(transform), aimDirection, accuracy);

		//if (verify && GetGame().GetTime() - m_LastUpdated > 1000.0) return false;

		//if (verify && m_Player.GetHumanInventory().GetEntityInHands() != m_Hands) return false;

		return true;
	}

	void UpdateArbiter(Man arbiter)
	{
		if (m_Arbiter != arbiter)
		{
			if (m_Arbiter)
			{
				SendRPC("OnEnd", null, false);
			}

			m_Arbiter = arbiter;

			if (m_Arbiter)
			{
				SendRPC("OnStart", null, false);
			}
		}
	}

	private void SendRPC(string function, Param param, bool g)
	{
		GetRPCManager().SendRPC("eAIAimingProfileManager", function, param, g, m_Arbiter.GetIdentity(), m_Player);	
	}

	void SyncToServer()
	{
#ifdef EAI_TRACE
		auto trace = CF_Trace_0(this, "SyncToServer");
#endif

		if (GetGame().IsServer()) return;

		int low, high;
		bool hasHands = m_Hands != null;
		if (hasHands)
		{
			m_Hands.GetNetworkID(low, high);
		}
		
		auto syncParams = new eAIAimingProfile_SyncParams(m_LastUpdated, hasHands, low, high, m_Position, m_Direction);

		GetRPCManager().SendRPC("eAIAimingProfileManager", "OnSync", syncParams, false, null, m_Player);
	}

	void Serialize(ParamsWriteContext ctx)
	{
		ctx.Write(m_LastUpdated);

		ctx.Write(m_Hands != null);
		if (m_Hands)
		{
			int low, high;
			m_Hands.GetNetworkID(low, high);
			ctx.Write(low);
			ctx.Write(high);
		}

		ctx.Write(m_Position);
		ctx.Write(m_Direction);
	}

	void Deserialize(ParamsReadContext ctx)
	{
		ctx.Read(m_LastUpdated);

		bool hasHands;
		ctx.Read(hasHands);
		if (hasHands)
		{
			int low, high;
			ctx.Read(low);
			ctx.Read(high);
			
			Class.CastTo(m_Hands, GetGame().GetObjectByNetworkId(low, high));
		}

		ctx.Read(m_Position);
		ctx.Read(m_Direction);
	}
};