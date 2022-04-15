class ExpansionContaminatedArea : House
{
	static ref array<ExpansionContaminatedArea> s_ContaminatedAreas = new array<ExpansionContaminatedArea>();

	/**
	 * @note Only can be called on the client
	 */
	static ExpansionContaminatedArea GetArea(vector position)
	{
		int closest = -1;
		float minDist = FLT_MAX;

		for (int i = 0; i < s_ContaminatedAreas.Count(); i++)
		{
			float dist = vector.DistanceSq(s_ContaminatedAreas[i].GetWorldPosition(), position);
			if (dist <= s_ContaminatedAreas[i].m_Radius && dist < minDist)
			{
				closest = i;
				minDist = dist;
			}
		}

		if (closest == -1)
			return null;

		return s_ContaminatedAreas[closest];
	}

	int m_DecayState = eAreaDecayStage.INIT;
	int m_DecayStateSynch = eAreaDecayStage.INIT;
	float m_Radius = 100;
	float m_PositiveHeight = 25;
	float m_NegativeHeight = 10;
	int m_InnerRings = 1;
	int m_InnerSpacing = 35;
	bool m_OuterRingToggle = true;
	int m_OuterRingOffset = -5;
	int m_OuterSpacing = 20;
	int m_VerticalLayers = 0;
	int m_VerticalOffset = 10;
	int m_ParticleID = ParticleList.CONTAMINATED_AREA_GAS_BIGASS;
	int m_AroundParticleID = ParticleList.CONTAMINATED_AREA_GAS_AROUND;
	int m_TinyParticleID = ParticleList.CONTAMINATED_AREA_GAS_TINY;
	string m_PPERequesterType = "PPERequester_ContaminatedAreaTint";
	int m_PPERequesterIdx;

	ref array<Particle> m_ToxicClouds = new array<Particle>();

	ref Timer m_StartupTimer;
	ref Timer m_FXTimer;
	FlareLight m_FlareLight;
	ShellLight m_ShellLight; // Light used upon ariborne shell detonation
	vector m_OffsetPos;		 // This will be the position at which we spawn all future airborne FX

	// Constants used for startup events
	const int AIRBORNE_EXPLOSION_DELAY = 20;
	const int AREA_SETUP_DELAY = 10;
	const float AIRBORNE_FX_OFFSET = 50;
	const float ARTILLERY_SHELL_SPEED = 100; // Units per second

	// Constants used for dissapearing events
	const float DECAY_START_PART_SIZE = 32;
	const int DECAY_START_PART_BIRTH_RATE = 1;
	const float DECAY_END_PART_SIZE = 17;
	const int DECAY_END_PART_BIRTH_RATE = 1;

	void ExpansionContaminatedArea()
	{
		if (IsMissionClient())
		{
			s_ContaminatedAreas.Insert(this);
		}

		RegisterNetSyncVariableInt("m_DecayStateSynch");

		RegisterNetSyncVariableFloat("m_Radius", 0, 0, 2);
		RegisterNetSyncVariableFloat("m_PositiveHeight", 0, 0, 2);
		RegisterNetSyncVariableFloat("m_NegativeHeight", 0, 0, 2);

		RegisterNetSyncVariableInt("m_InnerRings");
		RegisterNetSyncVariableInt("m_InnerSpacing");
		RegisterNetSyncVariableInt("m_OuterRingOffset");
		RegisterNetSyncVariableInt("m_OuterSpacing");
		RegisterNetSyncVariableInt("m_VerticalLayers");
		RegisterNetSyncVariableInt("m_VerticalOffset");
		RegisterNetSyncVariableInt("m_ParticleID");
		RegisterNetSyncVariableInt("m_AroundParticleID");
		RegisterNetSyncVariableInt("m_TinyParticleID");
		RegisterNetSyncVariableInt("m_PPERequesterIdx");

		RegisterNetSyncVariableBool("m_OuterRingToggle");
	}

	void ~ExpansionContaminatedArea()
	{
		if (IsMissionClient())
		{
			s_ContaminatedAreas.RemoveItemUnOrdered(this);
		}
	}

	override void EEInit()
	{
		super.EEInit();

		if (GetGame().IsMultiplayer() && GetGame().IsServer())
			return;

		OnVariablesSynchronized();
	}

	override void EEDelete(EntityAI parent)
	{
		if (GetGame().IsClient() && m_ToxicClouds)
		{
			for (int i = 0; i < m_ToxicClouds.Count(); i++)
			{
				m_ToxicClouds.Get(i).Stop();
			}
		}

		super.EEDelete(parent);
	}

	void SetDecayState(int newState)
	{
		if (m_DecayState == newState)
			return;

		m_DecayState = newState;
		m_DecayStateSynch = newState;
		SetSynchDirty();
	}

	override void OnVariablesSynchronized()
	{
		super.OnVariablesSynchronized();

		if (m_DecayState == m_DecayStateSynch)
			return;

		m_DecayState = m_DecayStateSynch;

		switch (m_DecayState)
		{
		case eAreaDecayStage.START:
			PlayExplosionLight();
			break;
		case eAreaDecayStage.LIVE:
			PlaceParticles(GetWorldPosition(), m_Radius, m_InnerRings, m_InnerSpacing, m_OuterRingToggle, m_OuterSpacing, m_OuterRingOffset, m_ParticleID);

			break;
		case eAreaDecayStage.DECAY_START:
		{
			// We go through all the particles bound to this area and update relevant parameters
			//Debug.Log("We start decay");
			for (int i = 0; i < m_ToxicClouds.Count(); i++)
			{
				m_ToxicClouds.Get(i).SetParameter(0, EmitorParam.BIRTH_RATE, DECAY_START_PART_BIRTH_RATE);
				m_ToxicClouds.Get(i).SetParameter(0, EmitorParam.SIZE, DECAY_START_PART_SIZE);
			}

			break;
		}
		case eAreaDecayStage.DECAY_END:
		{
			// We go through all the particles bound to this area and update relevant parameters
			//Debug.Log("We finish decay");
			for (int j = 0; j < m_ToxicClouds.Count(); j++)
			{
				m_ToxicClouds.Get(j).SetParameter(0, EmitorParam.BIRTH_RATE, DECAY_END_PART_BIRTH_RATE);
				m_ToxicClouds.Get(j).SetParameter(0, EmitorParam.SIZE, DECAY_END_PART_SIZE);
			}

			break;
		}
		default:
			break;
		}
	}

	void PlayFX()
	{
		if (GetGame().IsServer())
		{
			Param1<vector> pos;		 // The value to be sent through RPC
			array<ref Param> params; // The RPC params

			// We prepare to send the message
			pos = new Param1<vector>(vector.Zero);
			params = new array<ref Param>;

			// We send the message with this set of coords
			pos.param1 = m_OffsetPos;
			params.Insert(pos);
			GetGame().RPC(null, ERPCs.RPC_SOUND_CONTAMINATION, params, true);

			// We go to the next stage
			m_DecayState = eAreaDecayStage.START;
			SetSynchDirty();
		}
	}

	void PlayExplosionLight()
	{
		m_ShellLight = ShellLight.Cast(ScriptedLightBase.CreateLight(ShellLight, m_OffsetPos));
	}

	void PlayFlareVFX()
	{
		if (GetGame().IsClient() || (GetGame().IsServer() && !GetGame().IsMultiplayer()))
		{
			// We spawn locally the dummy object which will be used to move and manage the particle
			DynamicArea_Flare dummy = DynamicArea_Flare.Cast(GetGame().CreateObjectEx("DynamicArea_Flare", m_OffsetPos, ECE_SETUP | ECE_LOCAL));

			// We add some light to reinforce the effect
			m_FlareLight = FlareLightContamination.Cast(ScriptedLightBase.CreateLight(FlareLightContamination, m_OffsetPos));
		}
	}

	void PlaceParticles(vector pos, float radius, int nbRings, int innerSpacing, bool outerToggle, int outerSpacing, int outerOffset, int partId)
	{
		// Determine if we snap first layer to ground
		//bool snapFirstLayer = true;
		//if (m_Type == eZoneType.STATIC && pos[1] != GetGame().SurfaceRoadY(pos[0], pos[2]))
		//	snapFirstLayer = false;

		// BEGINNING OF SAFETY NET
		// We want to prevent divisions by 0
		if (radius == 0)
		{
			// In specific case of radius, we log an error and return as it makes no sense
			Error("[WARNING] :: [EffectArea PlaceParticles] :: Radius of contaminated zone is set to 0, this should not happen");
			return;
		}

		if (outerToggle && radius == outerOffset)
		{
			Error("[WARNING] :: [EffectArea PlaceParticles] :: Your outerOffset is EQUAL to your Radius, this will result in division by 0");
			return;
		}

		// Inner spacing of 0 would cause infinite loops as no increment would happen
		if (innerSpacing == 0)
			innerSpacing = 1;

		// END OF SAFETY NET

		int partCounter = 0;	  // Used for debugging, allows one to know how many emitters are spawned in zone
		int numberOfEmitters = 1; // We always have the central emitter

		//Debug.Log("We have : " + nbRings + " rings");
		//Debug.Log("We have : " + m_VerticalLayers + " layers");

		float angle = 0; // Used in for loop to know where we are in terms of angle spacing ( RADIANS )

		// We also populate vertically, layer 0 will be snapped to ground, subsequent layers will see particles floating and relevant m_VerticalOffset
		for (int k = 0; k <= m_VerticalLayers; k++)
		{
			vector partPos = pos;
			// We prevent division by 0
			// We don't want to tamper with ground layer vertical positioning
			if (k != 0)
			{
				partPos[1] = partPos[1] + (m_VerticalOffset * k);
			}

			// We will want to start by placing a particle at center of area
			m_ToxicClouds.Insert(Particle.PlayInWorld(partId, partPos));
			partCounter++;

			// For each concentric ring, we place a particle emitter at a set offset
			for (int i = 1; i <= nbRings + outerToggle; i++)
			{
				//Debug.Log("We are on iteration I : " + i );

				// We prepare the variables to use later in calculation
				float angleIncrement;	   // The value added to the offset angle to place following particle
				float ab;				   // Length of a side of triangle used to calculate particle positionning
				vector temp = vector.Zero; // Vector we rotate to position next spawn point

				// The particle density is not the same on the final ring which will only happen if toggled
				// Toggle uses bool parameter treated as int, thus i > nbRings test ( allows to limit branching )
				if (i > nbRings)
				{
					ab = radius - outerOffset; // We want to leave some space to better see area demarcation

					// We calculate the rotation angle depending on particle spacing and distance from center
					angleIncrement = Math.Acos(1 - ((outerSpacing * outerSpacing) / (2 * Math.SqrInt(ab))));
					temp[2] = temp[2] + ab;

					//Debug.Log("Radius of last circle " + i + " is : " + ab);
				}
				else
				{
					ab = (radius / (nbRings + 1)) * i; // We add the offset from one ring to another

					// We calculate the rotation angle depending on particle spacing and distance from center
					angleIncrement = Math.Acos(1 - ((innerSpacing * innerSpacing) / (2 * Math.SqrInt(ab))));
					temp[2] = temp[2] + ab;

					//Debug.Log("Radius of inner circle " + i + " is : " + ab);
				}

				for (int j = 0; j <= (Math.PI2 / angleIncrement); j++)
				{
					// Determine position of particle emitter
					// Use offset of current ring for vector length
					// Use accumulated angle for vector direction

					float sinAngle = Math.Sin(angle);
					float cosAngle = Math.Cos(angle);

					partPos = vector.RotateAroundZero(temp, vector.Up, cosAngle, sinAngle);
					partPos += pos;

					// We snap first layer to ground if specified
					//if (k == 0 && snapFirstLayer == true)
					//	partPos[1] = GetGame().SurfaceY(partPos[0], partPos[2]);
					//else if (k == 0 && snapFirstLayer == false)
					partPos[1] = partPos[1] - m_NegativeHeight;

					// We check the particle is indeed in the trigger to make it consistent
					if (partPos[1] <= pos[1] + m_PositiveHeight && partPos[1] >= pos[1] - m_NegativeHeight)
					{
						// Place emitter at vector end ( coord )
						Particle toxicParticle = Particle.PlayInWorld(partId, partPos);
						toxicParticle.SetOrientation(GetGame().GetSurfaceOrientation(partPos[0], partPos[2]));
						m_ToxicClouds.Insert(toxicParticle);

						partCounter++;
					}

					// Increase accumulated angle
					angle += angleIncrement;
				}

				angle = 0; // We reset our accumulated angle for the next ring
			}
		}

		//Debug.Log("Emitter count : " + partCounter );
	}
};
