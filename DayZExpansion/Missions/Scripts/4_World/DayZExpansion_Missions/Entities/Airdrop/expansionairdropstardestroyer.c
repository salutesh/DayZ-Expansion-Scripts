class ExpansionAirdropStarDestroyer: ExpansionAirdropPlaneBase
{
	bool m_Expansion_WarpIn;
	bool m_Expansion_WarpOut;
	bool m_Expansion_EngineSoundStop;
	float m_Expansion_DistToPlayer;
	float m_Expansion_WarpedOutDist = 1375;  //! Distance when fully warped out
	float m_Expansion_WarpedOutScale = 0.0001;  //! Scale when fully warped out
	float m_Expansion_WarpedInScale = 16;  //! Scale when fully warped in
	float m_Expansion_AirdropPositionX;
	float m_Expansion_AirdropPositionY;
	float m_Expansion_AirdropPositionZ;

	void ExpansionAirdropStarDestroyer()
	{
		RegisterNetSyncVariableFloat("m_Expansion_Speed");
		RegisterNetSyncVariableFloat("m_Expansion_AirdropPositionX");
		RegisterNetSyncVariableFloat("m_Expansion_AirdropPositionY");
		RegisterNetSyncVariableFloat("m_Expansion_AirdropPositionZ");
	}

	void ~ExpansionAirdropStarDestroyer()
	{
		if (g_Game)
			Expansion_DisableClientUpdate();
	}

	override void Expansion_OnSetupPlane()
	{
		m_Expansion_AirdropPositionX = m_Expansion_AirdropPosition[0];
		m_Expansion_AirdropPositionY = m_Expansion_AirdropPosition[1];
		m_Expansion_AirdropPositionZ = m_Expansion_AirdropPosition[2];

		if (HasNetworkID())
			SetSynchDirty();
	}

	override void OnVariablesSynchronized()
	{
		super.OnVariablesSynchronized();

		m_Expansion_AirdropPosition[0] = m_Expansion_AirdropPositionX;
		m_Expansion_AirdropPosition[1] = m_Expansion_AirdropPositionY;
		m_Expansion_AirdropPosition[2] = m_Expansion_AirdropPositionZ;

		if (!m_Expansion_IsUpdateEnabled)
			Expansion_EnableClientUpdate();
	}

	override void Expansion_EnableUpdate()
	{
		//! We disable terrain following as it would look awkward for such a huge object
		m_Expansion_FollowTerrainFraction = 0.0;

		super.Expansion_EnableUpdate();
	}

	void Expansion_EnableClientUpdate()
	{
	#ifndef SERVER
		//! Client or SP
		EXTrace.Print(EXTrace.MISSIONS, this, "Expansion_EnableClientUpdate " + m_Expansion_IsUpdateEnabled);

		if (!m_Expansion_IsUpdateEnabled)
		{
			g_Game.GetUpdateQueue(CALL_CATEGORY_SYSTEM).Insert(Expansion_OnClientUpdate);

			m_Expansion_IsUpdateEnabled = true;
		}
	#endif
	}

	void Expansion_DisableClientUpdate()
	{
	#ifndef SERVER
		//! Client or SP
		EXTrace.Print(EXTrace.MISSIONS, this, "Expansion_DisableClientUpdate " + m_Expansion_IsUpdateEnabled);

		if (m_Expansion_IsUpdateEnabled)
		{
			if (g_Game.GetUpdateQueue(CALL_CATEGORY_SYSTEM))
				g_Game.GetUpdateQueue(CALL_CATEGORY_SYSTEM).Remove(Expansion_OnClientUpdate);

			m_Expansion_IsUpdateEnabled = false;
		}
	#endif
	}

#ifndef SERVER
	override void Expansion_OnUpdate(float dt)
	{
		super.Expansion_OnUpdate(dt);

		if (!m_Expansion_IsUpdateEnabled)
		{
			//! Expansion_DisableUpdate was called in super, SD is about to be deleted
			return;
		}

		Expansion_OnClientUpdate(dt);
	}

	void Expansion_OnClientUpdate(float dt)
	{
	#ifdef EXTRACE_DIAG
		auto trace = EXTrace.Profile(EXTrace.MISSIONS, this, "Expansion_OnClientUpdate");
	#endif

		float scale = m_Expansion_WarpedOutScale;

		vector cameraPosition = g_Game.GetCurrentCameraPosition();
		if (Math.IsPointInCircle(m_Expansion_AirdropPosition, m_Expansion_WarpedOutDist, cameraPosition))
		{
			float dist = vector.Distance(cameraPosition, GetPosition());
			float warpedOutDist = m_Expansion_WarpedOutDist;

			float fogVisibility;
			float overcastVisibility;
			float rainVisibility;
			float snowVisibility;

			Environment.Expansion_GetWeatherVisibilityCurrent(fogVisibility, overcastVisibility, rainVisibility, snowVisibility);

			float visibility = Math.Min(fogVisibility, Math.Min(overcastVisibility, Math.Min(rainVisibility, snowVisibility)));

			if (warpedOutDist * visibility > m_Expansion_Speed * 0.5)
				warpedOutDist *= visibility;

			float warpedInDist = warpedOutDist - m_Expansion_Speed * 0.5;  //! 0.5 s for warp-in

			scale = ExpansionMath.PowerConversion(warpedOutDist, warpedInDist, dist, m_Expansion_WarpedOutScale, m_Expansion_WarpedInScale, 3.0);

			float warpInSoundDistThreshold = warpedOutDist * (1 + 0.26 * (m_Expansion_Speed / 77.0));
			float warpOutSoundDistThreshold = (warpedInDist - m_Expansion_Speed * 5 * (m_Expansion_Speed / 77.0)) * 0.82;

			if (dist < m_Expansion_DistToPlayer && dist < warpInSoundDistThreshold && !m_Expansion_WarpIn)
			{
				m_Expansion_WarpIn = true;
				SEffectManager.Expansion_PlaySoundOnObject("Expansion_StarDestroyer_WarpIn_SoundSet", this, 0.1, 0.1);
				PlaySoundSetLoop(m_Expansion_EngineSound, "Expansion_StarDestroyer_Engine_SoundSet", 1.0, 1.0);
			}
			else if (scale == m_Expansion_WarpedInScale && dist > m_Expansion_DistToPlayer && dist > warpOutSoundDistThreshold && !m_Expansion_WarpOut)
			{
				m_Expansion_WarpOut = true;
				SEffectManager.Expansion_PlaySoundOnObject("Expansion_StarDestroyer_WarpOut_SoundSet", this, 0.1, 0.1);
			}
			else if (m_Expansion_WarpOut && scale < m_Expansion_WarpedInScale && !m_Expansion_EngineSoundStop)
			{
				m_Expansion_EngineSoundStop = true;
				StopSoundSet(m_Expansion_EngineSound);
			}

			m_Expansion_DistToPlayer = dist;
		}

		SetScale(scale);
	}
#endif

	override float Expansion_GetDropOffset()
	{
		return 250;
	}
}
