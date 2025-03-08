class ExpansionAirdropStarDestroyer: ExpansionAirdropPlaneBase
{
	bool m_Expansion_WarpIn;
	bool m_Expansion_WarpOut;
	bool m_Expansion_EngineSoundStop;
	float m_Expansion_DistToPlayer;
	float m_Expansion_WarpedOutDist = 1375;  //! Distance when fully warped out
	float m_Expansion_WarpedInScale = 16;  //! Scale when fully warped in

	override void SetupPlane(vector dropPosition, string name, float maxRadius, bool heightIsRelativeToGround, float height, float followTerrainFrac, float speed, ExpansionLootContainer container, StringLocaliser warningProximityMsg = NULL, StringLocaliser airdropCreatedMsg = NULL, float containerLifeTime = 0.0)
	{
		//! We disable terrain following as it would look awkward for such a huge object
		followTerrainFrac = 0.0;

		super.SetupPlane(dropPosition, name, maxRadius, heightIsRelativeToGround, height, followTerrainFrac, speed, container, warningProximityMsg, airdropCreatedMsg, containerLifeTime);
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

		vector orientation = GetOrientation();
		orientation[0] = m_HeadingAngleDeg;
		
		float scale = 0.0;

		Man player = GetGame().GetPlayer();
		if (player && Math.IsPointInCircle(m_AirdropPosition, m_Expansion_WarpedOutDist, player.GetPosition()))
		{
			float dist = vector.Distance(player.GetPosition(), GetPosition());
			float warpedOutDist = m_Expansion_WarpedOutDist;

			float fogVisibility;
			float overcastVisibility;
			float rainVisibility;
			float snowVisibility;

			Environment.Expansion_GetWeatherVisibilityCurrent(fogVisibility, overcastVisibility, rainVisibility, snowVisibility);

			float visibility = Math.Min(fogVisibility, Math.Min(overcastVisibility, Math.Min(rainVisibility, snowVisibility)));

			if (warpedOutDist * visibility > m_Speed * 0.5)
				warpedOutDist *= visibility;

			float warpedInDist = warpedOutDist - m_Speed * 0.5;  //! 0.5 s for warp-in

			scale = ExpansionMath.PowerConversion(warpedOutDist, warpedInDist, dist, 0.0, m_Expansion_WarpedInScale, 3.0);

			if (dist < m_Expansion_DistToPlayer && dist < warpedOutDist * 1.26 && !m_Expansion_WarpIn)
			{
				m_Expansion_WarpIn = true;
				SEffectManager.Expansion_PlaySoundOnObject("Expansion_StarDestroyer_WarpIn_SoundSet", this, 0.1, 0.1);
				PlaySoundSetLoop(m_Expansion_EngineSound, "Expansion_StarDestroyer_Engine_SoundSet", 1.0, 1.0);
			}
			else if (scale == m_Expansion_WarpedInScale && dist > m_Expansion_DistToPlayer && dist > (warpedInDist - m_Speed * 5) * 0.82 && !m_Expansion_WarpOut)
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

		SetOrientation(orientation);
		SetScale(scale);
	}
#endif

	override float Expansion_GetDropOffset()
	{
		return 250;
	}
}
